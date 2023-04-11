/*
 * MFModuleObjectManager.h
 *
 *  Created on: 22.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULEOBJECTMANAGER_H_
#define MFENGINEMODULES_MFMODULEOBJECTMANAGER_H_
#include <glm/glm.hpp>
#include <MFGeometryManager.h>

#include "MFInterfacesModules/MFINetNotifier.h"

#include "MFBaseModule.h"
#include "MFBaseGeometries/MFSphere.h"
#include "MFBaseGeometries/MFBox.h"
#include "MFBaseGeometries/MFAxis.h"
#include "MFBaseGeometries/MFAbstractGeometry.h"
#include "../MFEngineModules/MFInputModules/MFInputModule.h"

#include "../MFSynchronisationClasses/MFSyncObject.h"
#include "../MFSynchronisationClasses/MFSyncObjectResources.h"

#include "../MFEngineStructs.h"
#include "MFModuleGroup.h"

#define MODULE_GROUP_DEFAULT 0

struct S_ObjectGroup{
  std::vector<MFSyncObject*> vecObjects;
};

/**
 * This class provides functionality to manage (add, create) objects for usage
 * in the engine. The add functions provide some predefined geometrical structures,
 * but allow to add a custom object too. The create functions can be used
 * to create sync objects.
 */
