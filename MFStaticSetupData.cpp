/*
 * MFStaticSetupData.cpp
 *
 *  Created on: 24.04.2020
 *      Author: michl
 */

#include "MFStaticSetupData.h"
std::string MFStaticSetupData::sm_glslCompilerPath=
		"/home/michl/eclipse-workspace-cpp/MFRenderLibraries/glslangValidator";
std::string MFStaticSetupData::sm_engineDataPath=
		"/home/michl/eclipse-workspace-cpp/MFTestings/";
MFSyncObjectResources* MFStaticSetupData::smp_syncRes=nullptr;
void* MFStaticSetupData::smp_moduleManager=nullptr;
std::mutex MFStaticSetupData::sLockStaticSetupData;


