/*
 * MFSyncStreamSink.cpp
 *
 *  Created on: 10.07.2020
 *      Author: michl
 */

#include "MFSyncStreamSink.h"

MFSyncStreamSink::MFSyncStreamSink(MFNetworkResources *pNetRes) {
	mp_netRes=pNetRes;
}

MFSyncStreamSink::~MFSyncStreamSink() {
}

bool MFSyncStreamSink::dispatchEvent(S_MF_NetworkEvent* pNE){
	if(mp_netRes->mp_vecSyncTasks->size()<1 || !mp_netRes->mp_loopCtl->isStarted()){
//		printInfo("MFSyncStreamSink::dispatchEvent - "
//				"no sync possible yet! mp_netRes->mp_vecSyncTasks count|loop->isStarted(): "+
//				std::to_string(mp_netRes->mp_vecSyncTasks->size())+"|"+
//				std::to_string(mp_netRes->mp_loopCtl->isStarted()));
		return true;
	}
	uint8_t* pData=pNE->pEvent->packet->data;
	uint32_t syncTaskIndex=MFModuleSyncTask::getSyncTaskIndex(pData);
	if(syncTaskIndex>=mp_netRes->mp_vecSyncTasks->size()){
		printErr("MFSyncStreamSink::dispatchEvent - "
				"no sync task for index:"+std::to_string(syncTaskIndex));
	}
	MFModuleSyncTask* pSyncTask=mp_netRes->mp_vecSyncTasks->at(0);
	//TODO its not 0!
	if(pSyncTask==nullptr){
		printWarning("MFSyncStreamSink::dispatchEvent - "
				"no sync task for index!");
		return true;
	}
	MFStructurableData* pOut=pSyncTask->takeModuleUpdateBuffer();
	if(pOut==nullptr){
		printErr("MFSyncStreamSink::dispatchEvent - no buffer available!");
		return false;
	}
	uint32_t size=pOut->getStructureSize();
	if(size>pNE->pEvent->packet->dataLength){
		size=pNE->pEvent->packet->dataLength;
	}
	memcpy(pOut->getStructureData(),pData,size);
	pSyncTask->updateModule(pOut);
//	pSyncTask->addModuleUpdateData(pOut);
	pSyncTask->retrunModuleUpdateBuffer(pOut);
	return true;
}
