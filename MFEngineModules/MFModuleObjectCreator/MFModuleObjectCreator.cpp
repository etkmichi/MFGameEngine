/*
 * MFModuleObejctCreator.cpp
 *
 *  Created on: 13.11.2020
 *      Author: michl
 */

#include <glm/gtc/random.hpp>

#include "MFModuleObjectCreator.h"

MFModuleObjectCreator::MFModuleObjectCreator(MFModuleObjectManager* pMgr) {
  mp_vecEnabledCreators=new std::vector<S_Creator*>();
  mp_vecCreators=new std::vector<S_Creator*>();
  mp_objectManager=pMgr;
  mp_objectInitSetup=new MFInitialObjectSetup();
  mp_objectInitSetup->initAllParameters();
  vecCreationCallbacks.clear();
}

MFModuleObjectCreator::~MFModuleObjectCreator() {
  // TODO Auto-generated destructor stub
}

MFSyncObject* MFModuleObjectCreator::createNextObject(){
  lockVecEnabledCreators.lock();
  if(mp_vecEnabledCreators->size()==0){
    lockVecEnabledCreators.unlock();
    MFObject::printWarning("MFModuleObejctCreator::createNextObject - no creators added!");
    return nullptr;
  }

  MFSyncObject* pSO=mp_objectManager->createSyncObject();
  pSO->setInitialSetup(mp_objectInitSetup);

  /*create random index for random creator*/
  uint32_t randCreationIndex=mp_vecEnabledCreators->size()-1;
  uint32_t min=0;
  randCreationIndex=glm::linearRand(min, randCreationIndex);
  S_Creator* pCreator=mp_vecEnabledCreators->at(randCreationIndex);

  /*pre creation call of this creator*/
  setupInitialData(pSO);
  if(!preCreation(pSO)){
    MFObject::printWarning("MFModuleObejctCreator::createNextObject - "
        "preCreation went wrong!");
  }

  /*create module objects with creators pre/post creation function*/
  for(uint32_t i=0;i<pCreator->pVecCreators->size();i++){
    MFIModuleObjectSetup* pMOC=pCreator->pVecCreators->at(i);
    if(!pMOC->preObjectCreation(pSO)){
      MFObject::printWarning("MFModuleObejctCreator::createNextObject - "
          "sth went wrong during MFIModuleObjectCreator::preCreation!");
    }

    MFBaseModule* pModule=pCreator->pVecModules->at(i);/*get the object related module*/
    MFModuleObject* pMO=pMOC->createModuleObject(pSO);/*creates a new module object*/

    /*add to module... pMO will be added to groups later*/
    if(pMO!=nullptr && pModule!=nullptr)
      pModule->addSyncObject(pSO, pMO);/*add the created object to the module*/


    if(!pMOC->postObjectCreation(pSO,pMO)){/*post creation call of the creator interface*/
      MFObject::printWarning("MFModuleObejctCreator::createNextObject - "
          "sth went wrong during MFIModuleObjectCreator::postCreation!");
    }
  }

  /*add to groups, force add = false -> if already added to a specific module,
   *it will not be added again!*/
  for(MFModuleGroup* pG:*pCreator->pVecGroups){
    if(pG!=nullptr)
      mp_objectManager->addCustomObject(pSO, pG, false, 0, true, false);
  }

  if(!postCreation(pSO)){/*post creation call of this object*/
    MFObject::printWarning("MFModuleObejctCreator::createNextObject - "
        "postCreation went wrong!");
  }

  lockVecEnabledCreators.unlock();
  return pSO;
}

int64_t MFModuleObjectCreator::addCreator(
    MFIModuleObjectSetup* pModOCreator,
    MFBaseModule* pModule,
    MFModuleGroup* pGroup,
    bool enable,
    int64_t creatorIndex){
  lockVecEnabledCreators.lock();

  S_Creator* pCreator=nullptr;
  int64_t creatorCount=(int64_t)(mp_vecCreators->size());
  if(creatorIndex>=0 && (creatorIndex < creatorCount)){
    pCreator=mp_vecCreators->at(creatorIndex);
  }else{
    pCreator=new S_Creator();
    pCreator->init();
    pCreator->staticIndex=mp_vecCreators->size();
    mp_vecCreators->push_back(pCreator);
  }

  pCreator->push_back(pModOCreator,pModule,pGroup);
  lockVecEnabledCreators.unlock();

  enableCreator(pCreator->staticIndex,enable);

  return pCreator->staticIndex;
}

void MFModuleObjectCreator::addCreationCallback(MFCreationCallback* pCallback){
  lockVecCreationCallbacks.lock();
  vecCreationCallbacks.push_back(pCallback);
  lockVecCreationCallbacks.unlock();
}

void MFModuleObjectCreator::enableCreator(uint32_t index,bool enable){
  if(index>=mp_vecCreators->size()){
    return;
  }
  S_Creator* pCreator=mp_vecCreators->at(index);
  if(pCreator->enabled && enable){//Xor
    return;
  }
  if(!pCreator->enabled && !enable){
    return;
  }
  pCreator->enabled=enable;
  lockVecEnabledCreators.lock();
  if(enable){
    pCreator->dynamicIndex=mp_vecEnabledCreators->size();
    mp_vecEnabledCreators->push_back(pCreator);
  }else{
    if(mp_vecEnabledCreators->size()>=1){
      mp_vecEnabledCreators->data()[pCreator->dynamicIndex]=mp_vecEnabledCreators->back();
      mp_vecEnabledCreators->back()->dynamicIndex=pCreator->dynamicIndex;
      mp_vecEnabledCreators->resize(mp_vecEnabledCreators->size()-1);
    }
  }
  lockVecEnabledCreators.unlock();
}
