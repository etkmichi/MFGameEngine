/*
 * MFRecycleArea.h
 *
 *  Created on: 23.03.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULERECYCLING_RECYCLEOBJECTS_MFRECYCLEAREA_H_
#define MFENGINEMODULES_MODULERECYCLING_RECYCLEOBJECTS_MFRECYCLEAREA_H_
#include "../../ModulePhysics/MFPhysicModuleObject.h"
#include "../../ModulePhysics/MFIPhysics/MFCollisionCallback.h"
#include "../MFRecycleObject.h"
#include "../../ModulePhysics/MFPhysicsModule.h"
#include "../../MFModuleGroupManager.h"
/**
 * This class provides physics collision based recycling.
 * If an object hits the physical recycle object it will be recycled by the
 * collision callback of this class.
 */
class MFRecycleArea :
    public MFRecycleObject,
    public MFCollisionCallback {
private:
  MFPhysicModuleObject
  *pRecyclingArea=nullptr;
  MFPhysicsModule
  *pPhysicsModule=nullptr;
  MFModuleGroupManager
  *pGroupManager=nullptr;
public: /*virtual functions MFICollisionCallback*/
  virtual bool collisionCallback(
      btManifoldPoint& collisionPoint,
      MFPhysicModuleObject* pCollisionObject0,
      MFPhysicModuleObject* pCollisionObject1);
public:
  /**
   * TODO what if multiple phyiscs simulation at once exist?
   * @param pRecyclingArea
   */
  MFRecycleArea(
      MFPhysicModuleObject* pRecyclingArea,
      MFPhysicsModule* pPhysicsModule,
      MFModuleGroupManager* pGroupManager);
  virtual ~MFRecycleArea();
};

#endif /* MFENGINEMODULES_MODULERECYCLING_RECYCLEOBJECTS_MFRECYCLEAREA_H_ */
