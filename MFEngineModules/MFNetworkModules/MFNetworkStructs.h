/*
 * MFNetworkStructs.h
 *
 *  Created on: 16.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKSTRUCTS_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKSTRUCTS_H_

#include <MFData/MFDataObject.h>
#include <MFData/MFDataObjectBuffer.h>
#include <vector>
#include "../MFBaseModule.h"

struct S_Notification {
  MFModuleGroup* pGroup;
  MFSyncObject* pObject;
  uint32_t spawnCycle;
};
struct S_ObjectsPackageBuffer{
	uint32_t m_objectCount=64;
	uint32_t m_usedObjectCount=0;
	uint32_t m_objectByteSize=20;
	MFDataObjectBuffer *pObjectBuffer;

	bool init(uint32_t objectByteSize){
		m_objectByteSize=objectByteSize;
		pObjectBuffer=new MFDataObjectBuffer(objectByteSize*m_objectCount);
		return pObjectBuffer->createBuffer();
	};
	void destroy(){
		delete pObjectBuffer;
	};
	MFDataObject* getNextObject(){
		if(m_usedObjectCount>=m_objectCount)return nullptr;
		MFDataObject *pO=pObjectBuffer->getNextDataObject(m_objectByteSize);
		if(pO==nullptr){
			MFObject::printErr("S_ObjectsPackageBuffer::getNextObject() - failed!");
		}
		return pO;
	}
	uint32_t getCurrentBufferIndex(){return m_usedObjectCount;};
};




#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKSTRUCTS_H_ */
