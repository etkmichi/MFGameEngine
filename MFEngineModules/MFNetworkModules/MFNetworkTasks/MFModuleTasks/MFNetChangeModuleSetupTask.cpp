/*
 * MFNetModuleSetupTask.cpp
 *
 *  Created on: 25.03.2020
 *      Author: michl
 */

#include "MFNetChangeModuleSetupTask.h"

MFNetChangeModuleSetupTask::MFNetChangeModuleSetupTask()
{
  m_surpressSetupWarning=true;
	setPackageDataSize(5000);
}

MFNetChangeModuleSetupTask::~MFNetChangeModuleSetupTask() {
	// TODO Auto-generated destructor stub
}

MFStructurableData* MFNetChangeModuleSetupTask::createNewEmptyData(){
	MFNetChangeModuleSetupTask *pDat=new MFNetChangeModuleSetupTask();
	return pDat;
}

bool MFNetChangeModuleSetupTask::processData(MFStructurableData* pData){
	//TODO use action task manager for retrieving action task.
	//iterate through pData's data and create sub data.
	// structure of pData:|setupDataCount| - N * (moduleID setup data|moduleID setupData) |
	//get module by id from module manager
	//get module's data setup
	//copy data to setup
	MFObject::printInfo("Debug MFNetModuleSetupTask - processData" );
	return false;
}
