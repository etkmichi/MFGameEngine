/*
 * MFMoveAction.h
 *
 *  Created on: 13.03.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFMOVEMENTACTIONS_MFMOVEACTION_H_
#define MFSCENEACTIONS_MFMOVEMENTACTIONS_MFMOVEACTION_H_

#include "../MFBasicActions/MFBaseActionTask.h"

class MFMoveAction: public MFBaseActionTask {
public:
	MFMoveAction();
	virtual ~MFMoveAction();

	virtual bool doWork();
	virtual bool undoWork();
};

#endif /* MFSCENEACTIONS_MFMOVEMENTACTIONS_MFMOVEACTION_H_ */
