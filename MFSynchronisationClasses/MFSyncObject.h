/*
 * MFSyncObject.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFSYNCHRONISATIONCLASSES_MFSYNCOBJECT_H_
#define MFSYNCHRONISATIONCLASSES_MFSYNCOBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <mutex>

#include <MFBaseGeometries/MFAbstractGeometry.h>

#include "../MFEngineModules/MFInterfacesModules/MFInputSyncNotifier.h"
#include "../MFObjectSetup/MFInitialObjectSetup.h"
#include "MFSyncObjectResources.h"

struct ModuleData{
  uint32_t moduleIndex;
  std::vector<uint32_t> moduleObjectIndices;
};


/**
 * This class represents a synchronisation object which must be used for all
 * derived classes of MFBaseModule (Game simulation modules  like physics engine,
 * renderer and input check).
 * This object provides the most basic data which is used for the different modules
 * and the engine loop (states (not yet), position, orientation angle)
 */
class MFSyncObject {//TODO create staged sync object
public:
  enum E_RecycleState {
    RECYCLE_STATE_NONE,
    RECYCLE_STATE_RECYCLE_DISABLED,
    RECYCLE_STATE_TRANS_RECYCLED,
    RECYCLE_STATE_RECYCLED,
    RECYCLE_STATE_TRANS_REUSE,
    RECYCLE_STATE_IN_USE
  };
  enum ObjectStateTransition {
    TRANSITION_NONE,
    TRANSITION_ENABLE,
    TRANSITION_DISABLE,
    TRANSITION_UPDATE,
    TRANSITION_REMOVE,
    TRANSITION_DELETE,
    TRANSITION_IDLE
  };
  enum ObjectState {
    STATE_UNDEFINED,
    STATE_CREATION,//TODO implement in modules to skip at state creation
    STATE_UPDATING,
    STATE_IDLE,
    STATE_DISABLED,
    STATE_REMOVED
  };

private:
  uint32_t referenceCount=0;
  void init();

  E_RecycleState
  m_recycleState;

  bool
  update=false,
  m_enableInputSync=true,
  m_enableOutputSync=true,
  m_despawn=false;

  MFSyncObjectResources
  *mp_syncResources;

  uint32_t
  *mp_moduleIndexCounter,
  *mp_objectIDCounter;

  std::vector<MFInputSyncNotifier*>
  *mp_moduleNotifiers;

  //TODO what if its a large world with a lot of objects?
  // optimization tasks must synchronize objectID's from server with clients
  std::mutex
  *mp_lockModuleIndexCounter,
  *mp_lockObjectIDCounter,
  lockRefCount;

  std::mutex /*public locks for sync.*/
  lockModelPosition,
  lockViewMatrix,
  lockProjectionMatrix,
  lockColor,
  lockTranslation,
  *pLockCommonObjects,
  lockUpdateOrientation,
  lockModelMatrixUpdate,
  lockModelMatrix,
  lockRecycleState;
  /*if a synchronistaion is done over a lot of objects at once,
   * this lock should be shared between the objects*/;
protected:
   std::vector<uint32_t>
   *mp_vecModuleGroups;

   uint32_t
   m_objectID=0,
   m_serverObjectID=0;

   glm::mat4x4
   *mp_localRotationModelMatrix,
   *mp_globalModelMatrix;

   glm::vec3
   *mp_modelPosition,
   *mp_translationUpdate,
   *mp_initialLookAt,
   *mp_localLookAt,
   *mp_globalLookAt,
   *mp_upwards;

   //*mp_rotationAxis;
   glm::vec4
   *mp_color;

   std::vector<glm::vec4>
   *mp_vecGlobalRotUpdates;

   std::vector<glm::vec4>
   *mp_vecLocalRotUpdates;

   float
   m_mass;

   MFAbstractGeometry
   *mp_geometry=nullptr;

   MFInitialObjectSetup
   *mp_physicsInitSetup=nullptr,
   *mp_innerPhysicsInitSetup;

   ObjectStateTransition
   m_stateTransition=TRANSITION_NONE;

   ObjectState
   m_state=STATE_UNDEFINED,
   m_previouseState=STATE_UNDEFINED;

   /**
    * The resolve data references the sync object with a module object
    */
   std::vector<ModuleData*>
   *mp_vecModuleResolveData,
   *mp_vecModuleRecycleResolveData;

   std::vector<uint32_t>
   *mp_vecOwningModuleSyncIDs;

   void triggerModuleStateUpdate();
public:
   /**
    * If this function is called, a notification will be sent to all owning
    * module notifiers (MFIInputSyncNotifier*).
    */
   void triggerModuleInputSync();
public:
   /**
    *
    * @param pSharedResources - provides resources for the sync object. The
    * sync object will take it's id from the shared resources and uses the shared
    * resources to trigger an update.
    */
   MFSyncObject(
       MFSyncObjectResources* pSharedResources);

