/*
 * MFModuleObject.cpp
 *
 *  Created on: 22.10.2019
 *      Author: michl
 */

#include "MFModuleObject.h"

MFModuleObject::MFModuleObject() {
	mp_syncObject=nullptr;//TODO dummy sync object
	mp_moduleObjectIndex=nullptr;
	mp_moduleIndex=nullptr;
	setSetupName("MFModuleObject");
}

MFModuleObject::~MFModuleObject() {
	// TODO Auto-generated destructor stub
}

void MFModuleObject::setupDataStructure(){
  if(structureInitialized){
    return;
  }
  structureInitialized=true;
	mp_moduleIndex=addData(&m_moduleIndex);
	mp_moduleObjectIndex=addData(&m_moduleObjectIndex);
	setupObjectDataStructure();
}
