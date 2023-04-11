/*
 * MF3DEgoView.cpp
 *
 *  Created on: 29.10.2019
 *      Author: michl
 */

#include "MF3DMouseEgoView.h"
#include <GLFW/glfw3.h>

MF3DMouseEgoView::MF3DMouseEgoView(glm::vec3* pLocalLookAt,glm::vec3* upwards) {
	mp_lookAt=pLocalLookAt;
	mp_viewUpwards=new glm::vec3();
	*mp_viewUpwards=*upwards;
	mp_sceneUpwards=upwards;
	mp_syncObject=nullptr;
}
MF3DMouseEgoView::MF3DMouseEgoView(MFSyncObject* pSyncObj) {
	mp_syncObject=pSyncObj;
	mp_lookAt=mp_syncObject->getLocalLookAt();
	*mp_lookAt=glm::vec3(1,0,0);
	mp_viewUpwards=new glm::vec3();
	*mp_viewUpwards=*mp_syncObject->getUpwardsDirection();
	mp_sceneUpwards=mp_syncObject->getUpwardsDirection();
	m_right=glm::cross(*mp_lookAt,*mp_sceneUpwards);
}
MF3DMouseEgoView::~MF3DMouseEgoView() {
	// TODO Auto-generated destructor stub
}

bool MF3DMouseEgoView::executeMouseAction(MouseDispatchData* dispatchData){
	if(dispatchData->pMouseListener->pollButtonState(GLFW_MOUSE_BUTTON_LEFT)
			==GLFW_PRESS){
		/*look left/right update*/
		m_rotationUpdate=glm::vec4(
			mp_sceneUpwards->x,
			mp_sceneUpwards->y,
			mp_sceneUpwards->z,
			m_rotationFactor*dispatchData->deltaX);
		mp_syncObject->addLocalRotationUpdate(m_rotationUpdate);

		/*look up down update*/
		m_right=glm::cross(*mp_syncObject->getLocalLookAt(),*mp_sceneUpwards);
		m_right=glm::normalize(m_right);
		m_rotationUpdate=glm::vec4(
			m_right.x,
			m_right.y,
			m_right.z,
			m_rotationFactor*dispatchData->deltaY);
		mp_syncObject->addLocalRotationUpdate(m_rotationUpdate);

		mp_syncObject->updateModelMatrix();
	}else{
		if(dispatchData->pMouseListener->
				pollButtonState(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
			*mp_lookAt=glm::vec3(1,0,0);//;+(*mp_syncObject->getModelPosition());
			*mp_lookAt=glm::normalize(*mp_lookAt);
		}
	}
	return true;
}

bool MF3DMouseEgoView::executeKeyAction(KeyDispatchData* dispatchData){

	return true;
}
