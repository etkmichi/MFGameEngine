/*
 * MFSoundModuleObject.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULESOUND_MFSOUNDMODULEOBJECT_H_
#define MFENGINEMODULES_MODULESOUND_MFSOUNDMODULEOBJECT_H_

#include <mutex>
#include <vector>
#include "../MFModuleObject.h"
#include "../../MFSynchronisationClasses/MFSyncObject.h"
#include <MFSoundPlaybackData.h>
#include <MFSoundPlayback/MFSoundPlayer.h>
#include "../ModulePhysics/MFPhysicModuleObject.h"

class MFSoundModuleObject :
    public MFModuleObject {
private:
  struct S_PlaybackCheck{
    MFModuleObject* pPlaybackReason=nullptr;
    uint32_t moduleStep=0;
  };
  MFSyncObject
  *pSoundObject,
  *pListenerObject;

  MFSoundPlaybackData
  *pSoundData;
  MFSoundPlayer
  *pSoundPlayer;

  /*TODO*/
  bool
  contiuousPlayback=false;

  uint32_t
  countinuousMillisPause=0;

  MFPhysicModuleObject
  *pVelocityProvider;
  std::vector<S_PlaybackCheck*>
  vecPlaybackCheck;
  std::mutex
  lockVecPlaybackCheck;

protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject);
  virtual void setupObjectDataStructure();

public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){P_WRN("No impl.!");return true;};
  virtual bool updateObject(){P_WRN("No impl.!");return true;};
  virtual void disable(){};
  virtual void enable(){};
  /**
   * This will update the sound scale depending on the position of the object.
   * @param syncObject
   * @return
   */
  virtual bool synchronizeInput();
  virtual bool synchronizeOutput();
public:
	MFSoundModuleObject(
	    MFSoundPlaybackData* pObjectSound,
	    MFSyncObject* pSoundObject,
	    MFSyncObject* pListenerObject,
	    MFSoundPlayer* pSoundPlayer);
	MFSoundModuleObject();
	virtual ~MFSoundModuleObject();


	void setVelocityScaleObject(MFPhysicModuleObject* pVelocityProvider);
	/*TODO rotational velo, ...*/

  bool isContiuousPlayback() const {
    return contiuousPlayback;
  }

  void setContiuousPlayback(bool contiuousPlayback = false) {
    this->contiuousPlayback = contiuousPlayback;
  }

  uint32_t getCountinuousMillisPause() const {
    return countinuousMillisPause;
  }

  void setCountinuousMillisPause(uint32_t countinuousMillisPause = 0) {
    this->countinuousMillisPause = countinuousMillisPause;
  }

  /**
   * Checks if a playback was already caused by a specific sync object at a specific module
   * step. If a playback was caused by pPlaybackCause within the same moduleStep, this function
   * will return true. If moduleStep differs from an added playback or pPlaybackReason is not
   * found, then this function will return false.
   * @param pPlaybackReason - a module object which caused/will cause a playback.
   * @param playbackStep - step counter of a module.
   * @return
   */
  bool checkPlayback(MFModuleObject* pPlaybackReason,uint32_t moduleStep);
  void addPlaybackCheck(MFModuleObject* pPlaybackReason,uint32_t moduleStep);

  MFSoundPlaybackData* getSoundData() const {
    return pSoundData;
  }

  void setPSoundData(MFSoundPlaybackData *pSoundData) {
    this->pSoundData = pSoundData;
  }
private:
  inline void fetchVelocityScale();
};

#endif /* MFENGINEMODULES_MODULESOUND_MFSOUNDMODULEOBJECT_H_ */
