/*
 * MFWorldSetup.h
 *
 *  Created on: 27.10.2020
 *      Author: michl
 */

#ifndef MFENGINESETUPS_MFWORLDSETUP_H_
#define MFENGINESETUPS_MFWORLDSETUP_H_
#include "../MFEngineModules/MFModuleManager.h"
#include "MFSetupInterfaces/MFIWorldSetup.h"
/**
 * Provides the setup for creating the static world
 */
class MFWorldSetup : public MFIWorldSetup{
protected:
  MFModuleManager
  *mp_moduleManager;

protected:/*virtual functions MFWorldSetup*/
  virtual void setupWorld(MFModuleManager* pModMgr){
    MFObject::printErr("MFWorldSetup::setupWorld - no impl.!");
    return;
  };

public:/*virtual functions MFIWorldSetup*/
  void setupWorld(){
    setupWorld(mp_moduleManager);
  }

public:
  MFWorldSetup(MFModuleManager* pModuleManager);
  virtual ~MFWorldSetup();

};

#endif /* MFENGINESETUPS_MFWORLDSETUP_H_ */
