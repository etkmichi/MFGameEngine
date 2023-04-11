/*
 * MFNetworkBaseTask.cpp
 *
 *  Created on: 14.05.2020
 *      Author: michl
 */

#include "MFNetworkBaseTask.h"

MFITaskProvider* MFNetworkBaseTask::smp_netTaskProvider=nullptr;

MFNetworkBaseTask::MFNetworkBaseTask(){

}

MFNetworkBaseTask::~MFNetworkBaseTask(){

}

void MFNetworkBaseTask::updateTaskProvider(
		MFITaskProvider* pTaskProvider,
		uint32_t dataCount,
		MFNetworkBaseTask* pNetworkTask,
		MFIStructurableDataCreator* pTaskDataCreator){
	MFStructurableDataBuffer *pStructurableDataBuffer;
	MFAbstractDataProcessingTask *pTask=pNetworkTask;
	MFIStructurableDataCreator *pDataCreator=pTaskDataCreator;

	pStructurableDataBuffer = new MFStructurableDataBuffer(
			pDataCreator,
			dataCount);
	pStructurableDataBuffer->initBuffer();
	pNetworkTask->setTaskManagerIndex(pTaskProvider->addTask(pTask,pStructurableDataBuffer));
}
