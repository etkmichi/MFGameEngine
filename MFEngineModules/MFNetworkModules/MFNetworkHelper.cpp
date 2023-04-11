/*
 * MFNetworkHelper.cpp
 *
 *  Created on: 24.07.2020
 *      Author: michl
 */

#include "MFNetworkHelper.h"

MFNetworkHelper::MFNetworkHelper(MFNetworkResources* pNetRes) {
	mp_netRes=pNetRes;
}

MFNetworkHelper::~MFNetworkHelper() {
	// TODO Auto-generated destructor stub
}

bool MFNetworkHelper::sendAddObject(MFNetClient* pClient){
	/*ToSend: GroupID, GeoID, ModuleObjectData*/
	return true;
}
