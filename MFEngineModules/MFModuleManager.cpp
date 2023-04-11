/*
 * MFModuleManager.cpp
 *
 *  Created on: 04.11.2019
 *      Author: michl
 */

#include "MFModuleManager.h"
#include <MFDataDeleterClasses/MFDataDeletion.h>
#include "MFNetworkModules/MFNetworkTasks/MFNetworkBaseTask.h"
MFModuleManager::MFModuleManager(MFSyncObjectResources *pSyncRes){
//  MFDataDeletion::initStaticDataDeletion();
  if(pSyncRes==nullptr)
    mp_syncRes=new MFSyncObjectResources();
  else
    mp_syncRes=pSyncRes;
  setSyncResources(pSyncRes);
  MFStaticSetupData::sLockStaticSetupData.lock();
  if(MFStaticSetupData::smp_syncRes==nullptr){
    MFStaticSetupData::smp_syncRes=mp_syncRes;
  }

  mp_syncRes->mp_loopCtl=this;//TODO another MFModMgr object will overwrite this!

  MFStaticSetupData::sLockStaticSetupData.unlock();
  mp_externalLooper=new MFTaskThread("TExternalLooper",false);
  mp_loopTask=new MFEngineLoopTask(this,nullptr);
  mp_externalLooper->enableTaskRepeat(true);

  mp_res=new MFModuleResources();
  mp_vecModules = new std::vector<MFBaseModule*>();
  mp_vecDynamicModules=new std::vector<MFBaseModule*>();

  mp_vecMainRenderModule=new std::vector<MFBaseModule*>();
  mp_vecMainInputModule=new std::vector<MFBaseModule*>();
  mp_vecMainPhysicsModule=new std::vector<MFBaseModule*>();
  mp_vecMainGUIModule=new std::vector<MFBaseModule*>();
  mp_vecWorldModule=new std::vector<MFBaseModule*>();
  mp_vecNetworkModules=new std::vector<MFBaseModule*>();
  mp_vecPlayerModules=new std::vector<MFBaseModule*>();

  mp_window=nullptr;
  mp_objectManager=new MFModuleObjectManager(mp_syncRes);
  mp_res->lockRes.lock();
  mp_res->addUser();
  if(mp_res->mp_vecCamObjects==nullptr)
    mp_res->mp_vecCamObjects=new std::vector<MFSyncObject*>();
  mp_res->m_deletionCounter++;
  mp_res->lockRes.unlock();
  mp_vecModuleCreators=new std::vector<MFIModuleCreator*>();
  mp_stateModule=new MFStateModule(mp_syncRes);
  mp_actRes=new MFActionResources();
  mp_actRes->m_printIndex=0;
  mp_actRes->mp_geometryManager=mp_objectManager->getGeometryManager();
  mp_actRes->mp_groupProvider=this;
  mp_actRes->mp_loopCtl=this;
  mp_actRes->mp_objectManager=mp_objectManager;
  mp_actRes->mp_syncObjectRes=mp_syncRes;
  mp_actionManager=new MFActionTaskManager(mp_actRes);
  mp_actRes->mp_actionManager=mp_actionManager;

  mp_failorModuleCreator=nullptr;
  mp_renderModuleCreator=nullptr;
  mp_physicsModuleCreator=nullptr;
  mp_inputModuleCreator=nullptr;
  mp_GUIModuleCreator=nullptr;
  mp_worldModuleCreator=nullptr;
  mp_networkModuleCreator=nullptr;
  mp_playerModuleCreator=nullptr;
  m_isInitialized=false;
  mp_externalStop=&m_internalStop;
  mp_vecGroupMenus=new std::vector<MFIMenu*>();
  mp_rootMenu=nullptr;
  mp_menuAddGroup=nullptr;
  mp_moduleCounter=nullptr;
}

