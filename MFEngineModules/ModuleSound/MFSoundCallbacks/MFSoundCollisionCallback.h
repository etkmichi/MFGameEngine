/*
 * MFSoundCollisionCallback.h
 *
 *  Created on: 15.02.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULESOUND_MFSOUNDCALLBACKS_MFSOUNDCOLLISIONCALLBACK_H_
#define MFENGINEMODULES_MODULESOUND_MFSOUNDCALLBACKS_MFSOUNDCOLLISIONCALLBACK_H_
#include "../../ModulePhysics/MFIPhysics/MFCollisionCallback.h"
#include "../../ModulePhysics/MFPhysicsModule.h"
#include "../../ModulePhysics/MFPhysicModuleObject.h"
#include "../../ModuleSound/MFSoundModule.h"
#include "../../ModuleSound/MFSoundModuleObject.h"
class MFSoundCollisionCallback: public MFCollisionCallback {
private:
  MFSoundModule
  *pSoundModule;
  MFPhysicsModule
  *pPhysicsModule;
public: /*virtual functions MFICollisionCallback*/
  virtual bool collisionCallback(
      btManifoldPoint& collisionPoint,
      MFPhysicModuleObject* pCollisionObject0,
      MFPhysicModuleObject* pCollisionObject1);
public:
  MFSoundCollisionCallback(
      MFSoundModule* pSoundModule,
      MFPhysicsModule* pPhysicsModule);
  virtual ~MFSoundCollisionCallback();
};

#endif /* MFENGINEMODULES_MODULESOUND_MFSOUNDCALLBACKS_MFSOUNDCOLLISIONCALLBACK_H_ */
