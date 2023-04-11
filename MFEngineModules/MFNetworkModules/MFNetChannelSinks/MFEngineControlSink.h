/*
 * MFEngineControlSink.h
 *
 *  Created on: 20.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETCHANNELSINKS_MFENGINECONTROLSINK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETCHANNELSINKS_MFENGINECONTROLSINK_H_

#include <MF0InterfacesNetwork/MFINetChannelSink.h>
#include <MFTasks/MFAbstractDataProcessingTask.h>
#include <MFPrinters/MFPrintSetup.h>
#include <vector>

#include "../MFNetworkResources.h"
#include "../MFPackageClasses/MFNetInputPackage.h"
/**
 * This sink dispatches data which is related to the engine control.
 * Data will be dispatched by a task provided by MFNetTaskManager.
 */
class MFEngineControlSink:
    public MFINetChannelSink,
    public MFPrintSetup {
private:
	std::vector<MFNetInputPackage*>
		*mp_vecEnqueuedPackages;

	std::vector<uint32_t>
		*mp_vecFreeIndices;

	MFNetInputPackage
		*mp_dispatchInput;

	MFNetworkResources
		*mp_netRes;

	uint16_t
		m_currentPackageType,
		m_currentPackageCounter,
		m_nextPackageCounter=1,
		m_lowestEnqueuedPkgCounter=0xFFFE;

	bool
		m_packageEnqueued=false,
		m_checkPackageQueue=false;

	MFNetInputPackage* getPkg(
			MFStructurableDataBuffer *pDataBuffer,
			MFIStructurableDataCreator* pDataCreator);

	bool dispatchInputPackage(MFNetInputPackage* pBP,S_MF_NetworkEvent* pNE);
	bool processInputData(MFAbstractDataProcessingTask* pTask,MFNetInputPackage *pPackage);
public:

	/*MFINetChannelSink functions:*/
	virtual bool dispatchEvent(S_MF_NetworkEvent* pNE);

public:
	MFEngineControlSink(MFNetworkResources* pNetRes);
	virtual ~MFEngineControlSink();
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETCHANNELSINKS_MFENGINECONTROLSINK_H_ */
