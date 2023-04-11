/*
 * MFSpawnGeometry.h
 *
 *  Created on: 09.01.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFSPAWNACTIONS_MFSPAWNGEOMETRY_H_
#define MFSCENEACTIONS_MFSPAWNACTIONS_MFSPAWNGEOMETRY_H_
#include "../MFBasicActions/MFBaseActionTask.h"
#include "../../MFEngineModules/MFModuleManager.h"
#include "../../MFEngineModules/MFModuleObjectManager.h"
#include "../../MFObjectSetup/MFInitialObjectSetup.h"
#include <glm/glm.hpp>
#include <MFBaseGeometries/MFAbstractGeometry.h>
#include "../../MFEngineModules/MFModuleGroup.h"
/**
 * This class provides functionality for spawning advanced scene objects.
 * Attention: If the set...Ptr functions or the constructor with pointers are used, the
 * values set by set functions will be ignored! The values will be overwritten by the
 * pointed values during doWork call!
 * The set...Ptr functions can be used for dynamically changing the spawn setup without
 * calling the set functions, be patient to use valid pointers!
 */
class MFSpawnGeometry : public MFBaseActionTask {
private:
	void init();
	static MFTaskThread
		*mps_prepareThread;
	MFSyncObject
		*mp_lastSpawnedObject=nullptr;

	MFAbstractGeometry
		*mp_geometry;
	MFModuleManager
		*mp_moduleManager;
	MFModuleObjectManager
		*mp_objectManager;
	MFInitialObjectSetup
		*mp_initialSetup;
	MFModuleGroup
		*mp_spawnGroup;

public:
	MFSpawnGeometry(MFModuleManager* pModMgr);

	virtual ~MFSpawnGeometry();

	bool doWork();
	bool undoWork();
	void updateSetupValues();

	/**
	 * Use this method to get the object setup for changing its pointer.
	 * @return
	 */
	MFInitialObjectSetup* getObjectSetup(){return mp_initialSetup;};
	MFSyncObject* getLastSpawnedObject(){return mp_lastSpawnedObject;};

	void setSpawnGroup(MFModuleGroup* pSpwnG){mp_spawnGroup=pSpwnG;};
};

#endif /* MFSCENEACTIONS_MFSPAWNACTIONS_MFSPAWNGEOMETRY_MFSPAWNGEOMETRY_H_ */
