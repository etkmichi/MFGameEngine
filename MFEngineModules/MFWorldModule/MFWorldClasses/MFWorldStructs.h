/*
 *MFWorldStructs.h
 *
 * Created on: 14.02.2020
 *     Author: michl
 */

#ifndef MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFWORLDSTRUCTS_H_
#define MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFWORLDSTRUCTS_H_

#include <mutex>
#include <MFNoise/MFNoiseSetup.h>
#include <MFBaseGeometries/MFNoiseGeometry.h>

#include "../../../MFSynchronisationClasses/MFSyncObject.h"
//TODO take care, MFSyncObject includes MFModuleObject and MFBaseModule
//and MFTileCollection includes this header

struct S_RangeSetup{
	float
		tileLenth=20.0f;
	uint32_t
		pointsPerTileLength=1,
		areaSideTileCount=5;//count of
	void operator=(S_RangeSetup& other){
		tileLenth=other.tileLenth;
		pointsPerTileLength=other.pointsPerTileLength;
		areaSideTileCount=other.areaSideTileCount;
	}
};

struct S_Tile{
	std::mutex
		lockTile;
	MFNoiseGeometry
		*pGeometry;
	MFNoiseTile
		*pTile=nullptr;
	MFSyncObject
		*pParentObject=nullptr,
		*pTileSO=nullptr;

	S_Tile
		*pXNext=nullptr,
		*pXPrevious=nullptr,
		*pXFirst=nullptr,
		*pXLast=nullptr;

	S_Tile
		*pYNext=nullptr,
		*pYPrevious=nullptr,
		*pYFirst=nullptr,
		*pYLast=nullptr;

	void init(
			glm::vec3 startPosition,
			S_RangeSetup rangeSetup,
			MFNoiseSetup*pNoiseSetup){
		if(pTile==nullptr)
			pTile=new MFNoiseTile();
		pTile->setNoiseSetup(pNoiseSetup);
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition=startPosition;
		tileSetup->xRange=rangeSetup.tileLenth;
		tileSetup->yRange=rangeSetup.tileLenth;
		tileSetup->xSteps=rangeSetup.pointsPerTileLength;
		tileSetup->ySteps=rangeSetup.pointsPerTileLength;
		if(pGeometry==nullptr)
			pGeometry=new MFNoiseGeometry(pTile);
		pGeometry->initGeometry(E_CREATE_BY_COLLECTION);
		pGeometry->createCollisionShape();
	}

	void setStartPosition(glm::vec3 startPosition){
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition=startPosition;
	}

	void moveStartPosition(glm::vec3& movePosition){
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition+=movePosition;
		if(pTileSO!=nullptr){
			pTileSO->setModelPosition(tileSetup->startPosition);
		}
	}

	void moveXNeg(){
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition+=glm::vec3(-tileSetup->xRange,0,0);
		if(pTileSO!=nullptr){
			pTileSO->setModelPosition(tileSetup->startPosition);
		}
	}

	void moveXPos(){
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition+=glm::vec3(tileSetup->xRange,0,0);
		if(pTileSO!=nullptr){
			pTileSO->setModelPosition(tileSetup->startPosition);
		}
	}

	void moveYPos(){
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition+=glm::vec3(0,tileSetup->yRange,0);
		if(pTileSO!=nullptr){
			pTileSO->setModelPosition(tileSetup->startPosition);
		}
	}

	void moveYNeg(){
		S_MFCalculationSetup* tileSetup=pTile->getCalculationSetup();
		tileSetup->startPosition+=glm::vec3(0,-tileSetup->yRange,0);
		if(pTileSO!=nullptr){
			pTileSO->setModelPosition(tileSetup->startPosition);
		}
	}

	void deleteResources(){
		delete pGeometry;
		delete pTile;
	}

	bool updateTileData(){
		return pGeometry->updateData();
	}

	glm::vec3 getCurrentPosition(){
		return pTile->getCalculationSetup()->startPosition;
	}

	void disableModuleExecution(){
		if(pTileSO!=nullptr)
			pTileSO->disableObject();
	}
	void enableModuleExecution(){
		if(pTileSO!=nullptr)
			pTileSO->enableObject();
	}
	void removeFromModules(){
		if(pTileSO!=nullptr)
			pTileSO->setStateTransition(MFSyncObject::TRANSITION_REMOVE);
	}
};



#endif /* MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFWORLDSTRUCTS_H_ */
