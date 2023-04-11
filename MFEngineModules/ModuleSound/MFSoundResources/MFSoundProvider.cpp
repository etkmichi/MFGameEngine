/*
 * MFSoundProvider.cpp
 *
 *  Created on: 15.03.2021
 *      Author: michl
 */

#include "MFSoundProvider.h"

MFSoundProvider::MFSoundProvider(MFGameSetupResources* pGameRes) {
  //Use geometry manager for geometry/sound mapping
  pGameRes->mp_objectManager->getGeometryManager()->initGeometryCreators();
  this->pGameRes=pGameRes;
  pCollisionCallback=new MFSoundCollisionCallback(pGameRes->pSoundModule, pGameRes->pPhysicsModule);
}

MFSoundProvider::~MFSoundProvider() {
  for(MFSoundPlaybackData* pSBD:vecSounds){
    if(pSBD!=nullptr)delete pSBD;
  }
}

void MFSoundProvider::initCollisionSoundPlayback(MFPhysicModuleObject* pModuleObject){
  MFSyncObject* pSO=pModuleObject->getSyncObject();
  if(pSO->isReused())return;
  MFGeometryManager* pGM=pGameRes->mp_objectManager->getGeometryManager();
  uint32_t geometryIndex=pGM->getIndexGeometry(pSO->getGeometry());

  if(geometryIndex>=vecSounds.size()){
    P_WRN("No sound for geometry index! index="+std::to_string(geometryIndex));
    return;
  }

  MFSoundPlaybackData* pSound=vecSounds.at(geometryIndex);
  MFSoundModuleObject* pSMO=new MFSoundModuleObject(
      pSound ,
      pSO,
      pGameRes->mp_camObject,
      pGameRes->pSoundModule->getSoundPlayer());
  pGameRes->pSoundModule->addSyncObject(pSO, pSMO);
  pModuleObject->addCollisionCallback(pCollisionCallback);
}
void MFSoundProvider::initCollisionSoundPlayback(MFSyncObject* pPhysicsSyncObject){
  MFSyncObject* pSO=pPhysicsSyncObject;
  if(pSO->isReused())return;
  MFPhysicModuleObject* pModuleObject=(MFPhysicModuleObject*)(
      pGameRes->pPhysicsModule->getModuleObject(pSO));
  MFGeometryManager* pGM=pGameRes->mp_objectManager->getGeometryManager();
  uint32_t geometryIndex=pGM->getIndexGeometry(pSO->getGeometry());

  if(geometryIndex>=vecSounds.size()){
    P_WRN("No sound for geometry index! index="+std::to_string(geometryIndex));
    return;
  }

  MFSoundPlaybackData* pSound=vecSounds.at(geometryIndex);
  MFSoundModuleObject* pSMO=new MFSoundModuleObject(
      pSound ,
      pSO,
      pGameRes->mp_playerModule->getVecModuleObjects()->at(0)->pSyncData,
      pGameRes->pSoundModule->getSoundPlayer());
  pGameRes->pSoundModule->addSyncObject(pSO, pSMO);
  pModuleObject->addCollisionCallback(pCollisionCallback);
}

void MFSoundProvider::initSounds(){
  MFGeometryManager* pGM=pGameRes->mp_objectManager->getGeometryManager();
  pGM->initGeometryCreators();

  MFSFXRSoundData* pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::blip);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->initSoundData();
  uint32_t index=pGM->getIndexSphere();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;

  pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::explosion);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->initSoundData();
  index=pGM->getIndexAxis();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;

  pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::blip);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->initSoundData();
  index=pGM->getIndexBox();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;

  pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::blip);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->getSoundSetup()->master_vol=0.02f;
  pSFXRSound->getSoundSetup()->sound_vol=0.15f;
  pSFXRSound->initSoundData();
  index=pGM->getIndexNoise();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;

  pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::blip);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->getSoundSetup()->master_vol=0.02f;
  pSFXRSound->initSoundData();
  index=pGM->getIndexCylinder();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;

  pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::blip);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->getSoundSetup()->master_vol=0.02f;
  pSFXRSound->getSoundSetup()->sound_vol=0.15f;
  pSFXRSound->initSoundData();
  index=pGM->getIndexCone();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;

  pSFXRSound=new MFSFXRSoundData(E_SoundBaseSetup::blip);
  pSFXRSound->getSoundSetup()->setAttackScale(0);
  pSFXRSound->getSoundSetup()->setSustainScale(50000);
  pSFXRSound->getSoundSetup()->setDecayScale(0);
  pSFXRSound->getSoundSetup()->master_vol=0.02f;
  pSFXRSound->getSoundSetup()->sound_vol=0.15f;
  pSFXRSound->initSoundData();
  index=pGM->getIndexCapsule();
  if(vecSounds.size()<index+1){
    vecSounds.resize(index+1);
  }
  vecSounds.data()[index]=pSFXRSound;
}
