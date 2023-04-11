/*
 * MFNetErrorTask.h
 *
 *  Created on: 21.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETERRORTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETERRORTASK_H_

#include <MFTasks/MFAbstractDataProcessingTask.h>
#include <mutex>
#include "MFNetworkBaseTask.h"

class MFNetErrorTask:
    public MFNetworkBaseTask,
    public MFNetInputPackage{
private:
	static void* smp_data;
	static std::mutex sLockData;
protected:
	void setupDataStructure();
public:
	MFNetErrorTask();
	virtual ~MFNetErrorTask();
	MFStructurableData* createNewEmptyData();
	bool processData(MFStructurableData* pData);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETERRORTASK_H_ */
