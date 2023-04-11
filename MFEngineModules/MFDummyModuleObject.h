/*
 * MFDummyModuleObject.h
 *
 *  Created on: 28.04.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFDUMMYMODULEOBJECT_H_
#define MFENGINEMODULES_MFDUMMYMODULEOBJECT_H_

#include "MFModuleObject.h"

class MFDummyModuleObject: public MFModuleObject {
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
  MFDummyModuleObject();
  virtual ~MFDummyModuleObject();
};

#endif /* MFENGINEMODULES_MFDUMMYMODULEOBJECT_H_ */
