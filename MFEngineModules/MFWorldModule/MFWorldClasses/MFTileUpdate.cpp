/*
 * MFTileUpdate.cpp
 *
 *  Created on: 14.02.2020
 *      Author: michl
 */

#include "MFTileUpdate.h"
#include <MFNoise/MFNoiseStructs.h>

MFTileUpdate::MFTileUpdate() {
	mp_vecNextTiles=new std::vector<S_Tile*>();
	pDebugNoise=new MFNoiseSetup();
	pDebugNoise->setNoiseType(NOISE_TYPE_CONST);
}

MFTileUpdate::~MFTileUpdate() {
	delete mp_vecNextTiles;
}

void MFTileUpdate::addTile(S_Tile* pTile){
	lockNextTile.lock();
	mp_vecNextTiles->push_back(pTile);
//	if(mp_vecNextTiles->size() > 2){
//		MFObject::printWarning("MFTileUpdate::addUpdateableTile - "
//				"more than one updates enqueued! "
//				"CPU to slow for fast update!");
//	}
	lockNextTile.unlock();
}

bool MFTileUpdate::doWork(){//TODO check if new position changed, if not skip update
	bool ret=true;
	lockNextTile.lock();
	lockTile.lock();
	if(mp_vecNextTiles->size()>0 && mp_currentTile==nullptr){
		mp_currentTile=mp_vecNextTiles->back();//TODO lifo is not the best way!
		mp_vecNextTiles->pop_back();
	}
	lockNextTile.unlock();
	if(mp_currentTile==nullptr){
		lockTile.unlock();
		return true;
	}
	mp_currentTile->lockTile.lock();
	//TODO init object with MFAbstractSetup::setupObject(MFVkSceneObject* object)
	//TODO test from another thread - use multithreading copying data takes a lot of time
	if(mp_currentTile->pTileSO->getState()==MFSyncObject::STATE_DISABLED){
	}

//	mp_currentTile->pTile->setNoiseSetup(pDebugNoise);
	if(!mp_currentTile->pGeometry->isCreated()){
		MFObject::printInfo("MFTileUpdate::doWork()- tile not created, returning!");
		mp_currentTile->lockTile.unlock();
		lockTile.unlock();
		return true;
	}
	ret=mp_currentTile->updateTileData();
//	mp_currentTile->enableModuleExecution();
	mp_currentTile->lockTile.unlock();
	mp_currentTile->pTileSO->setGeometry(mp_currentTile->pGeometry);
	mp_currentTile->pTileSO->updateObject();
	mp_currentTile=nullptr;
	lockTile.unlock();
	return ret;
	//TODO search for disabled?
//	mp_currentTile->lockTile.unlock();
//	lockTile.unlock();
//	return ret;
}
