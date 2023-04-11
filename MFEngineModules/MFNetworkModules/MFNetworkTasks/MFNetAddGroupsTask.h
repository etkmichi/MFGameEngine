/*
 * MFNetAddGroupTask.h
 *
 *  Created on: 28.05.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETADDGROUPSTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETADDGROUPSTASK_H_
#include "../../MFInterfacesModules/MFIModuleGroupControl.h"
#include "MFNetworkBaseTask.h"
/**
 * This task sets up groups on a client.
 * Implementation of MFBasePackage:
 * This package contains data for adding groups to the module manager.
 * Package structure:
 * |group count | name size 1. group | name of 1. group |... till group count group
 */
class MFNetAddGroupsTask : public MFNetworkBaseTask,public MFNetInputPackage{

private:
	MFIModuleGroupControl
		*mp_groupProvider;
public:
	MFNetAddGroupsTask(MFIModuleGroupControl* pGroupSetup);
	virtual ~MFNetAddGroupsTask(){};
	MFStructurableData* createNewEmptyData();
	bool processData(MFStructurableData* pData);
	
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETADDGROUPSTASK_H_ */
