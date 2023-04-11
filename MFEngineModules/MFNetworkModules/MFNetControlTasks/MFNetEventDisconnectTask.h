/*
 * MFNetEventDisconnectTask.h
 *
 *  Created on: 09.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETCONTROLTASKS_MFNETEVENTDISCONNECTTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETCONTROLTASKS_MFNETEVENTDISCONNECTTASK_H_

#include <MFNetworkTasks/MFNetEventDispatchTask.h>

class MFNetEventDisconnectTask: public MFNetEventDispatchTask {
public:
	MFNetEventDisconnectTask();
	virtual ~MFNetEventDisconnectTask();
	virtual bool dispatchEvent(S_MF_NetworkEvent* pNE);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETCONTROLTASKS_MFNETEVENTDISCONNECTTASK_H_ */