   MFSyncObject(
       MFSyncObjectResources* pSharedResources,
       MFInitialObjectSetup* pInitialSetup);

   MFSyncObject(
       MFSyncObjectResources* pSharedResources,
       glm::vec3 position,
       glm::vec3 lookAt,
       glm::vec4 color,
       float mass);

//   void setRecycleSink(MFIRecycleObject *pRecycler){};
   virtual ~MFSyncObject();
   void setGeometry(MFAbstractGeometry *pGeometry){mp_geometry=pGeometry;};
   MFAbstractGeometry* getGeometry(){return mp_geometry;};

   void setInitialSetup(MFInitialObjectSetup* pIniSetup);
   MFInitialObjectSetup* getInitialSetup(){return mp_physicsInitSetup;};

   void setModelPosition(const glm::vec3& position);

   void setModelLookAtPosition(const glm::vec3 &lookAtPosition,const glm::vec3 &up);
   void setModelLookAtDirection(const glm::vec3 &lookAtDirection,const glm::vec3 &up);

   void addLocalRotationUpdate(glm::vec4 &update);

   void addLocalRotationUpdate(const glm::vec3 &axis,const float &angle);
   /**
    * The additional translation added by this function will be added after a full loop iteration.
    * A full loop iteration means that all writing modules did their writing (module execution).
    * @param additionalTranslation
    */
   void addTranslationUpdate(const glm::vec3 &additionalTranslation);

   /**
    * Adds a rotation update, which will be done after the translation update.
    * @param rotAxis
    * @param angle
    */
   void addGlobalRotationUpdate(
       const glm::vec3 &rotAxis,
       const float &angle);
   /**
    * Writes the translation vector to the position. Uses locks for thread safety.
    */
   void updateModelMatrix();
   glm::mat4x4* getLocalRotationMatrix(){return mp_localRotationModelMatrix;};
   glm::mat4x4 calculateGlobalLookAtMat4x4();
   glm::vec3* getLocalLookAt();
   glm::vec3* getGlobalLookAt();
   glm::vec3* getUpwardsDirection();
   glm::vec3* getForwardDirection();

   glm::vec3* getModelPosition();
   const uint32_t& getObjectID(){return m_objectID;};

   void setModelMatrixByGLMatrix(const void* pGLMatrix){
     *mp_globalModelMatrix=*((glm::mat4x4*)(pGLMatrix));
   }
   glm::mat4x4* getGlobalModelMatrix(){return mp_globalModelMatrix;};

   void setOrientation(const glm::mat3x3 &orientation);

   void setModelRotation(glm::mat4x4* pLocalRotMat);
   void setModelRotation(glm::vec3 &rotationAxis,float angle);

   /**
    * If this MFSyncObject is added to a specific module, the module adds its id and the
    * related module index to this sync object. If this function is used,
    * the module gets notified, when triggerInputSync is called. All modules can
    * resolve the related module object of a sync object without search.
    * @param moduleSybcID module sync id which was provided by a counter
    * of MFSyncObjectResources.
    * @param moduleObjectIndex index within the module for the related
    * MFModuleObject
    */
   void addModuleData(
       uint32_t moduleSyncID,
       uint32_t moduleObjectIndex);

   /**
    * Returns the vector which contains the module sync id's (Can be used to resolve
    * all modules and their objects which are related to this sync object).
    * To get the object indices of a specific module, use
    * getModuleObjectIndices(uint32_t moduleSyncID).
    * @return
    */
   std::vector<uint32_t>* getOwningModuleSyncIDs(){return mp_vecOwningModuleSyncIDs;};

   /**
    *
    * @param moduleSyncID - the id must reference a module which contains at least one
    * module object which is connected to this sync object. To get a list of owning
    * modules, use getOwningModuleSyncIDs().
    * @return
    */
   std::vector<uint32_t>* getModuleObjectIndices(uint32_t moduleSyncID);

   /**
    * Retruns a valid module object index if this object is related to the given module with
    * moduleSyncID.
    * This function doesn't search, it uses a table for resolving.
    * @param moduleSyncID - module sync id of a MFBaseModule (MFBaseModule::getSyncModuleID())
    * @return a valid index to the module with moduleSyncID or 0xFFFFFFFF if module doesn't provide a
    * connection to this sync object.
    */
   uint32_t getFirstModuleObjectIndex(uint32_t moduleSyncID);

   /**
    * This will change the transition to TRANSITION_DISABLE. If a owning module
    * implements the change object transition fucntion, it will remove the
    * object from the execution.
    */
   void disableObject(){
     m_stateTransition=TRANSITION_DISABLE;
     triggerModuleStateUpdate();
   };
   void enableObject(){
     m_stateTransition=TRANSITION_ENABLE;
     triggerModuleStateUpdate();
   };
   //TODO create queue for state transitions!
   void updateObject(){
     m_stateTransition=TRANSITION_UPDATE;
     triggerModuleStateUpdate();
   }

