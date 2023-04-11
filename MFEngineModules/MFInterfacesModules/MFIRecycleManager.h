/*
 * MFIRecycleManager.h
 *
 *  Created on: 12.04.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFIRECYCLEMANAGER_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFIRECYCLEMANAGER_H_
#include <vector>
#include <mutex>

struct S_RecycleCollection {
  void* pRecycleObject=nullptr;
  void* pRecycleSink=nullptr;
};
/**
 *
 */
class MFIRecycleManager {
private:
  std::mutex
  lockVecRecyclingInput,
  lockVecRecyclingOutput;
  std::vector<S_RecycleCollection*>
  *pVecUserObjectsInput,
  *pVecUserObjectsOutput;
protected: /*virtual functions of recycle manager*/
  virtual bool recycleObjects(std::vector<S_RecycleCollection*> *pVecObjects) = 0;
public:
  MFIRecycleManager(){
    pVecUserObjectsInput=new std::vector<S_RecycleCollection*>();
    pVecUserObjectsOutput=new std::vector<S_RecycleCollection*>();
  };
  virtual ~MFIRecycleManager(){
    delete pVecUserObjectsInput;
    delete pVecUserObjectsOutput;
    //TODO care about in/o void* objects!
  };

  bool doRecycling(){
    lockVecRecyclingInput.lock();
    lockVecRecyclingOutput.lock();
    if(pVecUserObjectsInput->size()==0){
      lockVecRecyclingOutput.unlock();
      lockVecRecyclingInput.unlock();
      return true;
    }
    std::vector<S_RecycleCollection*> *pV=pVecUserObjectsInput;
    pVecUserObjectsInput=pVecUserObjectsOutput;
    pVecUserObjectsOutput=pV;
    pVecUserObjectsInput->clear();
//    P_INF("step 1.1 - recycling object count: "+T_S(pVecUserObjectsOutput->size()));
    bool ret = recycleObjects(pVecUserObjectsOutput);
    lockVecRecyclingOutput.unlock();
    lockVecRecyclingInput.unlock();
    return ret;
  }

  void addRecycleObject(void* pRecylceObject,void* pRecycleSink){
    S_RecycleCollection *pUserObject=new S_RecycleCollection();
    pUserObject->pRecycleObject=pRecylceObject;
    pUserObject->pRecycleSink=pRecycleSink;
    lockVecRecyclingInput.lock();
    pVecUserObjectsInput->push_back(pUserObject);
    lockVecRecyclingInput.unlock();
  }
};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFIRECYCLEMANAGER_H_ */
