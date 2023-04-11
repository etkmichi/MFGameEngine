/*
 * MFNetOutputPackage.cpp
 *
 *  Created on: 27.07.2020
 *      Author: michl
 */

#include "MFNetOutputPackage.h"

MFNetOutputPackage::MFNetOutputPackage() {
  mp_sharedOutputBuffer=new MFDataObjectBuffer(m_maxSize);
  mp_sharedPrepareBuffer=new MFDataObjectBuffer(m_maxSize);
  mp_preparePackage=new MFStructurableData();
  mp_outputPackage=new MFStructurableData();
  mp_outputPackage->useSharedBuffer(mp_sharedOutputBuffer);
  mp_preparePackage->useSharedBuffer(mp_sharedPrepareBuffer);
}

MFNetOutputPackage::~MFNetOutputPackage() {
	// TODO Auto-generated destructor stub
}

bool MFNetOutputPackage::initOutputPackage(){
  bool ret=true;

  m_maxSize=calculateMaxDataSize();
  mp_sharedOutputBuffer->setBufferByteSize(m_maxSize);
  ret&=mp_sharedOutputBuffer->createBuffer();
  mp_sharedPrepareBuffer->setBufferByteSize(m_maxSize);
  ret&=mp_sharedPrepareBuffer->createBuffer();

  mp_outputPackage->setStructureName("NetOutputPkg1");
  mp_outputPackage->useSharedBuffer(mp_sharedOutputBuffer);
  mp_outputPackageCounter=mp_outputPackage->add16BitValue(E_DataType::DATA_TYPE_UINT16, nullptr);
  mp_outputPackageTypeID=mp_outputPackage->add16BitValue(E_DataType::DATA_TYPE_UINT16, nullptr);

  mp_preparePackage->setStructureName("NetOutputPkg2");
  mp_preparePackage->useSharedBuffer(mp_sharedPrepareBuffer);
  mp_preparePackageCounter=mp_preparePackage->add16BitValue(E_DataType::DATA_TYPE_UINT16, nullptr);
  mp_preparePackageTypeID=mp_preparePackage->add16BitValue(E_DataType::DATA_TYPE_UINT16, nullptr);

  if(ret){
    ret&=initOutputPackages(mp_preparePackage,mp_outputPackage);
  }else{
    MFObject::printErr("MFNetOutputPackage::initOutputPackage - failed to create output or prepare buffer!");
  }

  return ret;
}

bool MFNetOutputPackage::sendPackage(MFNetClient* pClient){
  lockOutputBuffer.lock();
  bool ret=true;
  if(m_validOutputByteCount>0){
    ret=pClient->sendData(
        (uint8_t*)mp_outputPackage->getStructureData(),
        m_validOutputByteCount,
        m_outputChannel,
        m_outputFlush,
        mp_packageCounter);
  }
  lockOutputBuffer.unlock();
  return ret;
}

bool MFNetOutputPackage::writePackageTypeID(uint16_t* pTypeID){
  bool ret=mp_outputPackageTypeID->writeData(pTypeID);
  ret&=mp_preparePackageTypeID->writeData(pTypeID);
  return ret;
}

bool MFNetOutputPackage::prepareOutputPackage(){
    lockPrepareBuffer.lock();
    lockOutputBuffer.lock();
    MFStructurableData *lastOutput=mp_outputPackage;
    mp_outputPackage=mp_preparePackage;
    mp_preparePackage=lastOutput;
    m_validOutputByteCount=m_validPrepareByteCount;
    m_validPrepareByteCount=0;

    MFDataObject* pSwitchable=mp_outputPackageTypeID;
    mp_outputPackageTypeID=mp_preparePackageTypeID;
    mp_preparePackageTypeID=mp_outputPackageTypeID;

    pSwitchable=mp_outputPackageCounter;
    mp_outputPackageCounter=mp_preparePackageCounter;
    mp_preparePackageCounter=pSwitchable;

    bool ret=prepOutputPackage(mp_outputPackage->getStructureData(), m_validOutputByteCount, m_maxSize);
    lockPrepareBuffer.unlock();
    lockOutputBuffer.unlock();
    return ret;
  }
