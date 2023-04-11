/*
 * MFEngineResources.cpp
 *
 *  Created on: 07.10.2020
 *      Author: michl
 */

#include "MFEngineResources.h"

MFEngineResources::MFEngineResources(MFModuleManager *pModuleManager) {
  mp_moduleManager=pModuleManager;
  mp_objectManager=nullptr;
  mp_actionTaskManager=nullptr;
}

MFEngineResources::~MFEngineResources() {
  // TODO Auto-generated destructor stub
}

bool MFEngineResources::init(){
  bool ret=mp_moduleManager->initModuleManager();
  mp_actionTaskManager=mp_moduleManager->getActionManager();
  mp_objectManager=mp_moduleManager->getObjectManager();
  return ret;
}
