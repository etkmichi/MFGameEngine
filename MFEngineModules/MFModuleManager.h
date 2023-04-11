/*
 * MFModuleManager.h
 *
 *  Created on: 04.11.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULEMANAGER_H_
#define MFENGINEMODULES_MFMODULEMANAGER_H_

#include <MFIGUISetup.h>
#include <MFCallback/MFICallback.h>
#include <MFPrinters/MFPrintSetup.h>

#include "MFNetworkModules/MFNetTaskManager.h"

#include "MFModuleGroupManager.h"
#include "MFBaseModule.h"

#include "MFInputModules/MFInputModule.h"
#include "ModulePhysics/MFPhysicsModule.h"
#include "ModuleRender/MFRenderModule.h"
#include "MFGUIIOModule/MFGUIIOModule.h"
#include "MFStateModule/MFStateModule.h"
#include "MFWorldModule/MFWorldModule.h"
#include "MFNetworkModules/MFNetworkModule.h"
#include "ModuleRecycling/MFRecyclingModule.h"

#include "MFInterfacesModules/MFIModuleCreator.h"
#include "MFInterfacesModules/MFIManagerControl.h"
#include "MFInterfacesModules/MFIEngineLoopControl.h"
#include "MFInterfacesModules/MFIModuleCreator.h"

#include "MFNetworkModules/MFNetTaskManager.h"


#include "../MFSynchronisationClasses/MFSyncObject.h"
#include "../MFSynchronisationClasses/MFSyncObjectResources.h"

#include "../MFSceneActions/MFActionResources.h"
#include "../MFSceneActions/MFActionTaskManager.h"

#include "../MFEngineTasks/MFEngineLoopTask.h"

#include "../MFEngineStructs.h"
#include "../MFModuleResources.h"

#include "MFModuleObjectManager.h"
#include "ModulePlayer/MFPlayerModule.h"

/**
 *
 * Structured data setup:
 * Header: 4 bytes for module counter (uint32_t)
 * Body: N x Module meta info (8byte type + 1 byte group)
 * TODO no type id, only module index
 * TODO add external module and external module execution. f.e. for render module
 * which shall render a scene but should not be handled like a normal module
 */
