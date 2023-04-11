/*
 * MFBaseModule.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFBASEMODULE_H_
#define MFENGINEMODULES_MFBASEMODULE_H_

#include <MFData/MFStructurableDataSetup.h>
#include <vector>
#include <mutex>
#include <MFPrinters/MFPrintSetup.h>
#include <MFBasicDefines.h>

#include "MFTasks/MFAbstractTask.h"
#include "MFThreadSystem/MFTaskThread.h"
#include "MFListenerInterfaces/MFInterfaceKeyListener.h"
#include "MFListenerInterfaces/MFInterfaceMouseListener.h"
#include "MFModuleObject.h"
#include "../MFSynchronisationClasses/MFSyncObject.h"
#include "../MFSynchronisationClasses/MFSyncObjectResources.h"
#include <MFPrinters/MFPrintSetup.h>
#include "../MFModuleResources.h"
#include "MFInterfacesModules/MFIModuleObjectSetup.h"
#include "MFInterfacesModules/MFInputSyncNotifier.h"

/*TODO as template class with type -> type must be derived from MFModuleObject*/
/*TODO as pure virtual class to force implementaiton of functions*/
/*TODO provide another MFBaseModule subclass with implemented virtual functions*/
/**
 * This is a base class for engine modules. It must be implemented to use other libraries within
 * a game loop. For synchronisation the MFBaseModule object is connected to every MFSyncObject (It will
 * call the MFSyncObject::getNextBaseModuleIndex() function to get an own index for resolving
 * the MFSyncObject's connection to the modules MFModuleObject.
 * A MFIModuleCreator implementation may be added to the MFModuleManager to provide network sync
 * for a specific module (engine module setup on client side). For network sync client side must
 * provide the same implementation (compiled class)!
 *
 * Creating a module object:
 *  By calling addSyncObject(MFSycObject*) a module object will be created with the implementation
 *  specific createModuleObject() function.
 *  MFModuleObject::initData() - called to initialize the objects data
 *  MFModuleObject::setupDataStructure() - called for creating the objects data structures
 *  initModuleObject() - module specific initialization of the object
 *
 *
 */
