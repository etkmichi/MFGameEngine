/*
 * MFGUIModuleObject.cpp
 *
 *  Created on: 07.02.2020
 *      Author: michl
 */

#include "MFGUIModuleObject.h"

MFGUIModuleObject::MFGUIModuleObject(
		MFDataObject* pData,
		MFIGUIManager *pMgr) {
	mp_guiMgr=pMgr;
	mp_dataObject=pData;
	mp_internalDataObject=nullptr;
}

MFGUIModuleObject::MFGUIModuleObject(MFIGUIManager *pMgr) {
	mp_guiMgr=pMgr;
	mp_internalDataObject=new MFDataObject();
	mp_dataObject=mp_internalDataObject;
}

MFGUIModuleObject::~MFGUIModuleObject() {
	if(mp_internalDataObject!=nullptr)delete mp_internalDataObject;
}


bool MFGUIModuleObject::synchronizeInput(){
	if(mp_syncObject==nullptr){
		MFObject::printWarning("MFGUIModuleObject::synchronizeInput() - "
				"No sync object set!");
		return true;
	}
	mp_dataObject->setData(mp_syncObject->getGlobalModelMatrix());

	return true;
}

bool MFGUIModuleObject::synchronizeOutput(){
	if(mp_syncObject==nullptr){
		MFObject::printWarning("MFGUIModuleObject::synchronizeOutput() - "
				"No sync object set!");
		return true;
	}
	return true;
}
