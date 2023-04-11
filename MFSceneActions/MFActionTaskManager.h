/*
 * MFActionManager.h
 *
 *  Created on: 13.03.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFACTIONMANAGER_H_
#define MFSCENEACTIONS_MFACTIONMANAGER_H_
#include <vector>
#include <MFTasks/MFTaskManager.h>

#include "MFObjectActions/MFActionAddObject.h"
#include "MFBasicActions/MFBaseActionTask.h"
#include "../MFSynchronisationClasses/MFSyncObjectResources.h"
#include "MFActionResources.h"
class MFActionResources;
/**
 * Event basierte und synchroniesierte steuerung mit zeitplan.
 */
/**
 * The action manager is used for registering actions. If a action is needed,
 * the action manager can be asked for a specific action.
 * TODO add permissions for using actions/tasks to prevent executing of dangerous
 * tasks. -> if network module wants a task it gives a tag which identifies the net-
 * work module. If the task is not permitted to be executed by the network module, the
 * action manager will not return the given task.
 */
class MFActionTaskManager : public MFTaskManager {
public:
  struct S_ACTION_TASK_INDEX {
    uint32_t
    no_task=0,
    add_object_task=0;
  };
  uint32_t
  m_taskDataCount=10;

private:
  S_ACTION_TASK_INDEX
  m_tasks;
  bool
  m_isInitialized=false;

  MFActionResources
  *mp_actRes;

public:/*virtual functions MFTaskManager*/
  /**
   * This function must be implemented to add all necessary tasks to the
   * task manager. The task must be added with the addTask function.
   * @return
   */
  virtual bool initTaskManager();
public:
	MFActionTaskManager(MFActionResources* pActRes);
	virtual ~MFActionTaskManager();
	void enqueueTask(MFAbstractTask* pTask);
	void enqueueTask(MFAbstractTask* pTask,uint32_t executionDelay);
	MFActionAddObject* getAddObjectAction(){
	  if(m_tasks.add_object_task==0)return nullptr;
	  return static_cast<MFActionAddObject*>(getTask(m_tasks.add_object_task));
	}

};

#endif /* MFSCENEACTIONS_MFACTIONMANAGER_H_ */
