/*
 * MFNetActionTask.h
 *
 *  Created on: 19.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETACTIONTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETACTIONTASK_H_

#include <mutex>

#include <MFTasks/MFAbstractDataProcessingTask.h>

#include <MFNetClientClasses/MFNetClient.h>

#include "../MFNetworkResources.h"
#include "../../MFInterfacesModules/MFINetNotifier.h"
#include "../../../MFSceneActions/MFActionTaskManager.h"
#include "../MFPackageClasses/MFNetInputPackage.h"
#include "../MFPackageClasses/MFNetOutputPackage.h"
#include "MFNetworkBaseTask.h"
/**
 * This class provides functionality to send a serialized action. The action must be derived from
 * MFBaseActionTask and it must be registered to the action manager (MFActionTaskManager).
 * MFNetworkBaseTask:
 * This task dispatches received data into an action id and action data. The action id
 * will be used to retrieve the action task from the MFActionManager. The data will be
 * copied to the MFActionManager and used for further processing.
 *
 * Datagram:
 * sizeof(uint16_t) pkg counter - MFNetInputPackage header // pkg counter for checking if pkgs were lost
 * sizeof(uint16_t) network task index
 * sizeof(uint16_t) - count of actions within the pkg
 * preiodic structure over action count:
 * sizeof(uint16_t) - action index
 * sizeof(uint32_t) - size of action | serialized action
 *
 * @Attention The pre allocated output buffer must be large enough to hold all occuring actions!
 */
class MFNetActionTask :
    public MFNetworkBaseTask,
    public MFNetInputPackage,
    public MFNetOutputPackage
{
  /*TODO implement MFNetOutputPackage*/
  //	virtual bool sendData(MFNetClient* pClient);
  //	virtual bool sendData(std::vector<MFNetClient*> *pVecClients);
private:
  MFNetworkResources
  *mp_netRes;

  uint16_t
  *mp_prepareActionCount=nullptr,/*count of actions within the prepare package*/
  *mp_outputActionCount=nullptr;/*count of actions within the output package*/

  uint32_t
  m_actionTag=0,
  m_maxActionCount=50,
  m_estimatedActionSize=100;//TODO setter

  const uint32_t
  TAG_CONFIRM_ACTION=1;

protected:/*virtual functions MFNetOutputPackage*/
  virtual bool initOutputPackages(
      MFStructurableData* p_preparePackage,
      MFStructurableData* p_outputPackage);
  /**
   * Prepares the output package (fills it with data).
   * @param pDst - destination to write the data
   * @param writtenBytes - count of written bytes. this value will be incremented, not overwritten!
   * @param maxByteSize - maximal bytes pDst can take
   * @return
   */
  virtual bool prepOutputPackage(void* pDst,uint64_t &writtenBytes,const uint64_t &maxByteSize);

  /**
   * If not set by setMaxOutputSize, this function will calculate the maximal size during initialization.
   * Output buffer will be allocated with the returned max size.
   * @return maximal count of bytes for the output buffer.
   */
  virtual uint64_t calculateMaxDataSize();

public: /*virtual functions of MFNetworkBaseTask*/
  virtual MFStructurableData* createNewEmptyData();

//  virtual bool createTaskDataBuffer();

//  /**
//   * This function creates a filled package. The data must be in one piece, that
//   * it can be sent over network. Take care to delete the created data when its
//   * not used any more (f.e. after sending process)
//   * @return
//   */
//  virtual MFNetInputPackage* createPackage();//TODO delete this function in parent

//  /**
//   * This function fills a package with the task related data.
//   * The data must be in one piece, that it can be sent over network.
//   * @param pPkg - data of pkg must be at one piece and large enough to hold the
//   * data.
//   * @return
//   */
//  virtual bool fillPackage(MFNetInputPackage* pPkg);

//  virtual uint32_t calculatePackageSize();

public: /*virtual functions of MFAbstractDataProcessingTask*/
  /**
   * This function will dispatch received data, which was added as input
   * (MFAbstractDataProcessingTask::addInputData most probably called from
   * MFEngineControlSink::processInputData).
   * Action data: not ready yet but sth like action index action ticket action data size act data
   * @param pData
   * @return
   */
  virtual bool processData(MFStructurableData* pData);

public:
  /**
   * The given action manager should only return actions which are secure for execution! The
   * actions will be executed depending on the content of the network traffic.
   * @param pActionManager
   */
  MFNetActionTask(MFNetworkResources* pNetRes);
  virtual ~MFNetActionTask();

  /**
   * This function serializes the given action and writes the data to the output package.
   * @param pActionTask
   * @return
   */
  bool addAction(MFBaseActionTask* pActionTask);

  /**
   * Enables the confirm action tag. This will ensure that a resond will be sent by
   * the receiver. The respond contains action related data.
   * @return
   */
  void enableConfirmAction(bool enable){
    if(enable){
      m_actionTag+=TAG_CONFIRM_ACTION;
    }
  }

//  /**
//   * pDst will be filled with:
//   *
//   * @return the valid bytes of the created package
//   */
//  uint32_t createActionPackageHeader(MFBaseActionTask* pActionTask,void* pDst, uint32_t maxBytes){
//    return m_validBytes;
//  }
//
//  /**
//   *
//   * @param pActionTask
//   * @param pDst
//   * @param maxBytes
//   * @return
//   */
//  uint32_t createActionPackage(MFBaseActionTask* pActionTask, void*pDst, uint32_t maxBytes);
  //	/**
  //	 * This function will sent the prepared notes.
  //	 * @return
  //	 */
  //	bool sendNotes();
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETACTIONTASK_H_ */
