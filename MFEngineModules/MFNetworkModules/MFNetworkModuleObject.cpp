/*
 * MFNetworkModuleObject.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include "MFNetworkModuleObject.h"
#include "MFPackageClasses/MFNetInputPackage.h"

MFNetworkModuleObject::MFNetworkModuleObject() {
//	mp_netClientRes->mp_objectManager->addNetworkNotifier(this);
	setSetupName("MFNetworkModuleObject");

//	mp_vecInNotesAddObject=new std::vector<S_Notification*>();
//	mp_vecOutNotesAddObject=new std::vector<S_Notification*>();
//
//	mp_nextNotePkg=new MFDataObject();
//	m_maxPkgDataSize=5000;
//	mp_nextNotePkg->allocate(MFNetInputPackage::getHeaderSize()+m_maxPkgDataSize);
//	m_remainingPkgSize=m_maxPkgDataSize;
//	mp_currentOutPkg=new MFDataObject();
//	mp_currentOutPkg->allocate(MFNetInputPackage::getHeaderSize()+5000);
//
//
//	m_crtBufferOffset=0;
//	m_crtBufferOffset+=MFNetInputPackage::getHeaderSize();
//	m_crtBufferOffset+=sizeof(uint16_t);/*note counter*/

	mp_netClient=nullptr;
}

MFNetworkModuleObject::~MFNetworkModuleObject() {
//	for(S_Notification* pNote:*mp_vecInNotesAddObject){
//		delete pNote;
//	}
//	delete mp_vecInNotesAddObject;
//
//	for(S_Notification* pNote:*mp_vecOutNotesAddObject){
//		delete pNote;
//	}
//	delete mp_vecOutNotesAddObject;
}

bool MFNetworkModuleObject::synchronizeInput(){
	return true;
}

bool MFNetworkModuleObject::synchronizeOutput(){
	return false;/*No output*/
}


//void MFNetworkModuleObject::sendAddNotes(){
////  lockInput.lock();//TODO max input count
////  while(m_nextNoteAddIndex>=mp_vecInNotesAddObject->size() ){
////    mp_vecInNotesAddObject->push_back(new S_Notification());
////  }
////  S_Notification* pNote=mp_vecInNotesAddObject->at(m_nextNoteAddIndex);
////  pNote->pGroup=pGroup;
////  pNote->pObject=pObject;
////  pNote->cycle=spawnCycle;
////  m_nextNoteAddIndex++;
////  lockInput.unlock();
//}

//bool MFNetworkModuleObject::processAddObjectNotes(
//		MFDataObject* pDstBuffer,
//		uint32_t &remainingSize){
//	lockInput.lock();
//	lockOutput.lock();
//	if(m_nextNoteAddIndex==0){//no input notes (dont use mp_vec...->size()!
//		lockInput.unlock();
//		lockOutput.unlock();
//		return true;
//	}
//
//	uint32_t noteAddCount=m_nextNoteAddIndex;
//	std::vector<S_Notification*>* pOld=mp_vecOutNotesAddObject;
//	mp_vecOutNotesAddObject=mp_vecInNotesAddObject;
//	mp_vecInNotesAddObject=pOld;
//	m_nextNoteAddIndex=0;
//	lockInput.unlock();
//	uint8_t* pDst=((uint8_t*)pDstBuffer->getData());
//	pDst+=m_crtBufferOffset;
//	for(uint32_t i=0;i<noteAddCount;i++){
//		S_Notification* pN=mp_vecOutNotesAddObject->at(i);
//		uint32_t objSize=getAddObjectNoteSize(pN);
//		if(remainingSize<objSize){
//			MFObject::printWarning("MFNetworkModuleObject::processAddObjectNotes - too many "
//					"input notes, pkg size not big enough! Reenqueueing note!");
//			notifyAddObject(
//					pN->pGroup,
//					pN->pObject,
//					pN->cycle);
//			continue;
//		}
//		/*Add object note count*/
//		/*
//		 * note id | group id | object index | geometry setup size |
//		 * geometry setup | spawnCycle*/
//		memcpy(pDst,&m_addObjectNoteID,sizeof(uint16_t));
//		pDst+=sizeof(uint16_t);//note id
//		m_crtBufferOffset+=sizeof(uint16_t);//note id
//
//		memcpy(pDst,&pN->pGroup->groupIndex,sizeof(uint16_t));
//		pDst+=sizeof(uint16_t);//group id
//		m_crtBufferOffset+=sizeof(uint16_t);//group id
//
//		memcpy(pDst,&pN->pObject->getObjectID(),sizeof(uint32_t));
//		pDst+=sizeof(uint32_t);//object index
//		m_crtBufferOffset+=sizeof(uint32_t);//object index
//
//		uint32_t serializedSize=0;
//		if(!pN->pObject->getGeometry()->serializeGeometrySetup(serializedSize,pDst)){
//			MFObject::printErr("MFNetworkModuleObject::processAddObjectNotes - "
//					"failed to copy object init data!");
//		}
//		pDst+=serializedSize;
//		m_crtBufferOffset+=serializedSize;
//
//		memcpy(pDst,&pN->cycle,sizeof(uint32_t));
//		pDst+=sizeof(uint32_t);//spawnCycle
//		m_crtBufferOffset+=sizeof(uint32_t);//spawnCycle
//
//		(*mp_notesCounter)++;
//		remainingSize-=objSize;
//	}
//	lockOutput.unlock();
//
//	return true;
//}

