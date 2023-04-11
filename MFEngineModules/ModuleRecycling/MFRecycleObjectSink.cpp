/*
 * MFIObjectRecycleSink.cpp
 *
 *  Created on: 22.03.2021
 *      Author: michl
 */

#include "MFRecycleObjectSink.h"
#include "../MFInterfacesModules/MFIRecycleManager.h"


MFRecycleObjectSink::MFRecycleObjectSink(MFSyncObjectResources* pRes){
  this->pRes=pRes;
  if(pRes==nullptr)
    P_ERR("nullptrs!");
}

MFRecycleObjectSink::~MFRecycleObjectSink() {
}

void MFRecycleObjectSink::enqueueRecycleObject(MFSyncObject* pRecyclingObject){
  if((pRecyclingObject->getRecycleState()==MFSyncObject::RECYCLE_STATE_NONE ||
      pRecyclingObject->getRecycleState()==MFSyncObject::RECYCLE_STATE_IN_USE)){
    //disabling module object should be done after state change from transition to recycled!
    if(!pRecyclingObject->changeRecycleState(MFSyncObject::RECYCLE_STATE_TRANS_RECYCLED)){
      P_WRN("failed to change recycle state to RECYCLE_STATE_TRANS_RECYCLED!");
      return;
    }
    P_INF("step 1 recycling transition of object | id: "+T_S(pRecyclingObject->getObjectID()));
    //TODO in game loop add end sync which will change recycle state to recycled
    //TODO the object is not recycled immediately,
    //reusing the object is not possible during transition!
    /*object changes to recycled*/
    pRes->pRecycleManager->addRecycleObject(pRecyclingObject,this);
    return;
  }

}

void MFRecycleObjectSink::addReusableObject(MFSyncObject* pSO){
  lockVecRecycledObjects.lock();
  if(!pSO->isRecycled()){
    P_ERR("sync object not recycled yet!!!");
    lockVecRecycledObjects.unlock();
    return;
  }
  lastValidIndex++;
  if(vecRecycledObjects.size()<=(lastValidIndex)){
    vecRecycledObjects.push_back(pSO);
    lockVecRecycledObjects.unlock();
    return;
  }
  vecRecycledObjects.data()[lastValidIndex]=pSO;
  lockVecRecycledObjects.unlock();
}

MFSyncObject* MFRecycleObjectSink::getRecycledObject(){
  lockVecRecycledObjects.lock();
  /*this will occure if lastValidIndex==0xFFFFFFFF*/
  if( lastValidIndex > vecRecycledObjects.size()) {
    lockVecRecycledObjects.unlock();
    return nullptr;
  }

  MFSyncObject* pSO=vecRecycledObjects.at(lastValidIndex);
  if(!pSO->isRecycled()){
    P_ERR("object not recycled!");
    lockVecRecycledObjects.unlock();
    return nullptr;
  }
  if(!pSO->changeRecycleState(MFSyncObject::RECYCLE_STATE_TRANS_REUSE)) {
    lockVecRecycledObjects.unlock();
    P_ERR("reusing recycled object failed!");
    return nullptr;
  }
//  P_INF("returning recycled object for reuse!");
  vecRecycledObjects.data()[lastValidIndex]=nullptr;
  lastValidIndex--;
  lockVecRecycledObjects.unlock();

  return pSO;
}

uint32_t MFRecycleObjectSink::getRecycledObjectCount(){
  return lastValidIndex+1;
}
