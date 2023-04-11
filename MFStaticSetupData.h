/*
 * MFStaticSetupData.h
 *
 *  Created on: 24.04.2020
 *      Author: michl
 */

#ifndef MFSTATICSETUPDATA_H_
#define MFSTATICSETUPDATA_H_
#include <string>
#include <mutex>
#include "MFSynchronisationClasses/MFSyncObjectResources.h"
class MFStaticSetupData {
public:
	static std::mutex
		sLockStaticSetupData;
	static std::string
		sm_glslCompilerPath,
		sm_engineDataPath;
	static MFSyncObjectResources
		*smp_syncRes;
	static void
		*smp_moduleManager;
};

#endif /* MFSTATICSETUPDATA_H_ */