class MFModuleManager :
    public MFStructurableDataSetup,
    public MFIManagerControl,
    public MFIEngineLoopControl,
    public MFIGUISetup,
    public MFICallback,
    public MFPrintSetup,
    public MFModuleGroupManager,
    private MFModuleResources {
public:
  static const uint32_t
    SWITCH_GROUP=0,
    SWITCH_ADD_MODULE=1;
private:
  bool
  m_isInitialized,
  m_internalStop=false,
  m_isStarted=false,
  *mp_externalStop;

  std::mutex
  lockCycleCounter;

  uint32_t
  m_cycleCounter=0;

  MFDataObject
  *mp_moduleCounter=nullptr;

  std::vector<MFBaseModule*>
  *mp_vecModules,/*for static setup*/
  *mp_vecDynamicModules;/*for changing setup*/

  std::vector<MFBaseModule*>
  *mp_vecMainRenderModule,
  *mp_vecMainInputModule,
  *mp_vecMainPhysicsModule,
  *mp_vecMainGUIModule,
  *mp_vecWorldModule,
  *mp_vecNetworkModules,
  *mp_vecPlayerModules;


  MFStateModule
  *mp_stateModule;

  std::vector<MFIModuleCreator*>
  *mp_vecModuleCreators;

  MFIModuleCreator
  *mp_failorModuleCreator,
  *mp_renderModuleCreator,
  *mp_physicsModuleCreator,
  *mp_inputModuleCreator,
  *mp_GUIModuleCreator,
  *mp_worldModuleCreator,
  *mp_networkModuleCreator,
  *mp_playerModuleCreator;

  MFRecyclingModule
  *pRecyclingModule=nullptr;

  MFActionTaskManager
  *mp_deletableActionManager=nullptr,
  *mp_actionManager;

  MFModuleObjectManager
  *mp_deletableObjectManager=nullptr,
  *mp_objectManager;

  GLFWwindow
  *mp_window;

  MFModuleResources
  *mp_res;
  MFSyncObjectResources
  *mp_syncRes;
  MFActionResources
  *mp_actRes;

  MFTaskThread
  *mp_externalLooper;
  MFEngineLoopTask
  *mp_loopTask;

  /*GUI*/
  MFIMenu
  *mp_rootMenu,
  *mp_menuAddGroup;

  std::vector<MFIMenu*>
  *mp_vecGroupMenus;

  std::mutex
  lockVecModules;

protected:
  /**
   * Updates the manager setup, which is used for serialization.
   * @param addedModule - module which was added to the engine
   * @param moduleGroup - group of the module
   */
  void updateManagerSetup(MFBaseModule* addedModule,uint32_t moduleGroup=0);
  void initNextModuleCreator(MFIModuleCreator* pModuleCreator);


public: /*virtual functions of MFIFunctionCallback*/
  /**
   * Implementation for gui.
   * @param switchVal - index into module creators
   * @return
   */
   virtual bool callback(uint32_t switchVal=0,void* userData=nullptr);

public:/* virtual functions of MFIGUISetup*/
  /**
   * The module managers gui:
   * - List of added modules as sub menu buttons (click to edit)
   * - Add Module (on top)
   * sub menu.
   * @param pRootMenu
   * @return
   */
  virtual bool setupGUI(MFIMenu *pRootMenu);

public: /* virtual functions of MFIEngineLoopControl */
  virtual bool executeModuleLoop(bool* externalClose);
  virtual bool executeModuleStep();
  /**
   * This task will start the module loop on an external thread. If the module manager
   * wasn't successfully initialized, it will also scedule an initializing task before
   * starting the loop.
   * @param externalClose
   * @param externalThread
   * @return
   */
  virtual bool executeExternalModuleLoop(
      bool* externalClose,
      MFTaskThread* externalThread=nullptr);
  virtual MFTaskThread* getExternalLooperThread(){return mp_externalLooper;};
  virtual bool stopModuleLoop();
  virtual bool stopExternalModuleLoop();
  virtual void setExternalClose(bool* externalClose){mp_externalStop=externalClose;};
  virtual bool isStarted(){return m_isStarted;};

  virtual uint32_t getCurrentCycle(){
    lockCycleCounter.lock();
    uint32_t cycle=m_cycleCounter++;
    lockCycleCounter.unlock();
    return cycle;
  };

  virtual void setCurrentCycle(uint32_t cycle){m_cycleCounter=cycle;};

public:/*MFStructurableDataSetup functions:*/
  virtual void initData();
  virtual void setupDataStructure();

public:/*MFIManagerControl functions:*/
  virtual bool initFromSetup(MFStructurableData* pData=nullptr);
  virtual bool initFromData(MFDataObject* pData);
  virtual MFBaseModule* getModule(uint32_t index){return mp_vecModules->at(index);};
  virtual std::vector<MFBaseModule*>* getVecModules(){return mp_vecModules;};
  virtual MFModuleObjectManager* getObjectManager(){return mp_objectManager;};
  virtual void setObjectManager(MFModuleObjectManager* pObjectMgr){mp_objectManager=pObjectMgr;};
  virtual MFActionTaskManager* getActionManager(){return mp_actionManager;};
  virtual void setActionManager(MFActionTaskManager* pActMgr){mp_actionManager=pActMgr;};
  virtual bool initModules(){
    printWarning("MFModuleManager::initModules - no implementation!");
    return true;
  }

public:
  /**
   *
   * @param pSyncRes - Can be specified to share the sync resources with an already
   * created MFModuleManager
   */
  MFModuleManager(MFSyncObjectResources *pSyncRes=nullptr);
  virtual ~MFModuleManager();

  /**
   * Initializes the module creators, which are needed to create modules from
   * serialized data (with the function addModule(uint32_t moduleCreatorIndex...).
   */
  virtual void initModuleCreators();

  /**
   * A default module will be added to the module group with index 0.
   * @param defaultModule
   * @param addAllObjects
   */
  void addDefaultModule(
      MFBaseModule* defaultModule,
      bool addAllObjects);

  bool initModuleManager();
  bool initRecyclingModule();

  MFSyncObjectResources* getSyncObjectResources(){return mp_syncRes;};
  void registerModuleCreator(MFIModuleCreator* pModuleCreator);

  MFBaseModule* addModule(
      uint32_t moduleCreatorIndex,
      uint32_t moduleGroup,
      bool initModule=true);

  void addModule(
      MFBaseModule* pModule,
      uint32_t moduleGroup,
      bool initModule=true);

  void addModule(
      MFBaseModule* pModule,
      bool initModule=true);

  /**
   * Module for dynamic usage. No network sync implemented, dynamic modules must
   * be hardcoded on server and client side if synchronization is needed.
   * @param pModule
   * @param moduleGroup
   * @param initModule
   */
  void addDynamicModule(
      MFBaseModule* pModule,
      uint32_t moduleGroup,
      bool initModule=true);

  void setRecycleManager(MFIRecycleManager* pRecycleManager){
    mp_syncRes->pRecycleManager=pRecycleManager;
  }

  MFGUIIOModule* addGUIModule(uint32_t moduleGroup, bool initModule=true);

  /**
   *
   * @param pWindow the glfw window for input dispatching. If nullptr the default window
   * will be used. If the default window is not set, nullptr will be returned.
   * @param addAllSyncObjects - adds this module as default module and adds all existing
   * objects to it. As default module, all new scene objects will be added in futur.
   * @return nullptr if no window is available (pWindow==nullptr && mp_window==nullptr)
   */
  MFInputModule* addInputModule(
      GLFWwindow* pWindow,
      uint32_t moduleGroup,
      bool initModule=true);

  /**
   *
   * @param addAllSyncObjects - adds this module as default module and adds all existing
   * objects to it. As default module, all new scene objects will be added in futur.
   * @return the physics module or nullptr if sth fails
   *///TODO fix objec sync, if render module will be updated after physics mod,
  //object positions will be wrong!!
  MFPhysicsModule* addPhysicModule(
      uint32_t moduleGroup,
      bool initModule=true);

  /**
   * Creates a render module and adds it to the inner module dispatch. If the main render
   * module is null, it will be set to the newly created render module. If the
   * added module is the main render module, the default window will be the modules
   * window.
   * @param addAllSyncObjects - adds this module as default module and adds all existing
   * objects to it. As default module, all new scene objects will be added in futur.
   * @param windowTitle
   * @param dataPath
   * @param compilePath
   * @return
   */
  MFRenderModule* addRenderModule(
      uint32_t moduleGroup,
      std::string windowTitle,
      std::string dataPath,
      std::string compilePath,
      bool initModule=true);

  MFWorldModule* addWorldModule(uint32_t moduleGroup,bool initModule=true);

  MFNetworkModule* addNetworkModule(uint32_t moduleGroup,bool initModule=true);

  MFPlayerModule* addPlayerModule(uint32_t moduleGroup,bool initModule=true);

  std::vector<MFBaseModule*>* getMainInputModule(){return mp_vecMainInputModule;};

  std::vector<MFBaseModule*>* getMainPhysicsModule(){return mp_vecMainPhysicsModule;};

  std::vector<MFBaseModule*>* getMainRenderModule(){return mp_vecMainRenderModule;};

  void setWindow(GLFWwindow* pWindow){mp_window=pWindow;};

//  bool connect(uint32_t netModuleIndex,std::string address,uint16_t port);
};

#endif /* MFENGINEMODULES_MFMODULEMANAGER_H_ */
