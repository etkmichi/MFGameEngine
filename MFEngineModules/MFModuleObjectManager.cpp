/*
 * MFModuleObjectManager.cpp
 *
 *  Created on: 22.10.2019
 *      Author: michl
 */

#include "MFModuleObjectManager.h"

MFModuleObjectManager::MFModuleObjectManager(MFSyncObjectResources* pSyncRes) {
	mp_syncRes=pSyncRes;
	mp_vecSyncObject=new std::vector<MFSyncObject*>();
	mp_geometryManager=new MFGeometryManager();
	mp_geometryManager->initGeometryCreators();
	mp_vecNetworkNotifier=new std::vector<MFINetNotifier*>();
}

MFModuleObjectManager::~MFModuleObjectManager() {
	for(MFSyncObject* pSyncO:*mp_vecSyncObject){
		delete pSyncO;
	}
	delete mp_vecSyncObject;
	for(MFAbstractGeometry* pGeometry:m_vecCleanupGeometries){
		delete pGeometry;
	}
}

MFSyncObject* MFModuleObjectManager::addBox(
		glm::vec3 position,
		glm::vec3 dimensions,
		glm::vec3 lookAt,
		float mass,
		glm::vec4 color,
		MFModuleGroup* pModuleGroup,
		uint32_t spawnCycleDelay,
    bool notifyAddObject){
	/*Create sync object with box as geometry*/
	MFAbstractGeometry* pBox=mp_geometryManager->createBox(dimensions);

	m_vecCleanupGeometries.push_back(pBox);

	MFSyncObject* pSyncObj=createSyncObject(
      pModuleGroup,
      true,
      position,
      lookAt,
      color,
      mass);

	setupObject(pSyncObj, pBox, spawnCycleDelay+mp_syncRes->getCurrentCycle());

	addSOToGroup(pModuleGroup, pSyncObj);

	if(notifyAddObject)notifyAdd(pSyncObj);
	return pSyncObj;
}

MFSyncObject* MFModuleObjectManager::addSphere(
		glm::vec3 position,
		float radius,
		float mass,
		glm::vec4 color,
		MFModuleGroup* pModuleGroup,
		uint32_t spawnCycleDelay,
    bool notifyAddObject){
	MFSyncObject* syncO=createSyncObject(
	    pModuleGroup,
      true,
	    position,
	    glm::vec3(1,0,0),color, mass);
	MFAbstractGeometry* pSphere = mp_geometryManager->createSphere(2*radius);
  m_vecCleanupGeometries.push_back(pSphere);

  setupObject(syncO,pSphere,spawnCycleDelay+mp_syncRes->getCurrentCycle());

  addSOToGroup(pModuleGroup, syncO);

	if(notifyAddObject)notifyAdd(syncO);

	return syncO;
}

bool MFModuleObjectManager::addSOToGroup(MFModuleGroup* pG,MFSyncObject* pSO){
  bool ret=pG->addSyncObject(pSO);
  return ret;
}

//bool MFModuleObjectManager::addSyncObjectToGroup(
//    MFModuleGroup* pG,
//    MFSyncObject* pSO,
//    bool dublicateAllowed){
//  pG->vecSyncObjects.push_back(pSO);
//  bool ret=true;
//  for(MFBaseModule *pM:pG->vecModules){
//    if(!dublicateAllowed &&
//        pSO->getFirstModuleObjectIndex(pM->getSyncModuleID())!=0xFFFFFFFF){
//       MFObject::printWarning("MFModuleObjectManager::addSOToGroup - object already connected to "
//           "module!");
//       continue;
//    }
//    if(!pM->addSyncObject(pSO)){
//      MFObject::printErr("MFModuleObjectManager::addSOToGroup - "
//          "failed to add sync object to module!");
//      ret=false;
//    }
//    //pM->getFirstModuleObject(pSyncObj)->updateData();
//  }
//  return ret;
//}

void MFModuleObjectManager::notifyAdd(MFSyncObject* pSyncObj){
  for(MFINetNotifier* pN:*mp_vecNetworkNotifier){
      pN->notifyAddObject(pSyncObj);
  }
}

