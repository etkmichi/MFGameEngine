/*
 * MFObjectSpawner.cpp
 *
 *  Created on: 05.11.2020
 *      Author: michl
 */

#include "MFObjectSpawner.h"

glm::vec3 dummy1,dummy2;

MFObjectSpawner::MFObjectSpawner(MFModuleObjectManager* pMgr) :
    MFModuleObjectCreator(pMgr)
{
  mp_objectManager=pMgr;
  mp_vecSpawnedObjects=new std::vector<MFSyncObject*>();
  mp_vecNextObjects=new std::vector<MFSyncObject*>();
  mp_vecSpawnGroups=new std::vector<MFModuleGroup*>();
  dummy1=glm::vec3(0,0,0);
  dummy2=glm::vec3(1,1,1);
  mp_minArea=&dummy1;
  mp_maxArea=&dummy2;
  m_spawnerPosition=glm::vec3(0,0,0);
}

MFObjectSpawner::~MFObjectSpawner() {
}

void MFObjectSpawner::addSpawnGroup(MFModuleGroup* pGroup){
  lockVecGroups.lock();
  mp_vecSpawnGroups->push_back(pGroup);
  lockVecGroups.unlock();
}

void MFObjectSpawner::addSpawnObject(MFSyncObject* pObject){
  lockVecNextObjects.lock();
  mp_vecNextObjects->push_back(pObject);
  lockVecNextObjects.unlock();
}

MFSyncObject* MFObjectSpawner::spawnNext(){
  MFSyncObject* pSO=createNextObject();
  //  for(* pG:*mp_vecSpawnGroups){
  //    pSO=mp_objectManager->addSphere(
  //        pos,
  //        1.0f,
  //        1.0f,
  //        glm::vec4(1.0f,0.3f,0.3f,1.0f),
  //        pG,
  //        0,
  //        true);
  //  }
  if(pSO!=nullptr)
    mp_vecSpawnedObjects->push_back(pSO);

  return pSO;
}

void MFObjectSpawner::setSpawnAreaExtend(glm::vec3* minPosition,glm::vec3* maxPosition){
  mp_minArea=minPosition;
  mp_maxArea=maxPosition;
}

void MFObjectSpawner::enableRandomPositioning(bool enable){
  m_enableRandomPositions=enable;
}

void MFObjectSpawner::generateNextObjects(uint32_t count){
  MFObject::printErr("MFObjectSpawner::generateNextObjects - not impl.!");
}

void MFObjectSpawner::setSpawnerPosition(const glm::vec3 &position){
  m_spawnerPosition=position;
}