//uint32_t MFNetworkModuleObject::getAddObjectNoteSize(S_Notification* pNote){
//	/*
//	 * note id | group id | object index | geometry setup size |
//	 * geometry setup | spawnCycle
//	 * */
//	uint32_t size=sizeof(uint16_t);//note id
//	size+=sizeof(uint16_t);//group id
//	size+=sizeof(uint32_t);//object index
//	size+=sizeof(uint32_t);//geometry setup size
//	size+=pNote->pObject->getGeometry()->getGeometrySetup()->getStructureSize();//geometry setup
//	size+=sizeof(uint32_t);//spawnCycle
//	return size;
//}

/**
 *
 * @param validBytes count of bytes which represent valid action data.
 * @return
 */
//MFDataObject* MFNetworkModuleObject::getNextOutputPackage(uint32_t &notesCount){
//	lockCurrentOutputPkg.lock();
//	lockNextOutputPkg.lock();
//
//	MFDataObject* pOld=mp_currentOutPkg;
//	mp_currentOutPkg=mp_nextNotePkg;
//	mp_nextNotePkg=pOld;
//	notesCount=*getPNoteCounter(mp_currentOutPkg);
//	m_remainingPkgSize=mp_nextNotePkg->getDataSize();
//	mp_notesCounter=getPNoteCounter(mp_nextNotePkg);
//	(*mp_notesCounter)=0;
//
//	m_crtBufferOffset=0;
//	m_crtBufferOffset+=MFNetInputPackage::getHeaderSize()+sizeof(uint16_t);
//	m_crtBufferOffset+=sizeof(uint16_t);/*note counter*/
//
//	lockNextOutputPkg.unlock();
//	lockCurrentOutputPkg.unlock();
//	return mp_currentOutPkg;
//}

//bool MFNetworkModuleObject::sendData(MFNetClient* pClient){
//	if(!pClient->isConnected()){
//		MFObject::printWarning("MFNetworkModuleObject::sendData(MFNetClient*) - "
//				"client not connected!");
//		return false;
//	}
//	lockCurrentOutputPkg.lock();
//	uint32_t bytesToSend=0;
//	if(m_lastSendDone){
//		lockNextOutputPkg.lock();
//		MFDataObject* pOld=mp_outputPackage;
//		mp_outputPackage=mp_nextOutputPackage;
//		mp_nextOutputPackage=pOld;
//		bytesToSend=mp_outputPackage->getDataSize()-m_remainingPkgSize;
//		m_remainingPkgSize=mp_nextOutputPackage->getDataSize();
//		mp_notesCounter=((uint32_t*)mp_nextOutputPackage->getData())+getHeaderSize();
//		(*mp_notesCounter)=0;
//		lockNextOutputPkg.unlock();
//	}else{
//		bytesToSend=m_lastSendSize;
//	}
//	m_lastSendDone=pClient->sendData(
//			mp_outputPackage->getData(),
//			bytesToSend,
//			0, true,
//			&mp_netRes->m_packageCounter
//			);
//	if(!m_lastSendDone){//store the byte count for next sendData call...
//		m_lastSendSize=bytesToSend;
//	}
//
//	lockCurrentOutputPkg.unlock();
//	return m_lastSendDone;
//}

