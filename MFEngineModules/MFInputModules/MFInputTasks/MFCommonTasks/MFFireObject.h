/*
 * MFFireObject.h
 *
 *  Created on: 30.01.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFCOMMONTASKS_MFFIREOBJECT_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFCOMMONTASKS_MFFIREOBJECT_H_
#include "../MFInputTask.h"
#include "../../../../MFSceneActions/MFSpawnActions/MFSpawnGeometry.h"
#include "../../../MFModuleManager.h"
#include "../../../../MFObjectSetup/MFInitialObjectSetup.h"
#include "../../../MFModuleGroup.h"
#include <MFBaseGeometries/MFAbstractGeometry.h>
#include <MFTime/MFTickCounter.h>
#include <glm/glm.hpp>

class MFFireObject: public MFInputTask {
	int
		m_keyboardKeyCode=GLFW_KEY_SPACE,
		m_mouseKeyCode=GLFW_MOUSE_BUTTON_LEFT,
		m_joystickKeyIndex=0,
		m_joystickAxeIndex=5,
		m_joystickLFAxeIndex=2;
	float
		m_reloadTimeMS=500,
		m_initialMass=5.0f,
		m_jsAxeSpawnTreshold=-0.95f,
		m_jsLAxeTreshold=-0.95f,
		m_initialSpeed=20.00f;
	glm::vec3
		m_initVelocity;

	MFAbstractGeometry
		*mp_geometry;
	MFSpawnGeometry
		*mp_spawnTask;
	MFInitialObjectSetup
		*mp_spawnSetup;
	MFModuleManager
		*mp_modMgr;
	MFSyncObject
		*mp_parentObject;
	MFTickCounter
		*mp_tickCounter;
	MFPhysicModuleObject
		*mp_latestPhysObject=nullptr;
	MFSyncObject
		*mp_latestSyncO=nullptr;
	MFBaseModule
		*mp_physicsModule=nullptr;

public:
	MFFireObject(
			MFAbstractGeometry* pGeo,
			MFModuleManager* pModMgr,
			MFSyncObject* pParentObject,
			MFModuleGroup* pSpawnGroup);
	virtual ~MFFireObject();
	void setGeometry(MFAbstractGeometry* pGeo){mp_geometry=pGeo;};
	MFSpawnGeometry* getSpawnTask(){return mp_spawnTask;};
	void setKeyBoardSpawnKey(int keyCode){m_keyboardKeyCode=keyCode;};
	void setMouseSpawnKey(int keyCode){m_mouseKeyCode=keyCode;};
	void setJoystickSpawnKey(int keyCode){m_joystickKeyIndex=keyCode;};
	void setJoystickSpawnAxe(int axeIndex){m_joystickAxeIndex=axeIndex;};

	bool executeKeyAction(KeyDispatchData* dispatchData);
	bool executeMouseAction(MouseDispatchData* pDispatchData);
	bool executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData);
	bool executeJoystickHatAction(JoystickAxeDispatchData* dispatchData);

	void updateInitialData();
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFCOMMONTASKS_MFFIREOBJECT_H_ */
