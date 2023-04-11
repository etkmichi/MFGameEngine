/*
 * MFFireObject.cpp
 *
 *  Created on: 30.01.2020
 *      Author: michl
 */

#include "MFFireObject.h"
#include <MFObjects/MFObject.h>
#include "../../../MFBaseModule.h"
MFFireObject::MFFireObject(
		MFAbstractGeometry* pGeo,
		MFModuleManager* pModMgr,
		MFSyncObject* pParentObject,
		MFModuleGroup* pSpawnGroup) {
	mp_geometry=pGeo;
	mp_modMgr=pModMgr;
	mp_spawnTask=new MFSpawnGeometry(mp_modMgr);
	mp_spawnTask->setSpawnGroup(pSpawnGroup);
	mp_spawnSetup=mp_spawnTask->getObjectSetup();
	mp_spawnSetup->lock();
	mp_spawnSetup->mp_initMass=&m_initialMass;
	mp_spawnSetup->mp_initVelocity=&m_initVelocity;
	if(mp_geometry!=nullptr){mp_spawnSetup->mp_geometry=mp_geometry;};
	mp_spawnSetup->unlock();
	mp_parentObject=pParentObject;
	updateInitialData();
	mp_tickCounter=new MFTickCounter();
}

MFFireObject::~MFFireObject() {
	delete mp_tickCounter;
	delete mp_spawnTask;
}

void MFFireObject::updateInitialData(){
	if(mp_parentObject!=nullptr){//()
	  mp_spawnSetup->lock();
		m_initVelocity=(*mp_parentObject->getLocalLookAt())*m_initialSpeed;
		mp_spawnSetup->mp_initPosition=mp_parentObject->getModelPosition();
		mp_spawnSetup->mp_initLookAt=mp_parentObject->getLocalLookAt();
    mp_spawnSetup->unlock();
	}
}

bool MFFireObject::executeKeyAction(KeyDispatchData* dispatchData){
	if(m_reloadTimeMS>mp_tickCounter->stop()){
		if(mp_latestPhysObject!=nullptr){
			m_initVelocity=(*mp_parentObject->getLocalLookAt())*m_initialSpeed;
			mp_latestPhysObject->setLinearVelocity(m_initVelocity);
		}
		return true;
	}
	if(dispatchData->action.key==m_keyboardKeyCode){
		updateInitialData();
		mp_spawnTask->doWork();
	}
	if(mp_physicsModule==nullptr && mp_modMgr->getMainPhysicsModule()->size()>0){
		mp_physicsModule=mp_modMgr->getMainPhysicsModule()->at(0);
	}
	mp_latestSyncO=mp_spawnTask->getLastSpawnedObject();
	if(mp_latestSyncO!=nullptr && mp_physicsModule!=nullptr){
		uint32_t index=mp_latestSyncO->getFirstModuleObjectIndex(
				mp_physicsModule->getSyncModuleID());
		mp_latestPhysObject=(MFPhysicModuleObject*)mp_physicsModule->getModuleObject(index);
	}
	mp_tickCounter->start();
	return true;
}

bool MFFireObject::executeMouseAction(MouseDispatchData* pDispatchData){

	return true;
}

bool MFFireObject::executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData){
	if(dispatchData->pAction->axesCount>m_joystickAxeIndex){
		float pRAxeValue=dispatchData->pAction->axesArray[m_joystickAxeIndex];
		float pLAxeValue=dispatchData->pAction->axesArray[m_joystickLFAxeIndex];
		if(pLAxeValue>m_jsLAxeTreshold && mp_latestSyncO!=nullptr && mp_physicsModule!=nullptr){
			MFPhysicModuleObject* pPO=mp_latestPhysObject;
			glm::vec3 viewPosition=
					pPO->getPosition()-(*mp_parentObject->getLocalLookAt());
			mp_parentObject->setModelPosition(viewPosition);
			if(pRAxeValue>m_jsAxeSpawnTreshold){
				m_initVelocity=(*mp_parentObject->getLocalLookAt())*m_initialSpeed;
				pPO->addImpulse(m_initVelocity/1000.0f);
			}
			return true;
		}
		if(pRAxeValue>m_jsAxeSpawnTreshold){
			if(m_reloadTimeMS>mp_tickCounter->stop()){
				if(mp_latestSyncO!=nullptr){
					m_initVelocity=(*mp_parentObject->getLocalLookAt())*m_initialSpeed;
					mp_latestPhysObject->setLinearVelocity(m_initVelocity);
				}
				return true;
			}
			updateInitialData();
 			mp_spawnTask->doWork();

 			if(mp_physicsModule==nullptr && mp_modMgr->getMainPhysicsModule()->size()>0)
 				mp_physicsModule=mp_modMgr->getMainPhysicsModule()->at(0);
 			mp_latestSyncO=mp_spawnTask->getLastSpawnedObject();
 			if(mp_latestSyncO!=nullptr && mp_physicsModule!=nullptr){
 				uint32_t index=mp_latestSyncO->getFirstModuleObjectIndex(
 						mp_physicsModule->getSyncModuleID());
 				mp_latestPhysObject=(MFPhysicModuleObject*)mp_physicsModule->getModuleObject(index);
 			}

 			mp_tickCounter->start();
		}
	}
	return true;
}

bool MFFireObject::executeJoystickHatAction(JoystickAxeDispatchData* dispatchData){

	return true;
}
