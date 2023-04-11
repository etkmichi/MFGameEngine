/*
 * MFRecycleArea.cpp
 *
 *  Created on: 23.03.2021
 *      Author: michl
 */

#include "MFRecycleArea.h"

MFRecycleArea::MFRecycleArea(
    MFPhysicModuleObject* pRecyclingArea,
    MFPhysicsModule* pPhysicsModule,
    MFModuleGroupManager* pGroupManager) {
  pRecyclingArea->addCollisionCallback(this);
  pRecyclingArea->getSyncObject()->enableRecycling(false);
  this->pRecyclingArea=pRecyclingArea;
  this->pPhysicsModule=pPhysicsModule;
  this->pGroupManager=pGroupManager;
}

MFRecycleArea::~MFRecycleArea() {
  // TODO Auto-generated destructor stub
}

bool MFRecycleArea::collisionCallback(
    btManifoldPoint& collisionPoint,
    MFPhysicModuleObject* pCollisionObject0,
    MFPhysicModuleObject* pCollisionObject1){
  MFPhysicModuleObject* pRecyclingObject=pCollisionObject1;
  if(pRecyclingObject==pRecyclingArea){
    pRecyclingObject=pCollisionObject0;
  }
  MFSyncObject* pSO=pRecyclingObject->getSyncObject();
  if(pSO->getRecycleState()==MFSyncObject::RECYCLE_STATE_RECYCLE_DISABLED)
    return true;
  MFModuleGroup* pG=pGroupManager->getModuleGroup(pSO->getVecModuleGroups()->at(0));

  pG->enqueueRecycleObject(pSO);
  return true;
}
