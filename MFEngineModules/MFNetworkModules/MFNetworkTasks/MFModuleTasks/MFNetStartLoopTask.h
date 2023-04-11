/*
 * MFNetStartLoopTask.h
 *
 *  Created on: 30.06.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETSTARTLOOPTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETSTARTLOOPTASK_H_

#include "../MFNetworkBaseTask.h"
#include "../../../MFInterfacesModules/MFIEngineLoopControl.h"
#include "../../MFNetworkResources.h"

class MFNetStartLoopTask :
		public MFNetworkBaseTask ,
		public MFNetInputPackage{
private:
	MFIEngineLoopControl
		*mp_loopCtl;
	MFNetworkResources
		*mp_netRes;

public:
	MFNetStartLoopTask(MFNetworkResources* pNetRes);
	virtual ~MFNetStartLoopTask();
	MFStructurableData* createNewEmptyData();
	bool processData(MFStructurableData* pData);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETSTARTLOOPTASK_H_ */
