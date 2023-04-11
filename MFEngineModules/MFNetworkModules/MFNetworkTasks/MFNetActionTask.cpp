/*
 * MFNetActionTask.cpp
 *
 *  Created on: 19.03.2020
 *      Author: michl
 */

#include "../MFNetworkTasks/MFNetActionTask.h"

MFNetActionTask::MFNetActionTask(MFNetworkResources* pNetRes){
  mp_netRes=pNetRes;
  m_surpressSetupWarning=true;
  setPackageDataSize(5000);
  setPackageCounter(&pNetRes->m_packageCounter);
}

MFNetActionTask::~MFNetActionTask(){
}

bool MFNetActionTask::initOutputPackages(
    MFStructurableData* p_preparePackage,
    MFStructurableData* p_outputPackage){
  uint8_t* pData=static_cast<uint8_t*>(p_preparePackage->getStructureData());
  pData+=2*sizeof(uint16_t);//skip pkg counter and net task index
  mp_prepareActionCount=(uint16_t*)(pData);//increment over header
  pData=static_cast<uint8_t*>(p_outputPackage->getStructureData());
  pData+=2*sizeof(uint16_t);//skip pkg counter and net task index
  mp_outputActionCount=(uint16_t*)(pData);//increment over header
  if(mp_outputActionCount!=nullptr && mp_prepareActionCount!=nullptr){
    uint16_t type=m_taskIndex;
    writePackageTypeID(&type);
    return true;
  }else{
    printErr("MFNetActionTask::initOutputPackages - counter pointer are nullptr!");
    return false;
  }
}

bool MFNetActionTask::prepOutputPackage(
    void* pDst,
    uint64_t &writtenBytes,
    const uint64_t &maxByteSize){
  if(mp_outputActionCount!=nullptr && mp_prepareActionCount!=nullptr){
    uint16_t* var=mp_outputActionCount;
    mp_outputActionCount=mp_prepareActionCount;
    mp_prepareActionCount=var;
    *mp_prepareActionCount=0;
    m_validPrepareByteCount=3*sizeof(uint16_t);/*set to start of data part*/
  }else{
    printErr("MFNetActionTask::prepareOutputPackage - counter pointer are nullptr!");
    return false;
  }
  return true;
}

bool MFNetActionTask::addAction(MFBaseActionTask* pActionTask){
  lockPrepareBuffer.lock();
  uint8_t* pDst=static_cast<uint8_t*>(mp_preparePackage->getStructureData());
  if(m_validPrepareByteCount==0){
    m_validPrepareByteCount=3*sizeof(uint16_t);/*do not overwrite the header*/
  }
  pDst+=m_validPrepareByteCount;/*current offset*/

  uint16_t *actionIndex=(uint16_t*)(pDst);
  (*actionIndex)=pActionTask->getTaskManagerIndex();
  pDst+=sizeof(uint16_t);
  m_validPrepareByteCount+=sizeof(uint16_t);

  uint32_t *actionSize=(uint32_t*)(pDst);
  (*actionSize)=0;
  pDst+=sizeof(uint32_t);
  m_validPrepareByteCount+=sizeof(uint32_t);

  bool ret=pActionTask->serializeAction(
      pDst,
      m_maxSize-m_validPrepareByteCount,
      (*actionSize));

  /*check for overflow ; m_maxSize -> value which was used to init buffers*/
  uint64_t sizeCheck=((*actionSize)+m_validPrepareByteCount);
  if(m_maxSize>sizeCheck){
    m_validPrepareByteCount+=(*actionSize);
  }else{
    /*this shouldn't happen*/
    printWarning("MFNetActionTask::addAction - serialized action size buffer overflow!");
    ret=false;
  }

  if(ret)
    (*mp_prepareActionCount)++;
  lockPrepareBuffer.unlock();
  return ret;
}

uint64_t MFNetActionTask::calculateMaxDataSize(){
  uint64_t dataSize=0;
  dataSize+=sizeof(uint16_t);//pkg counter
  dataSize+=sizeof(uint16_t);//action counter
  /* preiodic structure: size of action | serialized action */
  /* size of action * serialized action size (estimated) * maximal count of acitons */
  dataSize+=((sizeof(uint32_t))*m_estimatedActionSize*m_maxActionCount);/*estimated size for serialized actions*/
  return dataSize;
}

MFStructurableData* MFNetActionTask::createNewEmptyData(){
  MFNetActionTask *pDat=new MFNetActionTask(mp_netRes);
  pDat->setPackageDataSize(m_pkgDataSize);
  return pDat;
}

bool MFNetActionTask::processData(MFStructurableData* pData){
  /*Received data dispatching process*/
  MFObject::printInfo("Debug MFNetActionTask - processData" );
  /**
   * Datagram:
   * pkg counter 16bit - MFNetInputPackage header // pkg counter for checking if pkgs were lost
   * task index 16bit - MFNetInputPackage header //network task index
   * dataSize+=sizeof(uint16_t);- header extension //count of actions within the pkg
   * preiodic structure: size of action | serialized action
   */
  uint8_t* pSrc=(uint8_t*)(pData->getVecData()->at(2)->getData());
  uint16_t actionCount=*((uint16_t*)(pSrc));
  pSrc+=sizeof(uint16_t);

  MFActionTaskManager* pAMgr=mp_netRes->mp_actionManager;
  for(uint32_t i=0;i<actionCount;i++){
    uint16_t taskIndex=*((uint16_t*)(pSrc));
    pSrc+=sizeof(uint16_t);
    uint32_t taskSize=*((uint32_t*)(pSrc));
    pSrc+=sizeof(uint32_t);
    MFBaseActionTask* pTask=static_cast<MFBaseActionTask*>(pAMgr->getTask(taskIndex));
    pTask->addInputData(pData);
    pTask->deserializeAction(pSrc, taskSize);
  }

  return true;
}

//uint32_t MFNetActionTask::createActionPackage(
//    MFBaseActionTask* pActionTask,
//    void *pDst,
//    uint32_t maxBytes){
//  uint32_t *pDst32=static_cast<uint32_t*>(pDst);
//  uint32_t writtenBytes=createActionPackageHeader(pActionTask,pDst,maxBytes);
//}
