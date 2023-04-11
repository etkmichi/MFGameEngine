/*
 * MFNetworkResources.cpp
 *
 *  Created on: 30.07.2020
 *      Author: michl
 */
#include "MFNetworkResources.h"
#include "MFNetControlTasks/MFNetClientConnectDispatchTask.h"
#include "MFNetControlTasks/MFNetEventDisconnectTask.h"
#include "MFNetChannelSinks/MFEngineControlSink.h"
#include "MFNetChannelSinks/MFSyncStreamSink.h"

void MFNetworkResources::initDispatchTasks(){
	if(mp_dataDispatchTask==nullptr)
	  mp_dataDispatchTask=new MFNetEventDispatchTask();
	if(mp_connectDispatchTask==nullptr)
	  mp_connectDispatchTask=new MFNetClientConnectDispatchTask(this);
	if(mp_disconnectDispatchTask==nullptr)
	  mp_disconnectDispatchTask=new MFNetEventDisconnectTask();
	mp_dataDispatchTask->m_printIndex=m_printIndex;
	mp_connectDispatchTask->m_printIndex=m_printIndex;
	mp_disconnectDispatchTask->m_printIndex=m_printIndex;
}

void MFNetworkResources::initChannelSinks(){
  if(mp_vecChannelSinks==nullptr || mp_vecChannelSinks->size()==0){
    mp_vecChannelSinks=new std::vector<MFINetChannelSink*>();
    MFEngineControlSink* pCtr=new MFEngineControlSink(this);
    pCtr->m_printIndex=m_printIndex;
    mp_vecChannelSinks->push_back(pCtr);
    MFSyncStreamSink *pSSS=new MFSyncStreamSink(this);
    pSSS->m_printIndex=m_printIndex;
    mp_vecChannelSinks->push_back(pSSS);
  }
	mp_dataDispatchTask->setVecChannelSinks(mp_vecChannelSinks);
}




