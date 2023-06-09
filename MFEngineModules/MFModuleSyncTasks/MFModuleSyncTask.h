/*
 * MFModuleSyncTask.h
 *
 *  Created on: 08.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULESYNCTASKS_MFMODULESYNCTASK_H_
#define MFENGINEMODULES_MFMODULESYNCTASKS_MFMODULESYNCTASK_H_

#include <MFTasks/MFAbstractTask.h>
#include <MFData/MFDataObjectBuffer.h>
#include <MFData/MFStructurableDataBuffer.h>
#include <vector>
#include <mutex>

/**
 * This class provides functionality for streaming sync data.
 *
 * Every package must provide the m_sharedTaskIndex uint32_t value at the start of its
 * data.
 *
 */
class MFModuleSyncTask: public MFAbstractTask {
protected:
	std::mutex
		lockFreeUpdateBuffers,
		lockOutputQueue,
		lockVecProcessing,
		lockVecInput;

	MFStructurableDataBuffer
		*mp_updateDataBuffers,
		*mp_updateModuleBuffers;

	std::vector<MFStructurableData*>
		*mp_vecOutputQueue=nullptr,
		*mp_vecOutputBuffers=nullptr,
		*mp_vecInputQueue=nullptr,
		*mp_vecInputProcessing=nullptr;

	uint32_t
		m_sharedTaskIndex=0xFFFFFFFF,
		m_syncCycle=10,
		m_latestUpdatedStep=0,/*for checking that input is most recent*/
		m_lastUpdateCycleStep=0,
		m_dataCount=20,/*m_dataCount*16000byte buffers which can be filled*/
		m_maxBufferSize=16000;/*16kByte*/

	bool
		m_enableDataUpdate=true,
		m_enableModuleUpdate=true;
protected:
  /*virtual functions MFModuleSyncTask:*/

  /**
   * This function prepares the data which is needed for synchronizing. With
   * getNextDataSyncBuffer() the prepared data will be returned.
   */
  virtual void updateSyncData(){
    MFObject::printErr("MFModuleSyncTask::updateNextSyncPackage() - no impl.!");
    return;
  }
public:
	/*virtual functions MFModuleSyncTask:*/

//	/**
//	 * This function prepares the data which is needed for synchronizing. With
//	 * getNextDataSyncBuffer() the prepared data will be returned.
//	 */
//	virtual void updateSyncData(){
//		MFObject::printErr("MFModuleSyncTask::updateNextSyncPackage() - no impl.!");
//		return;
//	}
	virtual bool updateModule(MFDataObjectBuffer* inputData){
		MFObject::printErr("MFModuleSyncTask::updateModule(MFDataObjectBuffer*) - "
				"no impl.!");
		return false;
	}
	virtual bool updateModule(MFStructurableData* inputData){
		MFObject::printErr("MFModuleSyncTask::updateModule(MFStructurableData*) - "
				"no impl.!");
		return false;
	}
public:
	MFModuleSyncTask(
			uint32_t sharedModuleIndex,
			uint32_t syncCycleCount=50
			);
	virtual ~MFModuleSyncTask();

	bool initSyncTask();

	static uint32_t getSyncTaskIndex(uint8_t* pData){
		uint32_t taskIndex=*((uint32_t*)pData);
		return taskIndex;
	}

	virtual bool doWork(){
		if(m_enableDataUpdate)updateSyncData();

		/*update module data*/
		if(m_enableModuleUpdate){
			std::vector<MFStructurableData*> *pQ=mp_vecInputQueue;
			lockVecProcessing.lock();
			lockVecInput.lock();
			mp_vecInputQueue=mp_vecInputProcessing;
			mp_vecInputProcessing=pQ;
			lockVecInput.unlock();
			for(MFStructurableData *pDat:*mp_vecInputProcessing){
				if(pDat==nullptr || !updateModule(pDat)){
					MFObject::printErr("MFModuleSyncTask::doWork() - "
							"failed to update module!");
				}
				if(pDat!=nullptr)
					retrunModuleUpdateBuffer(pDat);
			}
			mp_vecInputProcessing->clear();
			lockVecProcessing.unlock();
		}
		return true;
	}

	/**
	 * Add data taken from this MFModuleSyncTask for module update.
	 * Data will be used and returned during doWork() function call. Data must not be
	 * returned by returnModuleUpdateBuffer()!!!
	 * @param pDat - taken by this task with takeModuleUpdateBuffer() and filled
	 * with data generated by this class.
	 */
	void addModuleUpdateData(MFStructurableData* pDat);

	void enableModuleUpdate(bool enable){
		m_enableModuleUpdate=enable;
	}
	void enableDataUpdate(bool enable){
		m_enableDataUpdate=enable;
	}

	/**
	 * Take buffers with this function for outgoing data which will be filled by a
	 * MFModuleSyncTask.
	 * @return
	 */
	std::vector<MFStructurableData*>* takeDataUpdateBuffers(){
		return mp_vecOutputBuffers;
	}

	/**
	 * Take buffers with this function for incomming data which were filled by
	 * a MFModuleSyncTask.
	 * @return
	 */
	MFStructurableData* takeModuleUpdateBuffer(){
		return mp_updateModuleBuffers->takeData();};

	/**
	 * This function returns a MFDataObjectBuffer which was taken by a
	 * takeModuleUpdateBuffer() function call.
	 * @param pBuffer - taken by a takeNextDataSyncBuffer() function call.
	 */
	void retrunModuleUpdateBuffer(MFStructurableData* pBuffer){
		if(mp_updateModuleBuffers!=nullptr){
			mp_updateModuleBuffers->returnData(pBuffer);
		}
		return;
	}
	/**
	 * This function returns a MFDataObjectBuffer which was taken by a
	 * takeNextDataSyncBuffer() function call.
	 * @param pBuffer - taken by a takeNextDataSyncBuffer() function call.
	 */
	void retrunDataSyncBuffer(MFStructurableData* pBuffer){
		if(mp_updateDataBuffers!=nullptr){
			mp_updateDataBuffers->returnData(pBuffer);
		}
		return;
	}
	/**
	 * This function returns a MFDataObjectBuffer which was taken by a
	 * takeNextDataSyncBuffer() function call.
	 * @param pBuffer - taken by a takeNextDataSyncBuffer() function call.
	 */
	void retrunDataSyncBuffer(std::vector<MFStructurableData*> *pVecDat){
		if(mp_updateDataBuffers!=nullptr){
			for(MFStructurableData* pD:*pVecDat)
				mp_updateDataBuffers->returnData(pD);
		}
		return;
	}
};

#endif /* MFENGINEMODULES_MFMODULESYNCTASKS_MFMODULESYNCTASK_H_ */
