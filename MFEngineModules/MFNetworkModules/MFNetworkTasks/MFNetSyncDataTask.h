/*
 * MFNetSyncDataTask.h
 *
 *  Created on: 19.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFPROCESSINGTASKS_MFNETSYNCDATATASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFPROCESSINGTASKS_MFNETSYNCDATATASK_H_

#include <MFTasks/MFAbstractDataProcessingTask.h>
#include "../../../MFSynchronisationClasses/MFSyncObject.h"
#include "MFNetworkBaseTask.h"
#include <glm/glm.hpp>

class MFNetSyncDataTask :
		public MFNetworkBaseTask,
		public MFNetInputPackage{
protected:
	virtual void setupDataStructure(){
		addNBitValue(getStructureSize()*8,nullptr);
	}
private:
	MFSyncObject
		*mp_currentSO=nullptr;
public:
	MFNetSyncDataTask();
	virtual ~MFNetSyncDataTask();

	MFStructurableData* createNewEmptyData();
	void setSyncObject(MFSyncObject* pSyncObject){mp_currentSO=pSyncObject;};
	bool processData(MFStructurableData* pData);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFPROCESSINGTASKS_MFNETSYNCDATATASK_H_ */
