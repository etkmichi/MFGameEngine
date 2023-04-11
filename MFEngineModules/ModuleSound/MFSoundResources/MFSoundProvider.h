/*
 * MFSoundProvider.h
 *
 *  Created on: 15.03.2021
 *      Author: michl
 */

#ifndef MFENGINESETUPS_MFGAMESETUPS_MFSETUPRESOURCES_MFSOUNDPROVIDER_H_
#define MFENGINESETUPS_MFGAMESETUPS_MFSETUPRESOURCES_MFSOUNDPROVIDER_H_
#include "MFBasicGameResources/MFGameSetupResources.h"
#include <MFSoundPlaybackData.h>
#include <MFSFXR/MFSFXRSoundData.h>
#include <MFSFXR/MFSFXRSoundCreation.h>
#include "MFEngineModules/ModulePhysics/MFPhysicModuleObject.h"
#include "MFEngineModules/ModuleSound/MFSoundCallbacks/MFSoundCollisionCallback.h"
class MFGameSetupResources;
/**
 * This class will provide sounds for physical objects within the game simulation.
 */
class MFSoundProvider {
private:
  std::vector<MFSoundPlaybackData*>
  vecSounds;
  MFGameSetupResources
  *pGameRes;
  MFSoundCollisionCallback
  *pCollisionCallback;
public:
  MFSoundProvider(MFGameSetupResources *pGameRes);

  virtual ~MFSoundProvider();

  void initSounds();

  /**
   * This function will add collision sound callback with a object dependent sound.
   * @param pModuleObject
   */
  void initCollisionSoundPlayback(MFPhysicModuleObject* pModuleObject);
  void initCollisionSoundPlayback(MFSyncObject* pPhysicsSyncObject);
};

#endif /* MFENGINESETUPS_MFGAMESETUPS_MFSETUPRESOURCES_MFSOUNDPROVIDER_H_ */
