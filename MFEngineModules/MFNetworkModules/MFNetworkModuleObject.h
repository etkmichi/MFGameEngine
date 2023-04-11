/*
 * MFNetworkModuleObject.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKMODULEOBJECT_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKMODULEOBJECT_H_
#include <MFNetClientClasses/MFNetClient.h>
#include "../MFInterfacesModules/MFINetNotifier.h"
#include "../MFModuleObject.h"
#include "MFNetworkStructs.h"
#include "../ModulePhysics/MFPhysicModuleObject.h"
#include <glm/glm.hpp>

/**
 * A network module object represents a client.?
 */
class MFNetworkModuleObject:
		public MFModuleObject{
private:
	MFNetClient
		*mp_netClient;
//	struct S_Notification{
//		S_ModuleGroup* pGroup;
//		MFSyncObject* pObject;
//		uint32_t cycle;
//	};
//	uint16_t
//		m_addObjectNoteID=0;
//	MFDataObject
//		*mp_nextNotePkg,
//		*mp_currentOutPkg;
//
//	uint32_t
//		m_nextNoteAddIndex=0,/*index for next input item*/
//		m_remainingPkgSize,/*Used to check if following notes can be added to the pkg*/
//		m_sendDataSize=0,/*Used to check if sth. should be sent and how much data should be sent*/
//		m_maxPkgDataSize,/*count of all added notes. ptr to mp_nextOutputPackage*/
//		m_crtBufferOffset=0;/*offset for writing to output pkg*/
//
//	std::mutex
//		lockInput,
//		lockOutput,
//		lockNextOutputPkg,
//		lockCurrentOutputPkg;
//	std::vector<S_Notification*>
//		*mp_vecInNotesAddObject,/*used for enqueueing notes*/
//		*mp_vecOutNotesAddObject;/*used for creation of a pkg*/
//
//	bool
//		m_lastSendDone=true;
//	uint32_t
//		m_lastSendSize=0;

private:
	/*
	 * AddObject note format:
	 * note id | group id | object index | geometry setup size |
	 * geometry setup | spawnCycle*/
	/**
	 *
	 * @param pDstBuffer
	 * @param remainingSize - the copy process will substract the count
	 * of written bytes from remainingSize
	 * @param writtenBytes
	 * @return
	 */
//	bool processAddObjectNotes(
//			MFDataObject* pDstBuffer,
//			uint32_t &remainingSize);
//	uint32_t getAddObjectNoteSize(S_Notification* pNote);



protected://virtual functions of MFModuleObject:
	void updateSyncObject(MFSyncObject* syncObject){};
	void setupObjectDataStructure(){
		//TODO think about what should be changable on a netwrork module object.
	};

public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){return true;};
  virtual bool updateObject(){return true;};
  virtual void disable(){};
  virtual void enable(){};
  virtual bool synchronizeInput();
  virtual bool synchronizeOutput();
public:
	MFNetworkModuleObject();
	virtual ~MFNetworkModuleObject();

	void setClient(MFNetClient* pClient){mp_netClient=pClient;};

//
//	/**
//	 * Sends the add notes data to the client.
//	 */
//	void sendAddNotes();

	/**
	 * Switches the output package data and returns the filled data object (containing,
	 * the added activity data). Another call must be after the returned MFDataObject
	 * of the last call is not needed anymore.
	 * @param validBytes - count of bytes which contain activity data. Can be 0!
	 * @return
	 */
//	MFDataObject* getNextOutputPackage(uint32_t &notesCount);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKMODULEOBJECT_H_ */
