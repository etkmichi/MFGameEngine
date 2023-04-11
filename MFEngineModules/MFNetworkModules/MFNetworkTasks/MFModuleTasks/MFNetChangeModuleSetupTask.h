/*
 * MFNetModuleSetupTask.h
 *
 *  Created on: 25.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETCHANGEMODULESETUPTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETCHANGEMODULESETUPTASK_H_

#include <MFTasks/MFAbstractDataProcessingTask.h>

#include "../MFNetworkBaseTask.h"

/**
 * TODO use this class to change a module's setup
 */
class MFNetChangeModuleSetupTask :
		public MFNetworkBaseTask,
		public MFNetInputPackage{
//protected:
//	/*MFBasePackage*/
//	virtual void setupDataStructure(){
//		addNBitValue(m_dataSize*8,nullptr);
//	}
public:
	MFNetChangeModuleSetupTask();
	virtual ~MFNetChangeModuleSetupTask();
	MFStructurableData* createNewEmptyData();
	bool processData(MFStructurableData* pData);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETCHANGEMODULESETUPTASK_H_ */
