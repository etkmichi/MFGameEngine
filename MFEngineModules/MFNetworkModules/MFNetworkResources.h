/*
 * MFNetworkResrouces.h
 *
 *  Created on: 28.05.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKRESOURCES_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKRESOURCES_H_
#include <MFData/MFStructurableDataSetup.h>
#include <MFNetworkTasks/MFNetEventDispatchTask.h>
#include <MF0InterfacesNetwork/MFINetChannelSink.h>
#include <MF0InterfacesNetwork/MFINetChannelSink.h>
#include <MFNetworkTasks/MFNetEventDispatchTask.h>
#include <vector>
#include "../MFInterfacesModules/MFIManagerControl.h"
#include "../MFInterfacesModules/MFIEngineLoopControl.h"
#include "../../MFSynchronisationClasses/MFSyncObjectResources.h"
#include "../MFInterfacesModules/MFIModuleGroupControl.h"
#include "MFInterfacesNetwork/MFINetClientControl.h"
#include "../MFModuleObjectManager.h"
#include "../MFModuleGroupManager.h"
#include "../MFModuleSyncTasks/MFModuleSyncTask.h"
#include "MFNetTaskManager.h"
class MFNetTaskManager;
class MFNetworkResources {
public:
  uint32_t
  m_printIndex=0;

  MFSyncObjectResources
  *mp_syncObjectRes;
  MFINetClientControl
  *mp_clientCtl;
  MFStructurableDataSetup
  *mp_moduleSetup;
  MFIManagerControl
  *mp_moduleProvider;
  MFIModuleGroupControl
  *mp_groupProvider;
  MFIEngineLoopControl
  *mp_loopCtl;

  MFActionTaskManager
  *mp_actionManager;
  MFModuleObjectManager
  *mp_objectManager;
  MFNetTaskManager
  *mp_netTaskManager;
  uint16_t
  m_packageCounter=1;

  std::vector<MFINetChannelSink*>
  *mp_vecChannelSinks;
  std::vector<MFModuleSyncTask*>
  *mp_vecSyncTasks;

  MFNetEventDispatchTask
  *mp_dataDispatchTask,
  *mp_connectDispatchTask,
  *mp_disconnectDispatchTask;

  void initDispatchTasks();
  void initChannelSinks();
};



#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKRESOURCES_H_ */
