/*
 * MFNetworkModule.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include <glm/glm.hpp>
#include <MFTime/MFTimeStructs.h>
#include "MFNetworkModule.h"
#include "MFNetworkModuleObject.h"
#include "../MFModuleManager.h"

MFNetworkModule::MFNetworkModule(MFSyncObjectResources* pSyncRes) :
MFBaseModule(pSyncRes),
MFIModuleCreator("MFNetworkModule"){
  enableInputSynchronisation(false);
  enableNotification(false);
  enableOutputSynchronisation(false);
  mp_netClientRes=new MFNetworkResources();
  mp_netServerRes=new MFNetworkResources();
  mp_serverInstance=nullptr;
  mp_vecServerClients=nullptr;
  mp_networkThread=nullptr;
  m_syncDataChannel=1;
  mp_vecClients=new std::vector<MFNetClient*>();
  mp_vecConnectionRequests=new std::vector<MFNetClient*>();
  mp_vecServerModuleSyncTasks=new std::vector<MFModuleSyncTask*>();
  mp_networkLog=nullptr;
  mp_ticker=new MFTickCounter();
  m_printIndex=0;
}

MFNetworkModule::~MFNetworkModule(){
  mp_networkThread->stop();
  mp_vecConnectionRequests->clear();
  while(!mp_networkThread->isStopped()){
    printInfo("MFNetworkModule::~MFNetworkModule - waiting for network thread to"
        "stop.");//TODO test what happens if task thread is waiting for a connection.
    std::this_thread::sleep_for(U_MillisDuration(200));
  }
  delete mp_serverInstance;
  delete mp_vecClients;
  delete mp_vecConnectionRequests;
}

/*virtual functions*/
bool MFNetworkModule::initModule(){
  printInfo("MFNetworkModule::initModule - init");
//  mp_networkLog=MFModuleResources::smp_guiManager->addPrinterWindow(
//      "NetworkModule"+std::to_string(getModuleID()),
//      mp_syncRes->mp_logWindow);
//  //TODO sometimes mp_net... ==nullptr!
//  mp_networkLog->registerPrinter();
//  m_printIndex=mp_networkLog->getPrinterIndex();
  /*client res will be init by connect*/
  /*server resources will by initialized by createServer function*/
  return true;
}

MFModuleObject* MFNetworkModule::createModuleObject(){
  return new MFNetworkModuleObject();
}

void MFNetworkModule::notifyAddObject(MFSyncObject* pObject){
  /*iterate over clients/server-clients and pending mp_vecConnectionRequests*/
  if(mp_netClientRes->mp_actionManager!=nullptr){
    MFActionAddObject* pAddO=mp_netClientRes->mp_actionManager->getAddObjectAction();
    if(pAddO!=nullptr){
      pAddO->addAddedObject(pObject);
      m_syncNotes=true;
    }
  }
  if(mp_netServerRes->mp_actionManager!=nullptr){
    MFActionAddObject* pAddO=mp_netServerRes->mp_actionManager->getAddObjectAction();
    if(pAddO!=nullptr){
      pAddO->addAddedObject(pObject);
      m_syncNotes=true;
    }
  }
}

void MFNetworkModule::addServerClient(MFNetClient* pClient){
  lockVecClients.lock();
  MFSyncObject* pSyncO=new MFSyncObject(mp_netServerRes->mp_syncObjectRes);
  MFNetworkModuleObject* pModO=static_cast<MFNetworkModuleObject*>(addSyncObject(pSyncO));
  if(pModO==nullptr){
    printErr("MFNetworkModule::addClient - "
        "failed to add client!");
    lockVecClients.unlock();
    return;
  }
  pModO->setClient(pClient);
  mp_vecServerClients->push_back(pClient);
  lockVecClients.unlock();
}

