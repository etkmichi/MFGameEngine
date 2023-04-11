/*
 * MFInputModuleObject.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include "MFInputModuleObject.h"

MFInputModuleObject::MFInputModuleObject(GLFWwindow* pWindow)
{
	mp_vecKeyboardTasks=new std::vector<MFInputTask*>();
	mp_vecMouseTasks=new std::vector<MFInputTask*>();
	mp_vecJoystickAxeTasks=new std::vector<MFInputTask*>();
	mp_vecJoystickButtonTasks=new std::vector<MFInputTask*>();
	mp_vecJoystickHatTasks=new std::vector<MFInputTask*>();
	setWindow(pWindow);
	setSetupName("MFInputModuleObject");
}

MFInputModuleObject::MFInputModuleObject(MFSyncObject *pSyncObject,GLFWwindow* pWindow)
{
	mp_vecKeyboardTasks=new std::vector<MFInputTask*>();
	mp_vecMouseTasks=new std::vector<MFInputTask*>();
	mp_vecJoystickAxeTasks=new std::vector<MFInputTask*>();
	mp_vecJoystickButtonTasks=new std::vector<MFInputTask*>();
	mp_vecJoystickHatTasks=new std::vector<MFInputTask*>();
	mp_syncObject=pSyncObject;
	setWindow(pWindow);
}

MFInputModuleObject::~MFInputModuleObject() {
	// TODO Auto-generated destructor stub
}

void MFInputModuleObject::addKeyboardTask(MFInputTask* task){
	mp_vecKeyboardTasks->push_back(task);
	task->setKeyListener(this);
	task->setSyncObject(mp_syncObject);
}

void MFInputModuleObject::addMouseTask(MFInputTask* task){
	mp_vecMouseTasks->push_back(task);
	task->setMouseListener(this);
	task->setSyncObject(mp_syncObject);
}

void MFInputModuleObject::addJoystickAxeTask(MFInputTask* task){
	mp_vecJoystickAxeTasks->push_back(task);
	task->setJoystickListener(this);
	task->setSyncObject(mp_syncObject);
}

void MFInputModuleObject::addJoystickButtonTask(MFInputTask* task){
	mp_vecJoystickButtonTasks->push_back(task);
	task->setJoystickListener(this);
	task->setSyncObject(mp_syncObject);
}

void MFInputModuleObject::addJoystickHatTask(MFInputTask* task){
	mp_vecJoystickHatTasks->push_back(task);
	task->setJoystickListener(this);
	task->setSyncObject(mp_syncObject);
}

void MFInputModuleObject::pollActions(){
	//poll keyboard events, poll joystrick events (if enabled), poll mouse actions
	if(mp_window==nullptr){
		MFObject::printWarning(
				"MFInputModuleObject::pollActions() - "
				"No window pointer set -> No key dispatch able!!");
	}
	pollKeyboardEvents(mp_window);/*calls keyAction() -> update of key input tasks*/
	pollJoystickEvents();/*calls joystick...Action() -> update of input tasks*/
}

void MFInputModuleObject::executeInputTasks(){
	for(MFInputTask *pTask:*mp_vecKeyboardTasks)
		pTask->execute();
	for(MFInputTask *pTask:*mp_vecMouseTasks){
		pTask->execute();
	}
	for(MFInputTask *pTask:*mp_vecJoystickAxeTasks){
		pTask->execute();
	}
	for(MFInputTask *pTask:*mp_vecJoystickButtonTasks)
		pTask->execute();
	for(MFInputTask *pTask:*mp_vecJoystickHatTasks)
		pTask->execute();
}

void MFInputModuleObject::updateSyncObject(MFSyncObject* syncObject){

}

bool MFInputModuleObject::synchronizeInput(){
	return true;
}

bool MFInputModuleObject::synchronizeOutput(){
	pollActions();
	executeInputTasks();
	return true;
}

//void MFInputModuleObject::keyAction(int key,int action,int mods){
//	KeyAction kAction={};
//	kAction.key=key;
//	kAction.action=action;
//	kAction.mods=mods;
//	for(MFInputTask *pTask: *mp_vecKeyboardTasks){
//		pTask->addKeyAction(kAction,mp_syncObject);
//	}
//}
void MFInputModuleObject::keyAction(const KeyAction &action){
  KeyAction kAction=action;
//  kAction.key=key;
//  kAction.action=action;
//  kAction.mods=mods;
  for(MFInputTask *pTask: *mp_vecKeyboardTasks){
    pTask->addKeyAction(kAction,mp_syncObject);
  }
}

void MFInputModuleObject::joystickAxeAction(JoystickAxisAction* axisActionData){
	for(MFInputTask *pTask:*mp_vecJoystickAxeTasks){
		pTask->addJoystickAxeAction(axisActionData,mp_syncObject);
	}
}

void MFInputModuleObject::joystickButtonAction(JoystickAction* btnActionData){
	for(MFInputTask *pTask:*mp_vecJoystickButtonTasks){
		pTask->addJoystickButtonAction(btnActionData,mp_syncObject);
	}
}

void MFInputModuleObject::joystickHatAction(JoystickAction* hatActionData){
	for(MFInputTask *pTask:*mp_vecJoystickHatTasks){
		pTask->addJoystickHatAction(hatActionData,mp_syncObject);
	}
}

