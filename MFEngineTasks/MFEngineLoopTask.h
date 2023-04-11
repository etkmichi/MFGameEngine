/*
 * MFEngineLoopTask.h
 *
 *  Created on: 01.07.2020
 *      Author: michl
 */

#ifndef MFENGINETASKS_MFENGINELOOPTASK_H_
#define MFENGINETASKS_MFENGINELOOPTASK_H_

#include <MFTasks/MFAbstractTask.h>
#include "../MFEngineModules/MFInterfacesModules/MFIEngineLoopControl.h"

class MFEngineLoopTask: public MFAbstractTask {
private:
	bool
		*mp_externalClose=nullptr;
	MFIEngineLoopControl
		*mp_loopCtl;
public:
	MFEngineLoopTask(MFIEngineLoopControl* pLoopCtrl,bool *pExternalClose);
	virtual ~MFEngineLoopTask();

	virtual bool doWork();
	virtual bool undoWork(){return true;};
	void setExternalClose(bool* externalClose){mp_externalClose=externalClose;};
	void setLoopCtl(MFIEngineLoopControl* pLoopCtrl){mp_loopCtl=pLoopCtrl;};

};

#endif /* MFENGINETASKS_MFENGINELOOPTASK_H_ */
