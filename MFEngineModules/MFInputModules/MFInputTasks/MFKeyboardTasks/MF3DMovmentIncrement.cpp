/*
 * MF3DWalkingIncrement.cpp
 *
 *  Created on: 29.10.2019
 *      Author: michl
 */

#include "MF3DMovmentIncrement.h"

MF3DMovmentIncrement::MF3DMovmentIncrement(
		glm::vec3 *vecForward,
		glm::vec3 *vecUpward) {
	mp_upwards=vecUpward;
	mp_forward=vecForward;
	mp_right=new glm::vec3();
	mp_syncObj=nullptr;
	*mp_right=glm::cross(*mp_upwards, *mp_forward);
	m_enableKeyExecution=true;
	m_movementUpdate=glm::vec3(0,0,0);
}
MF3DMovmentIncrement::MF3DMovmentIncrement(MFSyncObject* pSyncObj) {
	mp_upwards=pSyncObj->getUpwardsDirection();
	mp_forward=pSyncObj->getLocalLookAt();
	mp_right=new glm::vec3();
	*mp_right=glm::cross(*mp_upwards, *mp_forward);
	m_enableKeyExecution=true;
	mp_syncObj=pSyncObj;
}
MF3DMovmentIncrement::~MF3DMovmentIncrement() {
	// TODO Auto-generated destructor stub
}


bool MF3DMovmentIncrement::executeKeyAction(KeyDispatchData* dispatchData){
	MFSyncObject* pSyncObj=dispatchData->pObject;
	if(pSyncObj==nullptr){
		pSyncObj=mp_syncObj;
	}
	m_movementUpdate=glm::vec3(0,0,0);
	float durationFactor=getSecondsSinceLastExecution();
	if(dispatchData->action.key==GLFW_KEY_W){
		m_movementUpdate+=durationFactor*m_forwardScale*
				(*pSyncObj->getLocalLookAt());
	}
	if(dispatchData->action.key==GLFW_KEY_S){
		m_movementUpdate+=-durationFactor*m_backwardScale*
				(*pSyncObj->getLocalLookAt());
	}
	if(dispatchData->action.key==GLFW_KEY_A){
		m_movementUpdate+=durationFactor*m_sidewardScale*(*mp_right);
	}
	if(dispatchData->action.key==GLFW_KEY_D){
		m_movementUpdate+=(-durationFactor*m_sidewardScale*(*mp_right));
	}
	pSyncObj->addTranslationUpdate(m_movementUpdate);

	*mp_right=glm::cross(*pSyncObj->getUpwardsDirection(),
			*pSyncObj->getLocalLookAt());
	*mp_right=glm::normalize(*mp_right);
	return true;
}

bool MF3DMovmentIncrement::executeMouseAction(MouseDispatchData* dispatchData)
{
	return true;
}
