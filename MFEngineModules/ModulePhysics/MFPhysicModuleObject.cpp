/*
 * MFPhysicModuleObject.cpp
 *
 *  Created on: 23.10.2019
 *      Author: michl
 */

#include "MFPhysicModuleObject.h"
#include "MFPhysicStructs.h"
#include <MFObjects/MFObject.h>
#include <MFPhysicsHelper.h>
#include <MFTime/MFTickCounter.h>

MFPhysicModuleObject::MFPhysicModuleObject() {
	mp_internalPhysicalObject=new PhysicalObjectData();
	mp_physicalObject=mp_internalPhysicalObject;
	mp_position=new glm::vec3(0.0f,0.0f,0.0f);
	mp_gravity=new glm::vec3(0.0f,0.0f,0.0f);
	mp_linVelocity=new glm::vec3(0.0f,0.0f,0.0f);
	mp_rotVelocity=new glm::vec3(0.0f,0.0f,0.0f);
	mp_geometry=nullptr;
	mp_mass=(float*)std::malloc(sizeof(float));
	*mp_mass=0;
	pWorld=nullptr;
  outSyncTpLast=MFTickCounter::current();
	setSetupName("MFPhysicModuleObject");
	impulse=btVector3(0,0,0);
}

MFPhysicModuleObject::~MFPhysicModuleObject() {
	PhysicalObjectData* pPhysO=(PhysicalObjectData*)mp_internalPhysicalObject;
	delete pPhysO;
}

btCollisionObject* MFPhysicModuleObject::getCollisionObject(){
  PhysicalObjectData *pPhysObject=(PhysicalObjectData*)mp_physicalObject;
  return pPhysObject->pBody;
}

void MFPhysicModuleObject::addCollisionCallback(MFCollisionCallback* pCallback){
  lockVecCollisionCallbacks.lock();
  vecCollisionCallbacks.push_back(pCallback);
  lockVecCollisionCallbacks.unlock();
}

void MFPhysicModuleObject::executeCollisionCallbacks(
    btManifoldPoint& collisionPoint,
    MFPhysicModuleObject* other){
  /*check if externally executer or directly!*/
  /*add collision state for a specific module step and for every other object
   *  -> enable single collision can be used f.e. in sound by polling the collision state
   *  Collision states: getCollisionState(pOther) -> collision, none*/
  if(!mp_syncObject->isNotRecycled())
    return;
  lockVecCollisionCallbacks.lock();
  for(MFCollisionCallback* pCB:vecCollisionCallbacks){
    if(pCB==nullptr)continue;
    pCB->collisionCallback(collisionPoint,this, other);
  }
  lockVecCollisionCallbacks.unlock();
}

bool MFPhysicModuleObject::initPhysicalObject(){
	if(mp_syncObject==nullptr){
		P_ERR("failed, syncObject==nullptr!");
		return false;
	}
	if(mp_geometry==nullptr){
		P_ERR("failed, mp_geometry==nullptr!");
		return false;
	}

	PhysicalObjectData *pPhysObject=(PhysicalObjectData*)mp_physicalObject;
	if(mp_geometry->getBulletCollisionShape()==nullptr){
		pPhysObject->pShape=(btCollisionShape*)mp_geometry->createCollisionShape();
	}else{
		pPhysObject->pShape=(btCollisionShape*)mp_geometry->getBulletCollisionShape();
	}

	if(pPhysObject->pShape==nullptr){
		P_ERR("failed, mp_geometry->createCollisionShape()==nullptr!");
		return false;
	}

	MFInitialObjectSetup* pIniDat=mp_syncObject->getInitialSetup();

	pPhysObject->initPosition = GLMV3_TO_BTVEC3((*pIniDat->mp_initPosition));
	pPhysObject->initLinearVelocity=GLMV3_TO_BTVEC3((*pIniDat->mp_initVelocity));
	pPhysObject->mass=*pIniDat->mp_initMass;
	pPhysObject->initModelMatrix=(float*)mp_syncObject->getGlobalModelMatrix();
	pPhysObject->init();
	return true;
}

void MFPhysicModuleObject::setupObjectDataStructure(){
	mp_doMass=addData(mp_mass);
	mp_doPosition=add3DVec(mp_position);
	mp_doGravity=add3DVec(mp_linVelocity);
	mp_doLinVelocity=add3DVec(mp_rotVelocity);
	mp_doRotVelocity=add3DVec(mp_gravity);
}