void MFNetworkModule::addClient(MFNetClient* pClient){
  lockVecClients.lock();
  MFSyncObject* pSyncO=new MFSyncObject(mp_netClientRes->mp_syncObjectRes);
  MFNetworkModuleObject* pNetO=static_cast<MFNetworkModuleObject*>(
      addSyncObject(pSyncO));
  if(pNetO==nullptr){
    printErr("MFNetworkModule::addClient - "
        "failed to add client!");
    lockVecClients.unlock();
    return;
  }
  pNetO->setClient(pClient);
  lockVecClients.unlock();
}

bool MFNetworkModule::initModuleObject(
    MFSyncObject* syncObject,
    MFModuleObject* pObject){
  MFNetworkModuleObject* pNMO=(MFNetworkModuleObject*)pObject;
  pNMO->setSyncObject(syncObject);
  //	mp_netT TODO add client to module object and add module object to object manager
  return true;
}

bool MFNetworkModule::executeModule(){
  for(MFModuleSyncTask* pMST:*mp_vecServerModuleSyncTasks){
    if(!pMST->execute()){
      printWarning("MFNetworkModule::executeModule() - sth went wong");
      continue;
    }

    std::vector<MFStructurableData*>*
    pStreamingBuffers=pMST->takeDataUpdateBuffers();
    for(MFNetClient* pClients:*mp_vecServerClients){
      for(MFStructurableData* pSyncData:*pStreamingBuffers){
        if(mp_ticker->stop()>=10.0f){//every 200ms
          if(!pClients->sendData(
              (uint8_t*)pSyncData->getStructureData(),
              pSyncData->getStructureSize(),
              m_syncDataChannel,
              true)){
            printErr("MFNetworkModule::executeModule - "
                "pNC->sendData(...) failed to send sync data!!");
          }
          mp_ticker->start();
        }
        //TODO "send flag with no allocate" is supported by enet! go into
        //sendData(...) and change the enet flag, do not return data until
        //data was sent successfully!
        //TODO check if sendData waits till data was recived by client
      }
      break;
    }
    for(MFStructurableData* pData:*pStreamingBuffers){
      pMST->retrunDataSyncBuffer(pData);
    }
    pStreamingBuffers->clear();
  }

  synchronizeNotes();

  return true;
}

void MFNetworkModule::synchronizeNotes(){
  if(!m_syncNotes)
    return;
  m_syncNotes=false;

  if(mp_netClientRes->mp_netTaskManager!=nullptr && mp_netClientRes->mp_actionManager!=nullptr){
    MFNetActionTask* pNetAction=getNetActionTask(mp_netClientRes->mp_netTaskManager);
    MFBaseActionTask* pAction=mp_netClientRes->mp_actionManager->getAddObjectAction();
    if(!pNetAction->addAction(pAction)){
      MFObject::printWarning("MFNetworkModule::synchronizeNotes - failed to add action to MFNetActionTask!");
      return;
    }
    if(!pNetAction->prepareOutputPackage()){
      MFObject::printWarning("MFNetworkModule::synchronizeNotes - failed to prepare output package!");
      return;
    }
    for(MFNetClient* pClient:*mp_vecClients){
      if(!pNetAction->sendPackage(pClient)){
        MFObject::printErr("MFNetworkModule::synchronizeNotes - failed to send add object action!");
      }
    }
  }

  if(mp_netServerRes->mp_netTaskManager!=nullptr && mp_netServerRes->mp_actionManager!=nullptr){
    MFNetActionTask* pNetAction=getNetActionTask(mp_netServerRes->mp_netTaskManager);
    if(!pNetAction->addAction(mp_netServerRes->mp_actionManager->getAddObjectAction())){
      MFObject::printWarning("MFNetworkModule::synchronizeNotes - failed to add action to MFNetActionTask!");
      return;
    }
    if(!pNetAction->prepareOutputPackage()){
      MFObject::printWarning("MFNetworkModule::synchronizeNotes - failed to prepare output package!");
      return;
    }
    for(MFNetClient* pClient:*mp_vecServerClients){
      if(!pNetAction->sendPackage(pClient)){
        MFObject::printErr("MFNetworkModule::synchronizeNotes - failed to send add object action!");
      }
    }
  }

}

