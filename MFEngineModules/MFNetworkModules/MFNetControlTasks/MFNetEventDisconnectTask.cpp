/*
 * MFNetEventDisconnectTask.cpp
 *
 *  Created on: 09.04.2020
 *      Author: michl
 */

#include "MFNetEventDisconnectTask.h"

MFNetEventDisconnectTask::MFNetEventDisconnectTask(){
  setTaskName("MFNetEventDisconnectTask");
}
MFNetEventDisconnectTask::~MFNetEventDisconnectTask(){}

bool MFNetEventDisconnectTask::dispatchEvent(S_MF_NetworkEvent* pNE){
	MFObject::printInfo("MFNetEventDisconnectTask::dispatchEvent - "
			"disconnecting client!");
	return false;
}