void MFPhysicModuleObject::setPhysicalObject(PhysicalObjectData* pObject){
	mp_physicalObject=pObject;
};

void MFPhysicModuleObject::setMass(float mass){
	*mp_mass=mass;
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject!=nullptr && pPhysicalObject->pBody!=nullptr){
		pPhysicalObject->pBody->setMassProps((btScalar)mass,pPhysicalObject->localInertia);
	}
}

float MFPhysicModuleObject::getMass(){
  PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
  if(pPhysicalObject!=nullptr && pPhysicalObject->pBody!=nullptr){
    return pPhysicalObject->pBody->getMass();
  }
  P_ERR("no physical object!");
  return .0f;
}

PhysicalObjectData* MFPhysicModuleObject::getPhysicalObject(){return mp_physicalObject;};

btRigidBody* MFPhysicModuleObject::getRigidBody(){
  return mp_physicalObject->pBody;
}

void MFPhysicModuleObject::setGravity(glm::vec3 gVec){
	*mp_gravity=gVec;
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject!=nullptr && pPhysicalObject->pBody!=nullptr){
		pPhysicalObject->pBody->setGravity(btVector3(gVec.x,gVec.y,gVec.z));
	}
}

bool MFPhysicModuleObject::updateData(){
//	MFObject::printInfo("MFPhysicModuleObject::updateData - updating physical object");
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject==nullptr){
		MFObject::printErr("MFPhysicModuleObject::updateData - pPhysicalObject"
				"==nullptr!");
		return false;
	}
	*mp_mass=pPhysicalObject->pBody->getMass();

	btTransform worldTrafo=pPhysicalObject->pMotionState->m_graphicsWorldTrans;
	mp_position->x=worldTrafo.getOrigin().x();
	mp_position->y=worldTrafo.getOrigin().y();
	mp_position->z=worldTrafo.getOrigin().z();

	mp_gravity->x=pPhysicalObject->pBody->getGravity().x();
	mp_gravity->y=pPhysicalObject->pBody->getGravity().y();
	mp_gravity->z=pPhysicalObject->pBody->getGravity().z();

	mp_linVelocity->x=pPhysicalObject->pBody->getLinearVelocity().x();
	mp_linVelocity->y=pPhysicalObject->pBody->getLinearVelocity().y();
	mp_linVelocity->z=pPhysicalObject->pBody->getLinearVelocity().z();

	mp_rotVelocity->x=pPhysicalObject->pBody->getAngularVelocity().x();
	mp_rotVelocity->y=pPhysicalObject->pBody->getAngularVelocity().y();
	mp_rotVelocity->z=pPhysicalObject->pBody->getAngularVelocity().z();

	return true;
}

bool MFPhysicModuleObject::updateObject(){
//	MFObject::printInfo("MFPhysicModuleObject::updateObject - updating physical object");
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject==nullptr){
		MFObject::printErr("MFPhysicModuleObject::updateObject - pPhysicalObject"
				"==nullptr!");
		return false;
	}
	setMass(*mp_mass);
	setPosition(*mp_position);
	setGravity(*mp_gravity);
	setLinearVelocity(*mp_linVelocity);
	setAngularVelocity(*mp_rotVelocity);

	return true;
}

void MFPhysicModuleObject::setPosition(glm::vec3 &position){
	*mp_position=position;
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr && pPhysicalObject->pBody!=nullptr){
		pPhysicalObject->pBody->getWorldTransform().setOrigin(
				btVector3(position.x,position.y,position.z));
	}
}

void MFPhysicModuleObject::setLinearVelocity(glm::vec3 velocity){
	*mp_linVelocity=velocity;
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr && pPhysicalObject->pBody!=nullptr){
		pPhysicalObject->initLinearVelocity=btVector3(velocity.x,velocity.y,velocity.z);
		pPhysicalObject->pBody->setLinearVelocity(pPhysicalObject->initLinearVelocity);
	}
}

void MFPhysicModuleObject::setAngularVelocity(glm::vec3 velocity){
	*mp_rotVelocity=velocity;
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr && pPhysicalObject->pBody!=nullptr){
		pPhysicalObject->initRotVelocity=btVector3(velocity.x,velocity.y,velocity.z);
		pPhysicalObject->pBody->setAngularVelocity(pPhysicalObject->initRotVelocity);
	}
}

