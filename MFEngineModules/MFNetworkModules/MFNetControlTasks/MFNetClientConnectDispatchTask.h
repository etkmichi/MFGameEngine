/*
 * MFNetClientConnectDispatchTask.h
 *
 *  Created on: 02.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETCLIENTCONNECTDISPATCHTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETCLIENTCONNECTDISPATCHTASK_H_

#include <MFNetworkTasks/MFNetEventDispatchTask.h>
#include <MFData/MFStructurableDataSetup.h>
#include <MFNetworkLibraryHelper.h>

#include "../../MFInterfacesModules/MFIModuleGroupControl.h"
#include "../MFNetworkTasks/MFModuleTasks/MFNetAddModulesTask.h"
#include "../MFNetworkTasks/MFNetAddGroupsTask.h"
#include "../MFNetworkTasks/MFNetObjectSetupTask.h"
#include "../MFNetworkTasks/MFModuleTasks/MFNetStartLoopTask.h"
#include "../MFNetTaskManager.h"
#include "../MFNetworkResources.h"
#include "../MFNetworkTasks/MFModuleTasks/MFNetInitModuleGroupTask.h"

/**
 * This task handels server sided connect requests. It collects the module setups from the
 * module manager and sends them to the client. To handle connect requests, this
 * MFNetEventDispatchTask must be added to a MFServerClientInstance object (setReceive...).
 * @param pModuleSetup
 * @param pInitData On server side this init data will be used to create the package.
 *
 */
class MFNetClientConnectDispatchTask :
		public MFNetEventDispatchTask {
public:
	static uint32_t
		sm_moduleByteSize,
		sm_maxModuleCount;
private:
	MFNetAddModulesTask
		*mp_addModulesData;
	MFNetAddGroupsTask
		*mp_addGroupsData;
	MFNetInitModuleGroupTask
		*mp_initModulesData;
	MFNetObjectSetupTask
		*mp_objectSetupData;
	MFNetInputPackage
		*mp_moduleObjectSetupData;
	MFNetStartLoopTask
		*mp_startLoopData;

	MFDataObjectBuffer
		*mp_sbAddModules,
		*mp_sbAddGroups,
		*mp_sbInitModules,
		*mp_sbObjectSetup,
		*mp_sbModObjSetup,
		*mp_sbStartLoop;

	MFNetworkResources
		*mp_netServRes;

	bool sendManagerSetup(S_MF_NetworkEvent* pNE);
	bool sendGroupSetup(S_MF_NetworkEvent* pNE);
	bool sendModuleGroupSetup(S_MF_NetworkEvent* pNE);
	bool sendObjectSetup(S_MF_NetworkEvent* pNE);
	bool sendStartLoop(S_MF_NetworkEvent* pNE);
	bool sendModuleObjectSetup(S_MF_NetworkEvent* pNE);

	bool sendData(
		uint8_t* data,
		uint32_t byteSize,
		S_MF_NetworkEvent* pNE);

public:
	/**
	 *
	 * @param pModuleSetup setup of the module manager. This will be sent to clients.
	 * @param taskManagerIndex - index of add modules task within task manager of client. Should
	 * be the same as the one of the server (if code doesn't differ,
	 * see MFNetTaskManager::initTaskManager).
	 */
	MFNetClientConnectDispatchTask(MFNetworkResources* pServerRes);
	virtual ~MFNetClientConnectDispatchTask();
	virtual bool dispatchEvent(S_MF_NetworkEvent* pNE);
	void setModuleSetup(MFStructurableDataSetup* pModuleSetup){
		mp_netServRes->mp_moduleSetup=pModuleSetup;
	};
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETCLIENTCONNECTDISPATCHTASK_H_ */