void MFNetworkModule::addServerModuleSync(MFModuleSyncTask* pTask){
  mp_vecServerModuleSyncTasks->push_back(pTask);
  pTask->enableDataUpdate(true);
  pTask->enableModuleUpdate(true);
}

bool MFNetworkModule::syncOutputData(){
  //TODO get data from each client (at least data for orientation of player model)
  return true;
}

MFNetClient* MFNetworkModule::connect(
    std::string address,
    uint16_t port,
    uint32_t timeout,
    MFTaskThread* pConnectThread,
    bool useClientThread){
  //TODO implement for multiple connections
  //maybe subclass of MFNetClient which contains MFNetworkResources and is used for
  //mp_netServerRes->mp_objectManager->addNetworkNotifier(this); -> for client too
  /*Client resources*/
  /*This module manager is used for client network operations*/
  if(mp_netClientRes->mp_moduleProvider==nullptr){
    MFModuleManager *pNetModuleManager=new MFModuleManager(mp_syncRes);
    pNetModuleManager->m_printIndex=m_printIndex;
    pNetModuleManager->setSetupName("ModMgr-NetworkModuleClient");
    pNetModuleManager->initModuleCreators();
    //mp_netClientRes will be used during connect() function call. It will be set
    //as the clients network resources.
    mp_netClientRes->m_printIndex=m_printIndex;
    mp_netClientRes->mp_syncObjectRes=mp_syncRes;
    mp_netClientRes->mp_clientCtl=this;
    mp_netClientRes->mp_vecSyncTasks=mp_syncRes->getVecSyncTasks();

    mp_netClientRes->mp_moduleProvider=pNetModuleManager;
    mp_netClientRes->mp_groupProvider=pNetModuleManager;
    mp_netClientRes->mp_moduleSetup=pNetModuleManager;
    mp_netClientRes->mp_loopCtl=pNetModuleManager;
    mp_netClientRes->mp_actionManager=pNetModuleManager->getActionManager();
    mp_netClientRes->mp_objectManager=pNetModuleManager->getObjectManager();
    mp_netClientRes->mp_objectManager->addNetworkNotifier(this);

    mp_netClientRes->initDispatchTasks();
    mp_netClientRes->initChannelSinks();

    if(mp_netClientRes->mp_netTaskManager == nullptr){
      mp_netClientRes->mp_netTaskManager=new MFNetTaskManager(mp_netClientRes);
      mp_netClientRes->mp_netTaskManager->m_printIndex=m_printIndex;
      mp_netClientRes->mp_netTaskManager->initTaskManager();
    }
  }

  MFNetClient* pClient=new MFNetClient(8);
  pClient->initEnetInstance();
  /*set the task which dispatches incomming data*/
  pClient->setReceiveDispatchTask(mp_netClientRes->mp_dataDispatchTask);//one dispatch task for every client?
  pClient->m_printIndex=m_printIndex;
  //	pClient->setTaskThread(new MFTaskThread("TClientThread"), true);
  mp_vecClients->push_back(pClient);//TODO what if connect fails?
  //TODO important: what if cnct fails but the client will be notified -> mem leak

  MFNetConnectTask *taskConnectClient=new MFNetConnectTask(this);
  taskConnectClient->m_printIndex=m_printIndex;
  taskConnectClient->addConnectRequest(address, port, pClient);
  taskConnectClient->m_timeout=timeout;
  taskConnectClient->deleteAfterExecution(true);//verbuddel die leiche
  if(pConnectThread!=nullptr){
    pConnectThread->addTask(taskConnectClient);
  }else if(useClientThread){
    pClient->addTask(taskConnectClient);/*if connect was successfull, addClient(..) will be called*/
  }else{
    if(!taskConnectClient->execute()){/*if connect was successfull, addClient(..) will be called*/
      printErr("MFNetworkModule::connect - Failed to connect!");
      pClient->printInformations();
      return pClient;
    }
    delete taskConnectClient;
  }
  return pClient;
}

