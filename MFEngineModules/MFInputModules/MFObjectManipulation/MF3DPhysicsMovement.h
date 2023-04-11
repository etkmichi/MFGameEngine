/*
 * MF3DPhysicsMovement.h
 *
 *  Created on: 02.11.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFOBJECTMANIPULATION_MF3DPHYSICSMOVEMENT_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFOBJECTMANIPULATION_MF3DPHYSICSMOVEMENT_H_
#include "../../ModulePhysics/MFPhysicModuleObject.h"
class MF3DPhysicsMovement {
  //TODO enable air control/damping using m_upTreshold
protected:
  MFPhysicModuleObject
  *mp_physicsObject=nullptr;

  glm::vec3
  m_frontAxis,
  m_backAxis,
  m_leftAxis,
  m_rightAxis,
  m_upAxis,
  m_downAxis,
  m_upTreshold/*Treshold for appliing upwards impulse*/;

  float
  m_damping=0.1f,
  m_jumpImpulseFactor=10,
  m_upTresholdFactor=.0f/*used for multiplication after treshold check*/;

public:/*virtual functions MF3DPhysicsMovement*/
  virtual bool addFrontVelocity(float value);
  virtual bool addBackVelocity(float value);
  virtual bool addLeftVelocity(float value);
  virtual bool addRightVelocity(float value);
  virtual bool addVelocity(glm::vec3 direction,float value);

private:
  inline void interact(glm::vec3 direction,float value);
public:
  MF3DPhysicsMovement(MFPhysicModuleObject* pObject);
  MF3DPhysicsMovement();
  virtual ~MF3DPhysicsMovement();
  void setPhysicsMovementObject(MFPhysicModuleObject* pObject){
    mp_physicsObject=pObject;
  };
  MFPhysicModuleObject* getPhysicsMovementObject(){
    return mp_physicsObject;
  };
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFOBJECTMANIPULATION_MF3DPHYSICSMOVEMENT_H_ */