class MFModuleObjectManager :
    public MFStructurableDataSetup {
  //	std::vector<MFIObjectCreator*>
  //		*mp_vecObjectCreators;
private:
  std::vector<MFINetNotifier*>
  *mp_vecNetworkNotifier;

  std::vector<MFAbstractGeometry*>
  m_vecCleanupGeometries;

  std::vector<MFSyncObject*>
  *mp_vecSyncObject;

  MFSyncObjectResources
  *mp_syncRes;

  MFGeometryManager
  *mp_geometryManager;
  /**
   * sets up the sync object. sets geo, adds to inner vec<SO>, adds group index to so, creates initial
   * spawn cycle, sets initial spawn cycle.
   * @param pSO
   * @param pMG
   * @param pGeo
   * @param spawnCycle
   */
  void setupObject(MFSyncObject* pSO,MFAbstractGeometry* pGeo,uint32_t spawnCycle);
  bool addSOToGroup(MFModuleGroup* pG,MFSyncObject* pSO);
  void notifyAdd(MFSyncObject* pSyncObj);

public:
  MFModuleObjectManager(MFSyncObjectResources* pSyncRes);
  virtual ~MFModuleObjectManager();

  /**
   *
   * @param pG
   * @param pSO
   * @param dublicateAllowed - if true, object will be added again to a module, even it
   * was already added. The object will be connected to multple module objects!
   * @return
   */
//  bool addSyncObjectToGroup(MFModuleGroup* pG,MFSyncObject* pSO,bool dublicateAllowed=false);

  /**
   * Adds a notifier which will be notified if an object was added/removed(TODO)
   * @param pNotifier
   */
  void addNetworkNotifier(MFINetNotifier* pNotifier){
    mp_vecNetworkNotifier->push_back(pNotifier);
  }

  MFGeometryManager* getGeometryManager(){return mp_geometryManager;};

  /**
   *
   * @param position
   * @param radius
   * @param mass
   * @param color
   * @param pModuleGroup
   * @return
   */
  MFSyncObject* addSphere(
      glm::vec3 position,
      float radius,
      float mass,
      glm::vec4 color,
      MFModuleGroup* pModuleGroup,
      uint32_t spawnCycleDelay=0,
      bool notifyAdd=true);

  /**
   *
   * @param position
   * @param xColor
   * @param yColor
   * @param zColor
   * @return An object group with the x-axis, y-axis and z-axis stored in the vector
   * of S_ObjectGroup*.
   */
  S_ObjectGroup* addCoordSystem(
      glm::vec3 position,
      glm::vec4 xColor,
      glm::vec4 yColor,
      glm::vec4 zColor,
      MFModuleGroup* pModuleGroup,
      uint32_t spawnCycleDelay=0,
      bool notifyAdd=true);

  /**
   *
   * @param position
   * @param xColor
   * @param yColor
   * @param zColor
   * @param xAxis
   * @param yAxis
   * @param zAxis
   * @return An object group with the x-axis, y-axis and z-axis stored in the vector
   * of S_ObjectGroup*.
   *
   */
  S_ObjectGroup* addCoordSystem(
      glm::vec3 position,
      glm::vec4 xColor,
      glm::vec4 yColor,
      glm::vec4 zColor,
      glm::vec3* xAxis,float xLength,
      glm::vec3* yAxis,float yLength,
      glm::vec3* zAxis,float zLength,
      MFModuleGroup* pModuleGroup,
      uint32_t spawnCycleDelay=0,
      bool notifyAdd=true);

  /**
   *
   * @param position
   * @param dimensions
   * @param lookAt
   * @param mass
   * @param color
   * @param pModuleGroup
   * @return
   */
  MFSyncObject* addBox(
      glm::vec3 position,
      glm::vec3 dimensions,
      glm::vec3 lookAt,
      float mass,
      glm::vec4 color,
      MFModuleGroup* pModuleGroup,
      uint32_t spawnCycleDelay=0,
      bool notifyAdd=true);

  /**
   *
   * @param position
   * @param lookAt
   * @param pGeometry
   * @param mass
   * @param color
   * @param pModuleGroup
   * @return
   */
  MFSyncObject* addCustomObject(
      glm::vec3 position,
      glm::vec3 lookAt,
      MFAbstractGeometry* pGeometry,
      float mass,
      glm::vec4 color,
      MFModuleGroup* pModuleGroup,
      uint32_t spawnCycleDelay=0,
      bool notifyAdd=true);

  /**
   *
   * @param pSyncObject
   * @param pModuleGroup
   * @param deleteByManager - object will be added for deletion in the destructor. If object
   *  was reused (already used, then recycled, the used again), the it will not be added to
   *  deletion vector!
   * @param spawnCycleDelay
   * @param notifyAdd
   * @param forceAdd - adds an object to a module even if a module object exists
   */
  void addCustomObject(
      MFSyncObject* pSyncObject,
      MFModuleGroup* pModuleGroup,
      bool deleteByManager=false,
      uint32_t spawnCycleDelay=0,
      bool notifyAdd=true,
      bool forceAdd=false);

  // For more custom module objects:
  //TODO addCustomObject(pSO,pModuleObject,pModule,pModuleGroup)
  //  -> TODO MFModuleGroup::addSyncObject(pSO,pMO,pM,...)
  // For more multile custom module objects:
  //TODO addCustomObject(pSO,pVecModuleObjects,pVecModules,pModuleGroup)
  //  -> TODO MFModuleGroup::addSyncObject(pSO,pVecMO,pVecM,...)
  //At the moment custom objects must be added to module first then to this object manager

  //	/**
  //	 * This add function doesn't
  //	 * @param pSyncObject
  //	 * @param pModule
  //	 */
  //	void addCustomObject(
  //			MFSyncObject* pSyncObject,
  //			MFBaseModule* pModule,
  //			uint32_t spawnCycleDelay=0);

  /**
   * If a sync object was created externally, but the object manager shall decide
   * when to delete the MFSyncObject object, then it can be added by this function.
   * @param pSyncObject
   */
  void addDeleteableSyncObject(MFSyncObject* pSyncObject);

  std::vector<MFSyncObject*>* getSyncObjects(){return mp_vecSyncObject;};


  /**
   * This function will use recycled sync objects if possible. If no recycled sync object
   * is available, it will create a new one!
   * @param pMG - used for creating a sync object if not nullptr. Else new is used
   * @param position
   * @param lookAtDirection - direction where the object looks at in the objects original
   *  coordinate system
   * @param color
   * @param mass - 0 for static object (if MFPhysicsModule is used) and >0 for a normal
   *  physics calculation.
   * @param upDirection
   * @return
   */
  MFSyncObject* createSyncObject(
      MFModuleGroup* pMG=nullptr,
      const bool &allowRecycling=true,
      const glm::vec3 &position=glm::vec3(0,0,0),
      const glm::vec3 &lookAtDirection=glm::vec3(1.0f,0.0f,0.0f),
      const  glm::vec4 &color=glm::vec4(0,1,0,0),
      const float &mass=.0f,
      const glm::vec3 &upDirection=glm::vec3(0,1,0)
      );
  MFSyncObject* createSyncObject(
      MFInitialObjectSetup* pInitSetup,
      MFModuleGroup* pMG=nullptr,
      const bool &allowRecycling=true
      );

};

#endif /* MFENGINEMODULES_MFMODULEOBJECTMANAGER_H_ */
