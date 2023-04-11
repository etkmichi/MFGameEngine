/*
 * MFDebugObject.h
 *
 *  Created on: 23.12.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFDEBUGMODULE_MFDEBUGOBJECT_H_
#define MFENGINEMODULES_MFDEBUGMODULE_MFDEBUGOBJECT_H_
#include "../MFModuleObject.h"
class MFDebugObject: public MFModuleObject {
protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject){};
  virtual void setupObjectDataStructure(){};
public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){return true;};
  virtual bool updateObject(){return true;};
  virtual void disable(){};
  virtual void enable(){};
  virtual bool synchronizeInput(){return true;};
  virtual bool synchronizeOutput(){return true;};
public:
  MFDebugObject();
  virtual ~MFDebugObject();
};

#endif /* MFENGINEMODULES_MFDEBUGMODULE_MFDEBUGOBJECT_H_ */
