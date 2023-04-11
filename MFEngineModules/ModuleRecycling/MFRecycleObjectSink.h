/*
 * MFIObjectRecycleSink.h
 *
 *  Created on: 22.03.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFOBJECTRECYCLESINK_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFOBJECTRECYCLESINK_H_
#include <vector>
#include <MFCallback/MFICallback.h>
#include <MFThreadSystem/MFTaskThread.h>
#include <MFBasicDefines.h>

#include "../../MFSynchronisationClasses/MFSyncObject.h"
#include "../MFInterfacesModules/MFIRecycleObject.h"
#include "../../MFSynchronisationClasses/MFSyncObjectResources.h"
class MFRecycleObjectSink :
    public MFIRecycleObject {
public: /*virtual functions of MFIRecycleObject*/
  virtual void enqueueRecycleObject(void* pRecycleObject){
    MFSyncObject* pSO = static_cast<MFSyncObject*>(pRecycleObject);
    enqueueRecycleObject(pSO);
  }
private:
  MFSyncObjectResources
  *pRes;
  struct S_RecycleCallback {
    MFICallback* pCallback=nullptr;
    MFTaskThread *pExecutionThread=nullptr;
  };
  uint32_t
  lastValidIndex=0xFFFFFFFF;

  std::vector<MFSyncObject*>
  vecRecycledObjects,
  vecRecycleTransitionObjects;

  std::mutex
  lockVecRecycleTransitionObjects,
  lockVecRecycledObjects;
public: /*virtual funcitons MFRecycleObjectSink:*/
  /**
   * disables connected module object
   * @param pSO
   */
  virtual void disableModuleObject(MFSyncObject* pSO) = 0;

public:
  MFRecycleObjectSink(MFSyncObjectResources* pRes);//MFIRecycleManager* pRecylceManager);
  virtual ~MFRecycleObjectSink();
  void enqueueRecycleObject(MFSyncObject* pRecyclingObject);
  bool executeRecycleCallbacks();

  /**
   * Takes an object from the recycled vector and returns it. The object's recycle state will be
   * set to recycleReuseTransition!
   * @return
   */
  MFSyncObject* getRecycledObject();
  uint32_t getRecycledObjectCount();

  /**
   *
   * @param pCallback - callback to execute during returnObject(...) call. If external thread is
   * specified, the callback will be executed by the external thread (time of execution
   * not specified).
   * @param pExternalThread - callback will be executed by external thread.
   */
  void addRecycleCallback(MFICallback *pCallback,MFTaskThread* pExternalThread);
  void enableRecycling(bool enable,bool enableDeletion=false);
  void enableDeletion(bool enableDeletion);

  /**
   * Only use this for main recycle sink!
   * @param pSO
   */
  void addReusableObject(MFSyncObject* pSO);
};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFOBJECTRECYCLESINK_H_ */
