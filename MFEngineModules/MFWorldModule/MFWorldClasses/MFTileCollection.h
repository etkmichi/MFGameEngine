/*
 * MFTileCollection.h
 *
 *  Created on: 13.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFTILECOLLECTION_H_
#define MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFTILECOLLECTION_H_
#include <vector>
#include <MFNoise/MFNoiseTile.h>
#include <MFNoise/MFNoiseSetup.h>
#include <MFBaseGeometries/MFNoiseGeometry.h>
#include <MFThreadSystem/MFTaskThread.h>

#include "MFWorldStructs.h"
#include "MFTileUpdate.h"
#include "../../../MFSynchronisationClasses/MFSyncObject.h"
/**
 * This class calculates all necessary environment data in separated threads.
 * It will calculate all data from a specific middle point. If the middle point
 * moves this class will check for the position and update all close geometries.
 */
class MFTileCollection {
private:
	S_RangeSetup
		m_rangeSetup;
	MFSyncObject
		*mp_middleObject;
	glm::vec3
		m_lastPosition,
		m_newPosition,
		m_positionOffset;
	std::vector<S_Tile*>
		m_vecTiles;
	std::vector<S_Tile*>
		m_vecTilesYPos,
		m_vecTilesYNeg,
		m_vecTilesXNeg,
		m_vecTilesRight;
	MFNoiseSetup
		*mp_noiseSetup,
		*mp_innerNoiseSetup;
	std::vector<MFTileUpdate*>
		m_vecTileUpdateTasks;
	std::vector<MFTaskThread*>
		*mp_vecUpdaterThreads;
	uint32_t
		m_currentUpdateTask=0;
public:
	MFTileCollection();
	virtual ~MFTileCollection();
	std::vector<S_Tile*>* getVecTiles(){return &m_vecTiles;};
	bool createTiles();
	bool update();
	void updatePosition();
	bool checkForUpdate();
	void setMiddleObject(MFSyncObject* pMiddleObject){mp_middleObject=pMiddleObject;};
	void setPositionOffset(glm::vec3 posOffset){m_positionOffset=posOffset;};
	void setUpdaterThreadCount(uint32_t updateThreads);

	/**
	 * This function checks if the middle point is further away then the range of
	 * the tile collection.
	 * @return true if the middle point so was "teleported" out of the range of the
	 * collection.
	 */
	inline bool checkUpdateAllTiles();
	inline bool checkUpdateFront();
	inline bool checkUpdateBack();
	inline bool checkUpdateNegativeX();
	inline bool checkUpdatePositiveX();
	bool initUpdateXNeg();
	bool initUpdateXPos();
	bool initUpdateYPos();
	bool initUpdateYNeg();
	S_RangeSetup& getRangeSetup(){return m_rangeSetup;};

};

#endif /* MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFTILECOLLECTION_H_ */
