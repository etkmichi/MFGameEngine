/*
 * MFNetworkModule.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKMODULE_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKMODULE_H_
#include <MFNetClientClasses/MFNetClient.h>
#include <MFNetServerClasses/MFServerClientInstance.h>
#include <MF0InterfacesNetwork/MFINetChannelSink.h>
#include <MFNetworkTasks/MFNetEventDispatchTask.h>
#include <mutex>
#include <MFTime/MFTickCounter.h>

#include "../../MFModuleResources.h"
#include "../MFBaseModule.h"
#include "../ModulePhysics/MFPhysicsModule.h"
#include "../MFInterfacesModules/MFIModuleCreator.h"
#include "../MFModuleObjectManager.h"

#include "MFNetworkStructs.h"
#include "MFNetTaskManager.h"

#include "../../MFSceneActions/MFActionTaskManager.h"
#include "../../MFSceneActions/MFObjectActions/MFActionAddObject.h"
#include "../MFInterfacesModules/MFIManagerControl.h"
#include "MFInterfacesNetwork/MFINetClientControl.h"

#include "../MFModuleSyncTasks/MFModuleSyncTask.h"
#include "MFHelperTasks/MFNetConnectTask.h"

#include "MFNetworkResources.h"

#include "MFNetworkTasks/MFNetActionTask.h"

#include "../MFModuleGroup.h"

/**
 *
 */
class MFNetworkModule:
    public MFBaseModule,
    public MFIModuleCreator,
    public MFINetClientControl,
    public MFINetNotifier {
private:
  std::mutex
  lockVecClients;

  uint8_t
  m_syncDataChannel;

  MFTaskThread
  *mp_networkThread;

  MFServerClientInstance
  *mp_serverInstance;

  std::vector<MFNetClient*>
  *mp_vecServerClients,/*clients of a server instance*/
  *mp_vecClients,/*local clients which already connected to a server*/
  *mp_vecConnectionRequests;/*pending connection requests*/

  MFNetworkResources
  *mp_netClientRes,
  *mp_netServerRes;

  std::vector<MFModuleSyncTask*>
  *mp_vecServerModuleSyncTasks;
  MFModuleGroup
  *mp_clientAddGroup=nullptr;
  MFPrintTarget
  *mp_networkLog;

  MFTickCounter
  *mp_ticker;

  bool
  m_syncNotes=false;

  /**
   * Reads all notes (added by notifyAddObject()) and sends the corresponing network
   * signals.
   */
  void synchronizeNotes();

public:/*MFINetNotifier virtual functions*/
  /**
   * Iterates over connected clients/server-clients and pending connect requests for enqueueing a
   * MFNetActionTask.
   * @param pGroup
   * @param pObject
   * @param spawnCycle
   */
  virtual void notifyAddObject(MFSyncObject* pObject);

protected:/*virtual functions MFIModuleCreator*/
  virtual	MFBaseModule* createModule(){return new MFNetworkModule(mp_syncRes);};

public:
  /**
   * This function notifies that a connection to a client/server was made.
   */
  virtual void addClient(MFNetClient* pClient);
  virtual void addServerClient(MFNetClient* pClient);

protected:
  //TODO create connect dispatch task, which sends data for world/game creation/setup
  //TODO create disconnect by server task, which will disconnect a client by server
  //command this task shall be used if a client was disconnected by some network error like
  //a broken cabel, dsl reconnect ...
  bool initModuleObject(
      MFSyncObject* syncObject,
      MFModuleObject* pObject);
public:
  MFNetworkModule(MFSyncObjectResources* pSyncRes);
  virtual ~MFNetworkModule();

  /*inherited functions*/
  bool initModule();

  bool initNetworkTaskManager();

  MFNetActionTask* getNetActionTask(MFNetTaskManager* pTMgr){
    return static_cast<MFNetActionTask*>(pTMgr->getTask(pTMgr->getTaskIndices().action_task));
  }
  /**
   * Sets a module group which is used to add a client. This will be used
   * if this module is notified that a connection to a server/client was made.
   * @param pGroup
   */
  void setClientAddGroup(MFModuleGroup* pGroup){
    mp_clientAddGroup=pGroup;
  }

  MFModuleObject* createModuleObject();

  //TODO connect function
  /*	the connect function connects the first client to a server and waits
   * 	for server response. The server response will contain the setup for the
   * 	network module.
   * 		The network module must setup its own server instance accor-
   * 		ding to the servers response. The response will contain the max
   * 		participants and the current participant count.
   * 		This client must setup current participants count as MFNetClients and
   * 		connect each of this clients to all available participants.
   * 		The network module must setup client connections to all other
   * 		server instances of participating clients
   **/

  /**
   * Creates a client connection to a server and retruns the client.
   * If no pConnectThread is defined, this function will block till connection is
   * made or connection attempt timed out. If connection was made, the client will
   * add its poll task to the executing thread of this object.
   * @param address
   * @param port
   * @param useClientThread
   * @param pConnectThread if not nullptr, a connect task will be added to this
   * thread. The task will be automatically deleted, even if connection failed!
   * @return nullptr if no connection was made. MFNetClient* if connection was made
   * or connection task was enqueued.
   */
  MFNetClient* connect(
      std::string address,
      uint16_t port,
      uint32_t timeout,
      MFTaskThread* pConnectThread=nullptr,
      bool useClientThread=false);

  /**
   * @param pClient
   * @param timeout
   * @param pDisconnectThread
   * @return
   */
  bool disconnect(
      MFNetClient* pClient,
      uint32_t timeout,
      MFTaskThread* pDisconnectThread);

  /**
   * Creates a server instance on which clients can connect.
   * @param clientCount - max client count
   * @param pModuleManager - ptr to a MFModuleManager object. If nullptr, this
   * function will take the the module mgr from static resources.
   * @return true if successful
   */
  bool createServer(
      uint32_t clientCount,
      uint16_t port=42421,
      void* pModuleManager=nullptr);

  bool executeModule();

  bool syncOutputData();

  /**
   * Adds a MFModuleSyncTask, for synchronisation with all clients.
   * @param pModule - pointer to the module
   * @param moduleIndex - index of the module (same for server and client!)
   * @param syncExecStepCycle - after every syncExecStepCycle, the module data will
   * be copied for synchronisation over network.
   */
  void addServerModuleSync(MFModuleSyncTask* pTask);

  MFNetworkResources* getServerResources(){return mp_netServerRes;};

  MFNetworkResources* getClientResources(){return mp_netClientRes;};

  void setVecServerSyncTasks(std::vector<MFModuleSyncTask*>* pVecServerSync){
    mp_vecServerModuleSyncTasks=pVecServerSync;
  }
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKMODULE_H_ */
