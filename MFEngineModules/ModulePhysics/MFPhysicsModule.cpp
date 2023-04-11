/*
 * MFPhysicsModule.cpp
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#include "MFPhysicsModule.h"
#include "../MFModuleSyncTasks/MFPhysiscsModuleSync.h"
#include "../../MFObjectSetup/MFInitialObjectSetup.h"
#include <MFPhysicStructs.h>
#include <MFBasicDefines.h>

MFPhysicsModule::MFPhysicsModule(MFSyncObjectResources* pSyncRes) :
MFBaseModule(pSyncRes,"MFPhysicsModule"),
MFIModuleCreator("MFPhysicsModuleCreator"){
  setTaskName("MFPhysicsModule - Task");
	mp_physicsScene=new MFSimpleBulletManager();
	m_TStart=std::chrono::high_resolution_clock::now();
	m_TStop=std::chrono::high_resolution_clock::now();
	m_TCalcStart=std::chrono::high_resolution_clock::now();
	m_TCalcStop=std::chrono::high_resolution_clock::now();
	mp_physicsSyncTask=nullptr;
  gContactAddedCallback=MFPhysicsModule::mfCollisionCallback;
}

bool MFPhysicsModule::mfCollisionCallback(
    btManifoldPoint& cp,
    const btCollisionObjectWrapper* collisionObject0,
    int partId0,
    int index0,
    const btCollisionObjectWrapper* collisionObject1,
    int partId1,
    int index1){
  const btCollisionObject* colObject0=collisionObject0->getCollisionObject();
  const btCollisionObject* colObject1=collisionObject1->getCollisionObject();
  MFPhysicModuleObject* pMO0=nullptr;
  MFPhysicModuleObject* pMO1=nullptr;

  if(colObject0==nullptr){
    P_WRN("collision object 0 is nullptr!");
  }else{
    pMO0=static_cast<MFPhysicModuleObject*>(colObject0->getUserPointer());
  }

  if(colObject1==nullptr){
    P_WRN("collision object 1 is nullptr!");
  }else{
    pMO1=static_cast<MFPhysicModuleObject*>(colObject1->getUserPointer());
  }
  //TODO implement notifyCallback(cp,pMO...
  //TODO implement executeImmediateCallback()
  //TODO implement executeCollisionCallbacks in syncOutput()
  /*TODO will both objects pMO1&&pMO0 be executed?*/
  pMO0->executeCollisionCallbacks(cp,pMO1);
  pMO1->executeCollisionCallbacks(cp,pMO0);

  return true;
}

MFBaseModule* MFPhysicsModule::createModule(){
	MFPhysicsModule* pModule=new MFPhysicsModule(mp_syncRes);
	return pModule;
}

void MFPhysicsModule::initSyncTask(){
	mp_physicsSyncTask=new MFPhysiscsModuleSync(this,0,500);
	mp_physicsSyncTask->enableDataUpdate(false);
	mp_physicsSyncTask->enableModuleUpdate(false);
	mp_physicsSyncTask->initSyncTask();
	mp_syncRes->addSyncTask(mp_physicsSyncTask);
}

void MFPhysicsModule::updateResources(MFModuleResources* pRes){
}

MFPhysicsModule::~MFPhysicsModule() {
	// TODO Auto-generated destructor stub
}

bool MFPhysicsModule::initModule(){
	MFSimpleBulletManager *physicsScene=(MFSimpleBulletManager*)mp_physicsScene;
	m_TStart=std::chrono::high_resolution_clock::now();
	initSyncTask();
	return physicsScene->initBullet();
}

glm::vec3 MFPhysicsModule::getWorldGravity(){
  MFSimpleBulletManager *physicsScene=(MFSimpleBulletManager*)mp_physicsScene;
  return physicsScene->getGravity();
}

void MFPhysicsModule::setWorldGravity(const glm::vec3 &grav){
  MFSimpleBulletManager *physicsScene=(MFSimpleBulletManager*)mp_physicsScene;
  return physicsScene->setGravity(grav);
}

MFSimpleBulletManager* MFPhysicsModule::getPhysicsSceneManager(){
	return mp_physicsScene;
}

bool MFPhysicsModule::executeModule(){
	MFSimpleBulletManager *pPhysicsScene=(MFSimpleBulletManager*)mp_physicsScene;
	m_TStop=std::chrono::high_resolution_clock::now();
	m_TDuration=std::chrono::duration<float, std::chrono::seconds::period>(
			m_TStop-m_TStart).count();
	m_TCalcStart=std::chrono::high_resolution_clock::now();

	if(0==pPhysicsScene->stepSimulation(m_TDuration))
	  enableOutputSynchronisation(false);
	else
	  enableOutputSynchronisation(true);

	m_TCalcStop=std::chrono::high_resolution_clock::now();
	m_TStart=std::chrono::high_resolution_clock::now();
	m_TCalcDuation=std::chrono::duration<float, std::chrono::seconds::period>(m_TCalcStart-m_TCalcStop).count();

	m_TDurationSum+=m_TDuration;
	m_TCalcDurationSum+=m_TCalcDuation;
	m_durationAvgCounter++;
	m_calcDurationCounter++;

	if(m_TDuration>m_TDurationMax){
		m_TDurationMax=m_TDuration;
	}
	if(m_TDuration<m_TDurationMin){
		m_TDurationMin=m_TDuration;
	}
	if(m_TCalcDuation>m_TCalcDuationMax){
		m_TCalcDuationMax=m_TCalcDuation;
	}
	if(m_TCalcDuation<m_TCalcDuationMin){
		m_TCalcDuationMin=m_TCalcDuation;
	}
	return true;
}

btDynamicsWorld* MFPhysicsModule::getPhysicsWorld(){
  return mp_physicsScene->getWorld();
}

bool MFPhysicsModule::initModuleObject(
		MFSyncObject* syncObject,
		MFModuleObject* pObject){
	MFSimpleBulletManager *physicsScene=(MFSimpleBulletManager*)mp_physicsScene;
	MFPhysicModuleObject *pPhysicsObject=(MFPhysicModuleObject*)pObject;
	MFAbstractGeometry *pGeometry=syncObject->getGeometry();

	pPhysicsObject->setGeometry(pGeometry);
	if(!pPhysicsObject->initPhysicalObject()){
	  P_ERR("failed to init physical object!");
	  return false;
	}
	pPhysicsObject->setWorld(physicsScene->getWorld());

	physicsScene->addObject((PhysicalObjectData*)pPhysicsObject->getPhysicalObject());
  if(syncObject->isRecycleReuseTransition())
    pPhysicsObject->updateData();
	pPhysicsObject->setSyncObject(syncObject);
  pPhysicsObject->getCollisionObject()->setUserPointer(pPhysicsObject);
	return pPhysicsObject;
}


btRigidBody* MFPhysicsModule::getRigidBody(MFSyncObject* pSO){
  MFPhysicModuleObject* pPMO=
      static_cast<MFPhysicModuleObject*>(getModuleObject(pSO));
  return pPMO->getRigidBody();
}
