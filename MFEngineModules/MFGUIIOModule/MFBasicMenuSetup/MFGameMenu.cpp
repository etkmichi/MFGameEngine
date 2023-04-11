/*
 * MFGameMenu.cpp
 *
 *  Created on: 18.08.2020
 *      Author: michl
 */

#include "MFGameMenu.h"
#include <MFThreadSystem/MFTaskThread.h>

MFGameMenu::MFGameMenu(
    MFModuleManager* pSinglePlayer,
    MFModuleManager* pClientManager,
    MFModuleManager* pServer
) {
  mp_serverModuleManager=pServer;
  mp_clientManager=pClientManager;
  mp_localManager=pSinglePlayer;
  mp_gameMenu=nullptr;
  mp_createLocalGame=nullptr;
  mp_createServer=nullptr;
  mp_connectServer=nullptr;

  m_clientIP="127.000.000.001";
  mp_clientIP=new MFDataObject();
  mp_clientIP->setTypes(
      E_DataType::DATA_TYPE_STRING,
      E_GuiElement::GUI_TYPE_IPV4_EDIT);
  mp_clientIP->setDataByteSize(m_clientIP.size());
  mp_clientIP->writeData(m_clientIP.data());
  mp_clientIP->setObjectName("IPv4 GUI input");


  mp_clientPort=new MFDataObject();
  mp_clientPort->setTypes(
      E_DataType::DATA_TYPE_UINT16,
      E_GuiElement::GUI_TYPE_PORT_EDIT);
  mp_clientPort->setData(&m_clientPort);

  mp_networkModule=nullptr;
}

MFGameMenu::~MFGameMenu() {
  // TODO Auto-generated destructor stub
}

bool MFGameMenu::callback(uint32_t switchVal,void* userData){
  bool ret=true;
  switch (switchVal){
  case MFGameMenu::SWITCH_START_LOCAL:
    /*Add bool button for exiting the single player loop*/
//    uint32_t count=m_vecExitValues.size()+1;
//    m_vecExitValues.resize(count);
//    bool* pExit=&(m_vecExitValues.data()[count-1]);
//    MFDataObject* pExitButtonData=new MFDataObject();
//    pExitButtonData->setDataType(E_DataType::DATA_TYPE_BOOL);
//    pExitButtonData->setGuiType(E_GuiElement::GUI_TYPE_BUTTON);
//    pExitButtonData->setData(pExit);
//    std::string title="Exit player"+std::to_string(count-1);
//    mp_createLocalGame->addEntry(title, pExitButtonData);
//    //TODO add render module if another local is added
//    mp_localManager->executeExternalModuleLoop(pExit);
    break;
  case MFGameMenu::SWITCH_START_CONNECT:
    /*Add bool button for exiting the single player loop*/
    if(!mp_clientManager->isStarted()){
//      ret=mp_clientManager->initModuleManager();
      MFSyncObjectResources* pSyncRes=new MFSyncObjectResources();
//      mp_groupIDNetwork=mp_clientManager->addModuleGroup("NetworkGroup");
      mp_networkModule=new MFNetworkModule(pSyncRes);
//          mp_clientManager->addNetworkModule(
//          mp_groupIDNetwork->groupIndex,
//          true);
      mp_networkModule->initModule();/*sets mp_loopCtl*/

      m_clientIP=std::string(
          static_cast<char*>(mp_clientIP->getData()),
          mp_clientIP->getDataSize());
      mp_networkModule->connect(
          m_clientIP,
          m_clientPort,
          5000,
          new MFTaskThread("MFClientLooper", true),
          false);
      mp_networkModule->getClientResources()->mp_loopCtl->setExternalClose(
          &m_clientExit);
//      mp_clientManager->executeExternalModuleLoop(&m_clientExit);
    }
    break;
  case MFGameMenu::SWITCH_START_SERVER:
    m_serverExit=false;
    if(!mp_serverModuleManager->isStarted()){
      mp_serverModuleManager->executeExternalModuleLoop(&m_serverExit);
    }
    break;
  case MFGameMenu::SWITCH_STOP_SERVER:
    m_serverExit=true;
    if(mp_serverModuleManager->isStarted()){
      mp_serverModuleManager->stopExternalModuleLoop();
    }
    break;
  }
  return ret;
}

bool MFGameMenu::setupGUI(MFIMenu *pRootMenu){
  mp_gameMenu= pRootMenu->addSubMenu("Game");
//  mp_gameMenu->addEntry("Game setup", nullptr);
  /*Create game contains a button for starting a pre defined MFModuleManager loop*/
  mp_createLocalGame=mp_gameMenu->addSubMenu("Create Game");

  /*the connect menu contains ip,port edits and a connenct button*/
  mp_connectServer=mp_gameMenu->addSubMenu("Connect to Server");
  mp_connectServer->addEntry("IPv4", mp_clientIP);
  mp_connectServer->addEntry("Port:", mp_clientPort);
  mp_connectServer->addFunctionCallback(this, "Connect", SWITCH_START_CONNECT);

  mp_createServer =mp_gameMenu->addSubMenu("Create Server");
  mp_createServer->addFunctionCallback(this, "Start Server", SWITCH_START_SERVER);
  mp_createServer->addFunctionCallback(this, "Stop Server", SWITCH_STOP_SERVER);

  bool ret=true;
  for(MFIGUISetup* pSubSetup:*mp_vecSubSetups){
    if(pSubSetup!=nullptr){
      /*Use the mp_optionsMenu for sub menus of Option-Menu*/
      ret&=pSubSetup->setupGUI(mp_gameMenu);
    }
  }
  return ret;
}
