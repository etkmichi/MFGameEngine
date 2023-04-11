/*
 * MFNetObjectSetupTask.cpp
 *
 *  Created on: 25.03.2020
 *      Author: michl
 */

#include "../MFNetworkTasks/MFNetObjectSetupTask.h"
#include <MFGeometryManager.h>
MFNetObjectSetupTask::MFNetObjectSetupTask(MFNetworkResources *pNetRes) {
  mp_netRes=pNetRes;
  mp_sbObjectSetup=nullptr;
  m_surpressSetupWarning=true;
  mp_objectCount=nullptr;
  setPackageDataSize(16000);
}

MFNetObjectSetupTask::~MFNetObjectSetupTask() {
}

MFStructurableData* MFNetObjectSetupTask::createNewEmptyData(){
  MFNetObjectSetupTask *pDat=new MFNetObjectSetupTask(mp_netRes);
  pDat->setPackageDataSize(16000);//TODO what if a lot of objects cause lag of memory?
  return pDat;
}

MFNetInputPackage* MFNetObjectSetupTask::createPackage(){
  return nullptr;
}

uint32_t MFNetObjectSetupTask::calculatePackageSize(){
  std::vector<MFSyncObject*> *pVO=mp_netRes->mp_objectManager->getSyncObjects();
  /*	p1 = parameter setup of first sync object
   *	g1 = group setup of first sync object */
  /* package setup: objectCount (uint32_t) | size p1 | p1 |... | size g1 | g1 ...*/

  /*calculate size of geometry id and geometry parameters*/
  uint32_t objectParameterSize=0;//object count
  /*calculate size of group setups*/
  uint32_t objGrpSize=0;
  for(MFSyncObject *pSO:*pVO){//TODO what if one pSO==nullptr?
    MFStructurableDataSetup* pParamSetup=pSO->getGeometry()->getGeometrySetup();
    objectParameterSize+=sizeof(uint32_t);//size of size of param setup(min 0bytes)
    objectParameterSize+=pParamSetup->getStructureSize();//parameter setup

    objGrpSize+=sizeof(uint32_t);
    objGrpSize+=sizeof(uint32_t)*pSO->getVecModuleGroups()->size();
  }

  /*create package and fill it*/
  uint32_t size=
      sizeof(uint32_t)+//size of object count
      objectParameterSize+//size of object parameters
      objGrpSize;//size of object's group setups
  uint32_t sharedBufferSize=size+MFNetInputPackage::getHeaderSize();
  return sharedBufferSize;
}

bool MFNetObjectSetupTask::fillPackage(MFNetInputPackage* pPkg){
  //MFNetObjectSetupTask - sync added objects with geometries and groups
  //(no coords mass or sth else)
  //TODO enqueue add object calls during sendObjectSetup call -> objects which are
  //
  //added during this call are not added to the object setup
  if(mp_netRes->mp_objectManager==nullptr){
    printErr("MFNetClientConnectDispatchTask::sendObjectSetup - "
        "mp_netRes->mp_netRes->mp_objectManager==nullptr");
    return false;
  }
  std::vector<MFSyncObject*> *pVO=mp_netRes->mp_objectManager->getSyncObjects();
  /*	p1 = parameter setup of first sync object
   *	g1 = group setup of first sync object */
  /* package setup: objectCount (uint32_t) | size p1 | p1 |... | size g1 | g1 ...*/

  /*create data container*/
  uint32_t sharedBufferSize=calculatePackageSize();
  mp_sbObjectSetup=new MFDataObjectBuffer(sharedBufferSize);
  if(!mp_sbObjectSetup->createBuffer()){
    printErr("MFNetClientConnectDispatchTask::sendObjectSetup - failed"
        "to create shared buffer!");
    return false;
  }
  uint32_t size=sharedBufferSize-MFNetInputPackage::getHeaderSize();
  pPkg->setPackageDataSize(size);
  pPkg->useSharedBuffer(mp_sbObjectSetup);
  printInfo("MFNetObjectSetupTask::fillPackage - "
      "size of object setup: "+std::to_string(size));
  pPkg->initData();

  /*write task index and package counter*/
  uint16_t taskIndex=mp_netRes->mp_netTaskManager->getTaskIndices().object_setup_task;
  pPkg->getTypeIDObject()->writeData(&taskIndex);
  pPkg->getPackageCounter()->writeData(&mp_netRes->m_packageCounter);

  /*send object geometry-id and geometry parameters*/
  /*get dst pointer*/
  uint8_t* dst=static_cast<uint8_t*>(pPkg->getStructureData());
  dst+=MFNetInputPackage::getHeaderSize();
  uint32_t objectCount=pVO->size();

  /*write object count*/
  memcpy(dst,&objectCount,sizeof(uint32_t));
  dst+=sizeof(uint32_t);

  /*wirte geometry parameters*/
  for(MFSyncObject *pSO:*pVO){//TODO what if one pSO==nullptr?
    uint32_t serializedSize=0;
    if(!pSO->getGeometry()->serializeGeometrySetup(serializedSize,dst)){
      printErr("MFNetObjectSetupTask::fillPackage - "
          "failed to copy object init data!");
    }
    dst+=serializedSize;
  }

  /*write object's group setups*/
  for(MFSyncObject *pSO:*pVO){//TODO what if one pSO==nullptr?
    uint32_t gsSize=pSO->getVecModuleGroups()->size()*sizeof(uint32_t);
    memcpy(dst,&gsSize,sizeof(uint32_t));
    dst+=sizeof(uint32_t);
    memcpy(dst,pSO->getVecModuleGroups()->data(),gsSize);
    dst+=gsSize;
  }
  return false;

}

