/*
 * MFIPhysicsCollisionCallback.h
 *
 *  Created on: 09.02.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFPHYSICMODULES_MFIPHYSICS_MFCOLLISIONCALLBACK_H_
#define MFENGINEMODULES_MFPHYSICMODULES_MFIPHYSICS_MFCOLLISIONCALLBACK_H_

#include <MFCallback/MFICallback.h>
#include <MFStringHelpers/MFToString.h>
#include <MFTime/MFTickCounter.h>
#include <btBulletCollisionCommon.h>
#include "../MFPhysicModuleObject.h"
#include <MFBasicDefines.h>

class MFPhysicModuleObject;
/**
 * Callback-class for physical objects.
 * Take care one collision may lead to multiple callbacks (an object can collide with multiple
 * corners at the same simulation iteration)!
 */
class MFCollisionCallback: protected MFICallback {
private:
  MFTickCounter
  *pTickCounter;
  uint32_t
  callbackExecInterval=0;

protected: /*virtual functions of MFIFunctionCallback*/
  virtual bool functionCallback(uint32_t switchVal=0,void* userData=nullptr){
    MFObject::printErr("MFIFunctionCallback::guiCallback - no impl.! switchVal:"+std::to_string(switchVal)
                       +"\nuserData="+std::to_string(*(static_cast<uint64_t*>(userData))));
    return false;
  }

public: /*virtual functions MFICollisionCallback*/
  /*TODO what if parameters are set by physics module and funcitonCallback is executed instead of
   * collisionCallback(...)*/
  virtual bool collisionCallback(
      btManifoldPoint& collisionPoint,
      MFPhysicModuleObject* pCollisionObject0,
      MFPhysicModuleObject* pCollisionObject1);

public:
  MFCollisionCallback();
  virtual ~MFCollisionCallback();

  uint32_t getCallbackExecInterval() const {
    return callbackExecInterval;
  }

  /**
   * If this value is set, the collision callback will be executed if at least callbackExecInterval
   * milliseconds passed since last execution.
   * @param callbackExecInterval
   */
  void setCallbackExecInterval(uint32_t callbackExecIntervalMillis = 0) {
    this->callbackExecInterval = callbackExecIntervalMillis;
  }
};

#endif /* MFENGINEMODULES_MFPHYSICMODULES_MFIPHYSICS_MFCOLLISIONCALLBACK_H_ */
