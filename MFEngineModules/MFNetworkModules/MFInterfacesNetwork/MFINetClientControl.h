/*
 * MFINetClientControl.h
 *
 *  Created on: 31.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFINTERFACESNETWORK_MFINETCLIENTCONTROL_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFINTERFACESNETWORK_MFINETCLIENTCONTROL_H_
#include <MFNetClientClasses/MFNetClient.h>
class MFINetClientControl {
public:
	MFINetClientControl();
	virtual ~MFINetClientControl();
	virtual void addClient(MFNetClient *pClient){
		MFObject::printErr("MFINetClientControl::addClient no impl.!");
	}
	virtual void addServerClient(MFNetClient* pClient){
		MFObject::printErr("MFINetClientControl::addServerClient no impl.!");
	}
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFINTERFACESNETWORK_MFINETCLIENTCONTROL_H_ */
