/*
 * MFSpawnGeometry.cpp
 *
 *  Created on: 09.01.2020
 *      Author: michl
 */

#include "MFSpawnGeometry.h"
#include <MFObjects/MFObject.h>

//TODO move calculation of geometry data to a new thread.
//	- Do it in the render library
//	- Use default data first (created by MFAbstractGeo::create...
//	- replace default data dynamically during execution with the optimized data.
MFTaskThread* MFSpawnGeometry::mps_prepareThread=new MFTaskThread("MFSpawnGeometry");

MFSpawnGeometry::MFSpawnGeometry(MFModuleManager* pModMgr){
	mp_spawnGroup=nullptr;
	mp_geometry=mp_objectManager->getGeometryManager()->createSphere(0.5f);
	mp_moduleManager=pModMgr;
	mp_objectManager=pModMgr->getObjectManager();
	init();
}

void MFSpawnGeometry::init(){
	mp_initialSetup=new MFInitialObjectSetup();
	if(mp_geometry!=nullptr){
		mp_initialSetup->mp_geometry=mp_geometry;
	}
}

MFSpawnGeometry::~MFSpawnGeometry() {
	// TODO Auto-generated destructor stub
}

void MFSpawnGeometry::updateSetupValues(){

}

bool MFSpawnGeometry::doWork(){
	MFSyncObject* pSO=new MFSyncObject(
			mp_moduleManager->getSyncObjectResources(),
			mp_initialSetup);
//	(*mp_initialSetup->mp_initVelocity)=glm::vec3(10.0f,1000.0f,1.0f);
	if(mp_spawnGroup!=nullptr){
		mp_objectManager->addCustomObject(
				pSO,mp_spawnGroup);
		mp_objectManager->addDeleteableSyncObject(pSO);
	}
	mp_lastSpawnedObject=pSO;
	return true;
}

bool MFSpawnGeometry::undoWork(){return false;}
