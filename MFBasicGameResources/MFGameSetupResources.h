/*
 * MFGameSetupResources.h
 *
 *  Created on: 11.11.2020
 *      Author: michl
 */

#ifndef MFBASICGAMERESOURCES_MFGAMESETUPRESOURCES_H_
#define MFBASICGAMERESOURCES_MFGAMESETUPRESOURCES_H_

#include "../MFEngineModules/MFModuleManager.h"
#include "../MFEngineModules/MFModuleObjectManager.h"
#include "../MFEngineModules/ModuleRender/MFRenderModule.h"
#include "../MFEngineModules/MFInputModules/MFInputModule.h"
#include "../MFEngineModules/ModulePhysics/MFPhysicsModule.h"
#include "../MFEngineModules/ModuleSound/MFSoundModule.h"
#include "../MFEngineModules/ModuleSound/MFSoundResources/MFSoundProvider.h"
#include "../MFSynchronisationClasses/MFSyncObject.h"
#include "../MFEngineModules/MFInterfacesModules/MFIEngineLoopControl.h"
#include "../MFEngineModules/MFInterfacesModules/MFIManagerControl.h"
#include "../MFEngineModules/MFInterfacesModules/MFIModuleCreator.h"
#include "../MFEngineModules/MFInterfacesModules/MFIModuleGroupControl.h"
#include "../MFEngineModules/MFInterfacesModules/MFIModuleObjectSetup.h"
#include "../MFEngineModules/MFModuleGroup.h"
#include "../MFEngineModules/ModulePlayer/MFPlayerModule.h"

class MFSoundProvider;
class MFGameSetupResources {
public:
  MFModuleManager *mp_moduleManager=nullptr;
  MFModuleObjectManager *mp_objectManager=nullptr;

  MFRenderModule *mp_renderModule=nullptr;
  MFInputModule *mp_inputModule=nullptr;
  MFPhysicsModule *pPhysicsModule=nullptr;
  MFPlayerModule *mp_playerModule=nullptr;
  MFSoundModule *pSoundModule=nullptr;
  MFIModuleGroupControl *mp_groupControl=nullptr;
  MFIEngineLoopControl *mp_loopControl=nullptr;
  MFIManagerControl *mp_managerControl=nullptr;

  MFModuleGroup
  *mp_groupWorld=nullptr,
  *mp_groupRenderer=nullptr,
  *mp_groupPhysics=nullptr,
  *mp_groupInput=nullptr,
  *mp_groupSimulationWorld=nullptr,
  *mp_groupNetwork=nullptr,
  *pGroupSound=nullptr;

  MFSyncObject
  *mp_camObject=nullptr,
  *mp_manikinObject=nullptr;

  /**
   * player objects as synco for common usage
   */
  std::vector<MFSyncObject*>
  *mp_vecPlayerObjects;
  MFSoundProvider
  *pSoundProvider=nullptr;
public:
  MFGameSetupResources();
  virtual ~MFGameSetupResources();
  void init(MFModuleManager* pModMgr){
    mp_moduleManager=pModMgr;
    mp_objectManager=mp_moduleManager->getObjectManager();
    mp_groupControl=mp_moduleManager;
    mp_loopControl=mp_moduleManager;
    mp_managerControl=mp_moduleManager;
  }
  double getPhysicsMicroSecsTimeStep();
  double getDeltaTimeFactor();
public:
  void printGroupInfos(MFSyncObject* pSO){
    this->mp_groupControl->getModuleGroup(pSO->getVecModuleGroups()->at(0))->printGroupNames();
  }
};

#endif /* MFBASICGAMERESOURCES_MFGAMESETUPRESOURCES_H_ */
