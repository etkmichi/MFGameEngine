/*
 * MFTileCollection.cpp
 *
 *  Created on: 13.02.2020
 *      Author: michl
 */

#include "MFTileCollection.h"

//TODO update bullet collision shape if tile changes?!
MFTileCollection::MFTileCollection() {
	mp_middleObject=nullptr;
	m_positionOffset=glm::vec3(0,0,0);
	updatePosition();
	m_lastPosition=m_newPosition;
	mp_innerNoiseSetup=new MFNoiseSetup();
	mp_noiseSetup=mp_innerNoiseSetup;
	mp_vecUpdaterThreads=new std::vector<MFTaskThread*>();
	mp_vecUpdaterThreads->resize(1);
	m_vecTileUpdateTasks.resize(mp_vecUpdaterThreads->size());
	for(uint32_t i=0;i<mp_vecUpdaterThreads->size();i++){
		MFTaskThread* pThread=new MFTaskThread("MFTileCollection",true);
		mp_vecUpdaterThreads->data()[i]=pThread;
		m_vecTileUpdateTasks.data()[i]=new MFTileUpdate();
	}
}

MFTileCollection::~MFTileCollection() {
	delete mp_innerNoiseSetup;
	for(S_Tile* pGeo:m_vecTiles){
		pGeo->deleteResources();
		delete pGeo;
		pGeo=nullptr;
	}
	m_vecTiles.clear();
}
void MFTileCollection::setUpdaterThreadCount(uint32_t updateThreads){
	if(updateThreads==0){
		return;
	}
	if(updateThreads<mp_vecUpdaterThreads->size()){
		for(uint32_t delIndex=mp_vecUpdaterThreads->size()-1;
				delIndex>updateThreads-1;delIndex--){
			mp_vecUpdaterThreads->at(delIndex)->stop();
			delete mp_vecUpdaterThreads->at(delIndex);
			mp_vecUpdaterThreads->at(delIndex)=nullptr;
		}
	}
	mp_vecUpdaterThreads->resize(updateThreads);
}

bool MFTileCollection::createTiles() {
	updatePosition();
	m_lastPosition=m_newPosition;
	glm::vec3 transitionToStart=glm::vec3(
			(m_rangeSetup.areaSideTileCount)*(m_rangeSetup.tileLenth)/2.0f,
			(m_rangeSetup.areaSideTileCount)*(m_rangeSetup.tileLenth)/2.0f,0);
	//round the last position to the next lower whole number
	glm::vec3 startPosition=m_lastPosition-transitionToStart+m_positionOffset;
	startPosition=glm::floor(startPosition);
	float xStart=startPosition.x;
	uint32_t tileCounter=0;
	S_Tile
		*pYPrevious=nullptr;
	for(uint32_t i=0;i<m_rangeSetup.areaSideTileCount;i++){
		S_Tile
			*pXCurrentIterator=nullptr,
			*pTile=nullptr,
			*pXPrevious=nullptr,
			*pXFirst=nullptr,
			*pXLast=nullptr;
		S_Tile
			*pYCurrentIterator=nullptr,
			*pYFirst=nullptr;
		for(uint32_t j=0;j<m_rangeSetup.areaSideTileCount;j++){
			if(tileCounter<m_vecTiles.size()){
				pTile=m_vecTiles.at(tileCounter);
			}else{
				pTile=new S_Tile();
				m_vecTiles.push_back(pTile);
			}
			tileCounter++;
			pTile->pParentObject=mp_middleObject;
			pTile->init(startPosition, m_rangeSetup, mp_noiseSetup);
			startPosition+=glm::vec3(m_rangeSetup.tileLenth,0,0);
			if(pXPrevious!=nullptr){
				pXPrevious->pXNext=pTile;
				pTile->pXPrevious=pXPrevious;
				pXPrevious=pTile;
			}
			if(pXFirst!=nullptr){
				pTile->pXFirst=pXFirst;
			}
			if(j==0){//left side
				m_vecTilesXNeg.push_back(pTile);
				pXFirst=pTile;
				pTile->pXPrevious=nullptr;
				pTile->pXFirst=pXFirst;

				pXPrevious=pTile;
			}
			if(i==0){//Tile on the back
				pYFirst=pTile;
				pTile->pYFirst=pYFirst;
				pTile->pYPrevious=nullptr;
				pYPrevious=pTile;
				m_vecTilesYNeg.push_back(pTile);
			}
			if((i+1)==m_rangeSetup.areaSideTileCount){//Tile on the front
				m_vecTilesYPos.push_back(pTile);
				pTile->pYLast=pTile;
			}
		}
		m_vecTilesRight.push_back(pTile);//last tile is on the right side

		pXLast=pTile;
		pXCurrentIterator=pTile;
		while(pXCurrentIterator!=nullptr){//iterate backward (go left)
			pXCurrentIterator->pXLast=pXLast;
			if(i>0){
				pXCurrentIterator->pYPrevious=pYPrevious;
				pXCurrentIterator->pYFirst=pYPrevious->pYFirst;
				pYPrevious->pYNext=pXCurrentIterator;
				pYPrevious=pYPrevious->pXPrevious;//go one left
			}
			if((i+1)==m_rangeSetup.areaSideTileCount){//last y tiles
				pYCurrentIterator=pXCurrentIterator;
				while(pYCurrentIterator!=nullptr){
					pYCurrentIterator->pYLast=pXCurrentIterator;
					pYCurrentIterator=pYCurrentIterator->pYPrevious;
				}
			}
			pXCurrentIterator=pXCurrentIterator->pXPrevious;
		}

		pYPrevious=pTile->pXLast;// set the y previous to last of cur. row
		startPosition.x=xStart;
		startPosition+=glm::vec3(0,m_rangeSetup.tileLenth,0);
	}
	return true;
}

