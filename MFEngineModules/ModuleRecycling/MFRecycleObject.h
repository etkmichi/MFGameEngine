/*
 * MFRecycleObject.h
 *
 *  Created on: 22.03.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULERECYCLING_MFRECYCLEOBJECT_H_
#define MFENGINEMODULES_MODULERECYCLING_MFRECYCLEOBJECT_H_
#include "../MFModuleObject.h"
#include <MFBasicDefines.h>
#include <mutex>
/**
 * TODO vecInput snycOs shall be moved to a separated space (3D space). after objects were
 * moved, they are recycled and can be added to reusable vec. during movement objects shall
 * be in vec transition
 */
class MFRecycleObject : public MFModuleObject {
protected:
  struct S_RecycleData {
    MFSyncObject* pSO=nullptr;
    MFModuleObject* pMO=nullptr;
    uint32_t recycleState=0;
  };
  std::vector<S_RecycleData*>
  vecRecycleInput,
  vecRecycleTransition,
  vecReusableRecycleData;
  std::mutex
  lockVecRecycleInput,
  lockVecRecycleTransition,
  lockVecRecycleData;
protected: /*virtual functions MFRecycleObject:*/
  virtual void processInput(std::vector<S_RecycleData*> pVecInput){P_WRN("no impl.!");}
  virtual void processTransition(std::vector<S_RecycleData*> pVecInput){P_WRN("no impl.!");}

protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject){};
  virtual void setupObjectDataStructure(){};
public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){return true;};
  virtual bool updateObject(){return true;};
  virtual void disable(){};
  virtual void enable(){};
  virtual bool synchronizeInput(){
    P_WRN("not impl.!");
    return true;
  };

  /**
   * Writes the data to the given MFSyncObject.
   * @param syncObject
   * @return
   */
  virtual bool synchronizeOutput(){
    P_WRN("not impl.!");
    return true;
  };

public:
  MFRecycleObject();
  virtual ~MFRecycleObject();
};

#endif /* MFENGINEMODULES_MODULERECYCLING_MFRECYCLEOBJECT_H_ */
