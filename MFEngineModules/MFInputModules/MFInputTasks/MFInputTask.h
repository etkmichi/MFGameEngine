/*
 * MFInputTask.h
 *
 *  Created on: 29.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFINPUTTASK_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFINPUTTASK_H_

class MFInputTask;

#include <MFTasks/MFAbstractTask.h>
#include <MFListenerInterfaces/MFInterfaceJoystickListener.h>
#include <MFListenerInterfaces/MFInterfaceKeyListener.h>
#include <MFListenerInterfaces/MFInterfaceMouseListener.h>
#include <MFTime/MFTickCounter.h>
#include "../../../MFSynchronisationClasses/MFSyncObject.h"
struct KeyDispatchData{
	KeyAction action;
	MFSyncObject *pObject;
};
struct JoystickButtonDispatchData{
	JoystickAction* pAction;
	MFSyncObject *pObject;
};
struct JoystickHatDispatchData{
	JoystickAction* pAction;
	MFSyncObject *pObject;
};
struct JoystickAxeDispatchData{
	JoystickAxisAction* pAction;
	MFSyncObject *pObject;
};
struct MouseDispatchData{
	double deltaX=0.0;
	double deltaY=.0;
	MFSyncObject *pSyncObject=nullptr;
	MFInterfaceMouseListener *pMouseListener=nullptr;
};
/**
 * The MFInputTask is needed for dispatching mouse, key and joystick events.
 * It provides some functions for a specific setup and five virtual functions
 * for implementation of the actions:
    virtual bool executeMouseAction(MFSyncObject* syncObject)
	virtual bool executeKeyAction(KeyDispatchData* dispatchData)
	virtual bool executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData)
	virtual bool executeJoystickButtonAction(JoystickButtonDispatchData* dispatchData)
	virtual bool executeJoystickHatAction(JoystickHatDispatchData* dispatchData)
 *	A MFInputTask will be executed by an MFInputModuleObject object. The
 *	MFInputModuleObject provides the necessary information about input events by
 *	calling the implemented execute* functions.
 */
class MFInputTask:
		public MFAbstractTask,
		public MFInterfaceJoystickListener {
public:
	static uint32_t
		ms_inputActionQueueSize;
public:/*virtual functions of MFInputTask*/
  /**
   * This function must implement all needed actions for specific keys.
   * @param dispatchData - data of the pressed key and the synchronisation object which
   * should be manipulated.
   * @return
   */
  virtual bool executeKeyAction(KeyDispatchData* dispatchData){return true;};
  virtual bool executeMouseAction(MouseDispatchData* pDispatchData){return true;};
  virtual bool executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData){return true;};
  virtual bool executeJoystickButtonAction(JoystickButtonDispatchData* dispatchData){return true;};
  virtual bool executeJoystickHatAction(JoystickHatDispatchData* dispatchData){return true;};
  /**
   * The common update will be called every time doWork is executed by a thread and
   * execution is enabled. TODO add common task callback for easy extensions -> cb shall be executed
   * during doWork()
   * @return
   */
  virtual bool executeCommonInputUpdate(){return true;};
  virtual bool undoWork(){return false;};
protected:
	MFInterfaceKeyListener
		*mp_keyListener;

	MFInterfaceMouseListener
		*mp_mouseListener;

	MFInterfaceJoystickListener
		*mp_joystickListener;

	std::vector<KeyDispatchData*>
		*mp_vecKeyDispatchData;

	std::vector<JoystickButtonDispatchData*>
		*mp_vecJSBtnDispatchData;

	std::vector<JoystickAxeDispatchData*>
		*mp_vecJSAxeDispatchData;

	std::vector<JoystickHatDispatchData*>
		*mp_vecJSHatDispatchData;

	uint32_t
		m_inputQueueSize,
		m_currentVecKeyIndex,
		m_axeQIndex,
		m_hatQIndex,
		m_btnIndex;

	MFSyncObject
		*mp_camObject;

	bool
		m_enableExecution=true,
		m_enableKeyExecution=true,
		m_enableJoystickAxeExecution=true,
		m_enableJoystickButtonExecution=true,
		m_enableJoystickHatExecution=true,
		m_enableMouseExecution=true,
		m_enableCommonUpdate=true;
	MouseDispatchData
		*mp_mouseDispatchData;

	double
		m_currentX=0,
		m_currentY=0,
		m_lastX=0,
		m_lastY=0;
	int64_t
	m_timeTaskExecution=0,
	m_timeSinceLastExecution=0;

	std::chrono::high_resolution_clock::time_point
		m_TStart,
		m_TStop,
		m_TCalcStart,
		m_TCalcStop;
	MFTickCounter*
	mp_ticker;
