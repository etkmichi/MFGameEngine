/*
 * MFAddObject.h
 *
 *  Created on: 14.09.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFOBJECTACTIONS_MFACTIONADDOBJECT_H_
#define MFSCENEACTIONS_MFOBJECTACTIONS_MFACTIONADDOBJECT_H_
#include <MFGeometryManager.h>

#include "../MFActionResources.h"
#include "../MFBasicActions/MFBaseActionTask.h"
#include "../../MFSynchronisationClasses/MFSyncObject.h"
/**
 * This action task adds an object to a specific module group.
 * Datagramm:
 * Tags - uint32_t
 * Object count - uint32_t
 * #perObject:
 *  - GroupCount
 *    Group indices
 *  - GeometrySetupSize
 *    GeometrySetup
 *  - InitialSetupSize
 *    InitialSetup
 *  -> geometry parameter
 *  -> initial data
 * Group setups
 *  -> count of groups an object belongs too
 *  -> group indices for the current object
 */
class MFActionAddObject : public MFBaseActionTask {
private:
  MFActionResources
  *mp_actRes;
public:
  static const uint32_t
  TASK_FLAG_CONFIRM=1;

private:
  uint32_t
  m_taskFlags=0;

  std::mutex
  lockVecInput;

  std::vector<MFSyncObject*>
  *mp_vecInNotesAdd,
  *mp_vecOutNotesAdd;

  /**
   *
   * @param pSyncObject
   * @param dstBuffer - dst to write too. Will be incremented and points to next unused address!
   * @param maxByteSize
   * @param writtenBytes
   * @return
   */
  bool writeGroups(
      MFSyncObject* pSyncObject,
      uint32_t* &dstBuffer,
      const uint32_t &maxByteSize,
      uint32_t &writtenBytes);

  /**
   *
   * @param pSyncObject
   * @param dstBuffer
   * @param remainingBytes
   * @param writtenBytes
   * @return
   */
  bool writeGeometrySetup(
      MFSyncObject* pSyncObject,
      uint32_t* &dstBuffer,
      uint32_t &remainingBytes,
      uint32_t &writtenBytes);

  /**
   *
   * @param pSyncObject
   * @param dstBuffer
   * @param remainingBytes
   * @param writtenBytes
   * @return
   */
  bool writeInitialData(
      MFSyncObject* pSyncObject,
      uint32_t* &dstBuffer,
      uint32_t &remainingBytes,
      uint32_t &writtenBytes);

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
   * Serializes the action. Take care that current input is set (setInputData(vec<SyncO>))!
   * //TODO create base serialization with task id, action data and ticket
   * //TODO(some tasks may only need base serialization)
   * @param pDstData
   * @param maxByteSize
   * @param writtenBytes - will be incremented by the count of written bytes.
   * @return
   */
  virtual bool serializeAction(void* pDstData,uint32_t maxByteSize,uint32_t &writtenBytes);

  /**
   * Deserializes the action given by pInputData (serialized input data).
   * @param pInputData
   * @param maxInputSize
   * @return
   */
  virtual bool deserializeAction(void* pInputData,uint32_t maxInputSize);

public:/*virtual functions of MFAbstractDataProcessingTask*/
  /**
   * The prepareInputData is used for pre data processing tasks.
   * This function will be called during addInputData(...).
   * @param pInputData - input data which shall be prepared (f.e. decoded).
   */
  virtual void prepareInputData(MFStructurableData* pInputData){};

  virtual bool processData(MFStructurableData* pData){//TODO implement confirm cb
    MFObject::printWarning("MFBaseActionTask::processData - no"
        "implementation!");
    return true;
  };

  virtual bool undoWork(){return false;};
public:/*virtual functions of MFBaseActionTask*/
  virtual bool executeAction(MFSyncObject* pSyncObject){//TODO implement confirm cb
    printInfo("MFBaseActionTask::executeAction - no impl.!");
    return false;
  };
public:
  MFActionAddObject(MFActionResources* pActRes);
  virtual ~MFActionAddObject();
  void addAddedObject(MFSyncObject* pObject);
  void setAddedObjects(std::vector<MFSyncObject*> *pVecObjects){

  }
//  uint32_t calculateActionDataSize(std::vector<MFSyncObject*> *pVecObjects);
  /**
   * Fills add object action data to a specified dst buffer.
   * @param pVecObjects - vector of objects which shall be used for the add object action.
   * @param dstData - destination to which the add object data will be written.
   * @param maxByteSize - max bytes of dstData
   * @param writtenBytes - The count of bytes which were written after serialization.
   * @return the count of objects which were added to dstData
   */
  uint32_t serializeAddAction(
      std::vector<MFSyncObject*> *pVecObjects,
      void* dstData,
      uint32_t maxByteSize,
      uint32_t &writtenBytes);
};

#endif /* MFSCENEACTIONS_MFOBJECTACTIONS_MFACTIONADDOBJECT_H_ */