   void despawnObject(){m_despawn=true;}//TODO create module for recycling objects
   void reinitObject(){//TODO f.e. after despawn, reset all vars
     m_despawn=false;
   }
   bool isInputSyncEnabled(){return m_enableInputSync;};
   bool isOutputSyncEnabled(){return m_enableOutputSync;};
   /**
    * If a object does have specific data manipulations, this function should
    * be called for input synchronization with other modules.
    * @param synchrMFNetClientConnectDispatchTask::MFNetClientConnectDispatchTask::onizeInput
    */
   void enableInputSync(bool synchronizeInput){m_enableInputSync=synchronizeInput;};
   void enableOutputSync(bool synchronizeOutput){m_enableOutputSync=synchronizeOutput;};
   void setState(ObjectState state){
     m_state=state;
   };
   const ObjectState& getState(){return m_state;};
   void setStateTransition(ObjectStateTransition trans){
     m_stateTransition=trans;
   };
   void setPreviouseState(ObjectState state){m_previouseState=state;};
   const ObjectState& getPreviouseState(){return m_previouseState;};
   const ObjectStateTransition& getStateTransition(){return m_stateTransition;};
   void addGroupIndex(uint32_t groupIndex,bool checkIfAddedAlready=false);
   void addModuleRecycleData(uint32_t moduleID,uint32_t objectRecycleIndex);
   std::vector<uint32_t>* getModuleRecycleObjectIndices(uint32_t moduleID);

   /**
    * The retuned vector provides the goup indices for all groups which own this object.
    * @return a vector with group indices of all groups which own this object.
    */
   std::vector<uint32_t>* getVecModuleGroups(){return mp_vecModuleGroups;};

   void incrementReferenceCount(){
     lockRefCount.lock();
     referenceCount++;
     lockRefCount.unlock();
   }
   void decrementReferenceCount(){
     lockRefCount.lock();
     if(referenceCount>0)referenceCount--;
//     else //callback=?=
//       /*no reference callback=?*/;
     lockRefCount.unlock();
   }
   uint32_t getReferenceCount(){
     uint32_t ret=0;
     lockRefCount.lock();
     ret=referenceCount;
     lockRefCount.unlock();
     return ret;
   }

   /**
    * This function will cause the removal of this object.
    */
   void removeObject();
   //TODO triggerModulePositionSync()
   //TODO triggerModuleOrientationSync()
   //TODO triggerModule...Sync()

   void printModelMatrix(const std::string &printTitle);

   void enableRecycling(bool enable);
   void addRecycleSink(){};//TODO sitl needed?

   bool changeRecycleState(const E_RecycleState &state);
   const E_RecycleState& getRecycleState(){return m_recycleState;};

   void printRecycleState(){
     std::string info="recycle state: ";
     switch(m_recycleState){
     case RECYCLE_STATE_NONE:
       info+="RECYCLE_STATE_NONE";
       break;
     case RECYCLE_STATE_IN_USE:
       info+="RECYCLE_STATE_IN_USE";
       break;
     case RECYCLE_STATE_RECYCLED:
       info+="RECYCLE_STATE_RECYCLED";
       break;
     case RECYCLE_STATE_RECYCLE_DISABLED:
       info+="RECYCLE_STATE_RECYCLE_DISABLED";
       break;
     case RECYCLE_STATE_TRANS_RECYCLED:
       info+="RECYCLE_STATE_TRANS_RECYCLED";
       break;
     case RECYCLE_STATE_TRANS_REUSE:
       info+="RECYCLE_STATE_TRANS_REUSE";
       break;
     default:
       info+="UNKNOWN!!";
       break;
     }

     P_INF(info);
   }

   bool isRecycleStateTransRecycle(){
     return m_recycleState==RECYCLE_STATE_TRANS_RECYCLED;
   }
   bool isRecycled() const {
     return m_recycleState==RECYCLE_STATE_RECYCLED;
   }

   bool isRecycleReuseTransition(){
     return m_recycleState==RECYCLE_STATE_TRANS_REUSE;
   }

   bool isReused() const {
     return m_recycleState==RECYCLE_STATE_IN_USE;
   }
   bool isNotRecycled() {
     return (
         isReused() ||
         m_recycleState==RECYCLE_STATE_NONE ||
         m_recycleState==RECYCLE_STATE_RECYCLE_DISABLED);
   }
private: /*recycle state change functions*/
   bool recycleRecycleTransition();
   bool recycle();
   bool recycleReuseTransition();
   bool recycleReuse();
};

#endif /* MFSYNCHRONISATIONCLASSES_MFSYNCOBJECT_H_ */
