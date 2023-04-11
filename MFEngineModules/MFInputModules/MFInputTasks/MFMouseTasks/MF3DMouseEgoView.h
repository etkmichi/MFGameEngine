/*
 * MF3DEgoView.h
 *
 *  Created on: 29.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFMOUSETASKS_MF3DMOUSEEGOVIEW_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFMOUSETASKS_MF3DMOUSEEGOVIEW_H_
#include "../MFInputTask.h"
#include <glm/glm.hpp>

#include "../MFKeyboardTasks/MF3DMovmentIncrement.h"
class MF3DMouseEgoView : public MFInputTask{
private:
	glm::vec3
		*mp_lookAt,
		*mp_sceneUpwards,
		*mp_viewUpwards;
	float
		m_rotationFactor=0.001;
	MFSyncObject
		*mp_syncObject;
	glm::vec4
		m_rotationUpdate;
	glm::vec3
		m_right;
public:
	/**
	 *
	 * @param pLocalLookAt - pointer to a normalized vector.
	 * Should represent the view direction
	 * @param upwards - upwards direction needed for looking right and left.
	 */
	MF3DMouseEgoView(glm::vec3* pLocalLookAt,glm::vec3* upwards);
	MF3DMouseEgoView(MFSyncObject* pSyncObj);
	virtual ~MF3DMouseEgoView();

	/**
	 * If the walking input is set by this function, the execution of this
	 * task will update the "look at" vector of the given object.
	 * @param pWalkingInput
	 */
	void setWalkingInputTask(MF3DMovmentIncrement *pWalkingInput);

	void setLookAtPointer(glm::vec3* pLookAt);
	void setUpwardPointer(glm::vec3* upward);

	glm::vec3* getLookAt();
	glm::vec3* getUpwards();

	bool executeMouseAction(MouseDispatchData* dispatchData);
	bool executeKeyAction(KeyDispatchData* dispatchData);
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFMOUSETASKS_MF3DMOUSEEGOVIEW_H_ */