bool MFNetObjectSetupTask::processData(MFStructurableData* pData){
  /*iterate over object setups stored in pData
  * Datagramm:
  * taskIndex | pkg counter | object count | geometry parameters | group setups
  */
  uint8_t* src=((uint8_t*)pData->getVecData()->at(2)->getData());
  mp_objectCount=(uint32_t*)src;
  src+=sizeof(uint32_t);
  printInfo("MFNetObjectSetupTask::processData - adding objects, object count: " +
      std::to_string(*mp_objectCount));
  MFGeometryManager* pGeoMgr=mp_netRes->mp_objectManager->getGeometryManager();
  std::vector<MFSyncObject*> vecSO;
  /*Get the parameter setup from the received data*/
  for(uint32_t i=0;i<*mp_objectCount;i++){
    MFSyncObject* pSO=mp_netRes->mp_objectManager->createSyncObject();
    vecSO.push_back(pSO);
    uint32_t paramSize=0;
    memcpy(&paramSize,src,sizeof(uint32_t));
    src+=sizeof(uint32_t);
    MFAbstractGeometry* pGeo=pGeoMgr->createGeometry((void*)src);
    if(pGeo==nullptr){
      printErr("MFNetObjectSetupTask::processData - failed to "
          "create geometry! Adding box instead!");
      pGeo=pGeoMgr->createBox(glm::vec3(1,1,1));
    }
    pSO->setGeometry(pGeo);
    src+=paramSize;
  }
  std::string info="Objects group setup:\n";
  for(MFSyncObject* pSO:vecSO){//for every so, a group setup is provided by rec. dat.
    uint32_t gsSize=0;
    memcpy(&gsSize,src,sizeof(uint32_t));//size of group setup
    src+=sizeof(uint32_t);
    info+="group count="+std::to_string(gsSize/4)+" group names={";
    for(uint32_t i=0;i<gsSize/4;i++){
      uint32_t groupIndex=0xFFFFFFFF;
      memcpy(&groupIndex,src,sizeof(uint32_t));
      src+=sizeof(uint32_t);
      MFModuleGroup* pModuleGroup=mp_netRes->
          mp_groupProvider->getModuleGroup(groupIndex);
      if(pModuleGroup==nullptr){
        printErr("MFNetObjectSetupTask::processData - no module "
            "group found!");
      }else{
        info+=pModuleGroup->groupName+"; ";
      }
      mp_netRes->mp_objectManager->addCustomObject(
          pSO,
          pModuleGroup,
          true,
          0,
          false);
    }
    info+="}\n";
  }
  printInfo("MFNetObjectSetupTask::processData - "+info);
  return true;
}
