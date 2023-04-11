/*
 * MFSoundModule.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include "../ModuleSound/MFSoundModule.h"

MFSoundModule::MFSoundModule(MFSyncObjectResources* pSyncRes) :
MFBaseModule(pSyncRes){
  setSetupName("MFSoundModule");
  pSoundPlayer=nullptr;
  initAudioManager();
  format=AUDIO_F32SYS;
  sampleCount=E_SampleCount::E_Sound_2048_Samples;
  deviceName="";
  enableOutputSynchronisation(false);
  enableNotification(false);
}

MFSoundModule::~MFSoundModule() {
}

MFSoundModuleObject* MFSoundModule::createModuleObject(
    MFSoundPlaybackData* pObjectSound,
    MFSyncObject* pSoundObject,
    MFSyncObject* pListenerObject){
  MFSoundModuleObject* pSMO=new MFSoundModuleObject(
      pObjectSound,
      pSoundObject,
      pListenerObject,
      pSoundPlayer);
  return pSMO;
}

void MFSoundModule::updateResources(MFModuleResources* pRes){
}

/*virtual functions*/
bool MFSoundModule::initModule(){
  bool ret=true;
  ret&=initAudioManager();

  if( getVecDeviceNames().size() > 0 ){
    if(deviceName.length()==0){
      deviceName=getVecDeviceNames().at(0);
    }

    ret&=initSoundPlayer(deviceName);

  }else{
    ret=false;
  }

  return ret;
}

bool MFSoundModule::initSoundPlayer(std::string deviceName){
  this->deviceName=deviceName;
  pSoundPlayer=new MFSoundPlayer(
      format,
      sampleCount,
      this->deviceName);
  return pSoundPlayer->initSoundPlayer();
}

MFModuleObject* MFSoundModule::createModuleObject(){
	return new MFSoundModuleObject();
}

bool MFSoundModule::initModuleObject(
		MFSyncObject* syncObject,
		MFModuleObject* pObject){
  /*No init needed?*/
	return true;
}

bool MFSoundModule::executeModule(){
	return true;
}

bool MFSoundModule:: syncOutputData(){
  P_WRN("no output sync of this module!");
	return false;
}

void MFSoundModule::addBackgroundSound(MFSoundPlaybackData* pSound){
  lockVecBackgroundSounds.lock();
  vecBackgroundSounds.push_back(pSound);
  lockVecBackgroundSounds.unlock();
}
