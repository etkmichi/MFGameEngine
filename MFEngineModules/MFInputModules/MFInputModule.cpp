/*
 * MFInputModule.cpp
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#include "MFInputModule.h"
MFInputModule::MFInputModule(GLFWwindow* pWindow,
		MFSyncObjectResources* pSyncRes) :
MFBaseModule(pSyncRes),
MFIModuleCreator("MFInputModule") {
  setSetupName("MFInputModule");
	mp_defaultWindow=pWindow;
	enableInputSynchronisation(false);
}

MFInputModule::MFInputModule(MFSyncObjectResources* pSyncRes) :
MFBaseModule(pSyncRes),
MFIModuleCreator("MFInputModule") {
	mp_defaultWindow=nullptr;
	enableInputSynchronisation(false);
	MFObject::printWarning("MFInputModule::MFInputModule() - no window set! window is needed for"
			" input library!");
}

MFInputModule::~MFInputModule() {
}

bool MFInputModule::executeModule(){
	/*Iterate through the modules objects and poll the input actions*/
//	MFInputModuleObject* pIObject=nullptr;
//	uint64_t count=*((uint64_t*)mp_vecModuleObject);
//	MFObject::printInfo(std::to_string(count));
//	for(ObjectData *pObject : *mp_vecModuleObject){
//		pIObject=(MFInputModuleObject*)(pObject->pModuleData);
//		pIObject->pollActions();
//	}
	return true;
};

/*inherited functions*/
bool MFInputModule::initModule(){/*Get the glfw window for the module object creation*/
	if(mp_res!=nullptr && mp_defaultWindow==nullptr){
		if(mp_res->m_vecWindows.size()>0){
			mp_defaultWindow=mp_res->m_vecWindows.at(mp_res->m_vecWindows.size()-1);
		}else{
			MFObject::printWarning("MFInputModule::initModule - no window found!");
		}
	}
//	if(mp_res->mp_vecCamObjects!=nullptr && mp_res->mp_vecCamObjects->size()>0){
//		mp_camObject=mp_res->mp_vecCamObjects->at(mp_res->mp_vecCamObjects->size()-1);
//		if(addSyncObject(mp_camObject)){
//			mp_comModuleObject=(MFInputModuleObject*)getFirstModuleObject(mp_camObject);
//			mp_keyInputTask=new MF3DMovmentIncrement(mp_camObject);
//			mp_mouseInputTask=new MF3DMouseEgoView(mp_camObject);
//			mp_jsInputTask=new MF3DJoystickMovement();
//			mp_comModuleObject->addKey(GLFW_KEY_W);//TODO this is external!
//			mp_comModuleObject->addKey(GLFW_KEY_A);
//			mp_comModuleObject->addKey(GLFW_KEY_S);
//			mp_comModuleObject->addKey(GLFW_KEY_D);
//			mp_comModuleObject->addKey(GLFW_KEY_SPACE);
//			mp_comModuleObject->addKeyboardTask(mp_keyInputTask);
//			mp_comModuleObject->addMouseTask(mp_mouseInputTask);
//			mp_comModuleObject->addJoystick(0);
//			mp_comModuleObject->addJoystickAxeTask(mp_jsInputTask);
//		}
//	}
	return true;
}

bool MFInputModule::initModuleObject(
		MFSyncObject* syncObject,
		MFModuleObject* pMO){
	MFInputModuleObject *pObject=(MFInputModuleObject*)pMO;
	pObject->setSyncObject(syncObject);
	return true;
}

//bool MFInputModule::syncInputData(std::vector<MFSyncObject*>* pVecInputData){
//	return true;
//}

//bool MFInputModule::syncOutputData(){
//	/*Iterate through the module objects and execute their key actions*/
//	/*The data will be written by the module objects execution*/
//	MFInputModuleObject* pObject=nullptr;
////	MFInterfaceJoystickListener::printJoystickAxes(GLFW_JOYSTICK_1);
//	for(ObjectData* pModObject:*mp_vecModuleObject){
//		pObject=(MFInputModuleObject*)(pModObject->pModuleData);
//		pObject->setSyncObject(pModObject->pSyncData);
//		pObject->synchronizeOutput();
//	}
//	return true;
//}
void MFInputModule::printInputDevices(){
	MFInterfaceJoystickListener::printJoystickInputs();
}

void MFInputModule::updateResources(MFModuleResources* pRes){
}
