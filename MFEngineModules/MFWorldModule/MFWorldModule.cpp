/*
 * MFWorldModule.cpp
 *
 *  Created on: 12.02.2020
 *      Author: michl
 */

#include "MFWorldModule.h"

MFWorldModule::MFWorldModule(
		MFModuleObjectManager* pOM,
		MFIModuleGroupControl* pModuleGroupProvider,
		MFSyncObjectResources* pSyncRes) :
		MFBaseModule(pSyncRes),
    MFIModuleCreator("MFWorldModule") {
	mp_objectMgr=pOM;
	mp_groupProvider=pModuleGroupProvider;
}

void MFWorldModule::updateResources(MFModuleResources* pRes){
}

MFWorldModule::~MFWorldModule() {
}

bool MFWorldModule::initModule(){
	return true;
}

bool MFWorldModule::executeModule(){
	//update all module objects
	bool ret=true;
	for(ObjectData* pOD:*mp_vecModuleObjects){
		if(pOD!=nullptr && pOD->pModuleData!=nullptr){
			MFWorldModuleObject* pWMO=(MFWorldModuleObject*)pOD->pModuleData;
			ret&=pWMO->updateTiles();
		}//TODO move from syncInput... to executeModule!
	}
	return ret;
}

bool MFWorldModule::syncOutputData(){
	//add calculated tiles to scene by the object manager
	return true;//no output...
}

bool MFWorldModule::initCollection(MFWorldModuleObject* pWMO){
	bool ret=pWMO->initTiles(false);/*iterate over the tiles add them to the game engine and
	 * set each tiles MFSyncObject object*/
	//TODO test external creation
	pWMO->getTileCollection()->updatePosition();
	for(S_Tile* pT:*pWMO->getTileCollection()->getVecTiles()){
		MFSyncObject* pTileSO=mp_objectMgr->addCustomObject(
				pT->getCurrentPosition(),
				glm::vec3(1.0f,0.0f,0.0f),
				pT->pGeometry,
				0.0f,
				glm::vec4(0.0f,1.0f,1.0f,1.0f),
				mp_groupProvider->getModuleGroup(0));
		pT->pTileSO=pTileSO;
		ret&=(pTileSO!=nullptr);
	}
	return ret;
}

bool MFWorldModule::initModuleObject(
		MFSyncObject* pSO,
		MFModuleObject* pObject){
	S_RangeSetup sideTiles;
	sideTiles.areaSideTileCount=2;//TODO side tile count and tile leng depends on the
	sideTiles.tileLenth=40.0f;//middle tile!
	sideTiles.pointsPerTileLength=9;
	MFWorldModuleObject* pWMO=(MFWorldModuleObject*)(pObject);
	pWMO->setSyncObject(pSO);
	pWMO->getTileCollection()->setMiddleObject(pSO);
	pWMO->getTileCollection()->updatePosition();
	pWMO->getTileCollection()->getRangeSetup()=sideTiles;
	//TODO use S_WorldSetup for creation of all tiles...
	if(!initCollection(pWMO)){
		MFObject::printErr("MFWorldModule::addSyncObjectToModule - "
				"failed to set up main tile!");
		return false;
	}

	MFWorldModuleObject* pWMOXPos=pWMO->addSurroundingTile();
	MFWorldModuleObject* pWMOXNeg=pWMO->addSurroundingTile();
	MFWorldModuleObject* pWMOYPos=pWMO->addSurroundingTile();
	MFWorldModuleObject* pWMOYNeg=pWMO->addSurroundingTile();

	pWMOXPos->getTileCollection()->getRangeSetup()=sideTiles;
	pWMOXNeg->getTileCollection()->getRangeSetup()=sideTiles;
	pWMOYPos->getTileCollection()->getRangeSetup()=sideTiles;
	pWMOYNeg->getTileCollection()->getRangeSetup()=sideTiles;

	float dist=(0.25f*(
			(pWMO->getTileCollection()->getRangeSetup().areaSideTileCount*1.0f*
			pWMO->getTileCollection()->getRangeSetup().tileLenth*1.0f)+
			(sideTiles.areaSideTileCount*1.0f*sideTiles.tileLenth*1.0f)));

	pWMOXPos->getTileCollection()->setPositionOffset(glm::vec3( dist,0,0));
	pWMOXNeg->getTileCollection()->setPositionOffset(glm::vec3(-1.0f*dist,0,0));
	pWMOYPos->getTileCollection()->setPositionOffset(glm::vec3(0, dist,0));
	pWMOYNeg->getTileCollection()->setPositionOffset(glm::vec3(0,-1.0f*dist,0));
	if(!initCollection(pWMOXNeg))MFObject::printErr(
			"MFWorldModule::addSyncObjectToModule - failed to set up sub tile!");
	if(!initCollection(pWMOXPos))MFObject::printErr(
			"MFWorldModule::addSyncObjectToModule - failed to set up sub tile!");
	if(!initCollection(pWMOYPos))MFObject::printErr(
			"MFWorldModule::addSyncObjectToModule - failed to set up sub tile!");
	if(!initCollection(pWMOYNeg))MFObject::printErr(
			"MFWorldModule::addSyncObjectToModule - failed to set up sub tile!");
	return true;
}
