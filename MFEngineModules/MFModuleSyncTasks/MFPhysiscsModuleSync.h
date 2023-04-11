/*
 * MFPhysiscsModuleSync.h
 *
 *  Created on: 08.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULESYNCTASKS_MFPHYSISCSMODULESYNC_H_
#define MFENGINEMODULES_MFMODULESYNCTASKS_MFPHYSISCSMODULESYNC_H_

#include "MFModuleSyncTask.h"
#include "../ModulePhysics/MFPhysicModuleObject.h"
#include "../ModulePhysics/MFPhysicsModule.h"

/**
 * This class synchronizes server and client physics data. On server it will
 * copy all server data for streaming to a client. On a client it will copy streamed
 * data to the physics module's objects.
 */
class MFPhysiscsModuleSync : public MFModuleSyncTask {
private:
	MFPhysicsModule
		*mp_module;
public:
	/*virtual functions MFModuleSyncTask:*/
	virtual void updateSyncData();
	virtual bool updateModule(MFStructurableData* inputData);

public:
	/**
	 *
	 * @param pModule - module for creating the sync data
	 * @param sharedModuleIndex - index a external module which shall be syncronized
	 * @param syncCycleCount - cycles till a new sync copy shall be made.
	 */
	MFPhysiscsModuleSync(
			MFPhysicsModule* pModule,
			uint32_t sharedTaskIndex,
			uint32_t syncCycleCount=500
			);
	virtual ~MFPhysiscsModuleSync();
	void setModule(MFPhysicsModule* pModule){mp_module=pModule;};
};

#endif /* MFENGINEMODULES_MFMODULESYNCTASKS_MFPHYSISCSMODULESYNC_H_ */
