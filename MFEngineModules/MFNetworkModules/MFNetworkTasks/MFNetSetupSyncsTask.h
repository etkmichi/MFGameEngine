/*
 * MFNetSetupSyncsTask.h
 *
 *  Created on: 21.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETSETUPSYNCSTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETSETUPSYNCSTASK_H_

#include "../MFNetworkResources.h"
#include "MFNetworkBaseTask.h"

/**
 * This task sets up a clients data sync sinks (
 */
class MFNetSetupSyncsTask:
		public MFNetworkBaseTask,
		public MFNetInputPackage {
private:
	MFNetworkResources
		*mp_netRes;
protected:
	/*MFBasePackage*/
	virtual void setupDataStructure(){
		addNBitValue(getStructureSize()*8,nullptr);
	}
public:
	/*MFNetworkBaseTask*/
	virtual MFStructurableData* createNewEmptyData();
	virtual bool processData(MFStructurableData* pData);

public:
	MFNetSetupSyncsTask(MFNetworkResources *pResources);
	virtual ~MFNetSetupSyncsTask();
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETSETUPSYNCSTASK_H_ */
