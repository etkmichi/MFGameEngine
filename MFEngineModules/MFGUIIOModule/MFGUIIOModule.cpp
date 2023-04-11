/*
 * MMFGUIIOModule.cpp
 *
 *  Created on: 06.02.2020
 *      Author: michl
 */

#include "MFGUIIOModule.h"

MFGUIIOModule::MFGUIIOModule(MFSyncObjectResources* pSyncRes) :
		MFBaseModule(pSyncRes),
		MFIModuleCreator("MFGUIIOModule"){
//  mp_guiManager=MFModuleResources::smp_guiManager;
  mp_guiManager=nullptr;
	mp_vecDataObjects=new std::vector<MFDataObject*>();
	mp_vecMatrices=new std::vector<glm::mat4x4*>();
}

MFGUIIOModule::~MFGUIIOModule() {
}

bool MFGUIIOModule::initModule(){

	return true;
}

bool MFGUIIOModule::initModuleObject(
		MFSyncObject* pSyncObject,
		MFModuleObject* pObject){
	glm::mat4x4* pModelMatrix=new glm::mat4x4();
	mp_vecMatrices->push_back(pModelMatrix);
	*pModelMatrix=*pSyncObject->getGlobalModelMatrix();
	MFGUIModuleObject* pModO=(MFGUIModuleObject*)pObject;
	pModO->setSyncObject(pSyncObject);
	MFDataObject* pDataObject=pModO->getGUIDataObject();
	mp_vecDataObjects->push_back(pDataObject);
	pDataObject->setDataType(DATA_TYPE_MAT_4X4);
	pDataObject->setGuiType(GUI_TYPE_MATRIX);
	pDataObject->setData(pModelMatrix);
	return true;
}

bool MFGUIIOModule::syncOutputData(){
	for(ObjectData* pO:*mp_vecModuleObjects){
		if(pO!=nullptr)
			pO->pModuleData->synchronizeOutput();
		//TODO remove nullptr from vector
		// -maybe create vector with indices of nullptr
		//		- use indices for next position
	}
	return true;
};

MFModuleObject* MFGUIIOModule::createModuleObject(){
	MFGUIModuleObject* pModO=new MFGUIModuleObject(nullptr);
	//smp_guiManager
	return pModO;//pModO;
};

bool MFGUIIOModule::executeModule(){
	//No execution for gui...
	return true;
};

void MFGUIIOModule::updateResources(MFModuleResources* pRes){
}
