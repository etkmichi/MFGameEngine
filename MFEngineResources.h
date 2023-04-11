/*
 * MFEngineResources.h
 *
 *  Created on: 07.10.2020
 *      Author: michl
 */
#include "MFEngineModules/MFModuleManager.h"
#include "MFEngineModules/MFModuleObjectManager.h"
#include "MFSceneActions/MFActionTaskManager.h"
#ifndef MFENGINERESOURCES_H_
#define MFENGINERESOURCES_H_
/**
 * This class provides access to the most important resources of the engine.
 */
class MFEngineResources {
public:
  MFModuleManager
  *mp_moduleManager;
  MFActionTaskManager
  *mp_actionTaskManager;
  MFModuleObjectManager
  *mp_objectManager;
public:
  MFEngineResources(MFModuleManager *pModuleManager);
  bool init();
  virtual ~MFEngineResources();
};

#endif /* MFENGINERESOURCES_H_ */
