/*
 * MFIInputNotifier.cpp
 *
 *  Created on: 14.04.2020
 *      Author: michl
 */

#include "MFInputSyncNotifier.h"
#include <MFBasicDefines.h>

#include <MFObject.h>
#include <mutex>
uint32_t MFInputSyncNotifier::ms_queueSize=100;
uint32_t notifierID=0;
std::mutex lockID;

MFInputSyncNotifier::MFInputSyncNotifier(){
	mp_vecInputNotifications=new std::vector<uint32_t>();
	mp_vecOutputNotifications=new std::vector<uint32_t>();
	lockID.lock();
	m_notifierID=notifierID;//for debugging
	notifierID++;
	lockID.unlock();
	m_queueSize=ms_queueSize;
	mp_vecOutputNotifications->resize(m_queueSize);
	mp_vecInputNotifications->resize(m_queueSize);
	m_usedNoteCount=0;
}

MFInputSyncNotifier::~MFInputSyncNotifier(){
	lockInputNotifications.lock();
	delete mp_vecInputNotifications;
	delete mp_vecOutputNotifications;
}

void MFInputSyncNotifier::addSyncNotification(uint32_t objectIndex){
	if(!m_enableNotification)
		return;
	lockInputNotifications.lock();
	mp_vecInputNotifications->data()[m_usedNoteCount]=objectIndex;
	m_usedNoteCount++;
//	MFObject::printInfo("MFIInputSyncNotifier::addSyncNotification - notifier id: "+
//			std::to_string(m_notifierID));
	if(m_usedNoteCount>=m_queueSize){
		if(m_queueSize<0x00010000){
			m_queueSize+=ms_queueSize;
			mp_vecOutputNotifications->resize(m_queueSize);
			mp_vecInputNotifications->resize(m_queueSize);
			MFObject::printWarning("MFIInputSyncNotifier::addSyncNotification - too many input"
					" notifications! This may happen if a module was not executed by"
					" the game loop of MFModuleManager!\nNotifierName: "+m_notifierName);
		}else{
			m_usedNoteCount-=2;
			P_WRN("max size of input queue reached!! Overwriting last notification!\n"
			    "NotifierName: "+m_notifierName);
		}
	}
	//Set next index to an invalid value (most probably less objects are added)
	mp_vecInputNotifications->data()[m_usedNoteCount]=QUEUE_END_ENTRY;

	lockInputNotifications.unlock();
};

void MFInputSyncNotifier::notifySync(uint32_t index){
  if(!m_enableNotification)
    return;
  lockInputNotifications.lock();
  mp_vecInputNotifications->data()[m_usedNoteCount]=index;
  m_usedNoteCount++;
//  MFObject::printInfo("MFIInputSyncNotifier::addSyncNotification - notifier id: "+
//      std::to_string(m_notifierID));
  if(m_usedNoteCount>=m_queueSize){
    if(m_queueSize<0x00010000){
      m_queueSize+=ms_queueSize;
      mp_vecOutputNotifications->resize(m_queueSize);
      mp_vecInputNotifications->resize(m_queueSize);
      MFObject::printWarning("MFIInputSyncNotifier::addSyncNotification - too many input"
          " notifications! This may happen if a module was not executed by"
          " the game loop of MFModuleManager!\nNotifierName: "+m_notifierName);
    }else{
      m_usedNoteCount-=2;
      MFObject::printWarning("MFIInputSyncNotifier::addSyncNotification - "
          "max size of input queue reached!! Overwriting last notification!\n"
          "NotifierName: "+m_notifierName);
    }
  }
  //Set next index to an invalid value (most probably less objects are added)
  mp_vecInputNotifications->data()[m_usedNoteCount]=QUEUE_END_ENTRY;

  lockInputNotifications.unlock();
}

const std::vector<uint32_t>* MFInputSyncNotifier::getNextInputNotifications(){
	std::vector<uint32_t>* pVecNotes;
	lockInputNotifications.lock();
	pVecNotes=mp_vecInputNotifications;
	mp_vecInputNotifications=mp_vecOutputNotifications;
	mp_vecOutputNotifications=pVecNotes;
	m_usedNoteCount=0;
	mp_vecInputNotifications->data()[0]=QUEUE_END_ENTRY;
	lockInputNotifications.unlock();
	return mp_vecOutputNotifications;
}

void MFInputSyncNotifier::enableNotification(bool enable){
	m_enableNotification=enable;
};

void MFInputSyncNotifier::clearNotifications(){
  lockInputNotifications.lock();
  m_usedNoteCount=0;
  mp_vecInputNotifications->data()[0]=QUEUE_END_ENTRY;
  lockInputNotifications.unlock();
}