public:
	MFInputTask();
	virtual ~MFInputTask();

	void setSyncObject(MFSyncObject* syncObject){
		mp_camObject=syncObject;
		mp_mouseDispatchData->pSyncObject=mp_camObject;
	};

	void setKeyListener(MFInterfaceKeyListener* keyListener)
	{mp_keyListener=keyListener;};

	void setMouseListener(MFInterfaceMouseListener* mouseListener){
		mp_mouseListener=mouseListener;
		mp_mouseDispatchData->pMouseListener=mouseListener;
	};

	void setJoystickListener(MFInterfaceJoystickListener* joystickListener)
	{mp_joystickListener=joystickListener;};

	float getMillisSinceLastExecution(){return (m_timeSinceLastExecution);};

	/**
	 *
	 * @return the time for execution of the last doWork() iteration
	 */
	int64_t getTimeExecution(){return m_timeTaskExecution;};
	/**
	 * Can be used to enable/disable the execution. If disabled, the added key actions
	 * will still be cleared in the doWork()/execute() call.
	 * @param enable
	 */
	void enableExecution(bool enable){m_enableExecution=enable;};

	/**
	 * Adds a action which provides data for execution of this task. The doWork
	 * function will execute the tasks specific work with the added actions.
	 * @param action - the data of the action which occured.
	 * @param syncObject - the object which should be manipulated.
	 */
	void addKeyAction(KeyAction& action,MFSyncObject* syncObject);

  /**
   * Adds a action which provides data for execution of this task. The doWork
   * function will execute the tasks specific work with the added actions.
   * @param pAction - the data of the action which occured.
   * @param syncObject - the object which should be manipulated.
   */
	void addJoystickAxeAction(JoystickAxisAction* pAction,MFSyncObject* syncObject);

  /**
   * Adds a action which provides data for execution of this task. The doWork
   * function will execute the tasks specific work with the added actions.
   * @param pAction - the data of the action which occured.
   * @param syncObject - the object which should be manipulated.
   */
	void addJoystickButtonAction(JoystickAction* pAction,MFSyncObject* syncObject);

  /**
   * Adds a action which provides data for execution of this task. The doWork
   * function will execute the tasks specific work with the added actions.
   * @param pAction - the data of the action which occured.
   * @param syncObject - the object which should be manipulated.
   */
	void addJoystickHatAction(JoystickAction* pAction,MFSyncObject* syncObject);

	/**
	 * Depending on the implementation the do work will iterate over the added
	 * key-/mouse-/joystick-actions. Mouse actions are only ppolled by the interfaces
	 * poll functions.
	 * @return
	 */
	bool doWork();

	//TODO use bit mask with rotation -> enable(bitmask) -> easier disabling/enabling of multiple...
	void enableKeyExecution(bool enable){m_enableKeyExecution=enable;};
	void enableMouseExecution(bool enable){m_enableMouseExecution=enable;};
	void enableJoystickAxeExecution(bool enable){m_enableJoystickAxeExecution=enable;};
	void enableJoystickButtonExecution(bool enable){m_enableJoystickButtonExecution=enable;};
	void enableJoystickHatExecution(bool enable){m_enableJoystickHatExecution=enable;};
	void enableCommonUpdate(bool enable){m_enableCommonUpdate=enable;};

	/**
	 * This value may vary a lot! It depends on how often this task was enqueued!
	 * Don't use it for distance calculation!
	 * @return
	 */
	float getSecondsSinceLastExecution(){
	  float time=(m_timeSinceLastExecution/1000.0f);
		return time;
	};
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFINPUTTASK_H_ */
