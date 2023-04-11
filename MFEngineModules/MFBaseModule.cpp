/*
 * MFBaseModule.cpp
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */
#include "MFBaseModule.h"
#include "../MFStaticSetupData.h"

std::mutex MFBaseModule::lockModuleIDCounter;
uint32_t MFBaseModule::ms_moduleIDCounter=0;

MFBaseModule::MFBaseModule(MFSyncObjectResources *pSyncRes,const std::string &moduleName) :
	MFStructurableDataSetup(){
  setSetupName(moduleName);
	mp_syncRes=pSyncRes;
	mp_vecModuleGroups=new std::vector<uint32_t>();
	if(mp_syncRes==nullptr){
		MFStaticSetupData::sLockStaticSetupData.lock();
		mp_syncRes=MFStaticSetupData::smp_syncRes;
		MFStaticSetupData::sLockStaticSetupData.unlock();
		P_WRN("using sync res from static setup!");
	}
	lockModuleIDCounter.lock();
	m_moduleID=ms_moduleIDCounter;
  setNotifierName("Notifier"+moduleName+T_S(m_moduleID));
	ms_moduleIDCounter++;
	lockModuleIDCounter.unlock();
	m_syncObjectModuleID = mp_syncRes->getNextModuleID(this);
	mp_vecModuleObjects = new std::vector<ObjectData*>();
	mp_vecStateUpdateObjects = new std::vector<MFSyncObject*>();
	mp_vecRecycledObjects = new std::vector<ObjectData*>();
	mp_modObjectCreator=this;
	std::string info="sync object module id: "+T_S(m_syncObjectModuleID)+
			"\n"+"sync object resource id: "+T_S(mp_syncRes->m_syncResourceID)+
			"\n"+"module id: "+T_S(m_moduleID);
	P_WRN("module info:\n"+info);
	vecRecycleObjects.clear();
}

MFBaseModule::~MFBaseModule() {
	P_INF(getSetupName()+" deliting resources!");
	for(ObjectData* pOD:*mp_vecModuleObjects){
		if(pOD->pModuleData!=nullptr)delete pOD->pModuleData;
		delete pOD;
	}
	for(ObjectData* pOD:*mp_vecRecycledObjects){
		if(pOD->pModuleData!=nullptr)delete pOD->pModuleData;
		delete pOD;
	}
	delete mp_vecModuleObjects;
	delete mp_vecStateUpdateObjects;
	delete mp_vecRecycledObjects;
}

bool MFBaseModule::doWork(){
	lockCycleCounter.lock();
	m_moduleCycleCounter++;
	lockCycleCounter.unlock();
	if(m_enableStateUpdate){
		lockVecSateUpdate.lock();//TODO improve lock
		//TODO states still needed? -> remove states...
		if(!syncStates(mp_vecStateUpdateObjects)){
			P_WRN("failed to synchronize states!");
		}
		mp_vecStateUpdateObjects->clear();
		lockVecSateUpdate.unlock();
	}

  //getNextInputNotifications returns a vector with indices of the objects which should
  //be updated. This function will invalidate all notifications of past calls to
  //getNextInputNotifications!
  const std::vector<uint32_t>* pVec=getNextInputNotifications();
  if(!syncInputData(pVec)){
    P_WRN("failed to synchronize input data!");
  }

	if(!executeModule()){
		P_ERR("failed to execute the module!");
	}
	if(m_enableOutputSync){
		if(!syncOutputData()){
			P_WRN("failed to synchronize output data!");
		}
	}
	if(!recycleObjects()){
	  P_ERR("recycling failed!");
	}
	return true;
}
bool MFBaseModule::recycleObjects(){
  bool ret=true;
  for(MFSyncObject* pSO:vecRecycleObjects){
    /*get the indices which are referencing all related module objects*/
    std::vector<uint32_t>* pVecO=pSO->getModuleObjectIndices(m_syncObjectModuleID);
    if(pVecO==nullptr){
      P_ERR("no module object indices provided by sync object!");
      ret = false;
      continue;
    }
    if(pSO->getRecycleState()!=MFSyncObject::RECYCLE_STATE_TRANS_RECYCLED){
      P_ERR("cant recycle object which is not in state RECYCLE_STATE_TRANS_RECYCLED!");
      ret =false;
      continue;
    }
    P_INF(getSetupName()+" recycling module objects!");
    lockVecRecycledObjects.lock();
    for(uint32_t index:*pVecO){
      lockVecObjects.lock();
      if(mp_vecModuleObjects->at(index)==nullptr){
        lockVecObjects.unlock();
        P_ERR(getSetupName()+" object at given index is nullptr! idx="+std::to_string(index));
        continue;
      }
      if(mp_vecModuleObjects->size()<=index){
        lockVecObjects.unlock();
        P_ERR(getSetupName()+" index out of bounce!");
        continue;
      }
      ObjectData* pD=mp_vecModuleObjects->at(index);
      MFModuleObject* pMO=pD->pModuleData;
      if(pMO==nullptr){
        P_ERR(getSetupName()+"module object at given index is nullptr! idx="+std::to_string(index));
        lockVecObjects.unlock();
        continue;
      }
      /*call virtual recycle function (by default it will disable the module object)*/
  //    pMO->disable();
      recycleObject(pSO,pMO);

      uint32_t recycleIndex=mp_vecRecycledObjects->size();

      /*add recycle index to pSO, this will add all related module objects to the sync object's
       * recycle vector*/
      pSO->addModuleRecycleData(m_syncObjectModuleID, recycleIndex);
      pMO->setRecycleIndex(recycleIndex);
      mp_vecRecycledObjects->push_back(mp_vecModuleObjects->at(index));
      mp_vecModuleObjects->data()[index]=nullptr;
      m_recycleObjectsCount++;
      lockVecObjects.unlock();
      pSO->decrementReferenceCount();
    }
    lockVecRecycledObjects.unlock();

//    P_INF(getSetupName()+" recycling done!");
  }
  vecRecycleObjects.clear();
  return ret;
}
bool MFBaseModule::syncInputData(){
	if(!syncInputData(getNextInputNotifications())){
		P_WRN("failed to synchronize input data!");
	}
	return true;
}

