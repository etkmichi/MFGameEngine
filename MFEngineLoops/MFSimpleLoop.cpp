/*
 * MFSimpleLoop.cpp
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#include "MFSimpleLoop.h"

MFSimpleLoop::MFSimpleLoop() {
	mp_vecModules=new std::vector<MFBaseModule*>();
}

MFSimpleLoop::~MFSimpleLoop() {
	// TODO Auto-generated destructor stub
}

void MFSimpleLoop::addModule(MFBaseModule* module){
	mp_vecModules->push_back(module);
}
bool MFSimpleLoop::enterLoop(){
	for(MFBaseModule* pModule : *mp_vecModules){
		if(!pModule->execute()){
			MFObject::printWarning("A module failed to execute!");
		}
	}
	return true;
}


