/*
 * MFPeriodicObjectSpawner.h
 *
 *  Created on: 12.11.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFSPAWNACTIONS_MFOBJECTSPAWNER_MFPERIODICOBJECTSPAWNER_H_
#define MFSCENEACTIONS_MFSPAWNACTIONS_MFOBJECTSPAWNER_MFPERIODICOBJECTSPAWNER_H_

#include <MFTime/MFTickCounter.h>

#include "MFObjectSpawner.h"
//TODO set limited amount (reuse old pu's)
class MFPeriodicObjectSpawner: public MFObjectSpawner {
private:
  MFTickCounter
  *mp_ticker;
  int64_t
  m_spawnPeriod=300;
  uint32_t
  spawnLimit=0xFFFFFFFF;
public:/*virtual functions of MFObjectSpawner*/
  /**
   * By default, preiodically spawns a sphere.
   * @return
   */
  virtual MFSyncObject* spawnNext();

public:/*virtual functions MFModuleObejctCreator:*/
  virtual bool init(){return true;};
  virtual bool preCreation(MFSyncObject* pSyncObject){return true;};
  virtual bool postCreation(MFSyncObject* pSyncObject){return true;};

public:
  MFPeriodicObjectSpawner(MFModuleObjectManager* pMgr);
  virtual ~MFPeriodicObjectSpawner();
  void setSpawnPeriod(int64_t millis) {
    m_spawnPeriod = millis;
  }
  uint32_t getSpawnLimit() const {
    return spawnLimit;
  }

  void setSpawnLimit(uint32_t spawnLimit = 0xFFFFFFFF) {
    this->spawnLimit = spawnLimit;
  }

  ;
};

#endif /* MFSCENEACTIONS_MFSPAWNACTIONS_MFOBJECTSPAWNER_MFPERIODICOBJECTSPAWNER_H_ */