glm::vec3 MFPhysicModuleObject::getLinearVelocity(){
	glm::vec3 ret;
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr){
		ret.x=pPhysicalObject->pBody->getLinearVelocity().x();
		ret.y=pPhysicalObject->pBody->getLinearVelocity().y();
		ret.z=pPhysicalObject->pBody->getLinearVelocity().z();
	}
	return ret;
}

glm::vec3 MFPhysicModuleObject::getAngularVelocity(){
	glm::vec3 ret;
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr){
		ret.x=pPhysicalObject->pBody->getAngularVelocity().x();
		ret.y=pPhysicalObject->pBody->getAngularVelocity().y();
		ret.z=pPhysicalObject->pBody->getAngularVelocity().z();
	}
	return ret;
}

glm::vec3 MFPhysicModuleObject::getGravity(){
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr){
		mp_gravity->x=pPhysicalObject->pBody->getGravity().x();
		mp_gravity->y=pPhysicalObject->pBody->getGravity().y();
		mp_gravity->z=pPhysicalObject->pBody->getGravity().z();
	}
	return *mp_gravity;
}

void MFPhysicModuleObject::addImpulse(glm::vec3 additional){
  lockImpulse.lock();
  btVector3 additionalImpulse=btVector3(additional.x,additional.y,additional.z);
  impulse=impulse+additionalImpulse;
  impulseUpdate=true;
  lockImpulse.unlock();
}

void MFPhysicModuleObject::setOrientation(glm::vec3 rotationVector, float angle){
	btQuaternion orient=btQuaternion(
			btVector3(rotationVector.x,rotationVector.y,rotationVector.z),
			angle);
	PhysicalObjectData* pPhysicalObject = (PhysicalObjectData*)mp_physicalObject;
	btTransform trans=pPhysicalObject->pBody->getWorldTransform();
	trans.setRotation(orient);
}

void MFPhysicModuleObject::disable(){
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;

	if(pPhysicalObject->pBody!=nullptr)
    pPhysicalObject->pBody->setMassProps(.0f, btVector3(0,0,0));

	pWorld->removeRigidBody(pPhysicalObject->pBody);
}

void MFPhysicModuleObject::enable(){
  //TODO test
  P_WRN("not tested yet!!!");
  initPhysicalObject();
}

void MFPhysicModuleObject::update(){
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	pPhysicalObject->pShape=(btCollisionShape*)mp_syncObject->getGeometry()->createCollisionShape();
	pPhysicalObject->pBody->setCollisionShape(pPhysicalObject->pShape);
}

void MFPhysicModuleObject::updateCollisionShape(void* pShape){
	PhysicalObjectData*
			pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pBody!=nullptr){
		if(pShape!=nullptr){
			btCollisionShape* pS=(btCollisionShape*)pShape;
			pPhysicalObject->pBody->setCollisionShape(pS);
		}
	}
}

void MFPhysicModuleObject::updateSyncObject(MFSyncObject* syncObject){
//	setPosition(*syncObject->getModelPosition());
//	setOrientation(*syncObject->getModelOrientation());
}

bool MFPhysicModuleObject::synchronizeInput(){
//  setPosition(*mp_syncObject->getModelPosition());
  glm::mat4x4 *pGLMat=mp_syncObject->getGlobalModelMatrix();
	btScalar* pOGLMatrix=(btScalar*)pGLMat;
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	pPhysicalObject->pBody->getWorldTransform().setFromOpenGLMatrix(pOGLMatrix);
  lockImpulse.lock();
	if(impulseUpdate){
	  pPhysicalObject->pBody->applyCentralImpulse(impulse);
	  impulse=btVector3(0,0,0);
	  impulseUpdate=false;
	}
  lockImpulse.unlock();

//	pPhysicalObject->pMotionState->m_graphicsWorldTrans.setFromOpenGLMatrix(pOGLMatrix);
//	if(pPhysicalObject!=nullptr){
//		pPhysicalObject->mass=0;
//		pPhysicalObject->pBody->setMassProps(0, pPhysicalObject->pBody->getLocalInertia());
//	}

//  PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
//  btTransform worldTrafo=pPhysicalObject->pBody->getWorldTransform();
//
//  glm::vec3 posi;
//  posi.x=worldTrafo.getOrigin().x();
//  posi.y=worldTrafo.getOrigin().y();
//  posi.z=worldTrafo.getOrigin().z();
//  MFObject::printVec3(posi);
	return true;
}

