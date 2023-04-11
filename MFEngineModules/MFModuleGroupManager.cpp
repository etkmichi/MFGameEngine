/*
 * MFModuleGroupManager.cpp
 *
 *  Created on: 26.03.2021
 *      Author: michl
 */

#include "MFModuleGroupManager.h"
#include <MFBasicDefines.h>

MFModuleGroupManager::MFModuleGroupManager(MFSyncObjectResources* pRes) {
  mp_vecModuleGroups=new std::vector<MFModuleGroup*>();
  this->pRes=pRes;
  initDefaultGroup();
}
MFModuleGroupManager::MFModuleGroupManager(){
  mp_vecModuleGroups=new std::vector<MFModuleGroup*>();
  P_WRN("no default group and no sync resources!");
  pRes=nullptr;
}

MFModuleGroupManager::~MFModuleGroupManager() {
}

bool MFModuleGroupManager::addModuleToGroup(uint32_t groupIndex,MFBaseModule *pModule){
  lockVecGroups.lock();
  if(groupIndex>=mp_vecModuleGroups->size()){
    lockVecGroups.unlock();
    P_ERR("invalid group index!");
    return false;
  }

  if(pModule==nullptr){
    lockVecGroups.unlock();
    P_ERR("pModule==nullptr!");
    return false;
  }

  mp_vecModuleGroups->at(groupIndex)->vecModules.push_back(pModule);
  pModule->addModuleGroupIndex(groupIndex);
  lockVecGroups.unlock();

  return true;
}

MFModuleGroup* MFModuleGroupManager::addModuleGroup(std::string groupName){
  P_INF("name: "+groupName);
  if(pRes==nullptr){
    P_ERR("set sync resources before adding a module group!");
    return nullptr;
  }
  lockVecGroups.lock();
  MFModuleGroup* nextModuleGroup=new MFModuleGroup(pRes,groupName,mp_vecModuleGroups->size());
  mp_vecModuleGroups->push_back(nextModuleGroup);
  lockVecGroups.unlock();
  return nextModuleGroup;
}

MFModuleGroup* MFModuleGroupManager::getModuleGroup(uint32_t groupIndex){
  MFModuleGroup* pGr=nullptr;
  lockVecGroups.lock();
  if(groupIndex>mp_vecModuleGroups->size()){
    P_ERR("index out of bounce!");
  }else{
    pGr=mp_vecModuleGroups->at(groupIndex);
  }
  lockVecGroups.unlock();
  return pGr;
}
