/*
 * MFIInputNotifier.h
 *
 *  Created on: 14.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFINPUTSYNCNOTIFIER_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFINPUTSYNCNOTIFIER_H_
#include <mutex>
#include <vector>
/**
 * This class provides functionality for implementing notifications. Notifications are added with
 * addSyncNotifications(objectIndex). With getNextInputNotifications() a vector is returned which contains
 * the object indices which were added by addSyncNotification(...) since the last call to
 * getNextInputNotifications(). The retruned vector with input notes (getNextInputNotifications) must not
 * be used again after another call to getNextInputNotifications! The last valid entry of the returned
 * vector uses the value 0xFFFFFFFF, after this value all following entries are invalid!
 */
class MFInputSyncNotifier {
public:
	const uint32_t
		QUEUE_END_ENTRY=0xFFFFFFFF;
	static uint32_t
		ms_queueSize;
private:
	uint32_t
		m_notifierID=0;
	bool
		m_enableNotification=true;
	uint32_t
		m_queueSize,
		m_usedNoteCount;
	std::mutex
		lockInputNotifications;
	std::vector<uint32_t>
		*mp_vecInputNotifications,
		*mp_vecOutputNotifications;
	std::string
	m_notifierName="";
protected:
	/**
	 * This function will return the input queue. The new input queue will be switched to a cleared
	 * vector. A second call will clear the vector which was returned by the last call to this
	 * function.
	 * @return
	 */
	const std::vector<uint32_t>* getNextInputNotifications();
public:
	MFInputSyncNotifier();
	virtual ~MFInputSyncNotifier();
	void addSyncNotification(uint32_t objectIndex);
	void notifySync(uint32_t index);
	void enableNotification(bool enable);
	void setNotifierName(std::string name){m_notifierName=name;};
	void clearNotifications();
};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFINPUTSYNCNOTIFIER_H_ */