bool MFPhysicModuleObject::synchronizeOutput(){
  outSyncTpCurrent=MFTickCounter::current();
  outputSyncIntervall=outSyncTpCurrent-outSyncTpLast;
  outSyncTpLast=MFTickCounter::current();
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	if(pPhysicalObject->pMotionState==nullptr){
		MFObject::printErr("MFPhysicModuleObject::synchronizeOutput - "
				"pPhysicalObject->pMotionState==nullptr!");
		return false;
	}
	const btTransform& worldTrafo=pPhysicalObject->pBody->getCenterOfMassTransform();

//  glm::vec3 posi;
//  posi.x=worldTrafo.getOrigin().x();
//  posi.y=worldTrafo.getOrigin().y();
//  posi.z=worldTrafo.getOrigin().z();
//  MFObject::printVec3(posi);
	glm::mat4x4 *pGLMat=mp_syncObject->getGlobalModelMatrix();
//  MFObject::printInfo("rotation matrix upper left 3x3  pre sync / post sync");
//  MFObject::printVec3(glm::vec3((*pGLMat)[0]));
//  MFObject::printVec3(glm::vec3((*pGLMat)[1]));
//  MFObject::printVec3(glm::vec3((*pGLMat)[2]));
  worldTrafo.getOpenGLMatrix((btScalar*)pGLMat);
//  MFObject::printVec3(glm::vec3((*pGLMat)[0]));
//  MFObject::printVec3(glm::vec3((*pGLMat)[1]));
//  MFObject::printVec3(glm::vec3((*pGLMat)[2]));
  mp_syncObject->triggerModuleInputSync();

//	glm::vec3 posi;
//	posi.x=worldTrafo.getOrigin().x();
//	posi.y=worldTrafo.getOrigin().y();
//	posi.z=worldTrafo.getOrigin().z();
//	mp_syncObject->setModelPosition(posi);
//	glm::vec3 u(0,0,0);
//	mp_syncObject->updateTranslationVector(u);
//
//	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
//	btTransform tra;
//	pPhysicalObject->pMotionState->getWorldTransform(tra);
//	tra.getOpenGLMatrix((btScalar*)mp_syncObject->getGlobalModelMatrix());

//	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
//	btTransform worldTrafo=pPhysicalObject->pMotionState->m_graphicsWorldTrans;
//	glm::vec3 transition = (getPosition()-(*mp_syncObject->getModelPosition()));
//	mp_syncObject->updateTranslationVector(transition);
//	btQuaternion quat=worldTrafo.getRotation();
//	float angle =quat.getAngle();
//	glm::vec3 rotationAxis;
//	btVector3 axis=quat.getAxis();
//	rotationAxis.x=axis.x();
//	rotationAxis.y=axis.y();
//	rotationAxis.z=axis.z();
//	mp_syncObject->setModelRotation(rotationAxis, angle);
	return true;
}

glm::vec3& MFPhysicModuleObject::getPosition(){
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	btTransform worldTrafo=pPhysicalObject->pMotionState->m_graphicsWorldTrans;
//	btTransform* worldTrafo=(btTransform*)mp_btTransform;
	//m_position.x=pPhysicalObject->initialTransform.getOrigin().x();
	mp_position->x=worldTrafo.getOrigin().x();
	mp_position->y=worldTrafo.getOrigin().y();
	mp_position->z=worldTrafo.getOrigin().z();
//	printVec3(&worldTrafo.getOrigin());
	return *mp_position;
}

void MFPhysicModuleObject::get4x4ModelMatrix(float* dst){
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	btTransform worldTrafo=pPhysicalObject->pBody->getWorldTransform();
	worldTrafo.getOpenGLMatrix(dst);
}
void MFPhysicModuleObject::set4x4ModelMatrix(float* dst){
	PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
	btTransform worldTrafo=pPhysicalObject->pBody->getWorldTransform();
	worldTrafo.setFromOpenGLMatrix(dst);
	pPhysicalObject->pBody->setWorldTransform(worldTrafo);
}

void MFPhysicModuleObject::printVec3(void* dat){
	btVector3 data=*((btVector3*)dat);
	std::string info="vec3: ";
	info+=std::to_string(data.x())+" / "+
			std::to_string(data.y())+" / "+
			std::to_string(data.z());
	MFObject::printInfo(info);
}

void MFPhysicModuleObject::printVec3(glm::vec3 data){
	std::string info="vec3: ";
	info+=std::to_string(data.x)+" / "+
			std::to_string(data.y)+" / "+
			std::to_string(data.z);
	MFObject::printInfo(info);
}
