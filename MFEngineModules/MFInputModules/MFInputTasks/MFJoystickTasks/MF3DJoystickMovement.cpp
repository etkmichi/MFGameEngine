/*
 * MF3DJoystickMovement.cpp
 *
 *  Created on: 27.01.2020
 *      Author: michl
 */

#include "MF3DJoystickMovement.h"

MF3DJoystickMovement::MF3DJoystickMovement() {
	mp_sceneUpwards=nullptr;
	mp_syncObject=nullptr;
	mp_lookAt=nullptr;
}

MF3DJoystickMovement::~MF3DJoystickMovement() {
}

void MF3DJoystickMovement::printJoystickAxis(
		JoystickAxeDispatchData* dispatchData){
	JoystickAxisAction* pJAA=dispatchData->pAction;
	if(pJAA==nullptr){
		P_WRN("(pJAA==nullptr || pSO==nullptr)!");
		return;
	}
	if(pJAA->axesCount<=0){
		P_WRN("pJAA->axesCount<=0!");
		return;
	}
	std::string info="Axis values:\n";
	for(int i=0;i<pJAA->axesCount;i++){
		info+="axis no. "+std::to_string(i)+"=";
		info+=std::to_string((pJAA->axesArray[i]))+"\n";
	}
	P_INF(info);
}

bool MF3DJoystickMovement::checkInput(MFSyncObject* pSO,JoystickAxisAction* pJAA){
	if(pJAA==nullptr || pSO==nullptr){
		MFObject::printErr("MF3DJoystickMovement::checkInput - "
				"(pJAA==nullptr || pSO==nullptr)!");
		return false;
	}
	if(pJAA->axesCount<=0){
		MFObject::printWarning("MF3DJoystickMovement::checkInput - "
				"pJAA->axesCount<=0!");
		return false;
	}
	return true;
}

bool MF3DJoystickMovement::executeJoystickAxeAction(
		JoystickAxeDispatchData* dispatchData){
	MFSyncObject* pSO=dispatchData->pObject;
	JoystickAxisAction* pJAA=dispatchData->pAction;
	if(!checkInput(pSO,pJAA))return false;


	updateWatchRight(pSO,pJAA);
	updateWatchUp(pSO,pJAA);
	updateMovement(pSO,pJAA);

	pSO->updateModelMatrix();
	return true;
}

void MF3DJoystickMovement::updateWatchUp(
		MFSyncObject* pSO,
		JoystickAxisAction* pJAA){
	mp_lookAt=pSO->getLocalLookAt();
	m_right=glm::cross(*mp_lookAt,*mp_sceneUpwards);
	m_right=glm::cross(*pSO->getLocalLookAt(),*mp_sceneUpwards);

	m_watchUpUpdate=0.0f;
	if(pJAA->axesCount>m_axisIndexRightStickFront){
		if(checkThreshold(pJAA->axesArray[m_axisIndexRightStickFront]))
			m_watchUpUpdate=pJAA->axesArray[m_axisIndexRightStickFront];
	}
	if(m_watchUpUpdate==0.0f)return;
	if(mp_lookAt->z <-0.99f && m_watchUpUpdate > 0.0f)
		return;

	if(mp_lookAt->z >0.99f && m_watchUpUpdate < 0.0f)
		return;
	m_rotationUpdate=glm::vec4(
		m_right.x,
		m_right.y,
		m_right.z,
		-m_rotationFactor*m_watchUpUpdate*(getMillisSinceLastExecution()));

	pSO->addLocalRotationUpdate(m_rotationUpdate);
}

void MF3DJoystickMovement::updateWatchRight(
		MFSyncObject* pSO,
		JoystickAxisAction* pJAA){
	mp_sceneUpwards=pSO->getUpwardsDirection();
	m_watchRightUpdate=0.0f;
	if(pJAA->axesCount>m_axisIndexRightStickRight){
		if(checkThreshold(pJAA->axesArray[m_axisIndexRightStickRight]))
			m_watchRightUpdate=pJAA->axesArray[m_axisIndexRightStickRight];
	}
	if(m_watchRightUpdate==0.0f)return;
	m_rotationUpdate=glm::vec4(
		mp_sceneUpwards->x,
		mp_sceneUpwards->y,
		mp_sceneUpwards->z,
		-m_rotationFactor*m_watchRightUpdate*(getMillisSinceLastExecution()));
	pSO->addLocalRotationUpdate(m_rotationUpdate);
}

void MF3DJoystickMovement::updateMovement(
		MFSyncObject* pSO,
		JoystickAxisAction* pJAA){
	m_movementUpdate.x=0;
	m_movementUpdate.y=0;
	m_movementUpdate.z=0;
	m_moveFrontUpdate=0.0f;
	if(pJAA->axesCount>m_axisIndexLeftStickFront){
		if(checkThreshold(pJAA->axesArray[m_axisIndexLeftStickFront]))
			m_moveFrontUpdate=pJAA->axesArray[m_axisIndexLeftStickFront];
	}
	if(m_moveFrontUpdate!=0.0f){
		m_moveFrontUpdate=m_moveFrontUpdate*m_forwardScale*getMillisSinceLastExecution();
		m_movementUpdate-=(m_moveFrontUpdate*(*mp_lookAt));
	}

	m_moveRightUpdate=0.0f;
	if(pJAA->axesCount>m_axisIndexLeftStickRight){
		if(checkThreshold(pJAA->axesArray[m_axisIndexLeftStickRight]))
			m_moveRightUpdate=pJAA->axesArray[m_axisIndexLeftStickRight];
	}
	if(m_moveRightUpdate!=0.0f){
		m_moveRightUpdate=m_moveRightUpdate*m_sidewardScale*getMillisSinceLastExecution();
		m_movementUpdate+=(m_moveRightUpdate*m_right);
	}
	pSO->addTranslationUpdate(m_movementUpdate);
}

bool MF3DJoystickMovement::checkThreshold(float val){
	return ((m_frontThreshhold*m_frontThreshhold)<(val*val));
}

void MF3DJoystickMovement::useMSXBOX1ControlerSetup(){
	m_axisIndexLeftStickBack=1;
	m_axisIndexLeftStickFront=1;
	m_axisIndexLeftStickLeft=0;
	m_axisIndexLeftStickRight=0;
	m_axisIndexRightStickFront=4;
	m_axisIndexRightStickBack=4;
	m_axisIndexRightStickLeft=3;
	m_axisIndexRightStickRight=3;
}