MFModuleManager::~MFModuleManager(){
  delete mp_vecMainRenderModule;
  delete mp_vecMainInputModule;
  delete mp_vecMainPhysicsModule;
  delete mp_vecMainGUIModule;
  delete mp_vecNetworkModules;
  delete mp_vecPlayerModules;
  delete mp_vecWorldModule;
  delete mp_vecModules;
  delete mp_stateModule;
  delete mp_objectManager;
  delete mp_vecModuleCreators;
  delete mp_renderModuleCreator;
  delete mp_physicsModuleCreator;
  delete mp_inputModuleCreator;
  delete mp_GUIModuleCreator;
  delete mp_worldModuleCreator;
  delete mp_networkModuleCreator;
  mp_res->lockRes.lock();
  if(mp_res->m_deletionCounter==0){
    delete mp_res->mp_vecCamObjects;
    delete mp_res;
  }
  lockVecGroups.lock();
  for(MFModuleGroup* pModG:*mp_vecModuleGroups){
    for(MFBaseModule* pMod:pModG->vecModules){
      delete pMod;
    }
    delete pModG;
  }
}

bool MFModuleManager::callback(uint32_t switchVal,void* userData){
  switch(switchVal){
  case MFModuleManager::SWITCH_GROUP : {
    MFDataObject* pData=static_cast<MFDataObject*>(mp_menuAddGroup->getUserData());
    std::string groupName=std::string((char*)pData->getData());
    addModuleGroup(groupName);
    mp_rootMenu->addSubMenu(groupName);//TODO create a thread safe addSubMenu
    //TODO add to mp_vecGroupMenus
    break;
  }
  case MFModuleManager::SWITCH_ADD_MODULE : {
    break;
  }

  }
  //  MFBaseModule *pM=nullptr;
  //  if(switchVal<mp_vecModuleCreators->size()){
  //    pM=mp_vecModuleCreators->at(switchVal)->createNewModule(mp_res);
  //    addModule
  //  }
  return true;
}

bool MFModuleManager::setupGUI(MFIMenu *pRootMenu){
  mp_rootMenu=pRootMenu;

  /*Menu to add a new group*/
  mp_menuAddGroup=pRootMenu->addSubMenu("Add group");
  MFDataObject* pNextGroup=new MFDataObject();
  pNextGroup->setGuiType(E_GuiElement::GUI_TYPE_TEXT_EDIT);
  pNextGroup->setDataType(E_DataType::DATA_TYPE_STRING);
  mp_menuAddGroup->addEntry("Group name:", pNextGroup);
  mp_menuAddGroup->setUserData(pNextGroup);
  mp_menuAddGroup->addFunctionCallback(this, "Add", SWITCH_GROUP);

  /*Menu for each existing group with a add module function*/
  for(MFModuleGroup* pG:*mp_vecModuleGroups) {
    MFIMenu* pGroupMenu=pRootMenu->addSubMenu(pG->groupName);
    pGroupMenu->setUserData(pG);
    MFIMenu* pAddModuleMenu=pGroupMenu->addSubMenu("Add module");
    uint32_t index=0;
    for(MFIModuleCreator* pMC:*mp_vecModuleCreators){
      //TODO module creator wird in MFModuleManager::functionCallback benötigt!
      pAddModuleMenu->addFunctionCallback(this, pMC->getName(), SWITCH_ADD_MODULE);
      pAddModuleMenu->setUserData(pG);
      index++;
    }
    pAddModuleMenu->setUserData(pG);
  }
  return false;
}

void MFModuleManager::addDefaultModule(
    MFBaseModule* defaultModule,
    bool addAllObjects){
  lockVecGroups.lock();
  mp_vecModuleGroups->at(0)->vecModules.push_back(defaultModule);
  if(addAllObjects){
    for(MFSyncObject* pO:mp_vecModuleGroups->at(0)->vecSyncObjects){
      defaultModule->addSyncObject(pO);
    }
  }
  lockVecGroups.unlock();
}

bool MFModuleManager::initFromSetup(MFStructurableData* pData){
  if(pData!=nullptr)loadSetup(pData);
  for(MFDataObject *pDO:*getVecData()){
    printInfo(""+std::to_string(pDO->getDataSize()));
  }
  return true;
}