bool MFBaseModule::syncInputData(const std::vector<uint32_t>* pSyncIndices){
  //TODO clean up counter and cleanup
  uint32_t cleanUpCounter=0;
	for(uint32_t moIndex:*pSyncIndices){
		if(moIndex==MFInputSyncNotifier::QUEUE_END_ENTRY){
			return true;
		}

		if(mp_vecModuleObjects->size()<=moIndex || mp_vecModuleObjects->at(moIndex)==nullptr){
//		  P_WRN(getSetupName()+" module object index oob or null! idx="+T_S(moIndex));
		  cleanUpCounter++;
		  if(cleanUpCounter%50==0){
		    P_WRN("a lot of unused objects!");
		  }
		  continue;
		}

		MFModuleObject* pMO=mp_vecModuleObjects->at(moIndex)->pModuleData;
		if(pMO!=nullptr && pMO->getSyncObject()->isInputSyncEnabled())
			pMO->synchronizeInput();
	}
	return true;
}

bool MFBaseModule::syncOutputData(){
	for(ObjectData* pDat : *mp_vecModuleObjects){
		if(pDat!=nullptr && pDat->pSyncData->isOutputSyncEnabled()){
			pDat->pModuleData->synchronizeOutput();
		}
	}
	return true;
}

MFModuleObject* MFBaseModule::getModuleObject(uint32_t index){
	if(index >= mp_vecModuleObjects->size() || mp_vecModuleObjects->at(index)==nullptr)
		return nullptr;
	return mp_vecModuleObjects->at(index)->pModuleData;
}

bool MFBaseModule::disableObject(MFSyncObject *pObject)
{
	for(uint32_t index:*pObject->getModuleObjectIndices(m_syncObjectModuleID)){
		MFModuleObject* pMO=getModuleObject(index);
		if(pMO!=nullptr){
			pMO->disable();
		}
	}
	return true;
}

bool MFBaseModule::recycleObject(MFSyncObject* pSO,MFModuleObject* pMO)
{
  pMO->disable();//TODO do not disable in this function -> shall be pure virt...
  return true;
}

bool MFBaseModule::syncStates(std::vector<MFSyncObject*>* pVecStateChanges){
	for(MFSyncObject* pSO:*pVecStateChanges){
		if(pSO->getStateTransition()==MFSyncObject::TRANSITION_NONE){
			continue;
		}
//		for(uint32_t index:*pSO->getModuleObjectIndices(m_syncObjectModuleID)){
//			MFModuleObject* pMO=getModuleObject(index);
//			if(pSO->getStateTransition()==MFSyncObject::TRANSITION_UPDATE){
//				pMO->updateState();
//			}
//		}
    P_ERR("no impl.!");
//		if(pSO->getState()==MFSyncObject::STATE_DISABLED){
//			pMO->disable();
//		}
//		if(pSO->getState()==MFSyncObject::STATE_EXECUTING &&
//				pSO->getStateTransition()==MFSyncObject::TRANSITION_ENABLE){
//			pMO->enable();
//		}
//		if(pSO->getState()==MFSyncObject::STATE_REMOVED){
//			pMO->disable();
//			removeObject(pSO);
//		}
//		if(pSO->getState()==MFSyncObject::TRANSITION_DELETE){
//			//TODO delete things
////			pMO->disable();
////			removeObject(pSO);
//		}
	}
	return true;
}

void MFBaseModule::enqueueRecycleModuleObject(MFSyncObject* pSO){
  /*enqueue object for recycling after module execution (will be done in doWork())*/
  lockVecRecycleObjects.lock();
  initRecycling(getModuleObject(pSO));
  vecRecycleObjects.push_back(pSO);
  lockVecRecycleObjects.unlock();
}

