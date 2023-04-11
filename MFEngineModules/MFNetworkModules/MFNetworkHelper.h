/*
 * MFNetworkHelper.h
 *
 *  Created on: 24.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKHELPER_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKHELPER_H_

#include <MFNetClientClasses/MFNetClient.h>

#include "MFNetworkResources.h"

class MFNetworkHelper {
private:
	MFNetworkResources
		*mp_netRes;
public:
	MFNetworkHelper(MFNetworkResources* pNetRes);
	virtual ~MFNetworkHelper();
	bool sendAddObject(MFNetClient* pClient);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKHELPER_H_ */
