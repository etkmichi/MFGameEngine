/*
 * MFGameMenu.h
 *
 *  Created on: 18.08.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFGAMEMENU_H_
#define MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFGAMEMENU_H_

#include "MFMainSetup.h"
#include "../../MFModuleManager.h"
#include "../../MFNetworkModules/MFNetworkModule.h"
#include <MFCallback/MFICallback.h>
#include "../../MFModuleGroup.h"
/**
 *
 */
class MFGameMenu: public MFMainSetup,public MFICallback {
public:
  static const uint32_t
  SWITCH_START_LOCAL=0,
  SWITCH_START_CONNECT=1,
  SWITCH_START_SERVER=2,
  SWITCH_STOP_SERVER=3;
private:
  MFNetworkModule
  *mp_networkModule;

  MFModuleGroup
  *mp_groupIDNetwork=0;
  bool
  m_stopClient=false;

  MFIMenu
  *mp_gameMenu,
  *mp_createLocalGame,
  *mp_createServer,
  *mp_connectServer;

  std::vector<bool>
  m_vecExitValues;

  /*Server*/
  MFModuleManager
  *mp_serverModuleManager,
  *mp_clientManager,
  *mp_localManager;

  bool
  m_serverExit=false,
  m_clientExit=false;

  std::string
  m_clientIP;

  uint16_t
  m_clientPort=42421;

  MFDataObject
  *mp_clientIP,
  *mp_clientPort;
public: /*virtual functions of MFIFunctionCallback*/
  virtual bool callback(uint32_t switchVal=0,void* userData=nullptr);

public:/* virtual functions of MFIGUISetup*/
  virtual bool setupGUI(MFIMenu *pRootMenu);

public:
  MFGameMenu(
      MFModuleManager* pSinglePlayer,
      MFModuleManager* pClientManager,
      MFModuleManager* pServer);

  virtual ~MFGameMenu();
};

#endif /* MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFGAMEMENU_H_ */
