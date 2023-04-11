/*
 * MFNetPackageDispatchManager.h
 *
 *  Created on: 18.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETTASKMANAGER_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETTASKMANAGER_H_
#include <MFThreadSystem/MFTaskThread.h>
#include <MFTasks/MFAbstractTask.h>
#include <MFTasks/MFTaskManager.h>
#include <MFData/MFStructurableDataBuffer.h>
#include <MFData/MFInterfacesData/MFIStructurableDataCreator.h>
#include "../../MFSceneActions/MFActionTaskManager.h"
#include "../MFInterfacesModules/MFIManagerControl.h"
#include "../MFInterfacesModules/MFIModuleGroupControl.h"
#include "../MFModuleObjectManager.h"
#include "MFNetworkResources.h"
#include "MFNetworkTasks/MFNetworkBaseTask.h"
/**
 *  * TODO implement in basics library.
 * 	-> all implementation of other tasks can be used
 * 	-> provide unique identifier for tasks, if tasks use wrong id<->index!
 * 	-> use user authentification (not all tasks are allowed to be added with
 * 		the network manager or other modules.)
 * 	-> maybe only provide interfaces/base functionality for task manager
 * 		-> improves safety, if implementation is not global
 */
class MFNetworkResources;
class MFNetTaskManager : public MFTaskManager {
public:
	/*Indices of pre defined tasks*/
	struct S_NET_TASK_INDEX {
		uint16_t
			error_task=0,
			add_modules_task=0,
			change_module_task=0,
			sync_data_task=0,
			action_task=0,
			object_setup_task=0,
			module_setup_task=0,
			module_init_task=0,
			add_groups_task=0,
			loop_start_task=0,
			module_object_setup=0;
	};
private:
	S_NET_TASK_INDEX
		m_tasks;

	uint32_t
		m_syncDataCount,//TODO test which count is good with a lot of actions...
		m_actionDataCount,//TODO what if 100 data are needed at onece?
		m_objectSetupCount,
		m_moduleSetupCount,
		m_errDataCount,
		m_moduleInitCount;

	MFNetworkResources
		*mp_netRes;
	void addNetworkTask(MFNetworkBaseTask* pTask,uint32_t dataCount);
public:
	/**
	 *
	 * @param pNetRes - the network resources, which are needed by the tasks to do their
	 * job.
	 */
	MFNetTaskManager(MFNetworkResources* pNetRes);
	virtual ~MFNetTaskManager();
	/**
	 * Creates, initializes and adds all network tasks to this MFTaskManager.
	 * @return
	 */
	virtual bool initTaskManager();

	/**
	 *
	 *	error_task==0 -> invalid task, some init may failed!
	 * 	sync_data_task>0 ->synchronize data of object
		action_task>0 -> add/enque action
		object_setup_task>0 -> setup object/s
		module_setup_task>0 -> setup modules
		module_init_task>0 -> init modules within module manager
	 * @return
	 */
	const S_NET_TASK_INDEX& getTaskIndices(){
		return m_tasks;
	}
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETTASKMANAGER_H_ */
