/*
 * MFPhysicModuleObject.h
 *
 *  Created on: 23.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFPHYSICMODULES_MFPHYSICMODULEOBJECT_H_
#define MFENGINEMODULES_MFPHYSICMODULES_MFPHYSICMODULEOBJECT_H_

#include "../MFModuleObject.h"
#include <MFCallback/MFICallback.h>
#include <MFBulletWrapper/MFSimpleBulletManager.h>
#include "btBulletDynamicsCommon.h"

#include "MFIPhysics/MFCollisionCallback.h"

class MFCollisionCallback;

class MFPhysicModuleObject : public MFModuleObject {
private:
  int64_t
  outSyncTpCurrent=0,
  outSyncTpLast=0,
  outputSyncIntervall=1;
  btVector3
  impulse;
  bool
  impulseUpdate=false;
protected:
  btDynamicsWorld
  *pWorld;

  PhysicalObjectData
  *mp_internalPhysicalObject,
  *mp_physicalObject;

	MFAbstractGeometry
  *mp_geometry;

	std::mutex
	lockVecCollisionCallbacks;

	std::vector<MFCollisionCallback*>
	vecCollisionCallbacks;

public:
	std::mutex
  lockPublicAccess,
  lockImpulse;

	glm::vec3
  *mp_position,
  *mp_gravity,
  *mp_linVelocity,
  *mp_rotVelocity;

	MFDataObject
  *mp_doMass=nullptr,
  *mp_doPosition=nullptr,
  *mp_doGravity=nullptr,
  *mp_doLinVelocity=nullptr,
  *mp_doRotVelocity=nullptr;

	float
  *mp_mass;

protected:/*virtual functions MFStructurableDataSetup*/
  virtual void updateSyncObject(MFSyncObject* syncObject);
  virtual void setupObjectDataStructure();

public:/*virtual functions of MFModuleObject:*/
  virtual void initData(){};
  virtual bool updateData();
  virtual bool updateObject();
  virtual void disable();
  virtual void enable();
  virtual bool synchronizeInput();
  virtual bool synchronizeOutput();

public: /*virtual functions of MFPhysicsModuleObject*/
  virtual bool initPhysicalObject();

public:
	MFPhysicModuleObject();
	virtual ~MFPhysicModuleObject();
	void setWorld(btDynamicsWorld* pW){pWorld=pW;};
	/**
	 * Sets the underlying physical object data. Must be a PhysicalObjectData* struct!
	 * @param pObject
	 */
	void setPhysicalObject(PhysicalObjectData* pObject);
	PhysicalObjectData* getPhysicalObject();
	btRigidBody* getRigidBody();
	void setMass(float mass);
	float getMass();
	void setGravity(glm::vec3 gravitationVector);
	void setPosition(glm::vec3 &position);
	void setLinearVelocity(glm::vec3 velocity);
	void setAngularVelocity(glm::vec3 velocity);
	glm::vec3 getLinearVelocity();
	glm::vec3 getAngularVelocity();
	glm::vec3 getGravity();

	void addImpulse(glm::vec3 impulse);

	void setOrientation(glm::vec3 rotationVector, float angle);
	void setGeometry(MFAbstractGeometry* pGeo){mp_geometry=pGeo;};

  void addCollisionCallback(MFCollisionCallback* pCallback);

	btCollisionObject* getCollisionObject();
	void executeCollisionCallbacks(
	    btManifoldPoint& collisionPoint,
	    MFPhysicModuleObject* other);

	void update();
	void updateCollisionShape(void* pShape);
	/**
	 * Offset to the current position
	 * @param translation
	 */
	void translate(glm::vec3 translation);

	glm::vec3& getPosition();
	void get4x4ModelMatrix(float* dst);
	void set4x4ModelMatrix(float* dst);
	void printVec3(void* dat);
	void printVec3(glm::vec3 data);

  int64_t getOutputSyncIntervall() const {
    return outputSyncIntervall;
  }
};

#endif /* MFENGINEMODULES_MFPHYSICMODULES_MFPHYSICMODULEOBJECT_H_ */
