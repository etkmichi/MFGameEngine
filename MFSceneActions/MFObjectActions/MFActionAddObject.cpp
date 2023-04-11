/*
 * MFAddObject.cpp
 *
 *  Created on: 14.09.2020
 *      Author: michl
 */

#include "MFActionAddObject.h"

MFActionAddObject::MFActionAddObject(MFActionResources* pActRes) {
  mp_vecInNotesAdd=new std::vector<MFSyncObject*>();
  mp_vecOutNotesAdd=new std::vector<MFSyncObject*>();
  mp_actRes=pActRes;
}

MFActionAddObject::~MFActionAddObject() {
}

//uint32_t MFAddObject::calculateActionDataSize(std::vector<MFSyncObject*> *pVecObjects){
//  uint32_t dataSize=sizeof(uint32_t);//tags
//  dataSize+=sizeof(uint32_t);//group index
//  dataSize+=sizeof(uint32_t);//object count
//  for(MFSyncObject* pSO:*pVecObjects){
//    pSO->getGeometry()->serializeGeometrySetup(copiedBytes, pDst, maxSize)
//  }
//  return dataSize;
//}

bool MFActionAddObject::writeGroups(
    MFSyncObject* pSyncObject,
    uint32_t* &pDst32,
    const uint32_t &maxByteSize,
    uint32_t &writtenBytes){
  uint32_t remainingBytes=maxByteSize;
  uint32_t groupCount=pSyncObject->getVecModuleGroups()->size();
  uint32_t groupSize=8*groupCount;

  if((groupSize+4)>remainingBytes){
    MFObject::printWarning("MFAddObject::writeGroups - "
        "Couldn't write groups!");
    return false;
  }else{
    writtenBytes+=groupSize+4;
    remainingBytes-=groupSize+4;

    *pDst32=groupCount;
    pDst32++;

    memcpy(
        pDst32,
        pSyncObject->getVecModuleGroups()->data(),
        groupSize);

    pDst32+=groupCount;
  }
  return true;
}

bool MFActionAddObject::writeGeometrySetup(
    MFSyncObject* pSyncObject,
    uint32_t* &pDst32,
    uint32_t &remainingBytes,
    uint32_t &writtenBytes){
  uint32_t copiedGeometryBytes=0;
//  uint32_t *geometrySize=pDst32;
//  pDst32++;

  uint8_t* pDst=(uint8_t*)(pDst32);
  if(pSyncObject->getGeometry()==nullptr){
    P_ERR("geometry == nullptr!");
    return false;
  }
  if(!pSyncObject->getGeometry()->serializeGeometrySetup(
      copiedGeometryBytes,
      pDst,
      remainingBytes)){
    MFObject::printWarning("MFAddObject::fillActionData - "
        "couldn't copy geometry data!");
//    (*geometrySize)=0;
    return false;
  }else{
//    (*geometrySize)=copiedGeometryBytes;
    writtenBytes+=copiedGeometryBytes;
    pDst+=copiedGeometryBytes;
    pDst32=(uint32_t*)(pDst);
  }
  return true;
}

bool MFActionAddObject::writeInitialData(
    MFSyncObject* pSyncObject,
    uint32_t* &dstBuffer,
    uint32_t &remainingBytes,
    uint32_t &writtenBytes){
  MFInitialObjectSetup* pData=pSyncObject->getInitialSetup();
  uint32_t copiedBytes=0;

  uint8_t* pDst=(uint8_t*)(dstBuffer);
  if(!pData->serialize(pDst, remainingBytes, copiedBytes)){
    MFObject::printWarning("MFAddObject::writeInitialData - "
        "failed!");
    return false;
  }
  writtenBytes+=copiedBytes;
  remainingBytes-=copiedBytes;
  dstBuffer+=copiedBytes;
  return true;
}

void MFActionAddObject::addAddedObject(MFSyncObject* pObject){
  lockVecInput.lock();
  mp_vecInNotesAdd->push_back(pObject);
  lockVecInput.unlock();
}