bool MFNetworkModule::disconnect(
    MFNetClient* pClient,
    uint32_t timeout,
    MFTaskThread* pDisconnectThread){
  //TODO test if disconnecting is thread safe for clients and server-clients.
  //TODO MFNetworkModule::createServerimplement disc.Thread
  return pClient->disconnect(timeout);
}

bool MFNetworkModule::createServer(
    uint32_t clientsCount,
    uint16_t port,
    void* pModuleManager){
  //TODO implement multi server -> vector<MFNetworkResources>
  //maybe subclass of MFServerClientInstance which contains MFNetworkResources and is used for
  //mp_netServerRes->mp_objectManager->addNetworkNotifier(this); -> for client too
  /*Server resources*/
  mp_serverInstance=new MFServerClientInstance();
  mp_serverInstance->m_printIndex=m_printIndex;
  mp_vecServerClients=mp_serverInstance->getVecConnectedClients();
  mp_networkThread=mp_serverInstance;

  //  mp_serverInstance->setTaskThread(mp_networkThread, true);
  MFModuleManager *pNetModuleManager=(MFModuleManager*)pModuleManager;
  pNetModuleManager->m_printIndex=m_printIndex;

  if(pNetModuleManager==nullptr){
    pNetModuleManager=(MFModuleManager*)MFStaticSetupData::smp_moduleManager;
  }

  if(pNetModuleManager==nullptr){
    printErr("MFNetworkModule::createServer - "
        "");
    return false;
  }

  mp_netServerRes->m_printIndex=m_printIndex;
  mp_netServerRes->mp_clientCtl=this;
  mp_netServerRes->mp_syncObjectRes=pNetModuleManager->getSyncObjectResources();
  mp_netServerRes->mp_moduleProvider=pNetModuleManager;
  mp_netServerRes->mp_groupProvider=pNetModuleManager;
  mp_netServerRes->mp_moduleSetup=pNetModuleManager;
  mp_netServerRes->mp_loopCtl=pNetModuleManager;
  mp_netServerRes->mp_vecSyncTasks=mp_netServerRes->mp_syncObjectRes->getVecSyncTasks();
  mp_netServerRes->mp_actionManager=pNetModuleManager->getActionManager();
  mp_netServerRes->mp_objectManager=pNetModuleManager->getObjectManager();
  mp_netServerRes->mp_objectManager->addNetworkNotifier(this);
  mp_netServerRes->initDispatchTasks();
  mp_netServerRes->initChannelSinks();

  if(mp_netServerRes->mp_netTaskManager == nullptr){
    mp_netServerRes->mp_netTaskManager=new MFNetTaskManager(mp_netServerRes);
    mp_netServerRes->mp_netTaskManager->m_printIndex=m_printIndex;
    mp_netServerRes->mp_netTaskManager->initTaskManager();
  }

  m_syncDataChannel=1;

  if(mp_serverInstance==nullptr){
    mp_serverInstance=new MFServerClientInstance();
    mp_serverInstance->m_printIndex=m_printIndex;
    mp_vecServerClients=mp_serverInstance->getVecConnectedClients();
    mp_networkThread=mp_serverInstance;
    mp_serverInstance->setTaskThread(mp_networkThread, true);
  }
  mp_serverInstance->setPort(port);
  if(mp_serverInstance->isInitialized()){
    printInfo("MFNetworkModule::createServer - "
        "server already initialized!");
    return true;
  }
  mp_serverInstance->setChannelCount(8);
  mp_serverInstance->setConnectDispatchTask(mp_netServerRes->mp_connectDispatchTask);
  mp_serverInstance->setDisconnectDispatchTask(mp_netServerRes->mp_disconnectDispatchTask);
  mp_serverInstance->setReceiveDispatchTask(mp_netServerRes->mp_dataDispatchTask);
  bool ret=mp_serverInstance->initEnetInstance();
  mp_vecServerClients=mp_serverInstance->getVecConnectedClients();
  return ret;
}

