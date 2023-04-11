/*
 * MFModuleSyncTask.cpp
 *
 *  Created on: 08.07.2020
 *      Author: michl
 */

#include "MFModuleSyncTask.h"

MFModuleSyncTask::MFModuleSyncTask(
			uint32_t sharedModuleIndex,
			uint32_t syncCycleCount
			) {
	m_sharedTaskIndex=sharedModuleIndex;
	m_syncCycle=syncCycleCount;
	mp_updateDataBuffers=nullptr;
	mp_updateModuleBuffers=nullptr;
}

MFModuleSyncTask::~MFModuleSyncTask() {
	// TODO Auto-generated destructor stub
}

bool MFModuleSyncTask::initSyncTask() {
	/*base structure of buffers*/
	MFStructurableData *pDat=new MFStructurableData();
	pDat->addNBitValue(m_maxBufferSize*8, nullptr)->setDataByteSize(m_maxBufferSize);

	/*data buffer for copying from module*/
	mp_updateDataBuffers=new MFStructurableDataBuffer(pDat,m_dataCount);
	//TODO create setter fimctions for advanced setup
	if(!mp_updateDataBuffers->initBuffer()){
		MFObject::printErr("MFModuleSyncTask::initSyncTask - "
				"mp_updateDataBuffers failed to init buffer!");
		return false;
	}
	mp_updateDataBuffers->setBufferName(
			std::string("MFModuleSyncTask-updateDataBuffer"
					+std::to_string(m_sharedTaskIndex)));

	/*data buffer for copying to module*/
	mp_updateModuleBuffers=new MFStructurableDataBuffer(pDat,m_dataCount);
	//TODO create setter fimctions for advanced setup
	if(!mp_updateModuleBuffers->initBuffer()){
		MFObject::printErr("MFModuleSyncTask::initSyncTask - "
				"mp_updateDataBuffers failed to init buffer!");
		return false;
	}
	mp_updateModuleBuffers->setBufferName(
			std::string("MFModuleSyncTask-updateModuleBuffer"
					+std::to_string(m_sharedTaskIndex)));

	/*pDat only needed for initBuffer()*/
//	delete pDat;

	mp_vecInputQueue=new std::vector<MFStructurableData*>();
	mp_vecInputProcessing=new std::vector<MFStructurableData*>();
	mp_vecOutputQueue=new std::vector<MFStructurableData*>();
	mp_vecOutputBuffers=new std::vector<MFStructurableData*>();
	return true;
}
void MFModuleSyncTask::addModuleUpdateData(MFStructurableData* pDat){
	lockVecInput.lock();
	mp_vecInputQueue->push_back(pDat);
	lockVecInput.unlock();
}

