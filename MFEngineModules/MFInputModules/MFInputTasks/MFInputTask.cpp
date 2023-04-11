/*
 * MFInputTask.cpp
 *
 *  Created on: 29.10.2019
 *      Author: michl
 */

#include "MFInputTask.h"

uint32_t MFInputTask::ms_inputActionQueueSize=20;

MFInputTask::MFInputTask(){
	mp_keyListener=nullptr;
	mp_mouseListener=nullptr;
	mp_joystickListener=nullptr;
	mp_camObject=nullptr;

	m_inputQueueSize=ms_inputActionQueueSize;
	mp_ticker=new MFTickCounter();
	mp_vecKeyDispatchData=new std::vector<KeyDispatchData*>();
	mp_vecKeyDispatchData->resize(m_inputQueueSize);
	for(uint32_t i=0;i<m_inputQueueSize;i++){
		mp_vecKeyDispatchData->data()[i]=new KeyDispatchData();
	}

	mp_vecJSAxeDispatchData=new std::vector<JoystickAxeDispatchData*>();
	mp_vecJSAxeDispatchData->resize(m_inputQueueSize);
	for(uint32_t i=0;i<m_inputQueueSize;i++){
		mp_vecJSAxeDispatchData->data()[i]=new JoystickAxeDispatchData();
	}

	mp_vecJSBtnDispatchData=new std::vector<JoystickButtonDispatchData*>();
	mp_vecJSBtnDispatchData->resize(m_inputQueueSize);
	for(uint32_t i=0;i<m_inputQueueSize;i++){
		mp_vecJSBtnDispatchData->data()[i]=new JoystickButtonDispatchData();
	}

	mp_vecJSHatDispatchData=new std::vector<JoystickHatDispatchData*>();
	mp_vecJSHatDispatchData->resize(m_inputQueueSize);
	for(uint32_t i=0;i<m_inputQueueSize;i++){
		mp_vecJSHatDispatchData->data()[i]=new JoystickHatDispatchData();
	}

	mp_mouseDispatchData=new MouseDispatchData();

	m_currentVecKeyIndex=0;
	m_axeQIndex=0;
	m_hatQIndex=0;
	m_btnIndex=0;
}

MFInputTask::~MFInputTask() {
	delete mp_vecKeyDispatchData;
	delete mp_vecJSAxeDispatchData;
	delete mp_vecJSBtnDispatchData;
	delete mp_vecJSHatDispatchData;
}

bool MFInputTask::doWork(){
	bool ret=true;
	m_timeSinceLastExecution=mp_ticker->stop();
	mp_ticker->start();
	uint32_t indexCounter=0;
	if(m_enableExecution){
		if(m_enableKeyExecution){
			for(KeyDispatchData *dispatchData : *mp_vecKeyDispatchData){
				indexCounter++;
				if(indexCounter>m_currentVecKeyIndex){//TODO lock m_current...
					m_currentVecKeyIndex=0;
					indexCounter=0;
					break;
				}
				ret&=executeKeyAction(dispatchData);
			}
		}

		if(m_enableMouseExecution && mp_mouseListener!=nullptr){
			mp_mouseDispatchData->pMouseListener->pollPosition(
					m_currentX,
					m_currentY);
			mp_mouseDispatchData->deltaX=m_lastX-m_currentX;
			mp_mouseDispatchData->deltaY=m_lastY-m_currentY;
			m_lastX=m_currentX;
			m_lastY=m_currentY;
			ret&=executeMouseAction(mp_mouseDispatchData);
		}

		if(m_enableJoystickAxeExecution && mp_joystickListener!=nullptr){
			for(JoystickAxeDispatchData *dispatchData : *mp_vecJSAxeDispatchData){
				indexCounter++;
				if(indexCounter>m_axeQIndex){
					break;
				}
				ret&=executeJoystickAxeAction(dispatchData);
			}
      m_axeQIndex=0;
		}

		if(m_enableJoystickButtonExecution){
			indexCounter=0;
			for(JoystickButtonDispatchData *dispatchData : *mp_vecJSBtnDispatchData){
				indexCounter++;
				if(indexCounter>m_btnIndex){
					m_btnIndex=0;
					indexCounter=0;
					break;
				}
				ret&=executeJoystickButtonAction(dispatchData);
			}
		}

		if(m_enableJoystickHatExecution){
			for(JoystickHatDispatchData *dispatchData : *mp_vecJSHatDispatchData){
				indexCounter++;
				if(indexCounter>m_hatQIndex){
					m_hatQIndex=0;
					indexCounter=0;
					break;
				}
				ret&=executeJoystickHatAction(dispatchData);
			}
		}
		if(m_enableCommonUpdate){
		  executeCommonInputUpdate();
		}
	}
	m_timeTaskExecution=mp_ticker->stop();
  mp_ticker->start();
	return ret;
}

void MFInputTask::addKeyAction(KeyAction& action,MFSyncObject* syncObject){
  if(!m_enableExecution || !m_enableKeyExecution)return;
	if(m_currentVecKeyIndex>=m_inputQueueSize){
		P_WRN("input queue full! input data will be be lost! task name: "+getTaskName());
		m_currentVecKeyIndex=0;
	}
	KeyDispatchData* dispatchData=mp_vecKeyDispatchData->at(m_currentVecKeyIndex);
	m_currentVecKeyIndex++;

	dispatchData->action=action;
	dispatchData->pObject=syncObject;
}

void MFInputTask::addJoystickAxeAction(
		JoystickAxisAction* pAction,
		MFSyncObject* syncObject){
  if(!m_enableExecution || !m_enableJoystickAxeExecution)return;
	if(m_axeQIndex>=m_inputQueueSize){
	  P_WRN("input queue full! input data will be lost! task name: "+getTaskName());
		m_axeQIndex=0;
	}

	JoystickAxeDispatchData* dispatchData=mp_vecJSAxeDispatchData->at(m_axeQIndex);
	m_axeQIndex++;
	dispatchData->pAction=pAction;
	dispatchData->pObject=syncObject;//TODO improve code, no need for new for js
}

void MFInputTask::addJoystickButtonAction(
		JoystickAction* pAction,
		MFSyncObject* syncObject){
  if(!m_enableExecution || !m_enableJoystickButtonExecution)return;
	if(m_btnIndex>=m_inputQueueSize){
    P_WRN("input queue full! input data will be be lost! task name: "+getTaskName());
		m_btnIndex=0;
	}

	JoystickButtonDispatchData* dispatchData=mp_vecJSBtnDispatchData->at(m_btnIndex);
	m_btnIndex++;
	dispatchData->pAction=pAction;
	dispatchData->pObject=syncObject;
}

void MFInputTask::addJoystickHatAction(
		JoystickAction* pAction,
		MFSyncObject* syncObject){
  if(!m_enableExecution || !m_enableJoystickHatExecution)return;
	if(m_hatQIndex>=m_inputQueueSize){
    P_WRN("input queue full! input data will be be lost! task name: "+getTaskName());
		m_hatQIndex=0;
	}
	JoystickHatDispatchData* dispatchData=mp_vecJSHatDispatchData->at(m_hatQIndex);
	m_hatQIndex++;
	dispatchData->pAction=pAction;
	dispatchData->pObject=syncObject;
}