bool MFTileCollection::update(){
	//take care to call updatePosition before!
	if(!checkForUpdate())
		return true;

	if(checkUpdateAllTiles()){
 //TODO
	}

	float updateCountX=1.0f;
	updateCountX=glm::abs((m_newPosition.x-m_lastPosition.x)/m_rangeSetup.tileLenth);
	updateCountX=glm::ceil(updateCountX);

	float updateCountY=1.0f;
	updateCountY=glm::abs((m_newPosition.y-m_lastPosition.y)/m_rangeSetup.tileLenth);
	updateCountY=glm::ceil(updateCountY);

	if(checkUpdateNegativeX()){
		for(;updateCountX>.0f;updateCountX--)
			initUpdateXNeg();
		m_lastPosition.x=m_newPosition.x;
	}
	if(checkUpdatePositiveX()){
		for(;updateCountX>.0f;updateCountX--)
			initUpdateXPos();
		m_lastPosition.x=m_newPosition.x;
	}
	if(checkUpdateFront()){
		for(;updateCountY>.0f;updateCountY--)
			initUpdateYPos();
		m_lastPosition.y=m_newPosition.y;
	}
	if(checkUpdateBack()){
		for(;updateCountY>.0f;updateCountY--)
			initUpdateYNeg();
		m_lastPosition.y=m_newPosition.y;
	}
	return true;
}

bool MFTileCollection::initUpdateXNeg(){//negative x
	uint32_t index=0;

	for(S_Tile* pT:m_vecTilesRight){//take from r, recalc and put it left
		pT->lockTile.lock();
//		pT->disableModuleExecution();
		pT->setStartPosition(m_vecTilesXNeg.at(index)->getCurrentPosition());
		pT->moveXNeg();
		pT->lockTile.unlock();

		MFTileUpdate* pU=m_vecTileUpdateTasks.at(m_currentUpdateTask);
		pU->addTile(pT);
		mp_vecUpdaterThreads->at(m_currentUpdateTask)->addTask(pU);
		m_currentUpdateTask++;
		m_currentUpdateTask=m_currentUpdateTask%(m_vecTileUpdateTasks.size());

		m_vecTilesXNeg.at(index)=pT;
		if(pT->pXPrevious==nullptr){//take the one which should be on the right side
			m_vecTilesRight.at(index)=pT->pXLast;
			if(pT->pXLast==nullptr){
				MFObject::printErr("pT->pXLast nullptr!");
			}
		}else{
			m_vecTilesRight.at(index)=pT->pXPrevious;
		}
		index++;
	}
	return true;
}

bool MFTileCollection::initUpdateXPos(){
	uint32_t index=0;

	for(S_Tile* pT:m_vecTilesXNeg){//take from r, recalc and put it left
		pT->lockTile.lock();
		pT->setStartPosition(m_vecTilesRight.at(index)->getCurrentPosition());
		pT->moveXPos();
		pT->lockTile.unlock();

		MFTileUpdate* pU=m_vecTileUpdateTasks.at(m_currentUpdateTask);
		pU->addTile(pT);
		mp_vecUpdaterThreads->at(m_currentUpdateTask)->addTask(pU);
		m_currentUpdateTask++;
		m_currentUpdateTask=m_currentUpdateTask%(m_vecTileUpdateTasks.size());

		m_vecTilesRight.at(index)=pT;
		if(pT->pXNext==nullptr){
			m_vecTilesXNeg.at(index)=pT->pXFirst;
			if(pT->pXFirst==nullptr){
				MFObject::printErr("pT->pXFirst nullptr!");
			}
		}else{
			m_vecTilesXNeg.at(index)=pT->pXNext;
		}
		index++;
	}
	return true;
}

