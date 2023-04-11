/*
 * MF3DJoystickMovement.h
 *
 *  Created on: 27.01.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFJOYSTICKTASKS_MF3DJOYSTICKMOVEMENT_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFJOYSTICKTASKS_MF3DJOYSTICKMOVEMENT_H_
#include "../MFInputTask.h"
class MF3DJoystickMovement : public MFInputTask {
private:
	int
		m_axisIndexLeftStickBack=1,
		m_axisIndexLeftStickFront=1,
		m_axisIndexLeftStickLeft=0,
		m_axisIndexLeftStickRight=0,
		m_axisIndexRightStickFront=4,
		m_axisIndexRightStickBack=4,
		m_axisIndexRightStickLeft=3,
		m_axisIndexRightStickRight=3;

	float
		m_frontThreshhold=.10f,
		m_watchUpUpdate=0.0f,
		m_watchRightUpdate=.0f,
		m_moveFwdUpdate=.0f,
		m_rotationFactor=0.001,
		m_forwardScale	= 0.01f,
		m_sidewardScale	= 0.01f,
		m_upwardScale	= 0.01f,
		m_backwardScale	= 0.01f,
		m_downwardScale = 0.01f,
		m_moveFrontUpdate = .0f,
		m_moveBackUpdate = .0f,
		m_moveRightUpdate = .0f,
		m_moveLeftUpdate = .0f;

	glm::vec3
		*mp_lookAt,//forward
		*mp_sceneUpwards,
		m_right,
		m_movementUpdate;

	glm::vec4
		m_rotationUpdate;

	MFSyncObject
		*mp_syncObject;
public:
	MF3DJoystickMovement();
	virtual ~MF3DJoystickMovement();
	bool executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData);

	/**
	 * Sets up the movement task for usage with a ms controler.
	 */
	void useMSXBOX1ControlerSetup();

	void setAxisIndexFront(int axisIndex);
	void setAxisIndexBack(int axisIndex);
	void setAxisIndexRight(int axisIndex);
	void setAxisIndexLeft(int axisIndex);
	void setAxisIndexWatchUp(int axisIndex);
	void setAxisIndexWatchDown(int axisIndex);
	void setAxisIndexWatchLeft(int axisIndex);
	void setAxisIndexWatchRight(int axisIndex);

	void printJoystickAxis(JoystickAxeDispatchData* dispatchData);
private:
	void updateMovement(MFSyncObject* pSO,JoystickAxisAction* pJAA);
	void updateWatchUp(MFSyncObject* pSO,JoystickAxisAction* pJAA);
	void updateWatchRight(MFSyncObject* pSO,JoystickAxisAction* pJAA);
	bool checkInput(MFSyncObject* pSO,JoystickAxisAction* pJAA);
	bool checkThreshold(float value);
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFJOYSTICKTASKS_MF3DJOYSTICKMOVEMENT_H_ */
