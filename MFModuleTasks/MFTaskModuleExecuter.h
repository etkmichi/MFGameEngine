/*
 * MFTaskModuleExecuter.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFMODULETASKS_MFTASKMODULEEXECUTER_H_
#define MFMODULETASKS_MFTASKMODULEEXECUTER_H_

#include "MFTasks/MFAbstractTask.h"
#include "../MFEngineModules/MFBaseModule.h"

class MFTaskModuleExecuter : MFAbstractTask{
private:
	MFBaseModule
		*mp_executableModule;
public:
	MFTaskModuleExecuter(MFBaseModule *pExecutableModule);
	virtual ~MFTaskModuleExecuter();
	bool doWork(){
		return mp_executableModule->doWork();
	};
};

#endif /* MFMODULETASKS_MFTASKMODULEEXECUTER_H_ */
