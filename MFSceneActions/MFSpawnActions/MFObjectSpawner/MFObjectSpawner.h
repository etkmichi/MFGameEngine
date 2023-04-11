/*
 * MFObjectSpawner.h
 *
 *  Created on: 05.11.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFSPAWNACTIONS_MFOBJECTSPAWNER_H_
#define MFSCENEACTIONS_MFSPAWNACTIONS_MFOBJECTSPAWNER_H_
#include "../../MFBasicActions/MFBaseActionTask.h"
#include "../../../MFEngineModules/MFModuleObjectManager.h"
#include <vector>
#include <mutex>
#include <glm/gtc/random.hpp>

#include "../../../MFEngineModules/MFModuleObjectCreator/MFModuleObjectCreator.h"
#include "../../../MFEngineModules/MFModuleGroup.h"
/**
 * Spawns objects in a specific area.
 * TODO implement:
 *  -> a vector which provides masks for objects which can be spawned (sphere cube...)
 *  -> enable/disable random positionning
 *  -> geneateNextObjects function
 *    -> optional with external thread
 *    -> optional with random objects
 *    ->
 */
class MFObjectSpawner : public MFBaseActionTask, public MFModuleObjectCreator {
private:
  MFModuleObjectManager
  *mp_objectManager;


  std::vector<MFModuleGroup*>
  *mp_vecSpawnGroups;

  std::mutex
  lockVecGroups,
  lockVecSpawnedObjects,
  lockVecNextObjects;

  bool
  m_enableRandomPositions=true;

  glm::vec3
  *mp_minArea,
  *mp_maxArea,
  m_spawnerPosition;

protected:
  std::vector<MFSyncObject*>
  *mp_vecSpawnedObjects,
  *mp_vecNextObjects;

public:/*virtual functions MFModuleObejctCreator:*/
  virtual void setupInitialData(MFSyncObject* pSyncObject){
    float
    x=glm::linearRand(mp_minArea->x, mp_maxArea->x),
    y=glm::linearRand(mp_minArea->y, mp_maxArea->y),
    z=glm::linearRand(mp_minArea->z, mp_maxArea->z);
    glm::vec3 pos=glm::vec3(x,y,z)+m_spawnerPosition;
    *pSyncObject->getInitialSetup()->mp_initPosition=pos;
    pSyncObject->setModelPosition(pos);
  }
  virtual bool init(){return true;};
  virtual bool preCreation(MFSyncObject* pSyncObject){return true;};
  virtual bool postCreation(MFSyncObject* pSyncObject){return true;};

public:/*virtual functions of MFObjectSpawner*/
  /**
   * By default, spawns a sphere.
   * @return
   */
  virtual MFSyncObject* spawnNext();

public:
  MFObjectSpawner(MFModuleObjectManager* pMgr);
  virtual ~MFObjectSpawner();
  void addSpawnGroup(MFModuleGroup* pGroup);
  void addSpawnObject(MFSyncObject* pObject);

  /**
   * Automatically fills the next objects queue with count of objects.
   * @param count
   */
  void generateNextObjects(uint32_t count);
  void setSpawnerPosition(const glm::vec3 &position);
  void setSpawnAreaExtend(glm::vec3* minPosition,glm::vec3* maxPosition);
  const glm::vec3 getSpawnPosition(){return m_spawnerPosition;};
  void enableRandomPositioning(bool enable);
  std::vector<MFSyncObject*>* getVecSpawnedObjects(){return mp_vecSpawnedObjects;};
};

#endif /* MFSCENEACTIONS_MFSPAWNACTIONS_MFOBJECTSPAWNER_H_ */
