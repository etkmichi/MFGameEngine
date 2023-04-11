/*
 * MFWorldModuleObject.cpp
 *
 *  Created on: 13.02.2020
 *      Author: michl
 */

#include "MFWorldModuleObject.h"
MFTaskThread* MFWorldModuleObject::smp_initialThread=nullptr;

MFWorldModuleObject::MFWorldModuleObject() {
  if(smp_initialThread==nullptr)
    smp_initialThread=new MFTaskThread("MFWorldModuleObject");
	mp_vecSurroundingTiles=new std::vector<MFWorldModuleObject*>();
	mp_tileCollection=new MFTileCollection();
}

void MFWorldModuleObject::setupObjectDataStructure(){
	//TODO what structure??
	//the most reasonable setup is the one of the tile collections MFNoiseSetup
	// But thats a nested structure and its not really nice to have.
	//How to integrate multiple MFStructurableDataSetup's into one
	//MFStructurableDataSetup object.
}

MFWorldModuleObject::~MFWorldModuleObject() {
}

bool MFWorldModuleObject::initTiles(bool externalInitThread){
	mp_tileCollection=new MFTileCollection();
	mp_tileCollection->setMiddleObject(mp_syncObject);
	mp_syncObject->setState(MFSyncObject::STATE_CREATION);
	bool ret=true;
	if(!externalInitThread){
		ret&=mp_tileCollection->createTiles();
	}else{
		smp_initialThread->addTask(this);
		if(!smp_initialThread->isStarted())
			smp_initialThread->startDetached();
	}
	return ret;
}

void MFWorldModuleObject::disable(){
	m_enabled=false;//TODO stop running calculations...
}

void MFWorldModuleObject::enable(){
	m_enabled=true;//TODO start calculations again, with updated position...
}

bool MFWorldModuleObject::updateTiles(){
	bool ret=true;
	ret&=mp_tileCollection->update();
	for(MFWorldModuleObject* pWMO:*mp_vecSurroundingTiles){
		ret&=pWMO->getTileCollection()->update();
	}
	return ret;
}

bool MFWorldModuleObject::synchronizeInput(){
	if(!m_enabled)return true;
	for(MFWorldModuleObject* pWo:*mp_vecSurroundingTiles){
		pWo->getTileCollection()->updatePosition();
	}
	mp_tileCollection->updatePosition();
	return true;
}

bool MFWorldModuleObject::synchronizeOutput(){

	return true;
}

bool MFWorldModuleObject::doWork(){
	bool ret=mp_tileCollection->createTiles();
	if(ret){
		mp_syncObject->setStateTransition(MFSyncObject::TRANSITION_ENABLE);
	}else{
		MFObject::printErr("MFWorldModuleObject::doWork() - failed"
				" to create tiles!");
	}
	return ret;
}

bool MFWorldModuleObject::undoWork(){
	return true;
}

MFWorldModuleObject* MFWorldModuleObject::addSurroundingTile(){
	MFWorldModuleObject* pST=new MFWorldModuleObject();
	pST->setSyncObject(mp_syncObject);
	mp_vecSurroundingTiles->push_back(pST);
	return pST;
}
