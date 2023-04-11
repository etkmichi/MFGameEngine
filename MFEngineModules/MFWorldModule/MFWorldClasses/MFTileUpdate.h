/*
 * MFTileUpdate.h
 *
 *  Created on: 14.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFTILEUPDATE_H_
#define MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFTILEUPDATE_H_

#include <mutex>
#include <MFTasks/MFAbstractTask.h>
#include <MFNoise/MFNoiseSetup.h>

#include "MFWorldStructs.h"

/**
 * The task will update added tiles. Tile update queue will act as lifo. If
 * more than one tile is enqueued for update, the add function will print a warning.
 */
class MFTileUpdate: public MFAbstractTask {
private:
	S_Tile
		*mp_currentTile=nullptr;
	std::vector<S_Tile*>
		*mp_vecNextTiles;
	std::mutex
		lockTile,
		lockNextTile;
	MFNoiseSetup
		*pDebugNoise;
public:
	MFTileUpdate();
	virtual ~MFTileUpdate();
	void addTile(S_Tile* pTile);
	bool doWork();
};

#endif /* MFENGINEMODULES_MFWORLDMODULE_MFWORLDCLASSES_MFTILEUPDATE_H_ */
