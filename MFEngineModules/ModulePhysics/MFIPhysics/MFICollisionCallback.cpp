/*
 * MFIPhysicsCollisionCallback.cpp
 *
 *  Created on: 09.02.2021
 *      Author: michl
 */

#include <MFPhysicsHelper.h>
#include "MFCollisionCallback.h"

MFCollisionCallback::MFCollisionCallback() {
  pTickCounter=new MFTickCounter();
  pTickCounter->start();
}

MFCollisionCallback::~MFCollisionCallback() {
  // TODO Auto-generated destructor stub
}

bool MFCollisionCallback::collisionCallback(
      btManifoldPoint& collisionPoint,
      MFPhysicModuleObject* pCollisionObject0,
      MFPhysicModuleObject* other){
  if(pTickCounter->stop()<callbackExecInterval){
    return true;
  }
  pTickCounter->start();
  std::string collisionInfos="Collision Infos:\n";
  std::string collisionPosition="collision at position localPointA/B: A="+
      std::to_string(collisionPoint.m_localPointA.x())+"/"+
      std::to_string(collisionPoint.m_localPointA.y())+"/"+
      std::to_string(collisionPoint.m_localPointA.z())+ " B="+
      std::to_string(collisionPoint.m_localPointB.x())+"/"+
      std::to_string(collisionPoint.m_localPointB.y())+"/"+
      std::to_string(collisionPoint.m_localPointB.z())+"\n";
  if(pCollisionObject0!=nullptr && other!=nullptr){
    collisionPosition+=
      "Objet coordinates: A/B: "+
      MFToString::toString(pCollisionObject0->getPosition())+" B: "+
      MFToString::toString(other->getPosition())+"\n";
    collisionPosition+=
      "btManifoldPoint::getPositionWorldOnA/B: "+
      MFToString::toString(MFPhysicsHelper::toVec3(collisionPoint.getPositionWorldOnA()))+" B: "+
      MFToString::toString(MFPhysicsHelper::toVec3(collisionPoint.getPositionWorldOnB()))+"\n";
  }
  std::string impulse="friction direction A/B: "+//impulse seitwärts+
      MFToString::toString(MFPhysicsHelper::toVec3(collisionPoint.m_lateralFrictionDir1))+
      MFToString::toString(MFPhysicsHelper::toVec3(collisionPoint.m_lateralFrictionDir2));
//      std::to_string(collisionPoint.m_appliedImpulseLateral1)+"/"+
//      std::to_string(collisionPoint.m_appliedImpulseLateral2);
//  impulse +="\nappliedImpulse: "+std::to_string(collisionPoint.m_appliedImpulse)+"\n";
  std::string normal="nomalWorldOB: "+
      MFToString::toString(MFPhysicsHelper::toVec3(collisionPoint.m_normalWorldOnB));
  collisionInfos+=collisionPosition;
  collisionInfos+=impulse;
  collisionInfos+=normal;
//  P_INF(collisionInfos);
  return true;
}
