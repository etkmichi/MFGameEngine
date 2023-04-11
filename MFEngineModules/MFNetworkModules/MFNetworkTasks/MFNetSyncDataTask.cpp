
/*
 * MFNetSyncDataTask.cpp
 *
 *  Created on: 19.03.2020
 *      Author: michl
 */

#include "../MFNetworkTasks/MFNetSyncDataTask.h"
glm::mat4x4 dummyData;
MFNetSyncDataTask::MFNetSyncDataTask() {
  m_surpressSetupWarning=true;

}

MFNetSyncDataTask::~MFNetSyncDataTask() {
	// TODO Auto-generated destructor stub
}

bool MFNetSyncDataTask::processData(MFStructurableData* pData){
	MFObject::printInfo("Debug MFNetSyncDataTask - processData" );
	//No upcast possible, pData was taken from a buffer which doesnt know about
	//MFNetActionData. The buffers sub data's structure matches the MFNetActionData
	//structure.
	//disable input/output sync
	//enable output sync after input sync
	return false;
}

MFStructurableData* MFNetSyncDataTask::createNewEmptyData(){
	MFNetSyncDataTask* pSD=new MFNetSyncDataTask();
	return pSD;
}