bool MFTileCollection::initUpdateYPos(){
	uint32_t index=0;

	for(S_Tile* pT:m_vecTilesYNeg){//take from r, recalc and put it left
		pT->lockTile.lock();
		pT->setStartPosition(m_vecTilesYPos.at(index)->getCurrentPosition());
		pT->moveYPos();
		pT->lockTile.unlock();

		MFTileUpdate* pU=m_vecTileUpdateTasks.at(m_currentUpdateTask);
		pU->addTile(pT);
		mp_vecUpdaterThreads->at(m_currentUpdateTask)->addTask(pU);
		m_currentUpdateTask++;
		m_currentUpdateTask=m_currentUpdateTask%(m_vecTileUpdateTasks.size());

		m_vecTilesYPos.at(index)=pT;
		if(pT->pYNext==nullptr){//take the one which should be on the right side
			m_vecTilesYNeg.at(index)=pT->pYFirst;//TODO test x-y updates!if(pT==nullptr){
			if(pT->pYFirst==nullptr){
				MFObject::printErr("pT->pYFirst nullptr!");
			}
		}else{
			m_vecTilesYNeg.at(index)=pT->pYNext;
		}
		index++;
	}
	return true;
}

bool MFTileCollection::initUpdateYNeg(){
	uint32_t index=0;

	for(S_Tile* pT:m_vecTilesYPos){
		pT->lockTile.lock();
		pT->setStartPosition(m_vecTilesYNeg.at(index)->getCurrentPosition());
		pT->moveYNeg();
		pT->lockTile.unlock();

		MFTileUpdate* pU=m_vecTileUpdateTasks.at(m_currentUpdateTask);
		pU->addTile(pT);
		mp_vecUpdaterThreads->at(m_currentUpdateTask)->addTask(pU);
		m_currentUpdateTask++;
		m_currentUpdateTask=m_currentUpdateTask%(m_vecTileUpdateTasks.size());

		m_vecTilesYNeg.at(index)=pT;
		if(pT->pYPrevious==nullptr){//take the one which should be on the right side
			m_vecTilesYPos.at(index)=pT->pYLast;//TODO test x-y updates!
			if(pT->pYLast==nullptr){
				MFObject::printErr("pT->pYLast nullptr!");
			}
		}else{
			m_vecTilesYPos.at(index)=pT->pYPrevious;
		}
		index++;
	}
	return true;
}
bool MFTileCollection::checkUpdateAllTiles(){
	float maxRange=(m_rangeSetup.tileLenth*m_rangeSetup.areaSideTileCount);
	if(glm::abs(m_newPosition.y-m_lastPosition.y)>=maxRange)
		return true;
	if(glm::abs(m_newPosition.x-m_lastPosition.x)>=maxRange)
		return true;
	return false;
}
bool MFTileCollection::checkForUpdate(){
	if(m_newPosition.x-m_lastPosition.x > 0.1 || m_newPosition.x-m_lastPosition.x < -0.1){
		return true;
	}
	if(m_newPosition.y-m_lastPosition.y > 0.1 || m_newPosition.y-m_lastPosition.y < -0.1){
		return true;
	}
	return false;
}

void MFTileCollection::updatePosition(){
	if(mp_middleObject!=nullptr){
		m_newPosition=glm::floor(*mp_middleObject->getModelPosition()+m_positionOffset);
	}else{
		MFObject::printInfo("MFTileCollection::updatePosition() - "
				"no middle defined!");
		m_newPosition=glm::vec3(0,0,0)+m_positionOffset;
	}
}

bool MFTileCollection::checkUpdateFront(){
	if(m_newPosition.y-m_lastPosition.y>=m_rangeSetup.tileLenth)
		return true;
	return false;
}
bool MFTileCollection::checkUpdateBack(){
	if(m_newPosition.y-m_lastPosition.y<=-m_rangeSetup.tileLenth)
		return true;
	return false;
}
bool MFTileCollection::checkUpdateNegativeX(){
	if(m_newPosition.x-m_lastPosition.x<=-m_rangeSetup.tileLenth)
		return true;
	return false;
}
bool MFTileCollection::checkUpdatePositiveX(){
	if(m_newPosition.x-m_lastPosition.x>=m_rangeSetup.tileLenth)
		return true;
	return false;
}
