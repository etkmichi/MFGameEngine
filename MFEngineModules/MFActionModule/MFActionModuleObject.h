/*
 * MFActionModuleObject.h
 *
 *  Created on: 29.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFACTIONMODULE_MFACTIONMODULEOBJECT_H_
#define MFENGINEMODULES_MFACTIONMODULE_MFACTIONMODULEOBJECT_H_
#include "MFActionTriggers/MFBaseTrigger.h"
#include "../MFModuleObject.h"
#include "../../MFSceneActions/MFBasicActions/MFBaseActionTask.h"

class MFActionModuleObject:
		public MFModuleObject {
private:
  std::vector<MFBaseTrigger*>
  *mp_vecTriggers;
  std::vector<MFBaseActionTask*>
  *mp_vecActions;
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
	MFActionModuleObject();
	virtual ~MFActionModuleObject();

	/**
	 * Checks if conditions are triggered
	 * @return
	 */
	bool isTriggered();
	/*TODO*/
	/*syncInput -> trigger check (check if action needs to be executed)*/
  /*execute -> execute tasks check tasks for isDone() and remove*/
	/*syncOutput -> disable action if done*/
	/*create capsulated actions ->
	 * if many actions need to be added, this can safe processor time
	 * -> one action which checks if other actions must be checked*/
};

#endif /* MFENGINEMODULES_MFACTIONMODULE_MFACTIONMODULEOBJECT_H_ */