bool MFModuleManager::initFromData(MFDataObject* pData){
  /* *
   * data must be formated in MFModuleManager setup format:
   * 	header: 2 byte module counter -> see setupDataStructure()
   * 	body: N x (8+1)bytes all added modules meta info's -> see updateManagerSetup()
   * */

  setupDataStructure();
  initModuleCreators();

  float moduleCount=(pData->getDataSize()-4)/8;
  uint32_t storedCount=*((uint32_t*)pData->getData());
  printInfo("MFModuleManager::initFromData - "
      "dataSize | (dataSize-2)/moduleInitSize | module count:\n"+
      std::to_string(pData->getDataSize())+" | "+
      std::to_string(moduleCount) + " | "+
      std::to_string(storedCount));
  if(storedCount>10){
    printWarning("MFModuleManager::initFromData - "
        "more than 10 modules!?! "
        "if y, change code in MFModuleManager and recompile...");
  }
  uint32_t *creatorIndex=((uint32_t*)pData->getData())+1;
  uint32_t *moduleGroup=((uint32_t*)pData->getData())+2;
  printInfo("module count: "+std::to_string(storedCount));
  if(mp_moduleCounter==nullptr)mp_moduleCounter=addData((uint32_t*)nullptr);
  mp_moduleCounter->writeData(&storedCount);
  for(uint32_t i=0;i<storedCount;i++){
    printInfo("module data\n"
        "creator index: "+std::to_string(*creatorIndex)+"\n"
        "module group: "+std::to_string(*moduleGroup));
    if(! addModule(*creatorIndex, *moduleGroup, true)){
      printWarning("MFModuleManager::initFromData - failed to add module with"
          "creatorIndex/moduleGroup: "+
          std::to_string(*creatorIndex)+"/"+
          std::to_string(*moduleGroup));
    }
    //TODO init module
    creatorIndex+=2;
    moduleGroup+=2;
  }
  return true;
}

bool MFModuleManager::initModuleManager(){
  bool ret=true;
  if(mp_syncRes->mp_logWindow==nullptr){
//    P_INF("currently no log window support!")
//    mp_syncRes->mp_logWindow=
//        MFModuleResources::smp_guiManager->addPrinterWindow(getSetupName());
  }

  setupDataStructure();

  initModuleCreators();

  mp_actionManager->initTaskManager();
  setSyncResources(mp_syncRes);
  if(!m_isInitialized){
    ret&=initModules();
    m_isInitialized=ret;
  }

  return ret;
}

void MFModuleManager::updateManagerSetup(MFBaseModule* addedModule,uint32_t moduleGroup){
  MFDataObject* pDO0=add32BitValue(E_DataType::DATA_TYPE_UINT32, nullptr);//Module type id
  MFDataObject* pDO1=add32BitValue(E_DataType::DATA_TYPE_UINT32, nullptr);//module group
  uint32_t typeID=addedModule->getModuleTypeID();
  pDO0->writeData(&typeID);
  pDO1->writeData(&moduleGroup);
  uint32_t* modCounter=(uint32_t*)mp_moduleCounter->getData();
  (*modCounter)++;
  printInfo("MFModuleManager::updateManagerSetup - "
      "dataSize | module count:\n"+
      std::to_string(getStructureSize())+" | "+
      std::to_string(*modCounter));
}

void MFModuleManager::initData(){
  printInfo("MFModuleManager::initData debug - No initData");
};

void MFModuleManager::setupDataStructure(){
  if(mp_moduleCounter==nullptr){
    mp_moduleCounter=addData((uint32_t*)nullptr);//module counter
    uint32_t data=0;
    mp_moduleCounter->writeData(&data);
  }
};

void MFModuleManager::initNextModuleCreator(MFIModuleCreator* pModuleCreator){
  pModuleCreator->setTypeID(mp_vecModuleCreators->size());
  mp_vecModuleCreators->push_back(pModuleCreator);
}