class MFBaseModule :
    public MFAbstractTask,
    public MFStructurableDataSetup,
    public MFIModuleObjectSetup,
    public MFInputSyncNotifier {
public:
  struct ObjectData{
    MFSyncObject* pSyncData;
    MFModuleObject* pModuleData;
  };

protected:
  void initData(){printWarning("MFBaseModule::initData - not impl.");};
  void setupDataStructure(){
    printWarning("MFBaseModule::setupDataStructure - not impl.");
  };

private:
  static uint32_t
  ms_moduleIDCounter;

  static std::mutex
  lockModuleIDCounter;

  bool
  m_enableInputSync=true,
  m_enableOutputSync=true,
  m_enableStateUpdate=true;

  uint32_t
  m_moduleTypeID=0,
  m_moduleID=0,
  m_moduleManagerIndex=0,
  m_syncObjectModuleID=0,
  m_moduleCycleCounter=0,
  m_recycleObjectsCount=0;

  std::vector<uint32_t>
  *mp_vecModuleGroups;//stores all groups ID's, to which this module belongs to.

  bool
  m_useExternalThread=false;

  MFTaskThread
  *mp_externalThread=nullptr;

  std::mutex
  lockVecSateUpdate,
  lockVecInputSync,
  lockCycleCounter,
  lockVecObjects,
  lockVecRecycledObjects;

  std::vector<MFSyncObject*>
  *mp_vecStateUpdateObjects;

  MFIModuleObjectSetup
  *mp_modObjectCreator=nullptr;

  //TODO enable external input synchronisation -> multiplex input queues!
protected:
  MFSyncObjectResources
  *mp_syncRes;

  MFModuleResources
  *mp_res=nullptr;
  //todo integrate a inter module synchronisation function within the doWork function
  //this function shall change extraordinate data like color mass and
  //more module specific things
  /**
   * This vector stores all relevant data.
   */
  std::vector<ObjectData*>
  *mp_vecModuleObjects,
  *mp_vecRecycledObjects;

  std::vector<MFSyncObject*>
  vecRecycleObjects;
  std::mutex
  lockVecRecycleObjects;
  bool recycleObjects();

protected:/*virtual functions of MFBaseModule*/
  /**
   * This function will add the sync object to the modules underlying system (for example to the underlying physics engine or the render api...)
   * It will be called within the execution of MFBaseModule::addSyncObject(...)
   * @param syncObject
   * @return
   */
  virtual MFModuleObject* initNewModuleObject(MFSyncObject* syncObject){
    return nullptr;
  };

  virtual bool initModuleObject(MFSyncObject* syncObject,MFModuleObject* pObject){
    P_ERR("no impl.!");
    return false;
  };

public:/*virtual functions of MFIModuleObjectCreator*/
  /**
   * If multipe module objects are created or objects were added to module already,
   * nullptr may be returned.
   * @param pSO - sync object which shall be used for creation.
   * @return the created module object or nullptr.
   */
  virtual MFModuleObject* createModuleObject(MFSyncObject* pSO){
    return createModuleObject();
  };

public:/*virtual functions of MFBaseModule*/
  virtual MFModuleObject* createModuleObject(){
    printWarning("MFBaseModule::createModuleObject() - "
      "not implemented, returning nullptr!");
    return nullptr;
  };

  virtual void updateResources(MFModuleResources* pRes){
    printWarning("MFBaseModule::updateResources - not impl.!");
  }

  virtual bool initModule();

  virtual bool disableObject(MFSyncObject *pObject);

  virtual bool recycleObject(MFSyncObject* pSO,MFModuleObject* pMO);

  /**
   * Default implementation will iterate over all motified changes of
   * MFSyncObjects (notifyInputSync::notifyInputSync()).
   * @warning This function is not thread safe!!
   * @return
   */
  virtual bool syncInputData();

  /**
   * This function uses the given indices for synchronicing/updating module objects with
   * their sync objects. In default implementation this function will be called with
   * notified input sync objects (only objects which received a notification bc. the changed).
   * The notification call should be implemented at a reasonable point f.e. if physics
   * collision occures, a notification can be sent to a sound playback object.
   * @return
   */
  virtual bool syncInputData(const std::vector<uint32_t>* pSyncIndices);

  /**
   * The implementation of this function must do the work of the module.
   * For example the physics module will calculate physics and
   * the render module will render/update the scene.
   * @return
   */
  virtual bool executeModule(){return false;};

  /**
   * This function writes data from the inner module objects to the sync objects.
   * Default implementation will iterate over all module objects and calls the
   * MFModuleObject::synchronizeOutput() function.
   *
   * @return
   */
  virtual bool syncOutputData();

  /**
   * Some modules may not need to be synchroized. If the synchronization is disabled,
   * the addSyncObject will not add any sync data and the doWork function will skip input
   * sync.
   * @param enable
   */
  virtual bool syncStates(std::vector<MFSyncObject*>* pVecStateChanges);
  virtual void initRecycling(MFModuleObject* pMO){};
public:/*virtual funtions of MFAbstractTask*/
  /**
   * This function will do the input data synchronisation and executing the module.
   * @warning Do not use this function simultanous with syncInputData(*)!
   * @return
   */
  bool doWork();

public:
  /**
   *
   * @param pSyncRes shared resources. If nullptr, it will take it from static setup.
   */
  MFBaseModule(MFSyncObjectResources *pSyncRes=nullptr,const std::string &moduleName="MFBaseModule");
  virtual ~MFBaseModule();

  /**
   * Default implementation will set the ObjectData's index to nullptr. The
   * ObjectData is a struct which connects the sync object to a specific
   * module object.
   * TODO what if multiple mos r connected to one so=?
   * @warning Multiple connections from module objects to one sync objects are recycled too!
   * @param pSO
   */
  void enqueueRecycleModuleObject(MFSyncObject* pSO);

  void setModuleManagerIndex(uint32_t index){m_moduleManagerIndex=index;};
  void addModuleGroupIndex(uint32_t additionalGroup){
    mp_vecModuleGroups->push_back(additionalGroup);};
  std::vector<uint32_t>* getVecGroupIDs(){
    return mp_vecModuleGroups;
  }

  void enableInputSynchronisation(bool enable){
    enableNotification(enable);
    m_enableInputSync=enable;
  };
  void enableStateUpdate(bool enable){m_enableStateUpdate=enable;};
  void enableOutputSynchronisation(bool enable){m_enableOutputSync=enable;};

  MFModuleObject* getModuleObject(MFSyncObject* pSyncO){
    if(pSyncO==nullptr){
      P_WRN("sync object is nullptr!");
      return nullptr;
    }
    uint32_t objectIndex=pSyncO->getFirstModuleObjectIndex(m_syncObjectModuleID);
    if(objectIndex==0xFFFFFFFF)
      return nullptr;
    if(mp_vecModuleObjects->at(objectIndex)==nullptr)
      return nullptr;
    return mp_vecModuleObjects->at(objectIndex)->pModuleData;
  }

  /**
   * Returns a module object owned by this module, if available. Returns only the first object!
   * @param pModuleObject
   * @return
   */
  MFModuleObject* getModuleObject(MFModuleObject* pModuleObject){
    return getModuleObject(pModuleObject->getSyncObject());
  }

  MFModuleObject* getModuleObject(uint32_t index);

  /**
   * This function will use the recycled module object for a given sync object. If a module
   * object is available, it will be added MFBaseModule::addSyncObject(pSO,pModuleO) again.
   * If a nullptr is returned, the pSO wasn't connected to this module before!
   * @param pSO - a object which was already connected to a module object of this module.
   * @return a valid module object on success or nullptr on failure.
   */
  bool reuseRecycledModuleObject(MFSyncObject* pSO);
  std::vector<ObjectData*>* getVecModuleObjects(){return mp_vecModuleObjects;};

  uint32_t getCurrentModuleCycle(){
    uint32_t counter=0;
    lockCycleCounter.lock();
    counter=m_moduleCycleCounter;
    lockCycleCounter.unlock();
    return counter;
  };

  /**
   * Sets the module resources. The module resources are given to created sub resources
   * of this object. Former created subresources will not be updated!
   * @param res
   */
  void setEngineResources(MFModuleResources *res){mp_res=res;}

  /**
   * This sets an index at which all sync objects must hold the modules data.
   * @param syncObjectIndex
   */
  void setSyncModuleID(uint32_t syncObjectIndex){m_syncObjectModuleID=syncObjectIndex;}

  /**
   * For every created MFBaseModule sync objects will add a pointer to the module
   * and its module object.To get a modules
   * object of a specific sync object, just look at the syc object's module index.
   * @return
   */
  const uint32_t& getSyncModuleID(){return m_syncObjectModuleID;};

  bool isInputSynchronisationEnabled(){return m_enableInputSync;};
  bool isOutputSynchronisationEnabled(){return m_enableOutputSync;};

  uint32_t getModuleID(){return m_moduleID;};
  void setModuleTypeID(uint64_t typeID){m_moduleTypeID=typeID;};
  uint32_t getModuleTypeID(){return m_moduleTypeID;};


  /**
   * This function adds a sync object to the module. It will create
   * a module object which is connected to the sync object.
   * @param pSyncObject object to add for synchronisation within the engine loop.
   * @return the created module object if successful or nullptr if sth. went wrong
   */
  MFModuleObject* addSyncObject(MFSyncObject* pSyncObject);

  /**
   * This function adds a sync object and its corresponding module object to this module.
   * @param pSyncObject object to add for synchronisation within the engine loop.
   * @param pModO module object which will be connected to this module and the given sync object.
   * @warning pModO will be deleted in the destructor of this module!
   * @return true if nothing went wrong
   */
  bool addSyncObject(MFSyncObject* pSyncObject,MFModuleObject* pModO,bool recycled=false);

  /**
   * If another module manipulates data of a sync object, the sync object will add
   * it self to the input synchronisation queue of all MFBaseModule objects
   * which are using it.
   * @param inputSyncObject
   *///TODO maybe differen input sync queues, for each data which must be synchronised
  //f.e. position, orientation,.... this will lead to less data which must be copied
  //For common data sharing (sync of position of 1000sands of objects physics->renderer)
  //
  void notifyInputSync(MFSyncObject* inputSyncObject);

  void addStateUpdate(MFSyncObject* pSyncObject);

  /**
   * Sets the module object creator used for creating new module objects.
   * @param pObjectCreator
   */
  void setModuleObjectCreator(MFIModuleObjectSetup* pObjectCreator){
    mp_modObjectCreator=pObjectCreator;
  };
private:
  inline bool addSO(MFSyncObject* pSyncObject,MFModuleObject* pModO,bool recycle);
};

#endif /* MFENGINEMODULES_MFBASEMODULE_H_ */
