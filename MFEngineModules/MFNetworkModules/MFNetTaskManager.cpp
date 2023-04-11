/*
 * MFNetPackageDispatchManager.cpp
 *
 *  Created on: 18.03.2020
 *      Author: michl
 */

#include "MFNetTaskManager.h"

#include "MFNetworkTasks/MFModuleTasks/MFNetAddModulesTask.h"
#include "MFNetworkTasks/MFModuleTasks/MFNetChangeModuleSetupTask.h"
#include "MFNetworkTasks/MFModuleTasks/MFNetInitModuleGroupTask.h"
#include "MFNetworkTasks/MFModuleTasks/MFNetStartLoopTask.h"
#include "MFNetworkTasks/MFNetActionTask.h"
#include "MFNetworkTasks/MFNetErrorTask.h"
#include "MFNetworkTasks/MFNetObjectSetupTask.h"
#include "MFNetworkTasks/MFNetSyncDataTask.h"
#include "MFNetworkTasks/MFNetAddGroupsTask.h"
#include "MFNetworkTasks/MFNetModuleObjectSetup.h"

MFNetTaskManager::MFNetTaskManager(MFNetworkResources* pNetRes) {
	mp_netRes=pNetRes;
	m_syncDataCount=10;
	m_actionDataCount=10;
	m_objectSetupCount=10;
	m_moduleSetupCount=3;
	m_errDataCount=20;
	m_moduleInitCount=20;
}

MFNetTaskManager::~MFNetTaskManager() {
}

void MFNetTaskManager::addNetworkTask(MFNetworkBaseTask* pTask,uint32_t dataCount){
	MFStructurableDataBuffer *pStructurableDataBuffer;
	MFIStructurableDataCreator *pDataCreator=pTask;

	pStructurableDataBuffer=new MFStructurableDataBuffer(
			pDataCreator, dataCount);
	pStructurableDataBuffer->initBuffer();
	uint32_t index=addTask(pTask,pStructurableDataBuffer);
	pTask->setTaskManagerIndex(index);
}

bool MFNetTaskManager::initTaskManager(){
	MFNetworkBaseTask *pTask=new MFNetErrorTask();
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.error_task=pTask->getTaskManagerIndex();

	pTask=new MFNetAddModulesTask(mp_netRes->mp_moduleProvider);
	updateTaskProvider(this, m_moduleInitCount, pTask, pTask);
	m_tasks.add_modules_task=pTask->getTaskManagerIndex();

	pTask=new MFNetChangeModuleSetupTask();
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.change_module_task=pTask->getTaskManagerIndex();

	pTask=new MFNetInitModuleGroupTask(mp_netRes->mp_moduleProvider,mp_netRes);
	updateTaskProvider(this, m_moduleInitCount, pTask, pTask);
	m_tasks.module_init_task=pTask->getTaskManagerIndex();

	MFNetActionTask *pATask=new MFNetActionTask(mp_netRes);/*Action task index 4*/
	pTask=pATask;
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.action_task=pTask->getTaskManagerIndex();
  pATask->initOutputPackage();

	pTask=new MFNetObjectSetupTask(mp_netRes);
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.object_setup_task=pTask->getTaskManagerIndex();

	pTask=new MFNetSyncDataTask();
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.sync_data_task=pTask->getTaskManagerIndex();

	pTask=new MFNetAddGroupsTask(mp_netRes->mp_groupProvider);
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.add_groups_task=pTask->getTaskManagerIndex();

	pTask=new MFNetStartLoopTask(mp_netRes);
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.loop_start_task=pTask->getTaskManagerIndex();

	pTask=new MFNetModuleObjectSetup(mp_netRes);
	updateTaskProvider(this, m_errDataCount, pTask, pTask);
	m_tasks.module_object_setup=pTask->getTaskManagerIndex();

	if(!this->isStarted() || this->isStopped()){
		this->startDetached();
	}
	return true;
}

