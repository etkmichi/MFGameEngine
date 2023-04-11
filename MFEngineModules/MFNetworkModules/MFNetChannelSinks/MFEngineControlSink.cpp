/*
 * MFEngineControlSink.cpp
 *
 *  Created on: 20.07.2020
 *      Author: michl
 */

#include "MFEngineControlSink.h"

MFEngineControlSink::MFEngineControlSink(MFNetworkResources* pNetRes) {
	mp_netRes=pNetRes;
	mp_dispatchInput=new MFNetInputPackage();
	mp_dispatchInput->m_surpressSetupWarning=true;
	mp_dispatchInput->initData();
	mp_dispatchInput->mp_packageTypeID->setData(&m_currentPackageType);
	mp_dispatchInput->mp_packageCounter->setData(&m_currentPackageCounter);
	mp_vecEnqueuedPackages=new std::vector<MFNetInputPackage*>();
	mp_vecFreeIndices=new std::vector<uint32_t>();

}

MFEngineControlSink::~MFEngineControlSink() {
	delete mp_dispatchInput;
}

bool MFEngineControlSink::dispatchEvent(S_MF_NetworkEvent* pNE){
	if(!mp_dispatchInput->dispatchPackageHead(pNE->pEvent->packet)){
		printErr("MFEngineControlSink::dispatchEvent - "
				"failed to dispatch pkg head!");
		return false;
	}

	printInfo("MFEngineControlSink::dispatchEvent - "
			"(pkg counter | pkg type) : ("+
			std::to_string(m_currentPackageCounter)+" | "+
			std::to_string(m_currentPackageType)+")");

	MFAbstractDataProcessingTask* pTask=static_cast<MFAbstractDataProcessingTask*>(
			mp_netRes->mp_netTaskManager->getTask(mp_dispatchInput->getTypeIDData()));
	MFIStructurableDataCreator* pDC=mp_netRes->mp_netTaskManager->getDataCreator(pTask);
	MFStructurableDataBuffer *pDB=mp_netRes->mp_netTaskManager->getDataBuffer(pTask);
	if(pDC == nullptr || pDB==nullptr){
		printErr("MFEngineControlSink::dispatchEvent - "
				"pDC == nullptr || pDB==nullptr !");
		return true;
	}

	MFNetInputPackage *pPackage=getPkg(pDB,pDC);
	if(pPackage==nullptr){return true;}

	/*copies data from pNE to pPackage*/
	if(!dispatchInputPackage(pPackage, pNE)){
		printErr("MFNetPkgDispatchTask::dispatchEvent - "
				"failed to dispatch input package!");
		return true;
	}

	/*executes the pTask with the pPackage data in correct order*/
	if(!processInputData(pTask,pPackage)){
		printErr("MFNetPkgDispatchTask::dispatchEvent - "
				"failed to processInputData!");
		return true;
	}
	return true;
}

MFNetInputPackage* MFEngineControlSink::getPkg(
		MFStructurableDataBuffer *pDataBuffer,
		MFIStructurableDataCreator* pDataCreator){
	MFNetInputPackage *pPackage=nullptr;
	if(pDataBuffer!=nullptr){
		pPackage=(MFNetInputPackage*)pDataBuffer->takeData();
	}else{//TODO if pTask ==nullptr or ==dummyTask....
		printWarning("MFEngineControlSink::getPkg - pDataBuffer==nullptr");
		if(pDataCreator!=nullptr)
			pPackage=(MFNetInputPackage*)pDataCreator->createNewEmptyData();
		else{
			printErr("MFEngineControlSink::getPkg - can't create dispatchable "
					"package!");
			return nullptr;
		}
	}
	return pPackage;
}

bool MFEngineControlSink::dispatchInputPackage(
		MFNetInputPackage* pPackage,
		S_MF_NetworkEvent* pNE){
	//TODO data optimization task which unpacks received data and dispatches them again
	//TODO test if dispatching is fast enough for network thread
		// dispatching/copying data shall not block receive/send events for too long!
	if(!pPackage->dispatchPackageHead(pNE->pEvent->packet)) {
		printErr("MFEngineControlSink::dispatchPackage - "
				"failed to dispatch package head!");
		return false;
	}
	//TODO if return false then use some package check dispatch task
	if(!pPackage->dispatchPackageData(pNE->pEvent->packet)){
		printErr("MFEngineControlSink::dispatchPackage - failed to "
				"dispatch package data!");
		return false;
	}
	if(pNE->pEvent->packet->freeCallback!=nullptr){
//		printInfo("MFEngineControlSink::dispatchInputPackage - calling "
//				"free callback of enet packet");
//		pNE->pEvent->packet->freeCallback(pNE->pEvent->packet);
	}else{
		printInfo("MFEngineControlSink::dispatchInputPackage - free cb "
				"of packet == nullptr!");
	}
	return true;
}

bool MFEngineControlSink::processInputData(
		MFAbstractDataProcessingTask *pTask,
		MFNetInputPackage *pPackage){
  //TODO increment pkg counter at sender if pkgCounter==0!
	if(m_nextPackageCounter==m_currentPackageCounter ||  m_currentPackageCounter==0){
		printInfo("MFEngineControlSink::processInputData - processing "
				"next package. pkgNr="+std::to_string(m_nextPackageCounter));
		pTask->addInputData(pPackage);
		((MFTaskThread*)(mp_netRes->mp_netTaskManager))->addTask(pTask);
		if(m_currentPackageCounter!=0)m_nextPackageCounter++;
//		if(m_lowestEnqueuedPkgCounter==m_nextPackageCounter)
//			m_checkPackageQueue=true;
	}else{
		if(mp_vecFreeIndices->size()==0){//TODO send missing pkg
			mp_vecEnqueuedPackages->push_back(pPackage);
		}else{
			uint32_t index=*(mp_vecFreeIndices->end()-1);
			mp_vecFreeIndices->pop_back();
			mp_vecEnqueuedPackages->data()[index]=(pPackage);
		}
	}
//	if(m_currentPackageCounter<m_lowestEnqueuedPkgCounter)
//		m_lowestEnqueuedPkgCounter=m_currentPackageCounter;

	if(mp_vecEnqueuedPackages->size()>100){
		printWarning("MFEngineControlSink::dispatchEvent - "
				"mp_vecEnqueuedPackages->size()>10!");
	}

	for(int32_t i=mp_vecEnqueuedPackages->size()-1;i>=0; i--){
		MFNetInputPackage* pPkg=mp_vecEnqueuedPackages->at(i);
		if(pPkg==nullptr){
			continue;
		}
		if(pPkg->getCounter()==m_nextPackageCounter){
			//get task and add the input data.
			printInfo("MFEngineControlSink::processInputData - processing "
					"next package. pkgNr="+std::to_string(m_nextPackageCounter));
			MFAbstractDataProcessingTask* pEnqueuedTask=
					static_cast<MFAbstractDataProcessingTask*>(
						mp_netRes->mp_netTaskManager->getTask(pPkg->getTypeIDData()));
			pEnqueuedTask->addInputData(pPkg);
			mp_vecEnqueuedPackages->data()[i]=nullptr;
			mp_vecFreeIndices->push_back(i);
			((MFTaskThread*)(mp_netRes->mp_netTaskManager))->addTask(pEnqueuedTask);
			m_nextPackageCounter++;
			i=mp_vecEnqueuedPackages->size()-1;
		}
	}
	return true;//TODO its the only retrun here
}
