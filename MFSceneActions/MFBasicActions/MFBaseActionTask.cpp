/*
 * MFBaseAction.cpp
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#include "MFBaseActionTask.h"
#include <cstring>
MFBaseActionTask::MFBaseActionTask() {
  setTaskName("MFBaseActionTask");
	m_key=-1;
	m_userTicket=-1;
}

MFBaseActionTask::~MFBaseActionTask() {
	// TODO Auto-generated destructor stub
}

//bool MFBaseActionTask::checkUser(uint32_t userTag){
//	for(uint32_t t:m_vecUserTags){
//		if(userTag&t)return true;
//	}
//	return false;
//}

void MFBaseActionTask::serializeActionHeader(void* dstData,uint32_t maxByteSize,uint32_t &writtenBytes){
  /*contains ticket task manager index and ticket*/
  uint32_t* pDst=static_cast<uint32_t*>(dstData);

  memcpy(pDst,&getTaskManagerIndex(),sizeof(uint32_t));
  pDst++;
  writtenBytes+=sizeof(uint32_t);

  memcpy(pDst,&m_userTicket,sizeof(uint32_t));
  pDst++;
  writtenBytes+=sizeof(uint32_t);
  return;
}
