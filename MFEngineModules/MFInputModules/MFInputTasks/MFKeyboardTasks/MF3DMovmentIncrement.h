/*
 * MF3DWalkingIncrement.h
 *
 *  Created on: 29.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFKEYBOARDTASKS_MF3DMOVMENTINCREMENT_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFKEYBOARDTASKS_MF3DMOVMENTINCREMENT_H_
#include "../MFInputTask.h"
#include <glm/glm.hpp>
class MF3DMovmentIncrement : public MFInputTask {
public:
	glm::vec3
		*mp_upwards,
		*mp_forward,
		*mp_right;

	glm::vec3
		m_movementUpdate;

	float
		m_forwardScale	= 3.0f,
		m_sidewardScale	= 3.0f,
		m_upwardScale	= 3.0f,
		m_backwardScale	= 3.0f,
		m_downwardScale = 3.0f;

	MFSyncObject
		*mp_syncObj;

	MF3DMovmentIncrement(glm::vec3 *vecForward,glm::vec3 *vecUpward);
	MF3DMovmentIncrement(MFSyncObject* pSyncObj);

	virtual ~MF3DMovmentIncrement();
	void enableUpward(bool enable);
	bool isUpwardEnabled();
	void setKeys(int forward,int backward,int left,int right);

	/**
	 * Sets the inner forward vector to the given pointer,
	 * The data of the pointer must only be changed by the same
	 * thread!!!
	 * @param forwardVector
	 */
	void setForwardVectorPointer(glm::vec3 *forwardVector);

	glm::vec3& getPositionIncrement();

	bool executeKeyAction(KeyDispatchData* dispatchData);
	bool executeMouseAction(MouseDispatchData* dispatchData);

};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFMOUSETASKS_MF3DWALKINGINCREMENT_H_ */