void MFModuleManager::initModuleCreators(){
  //for each implementaiton of MFBaseModule
  //a module creator is needed. This comes in handy if the network connect task
  //wants to set up the module manager.
  if(mp_failorModuleCreator==nullptr){
    mp_failorModuleCreator=new MFIModuleCreator("No module");
    initNextModuleCreator(mp_failorModuleCreator);
  }

  if(mp_renderModuleCreator==nullptr){
    mp_renderModuleCreator=new MFRenderModule(mp_syncRes);
    initNextModuleCreator(mp_renderModuleCreator);
  }

  if(mp_physicsModuleCreator==nullptr){
    MFPhysicsModule *pM=new MFPhysicsModule(mp_syncRes);
    pM->setNotifierName("Physics-Module-Creator");
    pM->enableNotification(false);//TODO why does pM receive notifications???
    mp_physicsModuleCreator=pM;
    initNextModuleCreator(mp_physicsModuleCreator);
  }

  if(mp_inputModuleCreator==nullptr){
    mp_inputModuleCreator=new MFInputModule(mp_syncRes);
    initNextModuleCreator(mp_inputModuleCreator);
  }

  if(mp_GUIModuleCreator==nullptr){
    mp_GUIModuleCreator=new MFGUIIOModule(mp_syncRes);
    initNextModuleCreator(mp_GUIModuleCreator);
  }

  if(mp_worldModuleCreator==nullptr){
    mp_worldModuleCreator=new MFWorldModule(mp_objectManager,this,mp_syncRes);
    initNextModuleCreator(mp_worldModuleCreator);
  }

  if(mp_networkModuleCreator==nullptr){
    mp_networkModuleCreator=new MFNetworkModule(mp_syncRes);
    initNextModuleCreator(mp_networkModuleCreator);
  }

  if(mp_playerModuleCreator==nullptr){
    mp_playerModuleCreator=new MFPlayerModule(mp_syncRes);
    initNextModuleCreator(mp_playerModuleCreator);
  }

}

bool MFModuleManager::executeModuleLoop(bool* externalClose){
  mp_externalStop=externalClose;
  printInfo("MFModuleManager::executeModuleLoop - "
      "entering module execution loop:"
      "\nModule count: "+std::to_string(mp_vecModules->size()));
  while(*mp_externalStop==false){
    for(MFBaseModule* pModule:*mp_vecModules){
      if((*mp_externalStop!=true) && !pModule->execute() ){
        m_internalStop=true;
        return false;
      }
    }

    for(MFBaseModule* pModule:*mp_vecDynamicModules){
      if((*mp_externalStop!=true) && !pModule->execute() ){
        m_internalStop=true;
        return false;
      }
    }

    if(pRecyclingModule!=nullptr )
      if((*mp_externalStop!=true) && !pRecyclingModule->execute() ){
        m_internalStop=true;
        return false;
      }

    mp_stateModule->execute();
    lockCycleCounter.lock();
    m_cycleCounter++;
    lockCycleCounter.unlock();
  }
  m_internalStop=*externalClose;
  return true;
}

bool MFModuleManager::stopModuleLoop(){
  if(mp_externalStop!=nullptr)*mp_externalStop=true;
  m_internalStop=true;
  mp_externalLooper->stop();
  mp_externalLooper->clearTasks();
  m_isStarted=false;
  return true;
}

bool MFModuleManager::stopExternalModuleLoop(){
  if(mp_externalStop!=nullptr)*mp_externalStop=true;
  mp_externalLooper->stop();
  mp_externalLooper->clearTasks();
  m_internalStop=true;
  m_isStarted=false;
  return true;
}

/**
 * Task for init a module manager
 */
class MFInitTask : public MFAbstractTask{
public:
  MFModuleManager* mp_mgr;
  MFInitTask(MFModuleManager* pMgr){
    mp_mgr=pMgr;
    this->deleteAfterExecution(true);
    setTaskName("MFModuleManager.cpp - MFInitTask");
  }
  virtual bool doWork(){
    if(!mp_mgr->initModuleManager()){
      printErr("MFModuleManager.cpp - MFInitTask::doWork() - "
          "failed to init module manager!");
      return false;
    }
    return true;
  }

};

