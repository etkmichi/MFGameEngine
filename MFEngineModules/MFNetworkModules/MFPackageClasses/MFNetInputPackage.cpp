/*
 * MFNetInputPackage.cpp
 *
 *  Created on: 18.03.2020
 *      Author: michl
 */

#include "MFNetInputPackage.h"
#include <MFObjects/MFObject.h>
#include <memory.h>

MFNetInputPackage::MFNetInputPackage() {
	mp_packageCounter=nullptr;
	mp_packageTypeID=nullptr;
	m_headerDataCount=2;
}

MFNetInputPackage::~MFNetInputPackage() {
	// TODO Auto-generated destructor stub
}

bool MFNetInputPackage::dispatchPackageHead(ENetPacket* pPacket){
	if(pPacket->dataLength<m_headerSize){
		MFObject::printErr("MFNetInputPackage::dispatchPackageHead - "
				"failed, packet size < header size!");
		return false;
	}
	uint8_t* data=pPacket->data;
	m_packageDataLength=pPacket->dataLength;
	mp_packageCounter->writeData(data);
	data+=mp_packageCounter->getDataSize();
	mp_packageTypeID->writeData(data);
	return true;
}

bool MFNetInputPackage::dispatchPackageData(ENetPacket* pPacket){
	uint8_t* pDat=pPacket->data;
	pDat=pDat+m_headerSize;
	uint32_t remainingLength=pPacket->dataLength-m_headerSize;
	bool ret=true;
	//TODO check if its ok to use the packets data ptr
	// if its okay, data copying will not be necessary any more!
	// if its okay, packet data must be freed after engins intern processing!
	// ... change freeing packet of MFNetPkgDispatchTask:;:dispatchEvent
	if(getSharedBuffer()!=nullptr){
		if(getVecData()->size()<=m_headerDataCount){
			MFObject::printErr("MFNetInputPackage::dispatchPackageData - cant dispatch data, no "
					"data object available after header!");
			return false;
		}
		MFDataObject *destination=getVecData()->at(m_headerDataCount);
		if( (remainingLength) > (destination->getDataSize()) ){
			MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
				"cant dispatch data, data range "
				"surpasses buffer range!\nDebug - packet length | dst size:\n "+
				std::to_string(pPacket->dataLength)+" | "+
				std::to_string(destination->getDataSize()));
			return false;
		}

		memcpy(destination->getData(),pDat,remainingLength);
		return true;
	}

	for(uint32_t i=1;i<getVecData()->size();i++){
		MFDataObject *pDO=getVecData()->at(i);
		if(remainingLength<=pDO->getDataSize()){
			MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
					"package structure missmatch!");
			return false;
		}else{
			if(!pDO->writeData(pDat,remainingLength)){
				MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
						"copying data failed!");
				ret=false;
			}
			remainingLength-=pDO->getDataSize();
		}
	}
	return ret;

}

bool MFNetInputPackage::dispatchPackageData(void* data,uint32_t length){
	uint32_t remainingLength=length;

	if(getSharedBuffer()!=nullptr){
		uint8_t* pBufferStart=((uint8_t*)getSharedBuffer()->getBuffer())+m_headerSize;
		uint8_t* pBufferEnd=((uint8_t*)getSharedBuffer()->getBuffer())+
				getSharedBuffer()->getBufferByteSize();
		if( (pBufferStart+remainingLength) > pBufferEnd ){
			MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
					"cant dispatch data, data range"
					"surpasses buffer range!");
			return false;
		}
		memcpy(pBufferStart,data,remainingLength);
		return true;
	}

	bool ret=true;
	for(uint32_t i=1;i<getVecData()->size();i++){
		MFDataObject *pDO=getVecData()->at(i);
		if(remainingLength<=pDO->getDataSize()){
			MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
					"package structure missmatch!");
			return false;
		}else{
			if(!pDO->writeData(data)){
				MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
						"copying data failed!");
				ret=false;
			}
			remainingLength-=pDO->getDataSize();
		}
	}
	return ret;
}

bool MFNetInputPackage::dispatchPackageData(MFStructurableData *pData){
	uint32_t remainingLength=pData->getStructureSize();
	if(getSharedBuffer()!=nullptr){
		uint8_t* pBufferStart=((uint8_t*)getSharedBuffer()->getBuffer())+
				MFNetInputPackage::getHeaderSize();
		uint8_t* pBufferEnd=((uint8_t*)getSharedBuffer()->getBuffer())+
				getSharedBuffer()->getBufferByteSize();
		if( (pBufferStart+remainingLength) > pBufferEnd ){
			MFObject::printErr("MFNetInputPackage::dispatchPackageData - "
					"cant dispatch data, data range "
					"surpasses buffer range!");
			return false;
		}
		if(pData->getSharedBuffer()!=nullptr){
			memcpy(pBufferStart,pData->getStructureData(),remainingLength);
			return true;
		}
		for(MFDataObject* pDO:*pData->getVecData()){
			memcpy(pBufferStart,pDO->getData(),pDO->getDataSize());
			pBufferStart+=pDO->getDataSize();
			remainingLength-=pDO->getDataSize();
			//TODO create field for remaining length for public access
		}
		return true;
	}
	uint32_t count=getVecData()->size();
	if(pData->getVecData()->size() > count){
		MFObject::printWarning("MFNetInputPackage::dispatchPackageData(MFStructurableData *pData) - "
				"data count missmatch! pData->getVecData()->size>getVecData()->size()");
	}else{
		count=pData->getVecData()->size();
	}
	if(count==0){
		MFObject::printErr("MFNetInputPackage::dispatchPackageData(MFStructurableData *pData) - "
				"count==0 given pData or internal data vector size ==0!");
		return false;
	}

	bool ret=true;
	for(uint32_t i=1;i<count;i++){
		MFDataObject *pDest=getVecData()->at(i);
		MFDataObject *pSrc=pData->getVecData()->at(i);
		ret &= pDest->writeData(pSrc->getData(), pSrc->getDataSize());
		if(!ret)
			MFObject::printErr("MFNetInputPackage::dispatchPackageData(MFStructurableData *pData) - "
				"failed to copy single data object!");
	}
	return ret;
}

void MFNetInputPackage::setupPackageStructure(){
	mp_packageCounter=add16BitValue(E_DataType::DATA_TYPE_UINT16, nullptr);
	mp_packageTypeID=add16BitValue(E_DataType::DATA_TYPE_UINT16, nullptr);
	m_headerSize=4;
	setupDataStructure();
	m_dataLength=getStructureSize()-m_headerSize;
}
