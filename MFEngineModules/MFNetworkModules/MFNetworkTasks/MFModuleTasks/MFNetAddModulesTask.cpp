/*
 * MFNetInitModulesTask.cpp
 *
 *  Created on: 01.04.2020
 *      Author: michl
 */
#include "MFNetAddModulesTask.h"

#include <MFDataDeleterClasses/MFDataDeletion.h>

uint32_t MFNetAddModulesTask::sm_maxModuleCount=50;

MFNetAddModulesTask::MFNetAddModulesTask(MFIManagerControl* pModuleProvider){
  //TODO ensure that all modules cann be transmitted!
  m_surpressSetupWarning=true;
	mp_moduleManager=pModuleProvider;
	m_maxModuleCount=sm_maxModuleCount;
	setPackageDataSize(5000);
}

MFNetAddModulesTask::~MFNetAddModulesTask() {
}

MFStructurableData* MFNetAddModulesTask::createNewEmptyData(){
	//TODO calculate valid size
	MFNetAddModulesTask *pDat=new MFNetAddModulesTask(mp_moduleManager);
	pDat->setPackageDataSize(5000);//TODO what if lag of memory?
	return pDat;
}

bool MFNetAddModulesTask::processData(MFStructurableData* pData){
	MFObject::printInfo("Debug MFNetAddModulesTask - processData\n"
			"data size: "+std::to_string(pData->getStructureSize())+"\n"
			"data count: "+std::to_string(pData->getVecData()->size()));
//	pData->printMetaData();

	mp_moduleManager->initFromData(pData->getVecData()->at(2));

	return true;
}
