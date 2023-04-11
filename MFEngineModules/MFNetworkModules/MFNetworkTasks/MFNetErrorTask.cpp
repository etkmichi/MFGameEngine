/*
 * MFNetErrorTask.cpp
 *
 *  Created on: 21.04.2020
 *      Author: michl
 */

#include "../MFNetworkTasks/MFNetErrorTask.h"

#include <MFObjects/MFObject.h>
void* MFNetErrorTask::smp_data=nullptr;
std::mutex MFNetErrorTask::sLockData;

MFNetErrorTask::MFNetErrorTask(){
	sLockData.lock();
	setStructureSize(5000);
  m_surpressSetupWarning=true;
	if(smp_data==nullptr)
		smp_data=malloc(20000);
	sLockData.unlock();
}
MFNetErrorTask::~MFNetErrorTask(){

}
MFStructurableData* MFNetErrorTask::createNewEmptyData(){
	return new MFNetErrorTask();
}
bool MFNetErrorTask::processData(MFStructurableData* pData){
//	MFObject::printErr("MFNetErrorTask::processData - data print:\n");
//	pData->printMetaData();
	MFObject::printErr("MFNetErrorTask::processData - error package received!");

	return true;
}


void MFNetErrorTask::setupDataStructure(){
	sLockData.lock();
	addNBitValue(getStructureSize()*8, smp_data);
	sLockData.unlock();
}
