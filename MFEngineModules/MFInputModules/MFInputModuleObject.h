/*
 * MFInputModuleObject.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTMODULEOBJECT_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTMODULEOBJECT_H_

class MFInputModuleObject;

#include "../MFModuleObject.h"
#include "MFInputTasks/MFInputTask.h"
#include "MFListenerInterfaces/MFInterfaceKeyListener.h"
#include "MFListenerInterfaces/MFInterfaceMouseListener.h"
#include "MFListenerInterfaces/MFInterfaceJoystickListener.h"
/**
 * The input module object implements following interfaces for key listening:
 * MFInterfaceKeyListener, MFInterfaceMouseListener, MFInterfaceJoystickListener
 * To react on mouse, key or joystick input, an object of MFInputTask class must be added!
 * The MFInputTask object must implement the action of a input event.
 */
class MFInputModuleObject:
		public MFModuleObject,
		public MFInterfaceKeyListener,
		public MFInterfaceMouseListener,
		public MFInterfaceJoystickListener{
private:
	std::vector<MFInputTask*>
		*mp_vecKeyboardTasks,
		*mp_vecMouseTasks,
		*mp_vecJoystickAxeTasks,
		*mp_vecJoystickButtonTasks,
		*mp_vecJoystickHatTasks;

	std::mutex
		lockKeyboardTasks,
		lockMouseTasks,
		lockJoystickTasks;
	GLFWwindow
		*mp_window=nullptr;

protected:
	void setupObjectDataStructure(){
		//TODO use index of sync object to change sync object for input
		//TODO multiple objects could use one sync object - make sure that there is
		//no collision and the sync object returns a vector of module objects if
		//getModuleObject(...) is called!
		return;
	};
	void updateSyncObject(MFSyncObject* syncObject);

public:/*virtual functions MFInterfaceKeyListener*/
  /**
   * Iterates over the added keyboard taks and adds the given key action data.
   * The keyAction(...) function will be called by pollKeyboardEvents()!
   * @param key
   * @param action
   * @param mods
   */
//  virtual void keyAction(int key,int action,int mods);
  virtual void keyAction(const KeyAction &action);

public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){return true;};
  virtual bool updateObject(){return true;};
  virtual void disable(){};
  virtual void enable(){};
  virtual bool synchronizeInput();
  virtual bool synchronizeOutput();

public:
	MFInputModuleObject(GLFWwindow* pWindow);
	MFInputModuleObject(MFSyncObject *pSyncObject,GLFWwindow* pWindow);
	virtual ~MFInputModuleObject();

	/**
	 * This function binds a task for execution. All registered inputs will be given
	 * the MFInputTask* objects for execution. More than one action is possible for the same
	 * key. The keyAction(...) function will give all registered key actions to all
	 * added input tasks, even if they are not dispatched by the task!
	 * @param key
	 * @param keyTask
	 */
	void addKeyboardTask(MFInputTask* task);
	void addMouseTask(MFInputTask* task);
	void addJoystickAxeTask(MFInputTask* task);
	void addJoystickButtonTask(MFInputTask* task);
	void addJoystickHatTask(MFInputTask* task);

	/*Inherited from MFInterfaceJoystickListener*/
	void joystickAxeAction(JoystickAxisAction *axisActionData);
	void joystickButtonAction(JoystickAction *btnActionData);
	void joystickHatAction(JoystickAction* hatActionData);

	/**
	 * This function will poll for the implemented interfaces (keyboard, mouse, joystick).
	 */
	void pollActions();

	/**
	 * After pollActions() was called, this function can be used to execute the input tasks.
	 * TODO externalExecution with taskThread -> create addMouseAction...
	 */
	void executeInputTasks();

	void setWindow(GLFWwindow *pWindow){
		((MFInterfaceMouseListener*)this)->setWindow(pWindow);
		mp_window=pWindow;
	};
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTMODULEOBJECT_H_ */
