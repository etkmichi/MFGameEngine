/*
 * MFModuleObejctCreator.h
 *
 *  Created on: 13.11.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULEOBJECTCREATOR_H_
#define MFENGINEMODULES_MFMODULEOBJECTCREATOR_H_
#include "../MFBaseModule.h"
#include "../MFModuleObjectManager.h"
#include "../MFModuleObject.h"
#include "MFCreationCallback.h"
#include "../../MFObjectSetup/MFInitialObjectSetup.h"
#include "../MFModuleGroup.h"

/**
 * This class provides functionality to create random module objects with added creators.
 */
class MFModuleObjectCreator {
private:
  struct S_Creator {
    bool enabled=false;
    int32_t
    dynamicIndex=-1,
    staticIndex=-1;
    std::vector<MFIModuleObjectSetup*>
    *pVecCreators;
    std::vector<MFBaseModule*>
    *pVecModules;
    std::vector<MFModuleGroup*>
    *pVecGroups;
//    S_ModuleGroup* pModGroup=nullptr;
    void init(){
      pVecCreators=new std::vector<MFIModuleObjectSetup*>();
      pVecModules=new std::vector<MFBaseModule*>();
      pVecGroups=new std::vector<MFModuleGroup*>();
    }
    void push_back(
        MFIModuleObjectSetup* pMOC,
        MFBaseModule *pBM,
        MFModuleGroup *pG){
      pVecCreators->push_back(pMOC);
      pVecModules->push_back(pBM);
      pVecGroups->push_back(pG);
    }
  };
  std::mutex
  lockVecEnabledCreators,
  lockVecCreationCallbacks;
  std::vector<S_Creator*>
  *mp_vecEnabledCreators,
  *mp_vecCreators;
  MFModuleObjectManager
  *mp_objectManager;
  std::vector<MFCreationCallback*>
  vecCreationCallbacks;

protected:
  MFInitialObjectSetup
  *mp_objectInitSetup;
  MFInitialObjectSetup* getInitialSetup(){return mp_objectInitSetup;};

public:/*virtual functions MFModuleObejctCreator:*/
  virtual void setupInitialData(MFSyncObject* pSyncObject){return;};
  virtual bool init(){return true;};
  virtual bool preCreation(MFSyncObject* pSyncObject){return true;};
  virtual bool postCreation(MFSyncObject* pSyncObject){return true;};

public:
  MFModuleObjectCreator(MFModuleObjectManager* pObjectManager);
  virtual ~MFModuleObjectCreator();
//  void enableRandomCreator(bool randomize){m_puRandomize=randomize;};
  void enableCreator(uint32_t index,bool enable);

  /**
   *
   * @param pCreator
   * @param pModule
   * @param pGroup - the created sync object will be added to this group after all module
   * objects were created. If index is specified too, the MFSyncObject may be added to
   * multiple groups if pGroup is not nullptr!
   * @param enable
   * @param creatorIndex - adds pCreator as a subcreator to the creator at
   * the given index. If an object is created with this index, all added creators
   * with the same index are used (MFIModuleObjectCreator::create...)
   * @return returns a index for the added creator. The returned index can
   * be used to add sub-creators (needed if more than one object needs
   * to be created)
   */
  int64_t addCreator(
      MFIModuleObjectSetup* pCreator,
      MFBaseModule* pModule,
      MFModuleGroup* pGroup=nullptr,
      bool enable=true,
      int64_t creatorIndex=-1);

  /**
   * This function adds a creation callback which will be called after an object was created.
   * It can be used for additional inits and manipulation.
   * @param pCreationCallback
   */
  void addCreationCallback(MFCreationCallback* pCreationCallback);

  /**
   * This functions creates module object/s and returns the corresponding MFSyncObject.
   * This function will not add the module objects to a group, use
   * MFModuleObjectManager::addCustomObject to create additional module objects for a
   * specific group.
   * Use addCustomObject after a call to createNextObject to prevent multiple instances
   * of one sync object within a module. To prevent multiple module obejcts for one
   * module, the forceAdd parameter of addCustomObject must be false!
   * @return a sync object with its corresponding module objects. The module objects depend
   * on the added creator (MFModuleObejctCreator::addCreator(...))
   */
  MFSyncObject* createNextObject();
};

#endif /* MFENGINEMODULES_MFMODULEOBJECTCREATOR_H_ */
