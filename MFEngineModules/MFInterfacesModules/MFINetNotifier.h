/*
 * MFIAddObjectNotifier.h
 *
 *  Created on: 24.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFINETNOTIFIER_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFINETNOTIFIER_H_
#include <MFObject.h>
#include <MFNetClientClasses/MFNetClient.h>
#include "../MFModuleGroup.h"
#include "../../MFEngineStructs.h"
#include "../../MFSynchronisationClasses/MFSyncObject.h"
class MFINetNotifier {
public:
	/*MFINetNotifier virtual functions*/
	/**
	 * Adds a notifictaion. Implementation may not process the notification, because
	 * the processing may take too much time! Processing is done by the
	 * processNotifications function! Thread safety is implementation specific!
	 * @param pGroup
	 * @param pObject
	 * @param removeCycle
	 */
	virtual void notifyAddObject(
	    MFModuleGroup* pGroup,
			MFSyncObject* pObject,
			uint32_t spawnCycle){
		MFObject::printErr("MFIAddObjectNotifier::notifyAddObject - no impl!");
	};

  /**
   * Adds a notifictaion. Implementation may not process the notification, because
   * the processing may take too much time! Processing is done by the
   * processNotifications function! Thread safety is implementation specific!
   * @param pObject - object which shall be added. The object provides all necessary information.
   */
  virtual void notifyAddObject(MFSyncObject* pObject){
    MFObject::printErr("MFIAddObjectNotifier::notifyAddObject - no impl!");
  };

	virtual void notifyChangeObject(MFSyncObject* pObject){
		MFObject::printErr("MFIAddObjectNotifier::notifyChangeObject - no impl!");
	};
	/**
	 * Adds a notifictaion. Implementation may not process the notification, because
	 * the processing may take too much time! Processing is done by the
	 * processNotifications function! Thread safety is implementation specific!
	 * @param pGroup
	 * @param pObject
	 * @param removeCycle
	 */
	virtual void notifyRemoveObject(
	    MFModuleGroup* pGroup,
			MFSyncObject* pObject,
			uint32_t removeCycle){
		MFObject::printErr("MFIAddObjectNotifier::notifyRemoveObject - no impl!");
	};

	/**
	 * This function notifies that a connection to a client/server was made.
	 */
	virtual void notifyNewClient(
			MFNetClient* pClient){
		MFObject::printErr("MFIAddObjectNotifier::notifyNewClient - no impl!");
	}
	/**
	 * Processes added notification.
	 * @return
	 */
	virtual bool processNotes(){
		MFObject::printErr("MFIAddObjectNotifier::procesNotifications - no impl!");
		return false;
	};

public:
	MFINetNotifier(){};

	virtual ~MFINetNotifier(){};

};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFINETNOTIFIER_H_ */