bool MFBaseModule::reuseRecycledModuleObject(MFSyncObject* pSO){
  /*get the indices which are referencing all related module objects*/
  std::vector<uint32_t>* indices=pSO->getModuleRecycleObjectIndices(getSyncModuleID());
  if(indices==nullptr || indices->size()==0 || m_recycleObjectsCount==0){
    P_ERR(getSetupName()+" failed to reuse object! "+
        "indices==nullptr || indices->size()==0 || m_recycleObjectsCount==0");
    return false;
  }
  bool ret=true;
  P_INF(getSetupName()+" reusing recycled object!");
  lockVecRecycledObjects.lock();
  for(uint32_t index:*indices){
    ObjectData* pD=mp_vecRecycledObjects->at(index);
    if(pD==nullptr){
      P_ERR(getSetupName()+" nullptr within recycled objects!");
      ret=false;
      continue;
    }
    MFModuleObject* pMO = pD->pModuleData;
    pMO->getSyncObject()->enableInputSync(true);
    pMO->enable();
    //TODO reuseObject(pSO,pMO)?
    /*this will reuse the former position within the objects vector*/
    if(pMO==nullptr){
      P_ERR("module object is nullptr!!!");
      ret=false;
      continue;
    }
    addSyncObject(pSO, pMO,true);
    mp_vecRecycledObjects->data()[index]=nullptr;//indices->at(0)
    //TODO use vector with resizing as recycling vector -> retrieve object index in module
    //st else
    m_recycleObjectsCount--;
  }
  indices->clear();
  lockVecRecycledObjects.unlock();
  P_INF(getSetupName()+" reusing done!");
  return ret;
}

bool MFBaseModule::addSyncObject(MFSyncObject* pSyncObject,MFModuleObject* pModO,bool recycled){
  if(pModO==nullptr){
    P_ERR(getSetupName() +" failed! - pModO==nullptr");
    return false;
  }

  pModO->setSyncObject(pSyncObject);
  pModO->setupDataStructure();
  bool ret=initModuleObject(pSyncObject,pModO);
  if(!ret){
    P_ERR("module object initialization failed!");
    return false;
  }

  return addSO(pSyncObject,pModO,recycled);
}

MFModuleObject* MFBaseModule::addSyncObject(MFSyncObject* pSyncObject){
  MFModuleObject* pModO=mp_modObjectCreator->createModuleObject(pSyncObject);
  if(pModO==nullptr){
    P_ERR(getSetupName() +" failed! - pModO==nullptr");
    return nullptr;
  }
  pModO->initData();
  pModO->setSyncObject(pSyncObject);
  pModO->setupDataStructure();
  bool ret=initModuleObject(pSyncObject,pModO);
  if(!ret){
    delete pModO;
    return nullptr;
  }

  addSO(pSyncObject,pModO,false);
  return pModO;
}

bool MFBaseModule::addSO(MFSyncObject* pSyncObject,MFModuleObject* pModO,bool recycle){
  if(recycle){//TODO test! it was freitag abend!
    /*object already in mp_vecModuleObjects*/
    uint32_t objectIndex=pModO->getModuleIndex();
    uint64_t recycleIndex=pModO->getRecycleIndex();

    if( objectIndex >= mp_vecModuleObjects->size() ){
      P_ERR(getSetupName()+"objectIndex out of bounds! index="+T_S(objectIndex));
      return false;
    }
    if( recycleIndex >= mp_vecRecycledObjects->size()){
      P_ERR(getSetupName()+"recycleIndex out of bounds! index="+T_S(objectIndex));
      return false;
    }

    if(mp_vecRecycledObjects->data()[recycleIndex]==nullptr) {
      P_ERR(getSetupName()+"recycled object == nullptr!");
      return false;
    }
    if(mp_vecModuleObjects->data()[objectIndex]!=nullptr) {
      P_WRN(getSetupName()+" existing module object will be used!");
    }else{
      mp_vecModuleObjects->data()[objectIndex]=mp_vecRecycledObjects->data()[recycleIndex];
    }
  }else{
    ObjectData* pObject=new ObjectData();
    pObject->pSyncData=pSyncObject;
    pObject->pModuleData=pModO;
    pModO->setModuleIndex(mp_vecModuleObjects->size());
    pModO->setModuleID(m_moduleID);
    lockVecObjects.lock();
    pSyncObject->addModuleData(
      m_syncObjectModuleID,
      mp_vecModuleObjects->size());
    mp_vecModuleObjects->push_back(pObject);
    lockVecObjects.unlock();
  }
  pSyncObject->incrementReferenceCount();
  return true;
}

void MFBaseModule::notifyInputSync(MFSyncObject* pSyncObject){
	if(!m_enableInputSync){
		return;
	}
	for(uint32_t index:*pSyncObject->getModuleObjectIndices(m_syncObjectModuleID)){
		addSyncNotification(index);
	}
}

void MFBaseModule::addStateUpdate(MFSyncObject* pSyncObject){
	if(!m_enableStateUpdate){
		return;
	}
	lockVecSateUpdate.lock();
	mp_vecStateUpdateObjects->push_back(pSyncObject);
	lockVecSateUpdate.unlock();
}

/*virtual functions*/
bool MFBaseModule::initModule(){
	return true;
}