bool MFActionAddObject::deserializeAction(void* pInputData,uint32_t maxInputSize){
  /*
  * Datagramm:
  * Tags
  * Object count
  * #perObject:
  *  - GroupCount
  *    Group indices
  *  - GeometrySetupSize
  *    GeometrySetup
  *  - InitialSetupSize
  *    InitialSetup
  *  -> geometry parameter
  *  -> initial data
  * Group setups
  *  -> count of groups an object belongs too
  *  -> group indices for the current object
  **/
  uint8_t *pSrc=((uint8_t*)(pInputData));

  uint32_t tags=*((uint32_t*)(pSrc));
  pSrc+=sizeof(uint32_t);

  uint32_t objectCount=*((uint32_t*)(pSrc));
  pSrc+=sizeof(uint32_t);

  uint32_t groupCount=*((uint32_t*)(pSrc));
  pSrc+=sizeof(uint32_t);

  std::vector<uint32_t> vecGroups;
  vecGroups.resize(groupCount);
  for(uint32_t i=0;i<vecGroups.size();i++){
    vecGroups.data()[i]=*((uint32_t*)(pSrc));
    pSrc+=sizeof(uint32_t);
  }

  /*create geometry*/
  uint32_t geoSetupSize=*((uint32_t*)(pSrc));
  pSrc+=sizeof(uint32_t);

  MFAbstractGeometry* pGeo=mp_actRes->mp_geometryManager->createGeometry(pSrc);
  pSrc+=geoSetupSize;
  if(pGeo==nullptr){
    printErr("MFActionAddObject::deserializeAction - failed to "
        "create geometry! Adding box instead!");
    pGeo=mp_actRes->mp_geometryManager->createBox(glm::vec3(1,1,1));
  }

  uint32_t initSetupSize=*((uint32_t*)(pSrc));
  pSrc+=sizeof(uint32_t);

  MFInitialObjectSetup* pInitials=new MFInitialObjectSetup();
  pInitials->deserialize(pSrc);//TODO delete initial setup
  pInitials->mp_geometry=pGeo;
  MFSyncObject* pSO=new MFSyncObject(
      mp_actRes->mp_syncObjectRes,
      pInitials);

  pSrc+=initSetupSize;
  uint32_t fstGrpIndex=vecGroups.at(0);
  MFModuleGroup* pGroup=mp_actRes->mp_groupProvider->getModuleGroup(fstGrpIndex);
  printInfo("MFActionAddObject::deserializeAction - adding object to group "+pGroup->groupName);
  mp_actRes->mp_objectManager->addCustomObject(
      pSO,
      pGroup,
      true,
      *pSO->getInitialSetup()->mp_initSpawnCycle,
      false);

  for(uint32_t i=1;i<vecGroups.size();i++){
    //add object to group
    mp_actRes->mp_objectManager->addCustomObject(pSO, pGroup, true, 0, false);
  }

  printInfo("MFActionAddObject::deserializeAction - data:"
      "\nObjectCount="+std::to_string(objectCount)+
      "\ntags="+std::to_string(tags));
  return true;
}

bool MFActionAddObject::serializeAction(
    void* pDstData,
    uint32_t maxByteSize,
    uint32_t &writtenBytes){
  if(mp_vecOutNotesAdd->size()==0){
    lockVecInput.lock();
    if(mp_vecInNotesAdd->size()==0){
      lockVecInput.unlock();
      return true;/*nothing to do*/
    }
    std::vector<MFSyncObject*>* pSwitch=mp_vecInNotesAdd;
    mp_vecInNotesAdd=mp_vecOutNotesAdd;
    mp_vecOutNotesAdd=pSwitch;
    lockVecInput.unlock();
  }
  uint32_t outCount=mp_vecOutNotesAdd->size();
  uint32_t writtenObjects=serializeAddAction(
      mp_vecOutNotesAdd,
      pDstData,
      maxByteSize,
      writtenBytes);
  uint32_t remaining=outCount-writtenObjects;
  if(writtenBytes==0){
    MFObject::printErr("MFAddObject::serializeAction - failed to serialize input data!");
    return false;
  }
  if(writtenObjects==0){
    MFObject::printWarning("MFAddObject::serializeAction - not enough buffer size for "
        "serializing objects! \n"
        "serialized count = "+std::to_string(writtenObjects)+"\n"
            "remaining objects = "+std::to_string(remaining));
  }
  for(uint32_t i=1;i<(remaining);i++){
    addAddedObject(mp_vecOutNotesAdd->at(mp_vecOutNotesAdd->size()-i));
  }
  mp_vecOutNotesAdd->clear();/*clear to make sure same data will be sent again*/
  return true;
}

uint32_t MFActionAddObject::serializeAddAction(
    std::vector<MFSyncObject*> *pVecObjects,
    void* dstData,
    uint32_t maxByteSize,
    uint32_t &writtenBytes){
  /*
  * Datagramm:
  * Tags uint32_t
  * Object count uint32_t
  * #perObject:
  *  - GroupCount
  *    Group indices
  *  - GeometrySetupSize
  *    GeometrySetup
  *  - InitialSetupSize
  *    InitialSetup
  *  -> geometry parameter
  *  -> initial data
  * Group setups
  *  -> count of groups an object belongs too
  *  -> group indices for the current object
  **/
  writtenBytes=0;
  if(maxByteSize<8){
    MFObject::printWarning("MFAddObject::fillActionData - "
        "maxByteSize must be higher!");
    return 0;
  }
  writtenBytes=8;/*header size*/

  /*header start*/
  uint32_t *pDst32=static_cast<uint32_t*>(dstData);

  *pDst32=m_taskFlags;
  pDst32++;

  uint32_t *pObjectCount=pDst32;
  uint32_t remaining=maxByteSize-writtenBytes;

  *pObjectCount=0;//objects will be counted during iteration!
  pDst32++;
  /*header end*/

  for(MFSyncObject* pSO:*pVecObjects){
    /*write groups*/
    if(!writeGroups(pSO,pDst32,remaining,writtenBytes)){
      return (*pObjectCount);
    }

    /*write geometry setup*/
    if(!writeGeometrySetup(pSO,pDst32,remaining,writtenBytes)){
      return (*pObjectCount);
    }

    /*write initial setup*/
    if(!writeInitialData(pSO, pDst32, remaining, writtenBytes)){
      return (*pObjectCount);
    }
    (*pObjectCount)++;
  }
  return (*pObjectCount);
}
