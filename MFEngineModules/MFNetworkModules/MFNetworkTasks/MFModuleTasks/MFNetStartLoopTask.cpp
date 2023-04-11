/*
 * MFNetStartLoopTask.cpp
 *
 *  Created on: 30.06.2020
 *      Author: michl
 */

#include "MFNetStartLoopTask.h"

MFNetStartLoopTask::MFNetStartLoopTask(MFNetworkResources* pNetRes) {
  m_surpressSetupWarning=true;
	mp_netRes=pNetRes;
	mp_loopCtl=mp_netRes->mp_loopCtl;
	setPackageDataSize(5000);
	setTaskName("MFNetStartLoopTask");
}

MFNetStartLoopTask::~MFNetStartLoopTask() {
}

MFStructurableData* MFNetStartLoopTask::createNewEmptyData(){
	return new MFNetStartLoopTask(mp_netRes);
}

bool MFNetStartLoopTask::processData(MFStructurableData* pData){
	MFObject::printInfo("MFNetStartLoopTask::processData - "
			"starting external engine loop!");
	/*TODO add current loop step to the data*/
//	void* pSrc=pData->getData();
	mp_loopCtl->executeExternalModuleLoop(nullptr, nullptr);

//	mp_physicsSyncTask=new MFPhysiscsModuleSync(
//			mp_netRes->mp_moduleProvider->getPhysicsSyncTask(),0);
//	mp_physicsSyncTask->initSyncTask();
//	mp_netRes->mp_vecSyncTasks->push_back(mp_physicsSyncTask);
	return true;
}
