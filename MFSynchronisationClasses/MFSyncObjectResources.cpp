/*
 * MFSyncObjectResources.cpp
 *
 *  Created on: 08.06.2020
 *      Author: michl
 */

#include "MFSyncObjectResources.h"
#include <MFObject.h>
uint32_t MFSyncObjectResources::idCounter=0;
std::mutex MFSyncObjectResources::lockID;
MFSyncObjectResources::MFSyncObjectResources(){
	mp_moduleNotifiers=new std::vector<MFInputSyncNotifier*>();
	mp_vecModuleSyncTasks=new std::vector<MFModuleSyncTask*>();
	lockID.lock();
	m_syncResourceID=idCounter;
	idCounter++;
	lockID.unlock();
	m_objectIDCounter=0;
	m_moduleIndexCounter=0;
	mp_logWindow=nullptr;
}

MFSyncObjectResources::~MFSyncObjectResources(){
	delete mp_moduleNotifiers;
}

uint32_t MFSyncObjectResources::getNextModuleID(MFInputSyncNotifier* pInputNotifier){
	lockModuleIndexCounter.lock();
	uint32_t count=m_moduleIndexCounter++;
	mp_moduleNotifiers->push_back(pInputNotifier);
	lockModuleIndexCounter.unlock();
	MFObject::printInfo("MFSyncObjectResources::getNextModuleIndex() - new module index "
			"for sync objects:"+
			std::to_string(count));
	return count;
}
