/*
 * MFBasePackage.h
 *
 *  Created on: 18.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFPACKAGECLASSES_MFBASEPACKAGE_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFPACKAGECLASSES_MFBASEPACKAGE_H_

#include <MFPackageClasses/MFNetAdaptablePackage.h>
#include <enet/enet.h>
#include <MFData/MFStructurableData.h>
/**
 * This class provides a structure for an incomming network package. The received
 * package must be in a format which supports the implementation of this class.
 * The structure of an input package may be created with no shared buffer.
 */
class MFNetInputPackage :
    public MFStructurableData {
protected:
	uint32_t
		m_pkgDataSize=1000;
	void setupPackageStructure();
	/**
	 * Implementation of this function shall use the MFStructurableData's function
	 * to setup a structure for the package data. The dispatch functions will use the
	 * given setup for copying the data from the package. If no implementation exists,
	 * this function will use addNBitValue(dataSize*8,nullptr) with the size set by
	 * setDataSize(size).
	 */
	virtual void setupDataStructure(){
		if(!m_surpressSetupWarning){
			MFObject::printWarning("MFNetInputPackage::setupDataStructure() -"
					" no implementation!");
		}
		addNBitValue(m_pkgDataSize*8, nullptr);
		return;
	};
	/**
	 * If implemented, this function will reinit the data structure. This may be necessary if
	 * some setup changes.
	 * @return
	 */
	virtual bool reinitData(){return false;}
public:
	bool
		m_surpressSetupWarning=false,
		m_isSkippable=false;

	MFDataObject
		*mp_packageTypeID,/*identifies a package type, needed for processing the data*/
		*mp_packageCounter;/*each package which is sent will increment the counter*/

	uint32_t
		m_packageDataLength=0,
		m_dataLength=0,
		m_headerSize=4,
		m_headerDataCount=0;

	MFNetInputPackage();

	virtual ~MFNetInputPackage();

	static uint32_t getHeaderSize(){
		return 4;//2*16bit for package counter and type id
	}

	/**
	 * This will setup the data structure of the package.
	 * This function must be called before the package will be filled.
	 */
	void initData(){setupPackageStructure();};

	/**
	 * Can be used to define the size of the data part of a package. Implementation
	 * of this class must use m_dataSize during structureDat... //TODO its a bit shitty
	 * @param size
	 */
	void setPackageDataSize(uint32_t size){m_pkgDataSize=size;};

	/**
	 * This function copies header data of MFBasePackage to this object.
	 * @return
	 */
	bool dispatchPackageHead(ENetPacket* pPacket);

	/**
	 * This function will dispatch the remaining data of pPacket beginning from
	 * offset header size.
	 * @param pPacket
	 * @return
	 */
	bool dispatchPackageData(ENetPacket* pPacket);

	/**
	 * This function will dispatch the given data interpreted as data part of a package.
	 * @param data
	 * @param length
	 * @return
	 */
	bool dispatchPackageData(void* in_data,uint32_t length);

	/**
	 * This function writes data to the data part of a package.
	 * @param pData
	 * @return
	 */
	bool dispatchPackageData(MFStructurableData *pData);

	/**
	 * Returns if the package is skippable.
	 * @return
	 */
	bool isSkippable(){return m_isSkippable;};

	/**
	 * Sets the skippable field to skippable. If a package is skippable, the processing
	 * can be skipped (f.e. if some previouse packages are missing for processing, this
	 * package can be skipped).
	 * @param skippable
	 */
	void setSkippable(bool skippable){m_isSkippable=skippable;};
	static uint32_t getByteOffsetTypeID(){return 2;};
	static uint32_t getByteOffsetPkgCounter(){return 0;};
	MFDataObject* getTypeIDObject(){return mp_packageTypeID;};
	MFDataObject* getPackageCounter(){return mp_packageCounter;};
	uint16_t getTypeIDData(){return *((uint16_t*)mp_packageTypeID->getData());};
	uint16_t getCounter(){return *((uint16_t*)mp_packageCounter->getData());};
	uint16_t* getPTypeID(){return ((uint16_t*)mp_packageTypeID->getData());};
	uint16_t* getPCounter(){return ((uint16_t*)mp_packageCounter->getData());};
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFPACKAGECLASSES_MFBASEPACKAGE_H_ */
