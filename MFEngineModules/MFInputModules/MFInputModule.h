/*
 * MFInputModule.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULE_H_
#define MFENGINEMODULES_MFINPUTMODULE_H_

class MFInputModule;

#include "../MFBaseModule.h"
#include "MFInputModuleObject.h"
#include <GLFW/glfw3.h>
#include "../MFInterfacesModules/MFIModuleCreator.h"

#include "MFInputTasks/MFKeyboardTasks/MF3DMovmentIncrement.h"
#include "MFInputTasks/MFMouseTasks/MF3DMouseEgoView.h"
#include "MFInputTasks/MFJoystickTasks/MF3DJoystickMovement.h"


/**
 * The input module handles inputs with the glfw library and the already created
 * glfw window handle. Every sync object which is added to this module will be
 * connected to a MFInputModuleObject which must be set up with one or more
 * MFInputTask subclasses. The MFInputTask subclasses define how the sync object
 * must react to mouse,keyboard and joystick.
 */
class MFInputModule:
		public MFBaseModule,
		public MFIModuleCreator{
private:
	GLFWwindow
		*mp_defaultWindow;
//	MFSyncObject
//		*mp_camObject=nullptr;
//	MFInputModuleObject
//		*mp_comModuleObject=nullptr;
//	MF3DMovmentIncrement
//		*mp_keyInputTask=nullptr;
//	MF3DMouseEgoView
//		*mp_mouseInputTask=nullptr;
//	MF3DJoystickMovement
//		*mp_jsInputTask=nullptr;
protected:
	bool initModuleObject(
			MFSyncObject* syncObject,
			MFModuleObject* pObject);
	virtual MFModuleObject* createModuleObject(){
		return new MFInputModuleObject(mp_defaultWindow);
	};
public:
	/*inherited functions*/
	void updateResources(MFModuleResources* pRes);

	bool initModule();

//	bool syncInputData(std::vector<MFSyncObject*>* pVecInputData);
	bool executeModule();
//	bool syncOutputData();
	virtual MFBaseModule* createModule(){return new MFInputModule(mp_syncRes);};
public:
	MFInputModule(GLFWwindow* pWindow,MFSyncObjectResources* pSyncRes);
	MFInputModule(MFSyncObjectResources* pSyncRes);
	virtual ~MFInputModule();

	/**
	 * This function sets the default window for the glfw input polling.
	 * @param pWindow
	 */
	void setWindow(GLFWwindow *pWindow){mp_defaultWindow=pWindow;};

	void printInputDevices();
};

#endif /* MFENGINEMODULES_MFINPUTMODULE_H_ */
