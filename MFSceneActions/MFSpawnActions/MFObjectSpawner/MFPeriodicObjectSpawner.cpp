/*
 * MFPeriodicObjectSpawner.cpp
 *
 *  Created on: 12.11.2020
 *      Author: michl
 */

#include "MFPeriodicObjectSpawner.h"

MFPeriodicObjectSpawner::MFPeriodicObjectSpawner(MFModuleObjectManager* pMgr) :
MFObjectSpawner(pMgr){
  mp_ticker=new MFTickCounter();
  mp_ticker->start();
}

MFPeriodicObjectSpawner::~MFPeriodicObjectSpawner() {
  // TODO Auto-generated destructor stub
}

MFSyncObject* MFPeriodicObjectSpawner::spawnNext(){
  /**/
  if(spawnLimit<=mp_vecSpawnedObjects->size())return nullptr;
  if(mp_ticker->stop()>m_spawnPeriod){
    mp_ticker->start();
    MFSyncObject* pSO=createNextObject();
    if(pSO!=nullptr)
      mp_vecSpawnedObjects->push_back(pSO);
    return pSO;
  }

  return nullptr;
}
