/*
 * MFNetOutputPackage.h
 *
 *  Created on: 27.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFPACKAGECLASSES_MFNETOUTPUTPACKAGE_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFPACKAGECLASSES_MFNETOUTPUTPACKAGE_H_
#include <MFData/MFDataObjectBuffer.h>
#include <MFData/MFStructurableData.h>
#include <MFNetClientClasses/MFNetClient.h>
/**
 * Output package for network.
 * This class provides structure and functionality for preparing a package which shall be sent
 * over the network. This class wraps around stucturable data which will be set up with a shared buffer.
 * The shared buffer provides the needed memory in one piece.
 * @devel Functions are written with redundant Output in their name, because function names may overlap
 * with other subclass function names.
 */
class MFNetOutputPackage {
protected:
  MFDataObjectBuffer
  *mp_sharedOutputBuffer,
  *mp_sharedPrepareBuffer;
  //TODO check if two buffers mp_outputPackage/mp_preparePackage for sending and preparing are ok
  // ->lock wont take long if buffers are switch during send
  MFStructurableData
  *mp_preparePackage,
  *mp_outputPackage;
  uint64_t
  m_maxSize=5000,
  m_validPrepareByteCount=0,
  m_validOutputByteCount=0;

  /*sending setup*/
  bool
  m_outputFlush=true;
  uint8_t
  m_outputChannel=0;
  uint16_t
  *mp_packageCounter=nullptr;

  std::mutex
  lockOutputBuffer,
  lockPrepareBuffer;

  MFDataObject
    *mp_outputPackageTypeID=nullptr,/*identifies a package type, needed for processing the data*/
    *mp_outputPackageCounter=nullptr,/*each package which is sent will increment the counter*/
    *mp_preparePackageTypeID=nullptr,/*identifies a package type, needed for processing the data*/
    *mp_preparePackageCounter=nullptr;

protected:/*virtual functions MFNetOutputPackage*/
  virtual bool initOutputPackages(
      MFStructurableData* p_preparePackage,
      MFStructurableData* p_outputPackage){
    MFObject::printWarning("MFNetOutputPackage::initOutputPackages - no impl.!");//TODO maybe store pointers of packages to variables
    return true;
  }

  /**
   * Prepares the output package (fills it with data).
   * @param pDst - destination to write the data
   * @param writtenBytes - count of written bytes. this value will be incremented, not overwritten!
   * @param maxByteSize - maximal bytes pDst can take
   * @return
   */
	virtual bool prepOutputPackage(void* pDst,uint64_t &writtenBytes,const uint64_t &maxByteSize){
		MFObject::printWarning("MFNetOutputPackage::prepOutputPackage - "
				"no impl.!");
		return true;
	}

	/**
	 * If not set by setMaxOutputSize, this function will calculate the maximal size during initialization.
	 * Output buffer will be allocated with the returned max size.
	 * @return maximal count of bytes for the output buffer.
	 */
	virtual uint64_t calculateMaxDataSize(){
	  return m_maxSize;
	}

public:
	MFNetOutputPackage();
	virtual ~MFNetOutputPackage();
	bool initOutputPackage();

	/**
	 * This function will sent the prepared output package. To prepare the next output package call prepare
	 * @param pClient
	 * @return
	 */
	bool sendPackage(MFNetClient* pClient);

  /**
   * This function will switch the prepare package and output package. It will the subclasses
   * prepareOutputPackage function too! Normally used before sending the output package. Every call
   * will switch the output and prepare package, even if data wasn't sent yet!
   * @return
   */
  bool prepareOutputPackage();

  void setPackageCounter(uint16_t* pPkgCounter){mp_packageCounter=pPkgCounter;};

  void setOutputChannel(uint8_t channel){m_outputChannel=channel;};

  void enableOutputFlush(bool enable){m_outputFlush=enable;};

  bool writePackageTypeID(uint16_t* pTypeID);

	/**
	 * Sets the maximal size of the output package. This will limit the allocation of
	 * data for this package. Take care that the max pkg size is large enough to
	 * hold the data it is needed for. During send process, only the valid data of this
	 * package will be sent!
	 * @param maxSize
	 */
	void setMaxOutputSize(uint64_t maxSize){
	  m_maxSize=maxSize;
	}
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFPACKAGECLASSES_MFNETOUTPUTPACKAGE_H_ */
