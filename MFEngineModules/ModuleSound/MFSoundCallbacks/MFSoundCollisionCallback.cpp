/*
 * MFSoundCollisionCallback.cpp
 *
 *  Created on: 15.02.2021
 *      Author: michl
 */

#include "../../ModuleSound/MFSoundCallbacks/MFSoundCollisionCallback.h"

#include <glm/gtx/norm.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <MFPhysicsHelper.h>

MFSoundCollisionCallback::MFSoundCollisionCallback(
    MFSoundModule* pSoundModule,
    MFPhysicsModule* pPhysicsModule) {
  this->pSoundModule=pSoundModule;
  this->pPhysicsModule=pPhysicsModule;
}

MFSoundCollisionCallback::~MFSoundCollisionCallback() {
  // TODO Auto-generated destructor stub
}

bool MFSoundCollisionCallback::collisionCallback(
    btManifoldPoint& collisionPoint,
    MFPhysicModuleObject* pCollisionObject0,
    MFPhysicModuleObject* pCollisionObject1){
  //TODO check if pCollisionObject1 gets its own collision callback for sound generation
  MFSoundModuleObject* pSMO=
      static_cast<MFSoundModuleObject*>(pSoundModule->getModuleObject(pCollisionObject0));
  if(pSMO==nullptr){
    return true;
  }
  uint32_t physicsStep=pPhysicsModule->getCurrentModuleCycle();
  if(pSMO->checkPlayback(pCollisionObject1,physicsStep)){
    /*collision with object 1 already notified!*/
    /*TODO multiple collision with object1 may occure! fe a cube doesnt hit o1 with parallel
     * sureface, then the other cube corners which hit o1 should emit sound too!*/
    return true;
  }
  glm::vec3 vel0=pCollisionObject0->getLinearVelocity();
  glm::vec3 vel1=pCollisionObject1->getLinearVelocity();
  glm::vec3 velRel=vel1-vel0;
  glm::vec3 collisionNormal=MFPhysicsHelper::toVec3(collisionPoint.m_normalWorldOnB);
  glm::vec3 projection=glm::proj(velRel,collisionNormal);
  float scale=glm::length2(projection);
  //todo get mass if 0 then mass=10 if>10 then mass=10

  /*TODO One collsion may occure multiple times f.e. a cube on a flat surface will cause 4 collision
   * callbacks for each corner of the cube.
   *  -> maybe set collision callback flag within MFPhysicModuleObject
   *  -> or create addCallbackExecution(this)*/
  if(scale<0.1f)return true;
  if(scale>5.0f)scale=5.0f;
  pSMO->addPlaybackCheck(pCollisionObject1,physicsStep);
//  P_INF("scale/projection: "+std::to_string(scale)+"/"+TO_STRING_VEC3(projection));
  pSMO->getSoundData()->setVolumeScale(scale);
  pSoundModule->enableInputSynchronisation(true);
  pSoundModule->notifyInputSync(pSMO->getSyncObject());
  pSoundModule->enableInputSynchronisation(false);
  /*play a sound*/
  /*get sound from sound module*/
  return true;
}
