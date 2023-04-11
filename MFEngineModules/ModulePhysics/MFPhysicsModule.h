/*
 * MFPhysicsModule.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFPHYSICSMODULE_H_
#define MFENGINEMODULES_MFPHYSICSMODULE_H_

class MFPhysicsModule;

#include <stdio.h>
#include <vector>

#include "../MFBaseModule.h"
#include "../MFModuleObject.h"
#include "MFPhysicModuleObject.h"
#include "../MFInterfacesModules/MFIModuleCreator.h"
#include "../MFModuleSyncTasks/MFModuleSyncTask.h"
#include <MFBulletWrapper/MFSimpleBulletManager.h>
#include <btBulletCollisionCommon.h>
//#include <MFPhysicsObjectSetup/MFPhysicalObjectInitSetup.h>

class MFPhysicsModule :
		public MFBaseModule,
		public MFIModuleCreator {
private:
	std::chrono::high_resolution_clock::time_point
		m_TStart,
		m_TStop,
		m_TCalcStart,
		m_TCalcStop;

	float
		m_TDuration=0.0f,
		m_TDurationMax=0.0f,
		m_TDurationMin=1.0f,
		m_TDurationSum=0.0f,
		m_TCalcDuation=0.0f,
		m_TCalcDurationSum=0.0f,
		m_TCalcDuationMax=0.0f,
		m_TCalcDuationMin=1.0f;

	uint64_t
		m_durationAvgCounter=0,
		m_calcDurationCounter=0;

	MFSimpleBulletManager
		*mp_physicsScene;

	MFModuleSyncTask
		*mp_physicsSyncTask;

protected:/*virtual functions of MFBaseModule*/
	//TODO create MFPhysicalObjectInitSetup out of the sync objects init setup data.
	//TODO implement usage of MFPhysicalObjectInitSetup object in physics library
	virtual bool initModuleObject(MFSyncObject* syncObject,MFModuleObject* pObject);

public:/*virtual functions of MFBaseModule*/
	virtual MFModuleObject* createModuleObject(){
		MFPhysicModuleObject* pModuleObject=new MFPhysicModuleObject();
 		return pModuleObject;
	};
	virtual void updateResources(MFModuleResources* pRes);
	virtual MFBaseModule* createModule();
  virtual bool initModule();
  virtual bool executeModule();

public:/*collision callback function*/
  static bool mfCollisionCallback(
      btManifoldPoint& cp,
      const btCollisionObjectWrapper* collisionObject0,
      int partId0,
      int index0,
      const btCollisionObjectWrapper* collisionObject1,
      int partId1,
      int index1);

public:
	MFPhysicsModule(MFSyncObjectResources* pSyncRes);
	virtual ~MFPhysicsModule();
	MFModuleSyncTask* getPhysicsSyncTask(){return mp_physicsSyncTask;};
	bool setupSyncObject(MFSyncObject* syncObject);

	/**
	 * Initializes the synchronization task. The synchronization task will be added
	 * to the MFSyncObjectResources.
	 */
	void initSyncTask();

//	bool syncOutputData(); default is enough...
	/**
	 * returns a void* which is a MFSimpleBulletManager*!
	 */
	MFSimpleBulletManager* getPhysicsSceneManager();

  glm::vec3 getWorldGravity();
  void setWorldGravity(const glm::vec3 &grav);

  btRigidBody* getRigidBody(MFSyncObject* pSO);
  btDynamicsWorld* getPhysicsWorld();
};

#endif /* MFENGINEMODULES_MFPHYSICSMODULE_H_ */
