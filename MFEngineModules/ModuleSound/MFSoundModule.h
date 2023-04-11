/*
 * MFSoundModule.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULESOUND_MFSOUNDMODULE_H_
#define MFENGINEMODULES_MODULESOUND_MFSOUNDMODULE_H_
#include "../MFBaseModule.h"
#include "../../MFSynchronisationClasses/MFSyncObject.h"
#include <MFSoundDeviceManager.h>
#include <MFSoundPlaybackData.h>
#include <MFSoundPlayback/MFSoundPlayer.h>
#include <SDL2/SDL_audio.h>

#include "../ModuleSound/MFSoundModuleObject.h"
/**
 * This module handles sound playback. After creation, the device name should be set to one
 * of the names provided by MFSoundModule::getVecDeviceNames()! If no device name was set before
 * initialization, the MFSoundModule::initSoundPlayer(deviceName) must be called manually!
 * TODO create sound module with less resource usage:
 *  -> multiple objects shall use the same sound/module object for their own soundplayback
 *    -> with a lot objects which dont collide, a lot unnecessary itartion will occure...
 *
 */
class MFSoundModule:
		public MFBaseModule,
		public MFSoundDeviceManager {
private:
  SDL_AudioFormat
  format;
  E_SampleCount
  sampleCount;
  MFSoundPlayer
  *pSoundPlayer;
  std::string
  deviceName;

  bool
  backgroundMusicPlayback=false;
  std::vector<MFSoundPlaybackData*>
  vecBackgroundSounds;

  std::mutex
  lockVecBackgroundSounds;

protected:
	bool initModuleObject(
			MFSyncObject* syncObject,
			MFModuleObject* pObject);
public:
	void updateResources(MFModuleResources* pRes);
	virtual MFBaseModule* createModule(){return nullptr;};

public:/*virtual functions of MFBaseModule*/
  virtual MFModuleObject* createModuleObject();
  virtual bool initModule();
  virtual void removeObject(MFSyncObject* pSO){P_ERR("not impl.!");};
//  virtual bool syncInputData(const std::vector<uint32_t>* pSyncIndices){
//    P_ERR("not impl.!");
//    return false;
//  };
  virtual bool executeModule();
  virtual bool syncOutputData();

public:
	MFSoundModule(MFSyncObjectResources* pSyncRes);
	virtual ~MFSoundModule();
	bool initSoundPlayer(std::string deviceName);
	MFSoundModuleObject* createModuleObject(
    MFSoundPlaybackData* pObjectSound,
    MFSyncObject* pSoundObject,
    MFSyncObject* pListenerObject);
	MFSoundPlayer* getSoundPlayer(){return pSoundPlayer;};

	/**
	 * This will add a sound which will be played periodically. All sounds which were added
	 * will be played periodically after the previous added sound.
	 * @param pSound
	 */
	void addBackgroundSound(MFSoundPlaybackData* pSound);
	void enableBackgroundSounds(bool enable){backgroundMusicPlayback=enable;};

};

#endif /* MFENGINEMODULES_MODULESOUND_MFSOUNDMODULE_H_ */