void MFModuleObjectManager::setupObject(
    MFSyncObject* pSO,
    MFAbstractGeometry* pGeo,
    uint32_t spawnCycle){
  pSO->setGeometry(pGeo);
  pSO->getInitialSetup()->mp_initSpawnCycle=new uint32_t();//TODO add to deletable
  (*pSO->getInitialSetup()->mp_initSpawnCycle)=spawnCycle;
}

S_ObjectGroup* MFModuleObjectManager::addCoordSystem(
			glm::vec3 position,
			glm::vec4 xColor,
			glm::vec4 yColor,
			glm::vec4 zColor,
			MFModuleGroup* pModuleGroup,
			uint32_t spawnCycleDelay,
	    bool notifyAddObject){
	S_ObjectGroup* pObjectGroup=new S_ObjectGroup();
	MFAbstractGeometry* pAxisX=mp_geometryManager->createAxis(glm::vec3(1.0f,0.0f,0.0f), 10.0f);
	MFAbstractGeometry* pAxisY=mp_geometryManager->createAxis(glm::vec3(0.0f,1.0f,0.0f), 10.0f);
	MFAbstractGeometry* pAxisZ=mp_geometryManager->createAxis(glm::vec3(0.0f,0.0f,1.0f), 10.0f);

	pAxisX->initGeometry();
	pAxisY->initGeometry();
	pAxisZ->initGeometry();

	MFSyncObject* syncOX=new MFSyncObject(mp_syncRes,position, glm::vec3(1,0,0), xColor, 0);
	MFSyncObject* syncOY=new MFSyncObject(mp_syncRes,position, glm::vec3(0,1,0), yColor, 0);
	MFSyncObject* syncOZ=new MFSyncObject(mp_syncRes,position, glm::vec3(0,0,1), zColor, 0);

	setupObject(syncOX, pAxisX, spawnCycleDelay+mp_syncRes->getCurrentCycle());
  setupObject(syncOY, pAxisY, spawnCycleDelay+mp_syncRes->getCurrentCycle());
  setupObject(syncOZ, pAxisZ, spawnCycleDelay+mp_syncRes->getCurrentCycle());

  pObjectGroup->vecObjects.push_back(syncOX);
	pObjectGroup->vecObjects.push_back(syncOY);
	pObjectGroup->vecObjects.push_back(syncOZ);

	addSOToGroup(pModuleGroup, syncOX);
  addSOToGroup(pModuleGroup, syncOY);
  addSOToGroup(pModuleGroup, syncOZ);

	if(notifyAddObject)notifyAdd(syncOX);
	if(notifyAddObject)notifyAdd(syncOY);
	if(notifyAddObject)notifyAdd(syncOZ);

	return pObjectGroup;
}


MFSyncObject* MFModuleObjectManager::createSyncObject(
    MFModuleGroup* pMG,
    const bool &allowRecycling,
    const glm::vec3 &position,
    const glm::vec3 &lookAtDirection,
    const glm::vec4 &color,
    const float &mass,
    const glm::vec3 &upDirection
    ){
  MFSyncObject* pSO=nullptr;
  if(allowRecycling && pMG!=nullptr){
    pSO=pMG->getRecycledObject();
  }

  //TODO sync object creation by module group
  if(pSO==nullptr){
    pSO=new MFSyncObject(mp_syncRes,position, lookAtDirection, color, mass);
    mp_vecSyncObject->push_back(pSO);
  }else{
    P_INF("using recycled object!");
  }
  pSO->setModelPosition(position);
  pSO->setModelLookAtDirection(lookAtDirection,upDirection);
  *pSO->getInitialSetup()->mp_initMass=mass;
  *pSO->getInitialSetup()->mp_color=color;
//  pSO->setGeometry(pGeometry)TODO

  return pSO;
}

MFSyncObject* MFModuleObjectManager::createSyncObject(
    MFInitialObjectSetup* pInitSetup,
    MFModuleGroup* pMG,
    const bool &allowRecycling
    ){
  return createSyncObject(
      pMG,
      allowRecycling,
      *pInitSetup->mp_initPosition,
      *pInitSetup->mp_initLookAt,
      *pInitSetup->mp_color,
      *pInitSetup->mp_initMass,
      *pInitSetup->mp_initUpwards);
}

