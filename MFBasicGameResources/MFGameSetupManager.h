/*
 * MFGameSetupManager.h
 *
 *  Created on: 11.11.2020
 *      Author: michl
 */

#ifndef MFBASICGAMERESOURCES_MFGAMESETUPMANAGER_H_
#define MFBASICGAMERESOURCES_MFGAMESETUPMANAGER_H_

#include "../MFEngineModules/MFModuleManager.h"
#include "MFGameSetupResources.h"
/**
 * This manager provides some functionality to init basic game setups.
 * F.e. most games use a physics world group which provides two modules
 * (render and physics module).
 *
 */
class MFGameSetupManager {
public:
  MFGameSetupManager();
  virtual ~MFGameSetupManager();

  /**
   * Initializes following groups:
  *mp_groupRenderer,
  *mp_groupPhysics,
  *mp_groupInput,
  *mp_groupPhysicsWorld,
   *
   * @param pModuleManager
   * @param pGameSetupRes
   */
  void initMainGroups(
      MFIModuleGroupControl* pGroupManager,
      MFGameSetupResources* pGameSetupRes);
};

#endif /* MFBASICGAMERESOURCES_MFGAMESETUPMANAGER_H_ */
