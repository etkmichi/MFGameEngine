/*
 * MFNetSetupSyncsTask.cpp
 *
 *  Created on: 21.07.2020
 *      Author: michl
 */

#include "MFNetSetupSyncsTask.h"

MFNetSetupSyncsTask::MFNetSetupSyncsTask(MFNetworkResources *pResources) {
  m_surpressSetupWarning=true;
	mp_netRes=pResources;
}

MFNetSetupSyncsTask::~MFNetSetupSyncsTask() {
}


MFStructurableData* MFNetSetupSyncsTask::createNewEmptyData(){
	MFNetInputPackage *pData=new MFNetSetupSyncsTask(mp_netRes);
	pData->setPackageDataSize(8);
	return pData;
}

bool MFNetSetupSyncsTask::processData(MFStructurableData* pData){
	/*Server creates sync stream data and sends it over the sync stream channel*/
	/*client will dispatch received data with MFSyncStreamSink which will use a
	 * index from the data to the sync task
	 * 	- To setup this behavior this task must add the sync tasks to the client's
	 * 	MFSyncStreamSink. The MFSyncStreamSink uses std::vector<MFModuleSyncTask*>*
	 * 	of the network resources to retreive the sync task*/
	//TODO setup the needed sync tasks and add them to
	return false;
}

