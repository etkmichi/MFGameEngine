/*
 * MF3DPhysicsMovement.cpp
 *
 *  Created on: 02.11.2020
 *      Author: michl
 */

#include "MF3DPhysicsMovement.h"

MF3DPhysicsMovement::MF3DPhysicsMovement(MFPhysicModuleObject* pObject) {
  if(pObject==nullptr)
    P_ERR("physics object is nullptr!");
  mp_physicsObject=pObject;
  m_frontAxis=glm::vec3(1,0,0);
  m_backAxis=glm::vec3(-1,0,0);
  m_leftAxis=glm::vec3(0,1,0);
  m_rightAxis=glm::vec3(0,-1,0);
  m_upAxis=glm::vec3(0,0,1);
  m_downAxis=glm::vec3(0,0,-1);
  m_damping=5.0f;
}
MF3DPhysicsMovement::MF3DPhysicsMovement() {
  P_WRN("do not forget to set the physics object!");
  m_frontAxis=glm::vec3(1,0,0);
  m_backAxis=glm::vec3(-1,0,0);
  m_leftAxis=glm::vec3(0,1,0);
  m_rightAxis=glm::vec3(0,-1,0);
  m_upAxis=glm::vec3(0,0,1);
  m_downAxis=glm::vec3(0,0,-1);
  m_damping=5.0f;
}

MF3DPhysicsMovement::~MF3DPhysicsMovement() {
}

void MF3DPhysicsMovement::interact(glm::vec3 direction,float value){
  if(direction.x != 0.0f){
    glm::vec3 velocity=mp_physicsObject->getLinearVelocity();
    velocity.x=value*m_damping*direction.x;
    mp_physicsObject->setLinearVelocity(velocity);
  }
  if(direction.y != 0.0f){
    glm::vec3 velocity=mp_physicsObject->getLinearVelocity();
    velocity.y=value*m_damping*direction.y;
    mp_physicsObject->setLinearVelocity(velocity);
  }
  if(direction.z != 0.0f){
    glm::vec3 velocity=mp_physicsObject->getLinearVelocity();
    velocity.z=value*m_damping*direction.z;
    mp_physicsObject->setLinearVelocity(velocity);
  }
}

/*joystick*/
bool MF3DPhysicsMovement::addFrontVelocity(float value){
  interact(m_frontAxis,value);
  return true;
}

bool MF3DPhysicsMovement::addBackVelocity(float value){
  interact(m_backAxis,value);
  return true;
}
bool MF3DPhysicsMovement::addLeftVelocity(float value){
  interact(m_leftAxis,value);
  return true;
}
bool MF3DPhysicsMovement::addRightVelocity(float value){
  interact(m_rightAxis,value);
  return true;
}

bool MF3DPhysicsMovement::addVelocity(glm::vec3 direction,float value){
  interact(direction,value);
  return true;
}