bool MFModuleManager::executeExternalModuleLoop(
    bool* externalClose,
    MFTaskThread* externalThread){
  mp_externalStop=externalClose;
  printInfo("MFModuleManager::executeExternalModuleLoop - "
      "starting external looper."
      "\nModule count: "+std::to_string(mp_vecModules->size()));
  if(externalThread==nullptr){
    if(!m_isInitialized){
      mp_externalLooper->addTask(new MFInitTask(this));
    }
    if(externalClose!=nullptr)mp_loopTask->setExternalClose(externalClose);
    mp_externalLooper->addTask(mp_loopTask);
    if(!mp_externalLooper->isStarted())mp_externalLooper->startDetached();
  }
  else{
    externalThread->addTask(mp_loopTask);
    externalThread->enableTaskRepeat(true);
  }
  m_isStarted=true;

  return true;
}

bool MFModuleManager::executeModuleStep(){
  //updates the model matrix of all sync objects
  for(MFSyncObject* pSync:*mp_objectManager->getSyncObjects()){
    pSync->updateModelMatrix();
  }
  for(MFBaseModule* pModule:*mp_vecModules){
//    MFObject::printInfo("MFModuleManager::executeModuleStep - moduleCycle/moduleName/ModID: "+
//        std::to_string(m_cycleCounter%10)+"/"+
//        pModule->getStructureName()+"/"+
//        std::to_string(pModule->getModuleID()));
    if(!pModule->execute() ){// todo (*mp_externalStop!=true)
      m_internalStop=true;
      return false;
    }
  }
  for(MFBaseModule* pModule:*mp_vecDynamicModules){
    if(!pModule->execute() && (*mp_externalStop!=true)){
      m_internalStop=true;
      return false;
    }
  }
  if(pRecyclingModule!=nullptr )
    if((*mp_externalStop!=true) && !pRecyclingModule->execute() ){
      m_internalStop=true;
      return false;
    }
  mp_stateModule->execute();
  lockCycleCounter.lock();
  m_cycleCounter++;
  lockCycleCounter.unlock();
  return true;
}

MFGUIIOModule* MFModuleManager::addGUIModule(uint32_t moduleGroup, bool initModule){
  MFGUIIOModule *pModule=(MFGUIIOModule*)mp_GUIModuleCreator->createNewModule(mp_res);
  if(initModule)pModule->initModule();
  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    printWarning("MFModuleManager::addGUIModule - module group not existent! "
        "Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  mp_vecMainGUIModule->push_back(pModule);
  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecModules->push_back(pModule);

  updateManagerSetup(pModule,moduleGroup);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);

  return pModule;
}

MFInputModule* MFModuleManager::addInputModule(
    GLFWwindow* pWindow,
    uint32_t moduleGroup,
    bool initModule){
  GLFWwindow* pWin=pWindow;
  if(pWindow==nullptr){
    if(mp_window==nullptr){
      printErr("MFModuleManager::addInputModule - failed! No render module added!");
      return nullptr;
    }
    pWin=mp_window;
  }

  MFInputModule* pModule=nullptr;
  if(mp_inputModuleCreator!=nullptr){
    pModule=(MFInputModule*)mp_inputModuleCreator->createNewModule(mp_res);
    pModule->setWindow(pWin);
  }else{
    printWarning("MFModuleManager::addInputModule - creating module "
        "without module creator!");
    pModule=new MFInputModule(pWin,mp_syncRes);
  }

  if(initModule)pModule->initModule();

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    printWarning("MFModuleManager::addGUIModule - module group not existent! "
        "Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();


  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecMainInputModule->push_back(pModule);
  mp_vecModules->push_back(pModule);

  updateManagerSetup(pModule,moduleGroup);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);

  return pModule;
}

MFPhysicsModule* MFModuleManager::addPhysicModule(uint32_t moduleGroupIndex, bool initModule){
  MFPhysicsModule *pModule=nullptr;
  if(mp_physicsModuleCreator!=nullptr){
    pModule=(MFPhysicsModule*)mp_physicsModuleCreator->createNewModule(mp_res);
  }else{
    P_WRN("creating module without module creator!");
    pModule=new MFPhysicsModule(mp_syncRes);
  }
  if(initModule)pModule->initModule();

  lockVecGroups.lock();
  if(moduleGroupIndex>=mp_vecModuleGroups->size()){
    P_WRN("module group not existent! Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroupIndex)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroupIndex);
  }
  lockVecGroups.unlock();

  mp_vecMainPhysicsModule->push_back(pModule);
  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecModules->push_back(pModule);

  updateManagerSetup(pModule,moduleGroupIndex);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);

  return pModule;
}

