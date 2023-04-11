/*
 * MFActionManager.cpp
 *
 *  Created on: 13.03.2020
 *      Author: michl
 */

#include "MFActionTaskManager.h"
#include "MFBasicActions/MFNoAction.h"

MFActionTaskManager::MFActionTaskManager(MFActionResources* pActRes) {
  mp_actRes=pActRes;
}

MFActionTaskManager::~MFActionTaskManager() {
}

bool MFActionTaskManager::initTaskManager(){
  if(m_isInitialized){
    MFObject::printWarning("MFActionTaskManager::initTaskManager - already initialized, returning!");
    return true;
  }
  MFBaseActionTask *pTask=new MFNoAction();
  updateTaskProvider(this,m_taskDataCount,pTask,pTask,(m_tasks.no_task));

  pTask=new MFActionAddObject(mp_actRes);
  updateTaskProvider(this,m_taskDataCount,pTask,pTask,(m_tasks.add_object_task));

  m_isInitialized=true;
	return true;
}
