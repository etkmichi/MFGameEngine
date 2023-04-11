/*
 * MFSyncStreamSink.h
 *
 *  Created on: 10.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETCHANNELSINKS_MFSYNCSTREAMSINK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETCHANNELSINKS_MFSYNCSTREAMSINK_H_
#include <MF0InterfacesNetwork/MFINetChannelSink.h>
#include <MFPrinters/MFPrintSetup.h>
#include "../../MFModuleSyncTasks/MFModuleSyncTask.h"
#include "../MFNetworkModule.h"
class MFSyncStreamSink:
    public MFINetChannelSink ,
    public MFPrintSetup{
private:
	MFNetworkResources
		*mp_netRes;

/*virtual functions of MFINetChannelSink*/
public:
	/**
	 * The implementation of this function must dispatch the received data.
	 * If function is called by MFNetEventDispatchTask class, pNE must not be used any
	 * further after dispatchEvent(...) returned!
	 * @param pNE
	 * @return
	 */
	virtual bool dispatchEvent(S_MF_NetworkEvent* pNE);
public:
	MFSyncStreamSink(MFNetworkResources *pNetRes);
	virtual ~MFSyncStreamSink();
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETCHANNELSINKS_MFSYNCSTREAMSINK_H_ */