MFWorldModule* MFModuleManager::addWorldModule(uint32_t moduleGroup,bool initModule){
  MFWorldModule* pModule=nullptr;
  if(mp_worldModuleCreator!=nullptr){
    pModule=(MFWorldModule*)mp_worldModuleCreator->createNewModule(mp_res);
  }else{
    printWarning("MFModuleManager::addWorldModule - creating module without "
        "module creator!");
    pModule=new MFWorldModule(mp_objectManager,this,mp_syncRes);
  }
  if(initModule)pModule->initModule();

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    P_WRN("module group not existent! Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  mp_vecWorldModule->push_back(pModule);
  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecModules->push_back(pModule);

  updateManagerSetup(pModule,moduleGroup);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);

  return pModule;
}

MFNetworkModule* MFModuleManager::addNetworkModule(uint32_t moduleGroup,bool initModule){
  MFNetworkModule* pModule=nullptr;
  if(mp_networkModuleCreator!=nullptr){
    pModule=(MFNetworkModule*)mp_networkModuleCreator->createNewModule(mp_res);
  }else{
    printWarning("MFModuleManager::addNetworkModule - creating module without "
        "module creator!");
    pModule=new MFNetworkModule(mp_syncRes);
  }

  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecNetworkModules->push_back(pModule);
  mp_vecModules->push_back(pModule);

  /*set the server module manager, before initializing!*/
  pModule->getServerResources()->mp_groupProvider=this;
  pModule->getServerResources()->mp_moduleProvider=this;
  pModule->getServerResources()->mp_moduleSetup=this;
  pModule->getServerResources()->mp_loopCtl=this;
  pModule->getServerResources()->mp_objectManager=mp_objectManager;
  pModule->setEngineResources(mp_res);

  if(initModule)pModule->initModule();

  if(pModule->getClientResources()->mp_loopCtl!=nullptr)
    pModule->getClientResources()->mp_loopCtl->setExternalClose(&m_internalStop);
  else{
    P_WRN("No external close for client loop!");
  }

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    P_WRN("module group not existent! Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  updateManagerSetup(pModule,moduleGroup);

  return pModule;
}

bool MFModuleManager::initRecyclingModule(){
  if(pRecyclingModule!=nullptr)
  {
    P_WRN("recycling module already initialized!");
    return true;
  }
  pRecyclingModule=new MFRecyclingModule();
  setRecycleManager(pRecyclingModule);
  return pRecyclingModule->initModule();
}

MFPlayerModule* MFModuleManager::addPlayerModule(uint32_t moduleGroup,bool initModule){
  MFPlayerModule *pModule=nullptr;
  if(mp_networkModuleCreator!=nullptr){
    pModule=(MFPlayerModule*)mp_playerModuleCreator->createNewModule(mp_res);
  }else{
    P_WRN("creating module without module creator!");
    pModule=new MFPlayerModule(mp_syncRes);
  }
  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecPlayerModules->push_back(pModule);
  mp_vecModules->push_back(pModule);

  if(initModule)
    pModule->initModule();

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    P_WRN("module group not existent! Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  updateManagerSetup(pModule,moduleGroup);

  return pModule;
}

/**
 * Creates a render module and adds it to the module manager
 * @return
 */
MFRenderModule* MFModuleManager::addRenderModule(
    uint32_t moduleGroup,
    std::string windowTitle,
    std::string dataPath,
    std::string glslCompilerPath,
    bool initModule){
  MFSyncObject* pCamObject=new MFSyncObject(
      mp_syncRes,
      glm::vec3(-3.0,-3.0,3.0),
      glm::vec3(1.0f,0.0f,0.0f),
      glm::vec4(.0f,1.0f,.0f,1.0f),
      0.0f);

  mp_res->mp_vecCamObjects->push_back(pCamObject);

  MFRenderModule* pModule=nullptr;
  if(mp_renderModuleCreator!=nullptr){
    pModule=(MFRenderModule*)mp_renderModuleCreator->createNewModule(mp_res);
    pModule->setWindowTitle(windowTitle);
    pModule->setDataPath(dataPath);
    pModule->setGLSLCompilerPath(glslCompilerPath);
    pModule->setCamObject(pCamObject);
  }else{
    printWarning("MFModuleManager::addRenderModule - creating module without "
        "module creator!");
    pModule=new MFRenderModule(
        windowTitle,
        dataPath,
        glslCompilerPath,
        pCamObject,
        mp_syncRes);
  }
  if(initModule){
    if(!pModule->initModule()){
      printErr("failed to init module!!");
    }
  }

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    P_WRN("module group not existent! Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  mp_vecMainRenderModule->push_back(pModule);
  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecModules->push_back(pModule);

  if(mp_window==nullptr)mp_window=pModule->getWindow();

  updateManagerSetup(pModule,moduleGroup);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);

  return pModule;
}

void MFModuleManager::addModule(
    MFBaseModule* pModule,
    uint32_t moduleGroup,
    bool initModule){
  pModule->setModuleManagerIndex(mp_vecModules->size());
  if(initModule){
    if(!pModule->initModule())
      printErr("MFModuleManager::addModule - "
        "failed to init module!");
  }

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    printWarning("MFModuleManager::addModule - module group not existent! "
        "Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  updateManagerSetup(pModule,moduleGroup);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);
  mp_vecModules->push_back(pModule);
}

void MFModuleManager::addModule(
    MFBaseModule* pModule,
    bool initModule){
  pModule->setModuleManagerIndex(mp_vecModules->size());
  if(initModule){
    if(!pModule->initModule())
      printErr("MFModuleManager::addModule - "
        "failed to init module!");
  }
//TODO:
//  updateManagerSetup(pModule,moduleGroup);
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);
  mp_vecModules->push_back(pModule);
}

void MFModuleManager::addDynamicModule(
    MFBaseModule* pModule,
    uint32_t moduleGroup,
    bool initModule){
  pModule->setModuleManagerIndex(mp_vecDynamicModules->size());
  if(initModule){
    if(!pModule->initModule())printErr("MFModuleManager::addModule - "
        "failed to init module!");
  }
  pModule->setEngineResources(mp_res);
  mp_vecDynamicModules->push_back(pModule);
}

MFBaseModule* MFModuleManager::addModule(
    uint32_t moduleCreatorIndex,
    uint32_t moduleGroup,
    bool initModule){
  if(moduleCreatorIndex>=mp_vecModuleCreators->size()){
    printErr("MFModuleManager::addModule(modCreatorIndex...) - "
        "invalid moduleCreatorIndex!");
    return nullptr;
  }
  MFBaseModule* pModule=
      mp_vecModuleCreators->at(moduleCreatorIndex)->createNewModule(mp_res);
  if(pModule==nullptr){
    printErr("MFModuleManager::addModule(moduleCreatorIndex...) - "
        "module==nullptr!");
    return nullptr;
  }
  if(initModule)
    if(!pModule->initModule()){
      printErr("MFModuleManager::addModule(moduleCreatorIndex...) - "
          "module init failed!");
    }

  lockVecGroups.lock();
  if(moduleGroup>=mp_vecModuleGroups->size()){
    printWarning("MFModuleManager::addModule - module group not existent! "
        "Take care to keep track of module...");
  }else{
    mp_vecModuleGroups->at(moduleGroup)->vecModules.push_back(pModule);
    pModule->addModuleGroupIndex(moduleGroup);
  }
  lockVecGroups.unlock();

  pModule->setModuleManagerIndex(mp_vecModules->size());
  mp_vecModules->push_back(pModule);

  updateManagerSetup(pModule,moduleGroup);
  /* updates the resources to provide access to
		shared resources*/
  pModule->setEngineResources(mp_res);
  pModule->updateResources(mp_res);

  return pModule;
}


