/*
 * MFModuleGroup.cpp
 *
 *  Created on: 23.03.2021
 *      Author: michl
 */

#include "MFModuleGroup.h"

MFModuleGroup::MFModuleGroup(
    MFSyncObjectResources *pSyncRes,
    std::string groupName,
    uint32_t groupIndex):
    MFRecycleObjectSink(pSyncRes){
  this->groupIndex=groupIndex;
  this->groupName=groupName;
}

MFModuleGroup::~MFModuleGroup() {
  // TODO Auto-generated destructor stub
}

void MFModuleGroup::addParentGroup(MFModuleGroup* pMG){
  vecParentGroups.push_back(pMG);
}

void MFModuleGroup::disableModuleObject(MFSyncObject* pSO){
  /*parents recycling:*/
  for(MFModuleGroup* pG:vecParentGroups){
    pG->disableModuleObject(pSO);
  }

  /*this will enqueue the objects for recycling!*/
  for(MFBaseModule *pBM:vecModules){
    pBM->enqueueRecycleModuleObject(pSO);
  }
  return;
}

bool MFModuleGroup::recycleReuseTransition(
    MFBaseModule* pM,
    MFSyncObject* pSO,
    bool forceAdd){
  MFModuleObject* pMO=pM->getModuleObject(pSO);
  bool ret=true;
  bool add=false;
  if(pMO==nullptr){/*object was recycled*/
    /*reuse the object:*/
    bool reused=pM->reuseRecycledModuleObject(pSO);
    if(reused==false && !forceAdd){
      P_ERR("failed to add recycled object! Mod/Grp: "+pM->getSetupName()+"/"+groupName);
      ret=false;
      add=true;
    }
  }else{
    add=true;
  }
  if(forceAdd && add){
    if(!pM->addSyncObject(pSO)){
      P_ERR("failed to add sync object to module! Mod/Grp: "+
          pM->getSetupName()+"/"+groupName);
      ret=false;
    }
  }
  return ret;
}

bool MFModuleGroup::addSyncObject(MFSyncObject* pSO,bool forceAdd, bool mainGroup){
  bool ret=true;


  bool recycled=pSO->isRecycleReuseTransition();
  bool addSO=!recycled;

  lockVecModules.lock();
  for(MFBaseModule *pM:vecModules){
    /*Check if object was already added to module, then recycle or do nothing*/
    if(recycled){
      P_INF("reusing recycled object of group/module: "+groupName+"/"+pM->getSetupName());
      ret&=recycleReuseTransition(pM,pSO,forceAdd);
      continue;
    }

    /*add to module*/
    P_INF("adding object to group/module: "+groupName+"/"+pM->getSetupName());
    /*continue if object  was already added (custom objects)*/
    if(!forceAdd && pM->getModuleObject(pSO)!=nullptr)
      continue;
    if(!pM->addSyncObject(pSO)){
      P_ERR("failed to add sync object to module! Mod/Grp: "+pM->getSetupName()+"/"+groupName);
      ret=false;
    }
  }

  /*check if group index was added to sync object and if not, add it*/
  //this is not the best but at the moment only a few groups will be added to a syncO
//  for(uint32_t index:*pSO->getVecModuleGroups()){//TODO do better
//    if(groupIndex == index){
//      addGrp=false;
//      addSO=false;
//      break;
//    }
//  }
  if(addSO){
    lockVecSyncObjects.lock();
//    for(MFSyncObject* pTest:vecSyncObjects){
//      if(pTest==pSO){//TODO remove this its for debugging
//        P_ERR("sync object already within group!");
//        addSO=false;
//      }
//    }
//    if(addSO)
    vecSyncObjects.push_back(pSO);
    lockVecSyncObjects.unlock();
    pSO->addGroupIndex(groupIndex,false);
  }
  lockVecModules.unlock();

  /*add object to parent groups*/
  for(MFModuleGroup* pParent:vecParentGroups){
    ret&=pParent->addSyncObject(pSO, forceAdd,false);
  }

  /*if main group, then object state should be changed*/
  if(mainGroup){
    pSO->changeRecycleState(MFSyncObject::RECYCLE_STATE_IN_USE);
  }

  return ret;
}

void MFModuleGroup::addModule(MFBaseModule* pBM){
  lockVecModules.lock();
  if(pBM==nullptr){
    P_ERR("cant add nullptr!!!");
  }else
    vecModules.push_back(pBM);
  lockVecModules.unlock();
}
