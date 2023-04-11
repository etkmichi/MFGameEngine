/*
 * MFBaseAction.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFBASEACTION_H_
#define MFSCENEACTIONS_MFBASEACTION_H_
#include <vector>
#include <MFData/MFInterfacesData/MFIStructurableDataCreator.h>
#include <MFTasks/MFAbstractDataProcessingTask.h>

/**
 *
 */
class MFBaseActionTask :
    public MFAbstractDataProcessingTask{
private:
  std::vector<uint32_t>
  m_vecUserTags;

  uint32_t
  m_key,
  m_userTicket,/*the ticket can be used for a task chain (task with resulting task/s)*/
  m_dataSize=8;/*used for createNewEmptyData*/

  MFStructurableData
  *mp_actionData=nullptr,
  *mp_actionConfirmData=nullptr;

  uint32_t
  m_actionDataSize=0,
  m_actionConfirmDataSize=0;


public: /*virtual functions of MFIStructurableDataCreator*/
  /**
   * This function creates data for the specific action.
   * @return
   */
  virtual MFStructurableData* createNewEmptyData(){
    MFStructurableData* pData=new MFStructurableData();
    pData->addNBitValue(m_dataSize*8, nullptr);
    return pData;
  };

public:/*virtual functions of MFAbstractDataProcessingTask*/
  /**
   * The prepareInputData is used for pre data processing tasks.
   * This function will be called during addInputData(...).
   * @param pInputData - input data which shall be prepared (f.e. decoded).
   */
  virtual void prepareInputData(MFStructurableData* pInputData){};

  /**
   * This function shall read pData for preparation of the execution of this action. It is
   * called during input processing (MFAbstractDataProcessingTask::addInputData)
   * @param pData
   * @return
   */
  virtual bool processData(MFStructurableData* pData){
    MFObject::printWarning("MFBaseActionTask::processData - no"
        "implementation!");
    return true;
  };

  virtual bool undoWork(){return true;};

public:/*virtual functions of MFBaseActionTask*/
  /**
   * Can be created if an action shall be confirmable. The confirm data contains
   * action specific data.
   * @return
   */
  virtual MFStructurableData* createConfirmData(){
    //TODO create basic confirm with task id, confirm data and ticket
    MFObject::printWarning("MFBaseActionTask::createConfirmData - no"
        "implementation!");
    return nullptr;
  }

  /**
   * Serializes the action.
   * @param pDstData
   * @param maxByteSize
   * @param writtenBytes - will be incremented by the count of written bytes.
   * @return
   */
  virtual bool serializeAction(void* pDstData,uint32_t maxByteSize,uint32_t &writtenBytes){
    //TODO create base serialization with task id, action data and ticket
    //(some tasks need only base serialization)
    MFObject::printWarning("MFBaseActionTask::serializeAction - no impl!");
    return false;
  }
//  virtual bool executeAction(MFSyncObject* pSyncObject){
//    printInfo("MFBaseActionTask::executeAction - no impl.!");
//    return false;
//  };

  /**
   * Deserializes the action given by pInputData (serialized input data).
   * @param pInputData
   * @param maxInputSize
   * @return
   */
  virtual bool deserializeAction(void* pInputData,uint32_t maxInputSize){
    MFObject::printWarning("MFBaseActionTask::serializeAction - no impl!");
    return false;
  }

public:
  MFBaseActionTask();
  virtual ~MFBaseActionTask();

//  /**
//   * If a user was not added, execution will not be done for this user. ->not impl.!
//   * @param userTag
//   */
//  void addUserTag(uint32_t userTag){m_vecUserTags.push_back(userTag);};
//  bool checkUser(uint32_t userTag);
  void setDataSize(uint32_t size){m_dataSize=size;};
  uint32_t getDataSize(){return m_dataSize;};
  void serializeActionHeader(void* dstData,uint32_t maxByteSize,uint32_t &writtenBytes);
  /**
   * The MFBaseActionTask will use this data for serialization. A subclass of MFBaseActionTask may do
   * other things.
   * @param data - data to serialize for action.
   * @param dataSize - data size of data.
   */
  void setActionData(MFStructurableData* data,uint32_t dataSize){
    mp_actionData=data;
    m_actionDataSize=dataSize;
  }
  void setActionConfirmData(MFStructurableData* data,uint32_t dataSize){
    mp_actionConfirmData=data;
    m_actionConfirmDataSize=dataSize;
  }
};

#endif /* MFSCENEACTIONS_MFBASEACTION_H_ */
