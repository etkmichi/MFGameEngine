/*
 * MFSoundModuleObject.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include "../ModuleSound/MFSoundModuleObject.h"

#include <glm/gtx/norm.hpp>

MFSoundModuleObject::MFSoundModuleObject(
    MFSoundPlaybackData* pObjectSound,
    MFSyncObject* pSoundObject,
    MFSyncObject* pListenerObject,
    MFSoundPlayer* pSoundPlayer) {
  setSetupName("MFSoundModuleObject");
  this->pSoundObject=pSoundObject;
  this->pListenerObject=pListenerObject;
  pSoundData=pObjectSound;
  pVelocityProvider=nullptr;
  this->pSoundPlayer=pSoundPlayer;
}

MFSoundModuleObject::MFSoundModuleObject() {
	setSetupName("MFSoundModuleObject");
	pSoundObject=nullptr;
	pListenerObject=nullptr;
	pSoundData=nullptr;
  pVelocityProvider=nullptr;
  this->pSoundPlayer=nullptr;
}

MFSoundModuleObject::~MFSoundModuleObject() {
	// TODO Auto-generated destructor stub
}

void MFSoundModuleObject::setupObjectDataStructure(){
}

bool MFSoundModuleObject::checkPlayback(MFModuleObject* pPlaybackReason,uint32_t moduleStep){
  uint32_t shrinkCounter=0;
  lockVecPlaybackCheck.lock();
  uint32_t vecSize=vecPlaybackCheck.size();
  lockVecPlaybackCheck.unlock();
  for(uint32_t i=0;i<vecSize;i++){
    S_PlaybackCheck* pCheck=vecPlaybackCheck.at(i);
    if(pCheck==nullptr){
      shrinkCounter++;
      continue;
    }
    if(pCheck->pPlaybackReason==pPlaybackReason){
      if(pCheck->moduleStep==moduleStep){
        return true;
      }else{
        /*remove pCheck and continue, maybe it was added another time!*/
        vecPlaybackCheck.data()[i]=nullptr;
        delete pCheck;
        shrinkCounter++;
        continue;
      }
    }else{
      /*if same module, check if module already stepped forward -> if so, another
       * check of the pCheck is unneccessary!*/
      if(pCheck->pPlaybackReason->getModuleID()==pPlaybackReason->getModuleID()){
        if(pCheck->moduleStep!=moduleStep){
          /*its already another module step, remove this check*/
          vecPlaybackCheck.data()[i]=nullptr;
          shrinkCounter++;
          delete pCheck;
          continue;
        }
      }
      /*if pCheck remains, the vector shouldnt shrink!*/
      shrinkCounter=0;
      continue;
    }
  }
  lockVecPlaybackCheck.lock();
  if(vecSize==vecPlaybackCheck.size() && shrinkCounter>0){
    /*if no check was added since start of this function and last entries in vecPlaybackCheck
     * are nullptr then resize the vec*/
    vecPlaybackCheck.resize(vecSize-shrinkCounter);
  }
  //TODO test for memory leak and if the resizing will be done.
  //this function will be called if a collision occures.
  //if it takes long to check, it will get more reasonable that anothe check was added.
  //
  lockVecPlaybackCheck.unlock();
  return false;
}

void MFSoundModuleObject::addPlaybackCheck(MFModuleObject* pPlaybackReason,uint32_t moduleStep){
  S_PlaybackCheck *pCheck=new S_PlaybackCheck();
  pCheck->pPlaybackReason=pPlaybackReason;
  pCheck->moduleStep=moduleStep;
  lockVecPlaybackCheck.lock();
  vecPlaybackCheck.push_back(pCheck);
  lockVecPlaybackCheck.unlock();
}

bool MFSoundModuleObject::synchronizeInput(){
  /*get listener and soundObject positions and set tp pSoundData*/
  if(pListenerObject != nullptr){
    pSoundData->setReceiverPosition(*pListenerObject->getModelPosition());
    pSoundData->setReceiverLookAt(*pListenerObject->getLocalLookAt());
  } else {
    pSoundData->setReceiverPosition(glm::vec3(0,0,0));
    pSoundData->setReceiverLookAt(glm::vec3(1,0,0));
  }

  if(pSoundObject!=nullptr)
    pSoundData->setSourcePosition(*pSoundObject->getModelPosition());
  else
    pSoundData->setSourcePosition(glm::vec3(0,0,0));

  fetchVelocityScale();
  if(pSoundObject!=nullptr)
    this->pSoundPlayer->addSound(pSoundData);
	return true;
}

void MFSoundModuleObject::fetchVelocityScale(){
  if(pVelocityProvider==nullptr)return;
  float scale=glm::length2(pVelocityProvider->getLinearVelocity());
  MFObject::printInfo(std::to_string(scale));
  //TODO
}

bool MFSoundModuleObject::synchronizeOutput(){
  P_WRN("no output sync for module object!");
	return true;
}

void MFSoundModuleObject::updateSyncObject(MFSyncObject* syncObject){}
