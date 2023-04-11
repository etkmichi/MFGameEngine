/*
 * MFNetInitModulesTask.cpp
 *
 *  Created on: 13.05.2020
 *      Author: michl
 */

#include "MFNetInitModuleGroupTask.h"

#include <MFData/MFDataSerializer.h>

MFNetInitModuleGroupTask::MFNetInitModuleGroupTask(
		MFIManagerControl* pModuleProvider,
    MFNetworkResources *pNetServRes)
{
  m_surpressSetupWarning=true;
  mp_netServRes=pNetServRes;
	mp_moduleProvider=pModuleProvider;
	setPackageDataSize(50000);
}

MFNetInitModuleGroupTask::~MFNetInitModuleGroupTask(){

}

MFStructurableData* MFNetInitModuleGroupTask::createNewEmptyData(){
	//TODO what if lag of memory?
	MFNetInitModuleGroupTask* pDat=new MFNetInitModuleGroupTask(
			mp_moduleProvider,
			mp_netServRes);
	pDat->setPackageDataSize(50000);
	return pDat;
}

bool MFNetInitModuleGroupTask::processData(MFStructurableData* pData){

  /*
   * Datagram input pkg:
   * Pkg counter - uint16_t - header; counter for every pkg
   * taskIndex - uint16_t - header; index of the network task
   * ModuleCount - uint32_t - count of modules on server and client side
   * loop over local modules:
   *  group count0 - uint32_t - count of groups module0 shall belong too
   *  loop over count0 group indices:
   *    groupID0 - uint32_t group index of first group
   *    ....
   *  group count1 - uint32_t count of groups module1 shall belong too
   *    ...
   *  ..
   *
   */
  MFObject::printInfo("MFNetInitModulesTask::processData - adding modules to groups.");
  //first two entries are pkgCounter and task index
  uint8_t* src=(uint8_t*)pData->getVecData()->at(2)->getData();
  uint32_t moduleCount=0;
	memcpy(&moduleCount,src,sizeof(uint32_t));
	src+=sizeof(uint32_t);
	if(moduleCount!=mp_moduleProvider->getVecModules()->size()){
	  MFObject::printWarning("MFNetInitModulesTask::processData - module count missmatch:\n"
	      "Local moduleCount: "+std::to_string(mp_moduleProvider->getVecModules()->size())+"\n"
	          "Pkg moduleCount: "+std::to_string(moduleCount));
	}
	for(MFBaseModule* pM:*mp_moduleProvider->getVecModules()){
    if(moduleCount==0){
      break;
    }
		uint32_t moduleGroupCount=0;//count og grps the module belongs too
		memcpy(&moduleGroupCount,src,sizeof(uint32_t));
		src+=sizeof(uint32_t);

		for(uint32_t i=0;i<moduleGroupCount;i++){
			uint32_t groupID=0;
			memcpy(&groupID,src,sizeof(uint32_t));
			if(groupID<mp_netServRes->mp_groupProvider->getGroups()->size()){
			  MFModuleGroup* pMG=mp_netServRes->mp_groupProvider->getModuleGroup(groupID);
				MFObject::printInfo("MFNetInitModulesTask::processData - "
				    "adding module to group (module id | group name) :"
						+std::to_string(pM->getModuleID())+" | "+pMG->groupName);
				mp_netServRes->mp_groupProvider->addModuleToGroup(groupID, pM);
			}else{
				MFObject::printWarning("MFNetInitModulesTask::processData - "
						"groupID > group count -> group doesn't exist!!");
			}
			src+=sizeof(uint32_t);
		}
    moduleCount--;
	}
	return true;
}

uint64_t MFNetInitModuleGroupTask::calculateMaxDataSize(){
  uint64_t pkgDataSize=0;
  pkgDataSize+=sizeof(uint32_t);//moduleGroupSetupCount
  for(MFBaseModule* pM:*mp_moduleProvider->getVecModules()){
    uint32_t groupCount=pM->getVecGroupIDs()->size();//count of groups module belongs too
    uint64_t groupIdsSize=groupCount*sizeof(uint32_t);
    pkgDataSize+=sizeof(uint32_t);//groupCount
    pkgDataSize+=groupIdsSize;//groupID's
  }
  uint64_t sharedBufferSize=pkgDataSize+MFNetInputPackage::getHeaderSize();
  return sharedBufferSize;
}

bool MFNetInitModuleGroupTask::prepOutputPackage(
    void* pDst,
    uint64_t &writtenBytes,
    const uint64_t &maxByteSize){
  /*
   * Datagram Output pkg:
   * Pkg counter - uint16_t - header; counter for every pkg
   * taskIndex - uint16_t - header; index of the network task
   * Count of modules - uint32_t - header extended; count of modules for grouping
   * loop over all modules:
   *  group count of module0 - uint32_t - count of groups module0 belongs too
   *  loop over groops of module0:
   *    index group0 - uint32_t - index of first module
   *    ...
   *  group count of module1
   *  ...
   */
  printInfo("MFNetInitModulesTask::prepOutputPackage - preparing module-group setup output pkg.");
  uint8_t* pDst8=static_cast<uint8_t*>(pDst);
  uint64_t internalWrittenBytes=getHeaderSize()+sizeof(uint32_t);
  if(maxByteSize<internalWrittenBytes){
    MFObject::printErr("MFNetInitModulesTask::prepareOutputPackage 1 - failed, not enough maxByteSize!");
    return false;
  }
  uint16_t taskIndex=mp_netServRes->mp_netTaskManager->getTaskIndices().module_init_task;
  memcpy(pDst8+getByteOffsetPkgCounter(),&mp_netServRes->m_packageCounter,sizeof(uint16_t));
  memcpy(pDst8+getByteOffsetTypeID(),&taskIndex,sizeof(uint16_t));
  pDst8+=getHeaderSize();

  uint32_t moduleGroupsCount=mp_moduleProvider->getVecModules()->size();
  memcpy(pDst8,&moduleGroupsCount,sizeof(uint32_t));//cpy count of module
  pDst8+=sizeof(uint32_t);

  for(MFBaseModule* pM:*mp_moduleProvider->getVecModules()){
    internalWrittenBytes+=sizeof(uint32_t);
    if(maxByteSize<internalWrittenBytes){
      MFObject::printErr("MFNetInitModulesTask::prepareOutputPackage 2 - failed, not enough maxByteSize!");
      return false;
    }
    uint32_t groupCount=pM->getVecGroupIDs()->size();
    memcpy(pDst8,&groupCount,sizeof(uint32_t));//count of groups
    pDst8+=sizeof(uint32_t);

    for(uint32_t i=0;i<groupCount;i++){
      internalWrittenBytes+=sizeof(uint32_t);
      if(maxByteSize<internalWrittenBytes){
        MFObject::printErr("MFNetInitModulesTask::prepareOutputPackage 3 - failed, not enough maxByteSize!");
        return false;
      }
      uint32_t groupID=pM->getVecGroupIDs()->at(i);
      memcpy(pDst8,&groupID,sizeof(uint32_t));//count of groups
      pDst8+=sizeof(uint32_t);
    }
  }
  setPackageCounter(&mp_netServRes->m_packageCounter);
  writtenBytes+=internalWrittenBytes;
  return true;
}
