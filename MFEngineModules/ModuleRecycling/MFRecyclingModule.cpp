/*
 * MFRecyclingModule.cpp
 *
 *  Created on: 19.03.2021
 *      Author: michl
 */

#include "../ModuleRecycling/MFRecyclingModule.h"
#include "../../MFSynchronisationClasses/MFSyncObject.h"
#include "MFRecycleObjectSink.h"
MFRecyclingModule::MFRecyclingModule() {
  pVecReferenceCheck=new std::vector<S_RecycleCollection*>();
}

MFRecyclingModule::~MFRecyclingModule() {
}

void MFRecyclingModule::doReferenceCheck(){
  lockVecReferenceCheck.lock();
  if(pVecReferenceCheck->size()==0){
    lockVecReferenceCheck.unlock();
    return;
  }
  std::vector<S_RecycleCollection*> inUseBuffer;
  for(uint32_t i=0;i<pVecReferenceCheck->size();i++){
    S_RecycleCollection* pRC=pVecReferenceCheck->data()[i];
    MFSyncObject* pSO = static_cast<MFSyncObject*>(pRC->pRecycleObject);
    MFRecycleObjectSink* pRS = static_cast<MFRecycleObjectSink*>(pRC->pRecycleSink);
    if(pSO->getReferenceCount()==0){
      pSO->changeRecycleState(MFSyncObject::RECYCLE_STATE_RECYCLED);
      pRS->addReusableObject(pSO);
//      P_INF("step 3 recycling object | id: "+T_S(pSO->getObjectID()));
      delete pRC;
      pVecReferenceCheck->data()[i]=nullptr;
    }else{
      inUseBuffer.push_back(pRC);
    }
  }
  pVecReferenceCheck->resize(inUseBuffer.size());
  uint32_t index=0;
  for(S_RecycleCollection* pRC:inUseBuffer){
//    MFSyncObject* pSO = static_cast<MFSyncObject*>(pRC->pRecycleObject);
//    P_INF("step 2.1 reenqueueing object | id: "+T_S(pSO->getObjectID()));
    pVecReferenceCheck->data()[index]=pRC;
    index++;
  }
  lockVecReferenceCheck.unlock();
}

bool MFRecyclingModule::recycleObjects(std::vector<S_RecycleCollection*> *pVecUserObjects){
  /*indices are referencing sync objects*/
  for(S_RecycleCollection* pRecycle:*pVecUserObjects){
    MFRecycleObjectSink* pRS = static_cast<MFRecycleObjectSink*>(pRecycle->pRecycleSink);
    MFSyncObject* pSO = static_cast<MFSyncObject*>(pRecycle->pRecycleObject);
    switch(pSO->getRecycleState()){
    case MFSyncObject::RECYCLE_STATE_TRANS_RECYCLED:
      /*disabling module objects -> this will enqueue objects in module for disabling!*/
      pRS->disableModuleObject(pSO);
      lockVecReferenceCheck.lock();
//      P_INF("step 2.0 enqueuing f. recycleing object | id: "+T_S(pSO->getObjectID()));
      pVecReferenceCheck->push_back(pRecycle);
      lockVecReferenceCheck.unlock();
      break;
    case MFSyncObject::RECYCLE_STATE_TRANS_REUSE:
      P_WRN("recycling not possible, object state: RECYCLE_STATE_TRANS_REUSE!");
      break;
    case MFSyncObject::RECYCLE_STATE_RECYCLED:
      P_WRN("recycling not possible, object state: RECYCLE_STATE_RECYCLED!");
      break;
    case MFSyncObject::RECYCLE_STATE_IN_USE:
      P_WRN("recycling not possible, object state: RECYCLE_STATE_REUSED!");
      break;
    default:
      P_WRN("recycling not possible, no code for given object state!");
      break;
    }
  }
  pVecUserObjects->clear();
  return true;
}
