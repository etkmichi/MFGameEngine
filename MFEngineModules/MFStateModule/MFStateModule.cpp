/*
 * MFStateModule.cpp
 *
 *  Created on: 14.02.2020
 *      Author: michl
 */

#include "MFStateModule.h"

MFStateModule::MFStateModule(MFSyncObjectResources* pSyncRes) :
MFBaseModule(pSyncRes) {
	enableInputSynchronisation(false);
	enableOutputSynchronisation(false);
	enableStateUpdate(true);
	pDummy=new MFDummyModuleObject();
}

void MFStateModule::updateResources(MFModuleResources* pRes){
}

MFStateModule::~MFStateModule() {
	// TODO Auto-generated destructor stub
}

bool MFStateModule::syncStates(std::vector<MFSyncObject*>* pVecStateChanges){
//	for(MFSyncObject* pSO:*pVecStateChanges){
//		if(pSO->getStateTransition()==MFSyncObject::TRANSITION_NONE ){
//			continue;
//		}
//		if(pSO->getState()!=MFSyncObject::STATE_IDLE){
//			if(pSO->getStateTransition()==MFSyncObject::TRANSITION_IDLE){
//				pSO->setState(MFSyncObject::STATE_IDLE);
//				continue;
//			}
//		}
//		if(pSO->getState()!=MFSyncObject::STATE_UPDATING){
//			if(pSO->getStateTransition()==MFSyncObject::TRANSITION_UPDATE){
//				if(pSO->getState()==MFSyncObject::STATE_IDLE){
//					pSO->setState(MFSyncObject::STATE_UPDATING);
//					pSO->setPreviouseState(MFSyncObject::STATE_IDLE);
//					continue;
//				}
//				if(pSO->getState()==MFSyncObject::STATE_DISABLED){
//					pSO->setState(MFSyncObject::STATE_UPDATING);
//					pSO->setPreviouseState(MFSyncObject::STATE_IDLE);
//					continue;
//				}
//				MFObject::printWarning("MFStateModule::syncStates - "
//						"Unalowed state transition!");
//			}
//		}else{
//			if(pSO->getPreviouseState()==MFSyncObject::STATE_DISABLED)
//				pSO->setState(MFSyncObject::STATE_DISABLED);
//			if(pSO->getPreviouseState()==MFSyncObject::STATE_IDLE)
//				pSO->setState(MFSyncObject::STATE_IDLE);
//			pSO->setStateTransition(MFSyncObject::TRANSITION_NONE);
//		}
//
//		if(pSO->getState()!=MFSyncObject::STATE_DISABLED){
//			if(pSO->getStateTransition()==MFSyncObject::TRANSITION_DISABLE){
//				pSO->setState(MFSyncObject::STATE_DISABLED);
//				pSO->setStateTransition(MFSyncObject::TRANSITION_NONE);
//			}
//		}else{
//
//		}
//		if(pSO->getStateTransition()==MFSyncObject::TRANSITION_ENABLE){
//			pSO->setState(MFSyncObject::STATE_EXECUTING);
//			pSO->setStateTransition(MFSyncObject::TRANSITION_NONE);
//		}
//		if(pSO->getStateTransition()==MFSyncObject::TRANSITION_REMOVE){
//			pSO->setState(MFSyncObject::STATE_REMOVED);
//			pSO->setStateTransition(MFSyncObject::TRANSITION_NONE);
//		}
//	}
	return true;
}

bool MFStateModule::initModuleObject(
		MFSyncObject* syncObject,
		MFModuleObject* pObject){
	MFModuleObject* pO=(MFModuleObject*)pObject;
	pO->setSyncObject(syncObject);

	//TODO create MFStateModuleObject -> monitoring and
	//other things
	return true;
}
