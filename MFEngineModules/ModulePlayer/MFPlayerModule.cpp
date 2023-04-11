/*
 * MFPlayerModule.cpp
 *
 *  Created on: 02.12.2020
 *      Author: michl
 */

#include "../ModulePlayer/MFPlayerModule.h"

MFPlayerModule::MFPlayerModule(MFSyncObjectResources* pSyncRes) :
  MFBaseModule(pSyncRes),
  MFIModuleCreator("MFPlayerModule"){
  setStructureName("MFPlayerModule");
  setSetupName("MFPlayerModule");
}

MFPlayerModule::~MFPlayerModule() {
}

void MFPlayerModule::updateResources(MFModuleResources* pRes){

}
MFBaseModule* MFPlayerModule::createModule(){
  return new MFPlayerModule(mp_syncRes);
}
bool MFPlayerModule::initModule(){
  return true;
}
bool MFPlayerModule::executeModule(){
  return true;
}