S_ObjectGroup* MFModuleObjectManager::addCoordSystem(
				glm::vec3 position,
				glm::vec4 xColor,
				glm::vec4 yColor,
				glm::vec4 zColor,
				glm::vec3* xAxis,float xLength,
				glm::vec3* yAxis,float yLength,
				glm::vec3* zAxis,float zLength,
				MFModuleGroup* pModuleGroup,
				uint32_t spawnCycleDelay,
		    bool notifyAddObject){
	S_ObjectGroup* pObjectGroup=new S_ObjectGroup();
	MFAbstractGeometry* pAxisX=mp_geometryManager->createAxis(*xAxis, xLength);
	MFAbstractGeometry* pAxisY=mp_geometryManager->createAxis(*yAxis, yLength);
	MFAbstractGeometry* pAxisZ=mp_geometryManager->createAxis(*zAxis, zLength);

	pAxisX->initGeometry();
	pAxisY->initGeometry();
	pAxisZ->initGeometry();

	MFSyncObject* syncOX=createSyncObject(pModuleGroup,true,position, glm::vec3(1,0,0), xColor);
	MFSyncObject* syncOY=createSyncObject(pModuleGroup,true,position, glm::vec3(0,1,0), yColor);
	MFSyncObject* syncOZ=createSyncObject(pModuleGroup,true,position, glm::vec3(0,0,1), zColor);

  setupObject(syncOX, pAxisX, spawnCycleDelay+mp_syncRes->getCurrentCycle());
  setupObject(syncOY, pAxisY, spawnCycleDelay+mp_syncRes->getCurrentCycle());
  setupObject(syncOZ, pAxisZ, spawnCycleDelay+mp_syncRes->getCurrentCycle());

  pObjectGroup->vecObjects.push_back(syncOX);
  pObjectGroup->vecObjects.push_back(syncOY);
  pObjectGroup->vecObjects.push_back(syncOZ);

  addSOToGroup(pModuleGroup, syncOX);
  addSOToGroup(pModuleGroup, syncOY);
  addSOToGroup(pModuleGroup, syncOZ);

	pObjectGroup->vecObjects.push_back(syncOX);
	pObjectGroup->vecObjects.push_back(syncOY);
	pObjectGroup->vecObjects.push_back(syncOZ);

	if(notifyAddObject)notifyAdd(syncOX);
	if(notifyAddObject)notifyAdd(syncOY);
	if(notifyAddObject)notifyAdd(syncOZ);
	return pObjectGroup;
}

MFSyncObject* MFModuleObjectManager::addCustomObject(
		glm::vec3 position,
		glm::vec3 lookAt,
		MFAbstractGeometry* pGeometry,
		float mass,
		glm::vec4 color,
		MFModuleGroup* pModuleGroup,
		uint32_t spawnCycleDelay,
    bool notifyAddObject){
	MFSyncObject* syncO=createSyncObject(pModuleGroup,true,position,lookAt,color,mass);

	setupObject(syncO, pGeometry, spawnCycleDelay+mp_syncRes->getCurrentCycle());

	pModuleGroup->addSyncObject(syncO);

	if(notifyAddObject)notifyAdd(syncO);
	return syncO;
}

void MFModuleObjectManager::addCustomObject(
		MFSyncObject* pSyncObject,
		MFModuleGroup* pG,
		bool deleteByManager,
		uint32_t spawnCycleDelay,
		bool notifyAddObject,
    bool forceAdd){
  if(pSyncObject->getInitialSetup()->mp_initSpawnCycle==nullptr)
    pSyncObject->getInitialSetup()->mp_initSpawnCycle=new uint32_t();//TODO add to deletable
  (*pSyncObject->getInitialSetup()->mp_initSpawnCycle)=spawnCycleDelay+mp_syncRes->getCurrentCycle();

	pG->addSyncObject(pSyncObject,forceAdd,true);
	if(deleteByManager && !pSyncObject->isReused()){
		mp_vecSyncObject->push_back(pSyncObject);
	}
	if(notifyAddObject)notifyAdd(pSyncObject);
	return;
}


//void MFModuleObjectManager::addCustomObject(
//		MFSyncObject* pSyncObject,
//		MFBaseModule* pModule,
//		uint32_t spawnCycleDelay){
//	pModule->addSyncObject(pSyncObject);
//	return;
//}

void MFModuleObjectManager::addDeleteableSyncObject(MFSyncObject* pSyncObject){
	mp_vecSyncObject->push_back(pSyncObject);
}
