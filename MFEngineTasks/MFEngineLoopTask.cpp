/*
 * MFEngineLoopTask.cpp
 *
 *  Created on: 01.07.2020
 *      Author: michl
 */

#include "MFEngineLoopTask.h"

MFEngineLoopTask::MFEngineLoopTask(MFIEngineLoopControl* pLoopCtrl,bool *pExternalClose) {
	mp_externalClose=pExternalClose;
	mp_loopCtl=pLoopCtrl;
}

MFEngineLoopTask::~MFEngineLoopTask() {
	// TODO Auto-generated destructor stub
}

bool MFEngineLoopTask::doWork(){
	if(mp_externalClose==nullptr || !(*mp_externalClose) ){
		if(mp_loopCtl!=nullptr){
			mp_loopCtl->executeModuleStep();
		}
	}
	if(mp_externalClose!=nullptr && *mp_externalClose){
		mp_loopCtl->stopExternalModuleLoop();
	}
	return true;
}
