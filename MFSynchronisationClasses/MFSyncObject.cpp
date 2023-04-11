/*
 * MFSyncObject.cpp
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#include "MFSyncObject.h"
#include <glm/gtx/rotate_vector.hpp>
#include <MFBasicDefines.h>

MFSyncObject::MFSyncObject(MFSyncObjectResources* pSharedResources){
	mp_syncResources=pSharedResources;
	init();
	pLockCommonObjects=nullptr;
}

MFSyncObject::MFSyncObject(
		MFSyncObjectResources* pSharedResources,
		MFInitialObjectSetup* pInitialSetup){
	mp_syncResources=pSharedResources;
	mp_physicsInitSetup=pInitialSetup;
	init();
	pLockCommonObjects=nullptr;
	mp_geometry=mp_physicsInitSetup->mp_geometry;
	*mp_initialLookAt=*mp_physicsInitSetup->mp_initLookAt;
	*mp_localLookAt=*mp_physicsInitSetup->mp_initLookAt;
	glm::vec3 another=*mp_physicsInitSetup->mp_initLookAt;
	if(*mp_localLookAt!=glm::vec3(0.0f,1.0f,0.0f)){
		another=glm::vec3(0.0f,1.0f,0.0f);
	}else{
		another=glm::vec3(1.0f,0.0f,0.0f);
	}
	*mp_modelPosition=*mp_physicsInitSetup->mp_initPosition;
	setModelLookAtDirection(*mp_initialLookAt,glm::vec3(0,0,1));
  glm::mat4x4 translation=(glm::translate(glm::mat4x4(1.0f),*mp_modelPosition));
  (*mp_globalModelMatrix)=(translation*(*mp_localRotationModelMatrix));
}

glm::vec3* MFSyncObject::getModelPosition(){
  mp_modelPosition=(glm::vec3*)(&(*mp_globalModelMatrix)[3]);
//  mp_modelPosition->x=(*mp_globalModelMatrix)[3][0];
//  mp_modelPosition->y=(*mp_globalModelMatrix)[3][1];
//  mp_modelPosition->z=(*mp_globalModelMatrix)[3][2];
  return mp_modelPosition;
};

MFSyncObject::MFSyncObject(
		MFSyncObjectResources* pSharedResources,
		glm::vec3 position,
		glm::vec3 lookAt,
		glm::vec4 color,
		float mass){
	mp_syncResources=pSharedResources;
	init();
	pLockCommonObjects=nullptr;
	*mp_initialLookAt=lookAt;
	*mp_localLookAt=lookAt;
	m_mass=mass;
	*mp_modelPosition=position;
	mp_physicsInitSetup->mp_initPosition=mp_modelPosition;
	mp_physicsInitSetup->mp_initLookAt=mp_initialLookAt;
	mp_physicsInitSetup->mp_initMass=&m_mass;
	*mp_color=color;
	mp_physicsInitSetup->mp_color=mp_color;
  *mp_modelPosition=*mp_physicsInitSetup->mp_initPosition;
//  setModelLookAtDirection(*mp_initialLookAt,glm::vec3(0,0,1));
  glm::mat4x4 translation=(glm::translate(glm::mat4x4(1.0f),*mp_modelPosition));
  (*mp_globalModelMatrix)=(translation*(*mp_localRotationModelMatrix));
}

void MFSyncObject::setInitialSetup(MFInitialObjectSetup* pIniSetup){
  mp_physicsInitSetup=pIniSetup;
  setGeometry(pIniSetup->mp_geometry);
  *mp_modelPosition=*mp_physicsInitSetup->mp_initPosition;
  mp_color=pIniSetup->mp_color;
}

void MFSyncObject::init(){
	mp_vecModuleGroups=new std::vector<uint32_t>();
	mp_lockModuleIndexCounter=&mp_syncResources->lockModuleIndexCounter;
	mp_lockObjectIDCounter=&mp_syncResources->lockObjectIDCounter;
	mp_objectIDCounter=&mp_syncResources->m_objectIDCounter;
	mp_moduleIndexCounter=&mp_syncResources->m_moduleIndexCounter;
	mp_moduleNotifiers=mp_syncResources->mp_moduleNotifiers;
	mp_lockObjectIDCounter->lock();
	(*mp_objectIDCounter)++;
	m_objectID=*mp_objectIDCounter;
	mp_lockObjectIDCounter->unlock();
//	mp_rotationAxis=new glm::vec3(0.0f,0.0f,1.0f);
	mp_vecOwningModuleSyncIDs=new std::vector<uint32_t>();
	mp_vecModuleResolveData=new std::vector<ModuleData*>();
	mp_vecModuleRecycleResolveData=new std::vector<ModuleData*>();
	mp_modelPosition=new glm::vec3(3.0f,3.0f,3.0f);
	mp_translationUpdate=new glm::vec3(.0f,.0f,.0f);
	mp_upwards=new glm::vec3(.0f,.0f,1.0f);
	mp_initialLookAt=new glm::vec3(1.0f,.0f,.0f);
	mp_localLookAt=new glm::vec3(1.0f,.0f,.0f);
	mp_globalLookAt=new glm::vec3(1.0f,.0f,.0f);
	mp_color=new glm::vec4(0.0f,1.0f,.0f,1.0f);
	mp_globalModelMatrix=new glm::mat4x4(1.0f);
	mp_localRotationModelMatrix=new glm::mat4x4(1.0f);
	mp_vecLocalRotUpdates=new std::vector<glm::vec4>();
	mp_vecGlobalRotUpdates=new std::vector<glm::vec4>();
	if(mp_physicsInitSetup==nullptr){
		mp_innerPhysicsInitSetup=new MFInitialObjectSetup();
		mp_physicsInitSetup=mp_innerPhysicsInitSetup;
	}
  m_recycleState=RECYCLE_STATE_NONE;
	m_state=STATE_IDLE;
	m_stateTransition=TRANSITION_NONE;
}

MFSyncObject::~MFSyncObject() {
	// TODO Auto-generated destructor stub
	delete mp_initialLookAt;
	delete mp_localLookAt;
	for(ModuleData* pDat : *mp_vecModuleResolveData){
		delete pDat;
	}
  for(ModuleData* pDat : *mp_vecModuleRecycleResolveData){
    delete pDat;
  }
	mp_vecModuleRecycleResolveData->clear();
	delete mp_vecModuleRecycleResolveData;
	mp_vecModuleResolveData->clear();
	delete mp_vecModuleResolveData;
	if(mp_innerPhysicsInitSetup!=nullptr)delete mp_innerPhysicsInitSetup;
}

glm::mat4x4 MFSyncObject::calculateGlobalLookAtMat4x4(){
	glm::mat4x4 globalLookAt=glm::lookAt(
			*mp_modelPosition,
			*getGlobalLookAt(),
			*mp_upwards);
	return globalLookAt;
}

glm::vec3* MFSyncObject::getLocalLookAt(){
//  *mp_localLookAt=glm::vec3((*mp_localRotationModelMatrix)*glm::vec4(*mp_initialLookAt,0.0f));
	return mp_localLookAt;
}

glm::vec3* MFSyncObject::getGlobalLookAt(){
	*mp_globalLookAt=*mp_localLookAt+*mp_modelPosition;
	return mp_globalLookAt;
}

glm::vec3* MFSyncObject::getUpwardsDirection(){
	return mp_upwards;
}

glm::vec3* MFSyncObject::getForwardDirection(){
	return mp_globalLookAt;
}

void MFSyncObject::addTranslationUpdate(const glm::vec3 &additionalTranslation){
//	if(mp_translationUpdate->x==0.0f && mp_translationUpdate->y==0.0f
//			&&mp_translationUpdate->z==0.0f)return;
	lockModelMatrixUpdate.lock();
	mp_translationUpdate->x+=additionalTranslation.x;
	mp_translationUpdate->y+=additionalTranslation.y;
	mp_translationUpdate->z+=additionalTranslation.z;
	update=true;
	lockModelMatrixUpdate.unlock();
}

void MFSyncObject::addGlobalRotationUpdate(
    const glm::vec3 &rotAxis,
    const float &angle){
  lockModelMatrixUpdate.lock();
  mp_vecGlobalRotUpdates->push_back(glm::vec4(rotAxis.x,rotAxis.y,rotAxis.z,angle));
  update=true;
  lockModelMatrixUpdate.unlock();
}
void MFSyncObject::addGroupIndex(
    uint32_t groupIndex,
    bool checkIfAddedAlready){
  if(checkIfAddedAlready){
    for(uint32_t index:*mp_vecModuleGroups)
    {
      if(index==groupIndex)
        return;
    }
  }
  mp_vecModuleGroups->push_back(groupIndex);

}

void MFSyncObject::addLocalRotationUpdate(glm::vec4 &quaternion){
	if(quaternion.w==0.0f)
		return;
	lockModelMatrixUpdate.lock();
	mp_vecLocalRotUpdates->push_back(quaternion);
	update=true;
	lockModelMatrixUpdate.unlock();
}

void MFSyncObject::addLocalRotationUpdate(const glm::vec3 &axis,const float &angle){
	lockModelMatrixUpdate.lock();
	mp_vecLocalRotUpdates->push_back(glm::vec4(axis,angle));
	update=true;
	lockModelMatrixUpdate.unlock();
}

void MFSyncObject::setOrientation(const glm::mat3x3 &orientation){
  (*mp_globalModelMatrix)[0][0]=orientation[0][0];
  (*mp_globalModelMatrix)[0][1]=orientation[0][1];
  (*mp_globalModelMatrix)[0][2]=orientation[0][2];

  (*mp_globalModelMatrix)[1][0]=orientation[1][0];
  (*mp_globalModelMatrix)[1][1]=orientation[1][1];
  (*mp_globalModelMatrix)[1][2]=orientation[1][2];

  (*mp_globalModelMatrix)[2][0]=orientation[2][0];
  (*mp_globalModelMatrix)[2][1]=orientation[2][1];
  (*mp_globalModelMatrix)[2][2]=orientation[2][2];
}

void MFSyncObject::updateModelMatrix(){
	lockModelMatrixUpdate.lock();
	if(update){
	  printModelMatrix("update model matrix");
		/*update model orientation*///TODO thread safe!
		for(glm::vec4 rotUpdate:*mp_vecLocalRotUpdates){
//			glm::vec3 updateVector=glm::vec3(
//					rotUpdate.x,rotUpdate.y,rotUpdate.z);
			glm::mat4 rotMatrix=glm::rotate(/*create rotation matrix*/
          glm::mat4(1.0),//*mp_localRotationModelMatrix
          rotUpdate.w,/*angle*/
          (glm::vec3)rotUpdate/*rotation vector*/);
			*mp_localRotationModelMatrix=rotMatrix * (*mp_localRotationModelMatrix);
			*mp_localLookAt=glm::vec3(
					(*mp_localRotationModelMatrix)*glm::vec4(*mp_initialLookAt,0.0f));
//			*mp_rotationAxis=glm::cross(*mp_initialLookAt,*mp_localLookAt);
		}
		mp_vecLocalRotUpdates->clear();

		mp_modelPosition->x+=mp_translationUpdate->x;
		mp_modelPosition->y+=mp_translationUpdate->y;
		mp_modelPosition->z+=mp_translationUpdate->z;

		mp_translationUpdate->x=0.0f;
		mp_translationUpdate->y=0.0f;
		mp_translationUpdate->z=0.0f;
		glm::mat4 rotMatrix=glm::mat4(1.0);
    for(glm::vec4 rotUpdate:*mp_vecGlobalRotUpdates){
      rotMatrix=glm::rotate(/*create rotation matrix*/
          rotMatrix,//*mp_localRotationModelMatrix
          rotUpdate.w,/*angle*/
          (glm::vec3)rotUpdate/*rotation vector*/);
    }
    const glm::mat4 position=glm::translate(glm::mat4x4(1.0f),*mp_modelPosition);
		*mp_globalModelMatrix=rotMatrix*position*(*mp_localRotationModelMatrix);

		update=false;
		triggerModuleInputSync();
	}
	lockModelMatrixUpdate.unlock();

}

void MFSyncObject::setModelPosition(const glm::vec3& position){
  (*mp_globalModelMatrix)[3][0]=position.x;
  (*mp_globalModelMatrix)[3][1]=position.y;
  (*mp_globalModelMatrix)[3][2]=position.z;
}

void MFSyncObject::setModelRotation(glm::mat4x4* pLocalRotMat){
	*mp_localRotationModelMatrix=*pLocalRotMat;
	*mp_localLookAt=glm::vec3(
			(*mp_localRotationModelMatrix)*glm::vec4(*mp_initialLookAt,0.0f));
	*mp_globalModelMatrix=(*mp_localRotationModelMatrix)*(glm::translate(
		glm::mat4x4(1.0f),
		*mp_modelPosition));
	triggerModuleInputSync();
}

void MFSyncObject::printModelMatrix(const std::string &printTitle){
  std::string info="MFSyncObject::printModelMatrix - "+printTitle+" 4x4:\n";
  std::string
  v00=std::to_string((*mp_globalModelMatrix)[0][0]),
  v01=std::to_string((*mp_globalModelMatrix)[0][1]),
  v02=std::to_string((*mp_globalModelMatrix)[0][2]),
  v03=std::to_string((*mp_globalModelMatrix)[0][3]),
  v10=std::to_string((*mp_globalModelMatrix)[1][0]),
  v11=std::to_string((*mp_globalModelMatrix)[1][1]),
  v12=std::to_string((*mp_globalModelMatrix)[1][2]),
  v13=std::to_string((*mp_globalModelMatrix)[1][3]),
  v20=std::to_string((*mp_globalModelMatrix)[2][0]),
  v21=std::to_string((*mp_globalModelMatrix)[2][1]),
  v22=std::to_string((*mp_globalModelMatrix)[2][2]),
  v23=std::to_string((*mp_globalModelMatrix)[2][3]),
  v30=std::to_string((*mp_globalModelMatrix)[3][0]),
  v31=std::to_string((*mp_globalModelMatrix)[3][1]),
  v32=std::to_string((*mp_globalModelMatrix)[3][2]),
  v33=std::to_string((*mp_globalModelMatrix)[3][3]);
  info+=v00+" | "+v10+" | "+v20+" | "+v30+"\n";
  info+=v01+" | "+v11+" | "+v21+" | "+v31+"\n";
  info+=v02+" | "+v12+" | "+v22+" | "+v32+"\n";
  info+=v03+" | "+v13+" | "+v23+" | "+v33+"\n";
  MFObject::printInfo(info);
}

void MFSyncObject::setModelLookAtPosition(
    const glm::vec3 &lookAtPosition,const glm::vec3 &up){
  glm::vec3 lookAtVector=lookAtPosition-(*getModelPosition());
//  glm::vec3 normal = glm::cross(lookAtVector, *mp_localLookAt);
  lookAtVector=glm::normalize(lookAtVector);
  *mp_localLookAt=lookAtVector;
  (*mp_localRotationModelMatrix)=glm::orientation(lookAtVector, up);

  glm::mat4x4 translation=(glm::translate(glm::mat4x4(1.0f),*getModelPosition()));
  (*mp_globalModelMatrix)=(translation*(*mp_localRotationModelMatrix));
  triggerModuleInputSync();
}

void MFSyncObject::setModelLookAtDirection(
    const glm::vec3 &lookAtDirection,const glm::vec3 &up){
  *mp_localLookAt=lookAtDirection;
  *mp_localLookAt=glm::normalize(*mp_localLookAt);
  (*mp_localRotationModelMatrix)=glm::orientation(*mp_localLookAt, up);

  glm::mat4x4 translation=(glm::translate(glm::mat4x4(1.0f),*getModelPosition()));
  (*mp_globalModelMatrix)=(translation*(*mp_localRotationModelMatrix));
  triggerModuleInputSync();
}

void MFSyncObject::setModelRotation(glm::vec3 &rotationAxis,float angle){
	*mp_localRotationModelMatrix=glm::rotate(
			glm::mat4(1.0),//*mp_localRotationModelMatrix
			angle,//0.2f,//,
			rotationAxis);
	*mp_localLookAt=glm::vec3(
			(*mp_localRotationModelMatrix)*glm::vec4(*mp_initialLookAt,0.0f));
	*mp_globalModelMatrix=(*mp_localRotationModelMatrix)*(glm::translate(
		glm::mat4x4(1.0f),
		*mp_modelPosition));
	triggerModuleInputSync();
}

void MFSyncObject::addModuleData(
		uint32_t moduleID,
		uint32_t moduleObjectIndex){
	/*the mp_vecModuleResolveData is used for resolving to module objects.
	 * For every module subclass object which is instantiated, the mp_vec... stores a
	 * pointer to the specific module (if the sync object is added to the module!)
	 * The location of the module will stay the same for all module objects, this means
	 * no search is needed for syncObjcet<->moduleObject translation, if the module
	 * is known (the module object must be at the position of
	 * MFBaseModule::getSyncObjectIndex()). If the object was not added to the module,
	 * a nullptr is stored within the mp_vec...
	 * */
	//TODO test what happens if module count is large and many objects are added to many modules.
	//find solution, maybe documentation note...
	//mp_vecModuleResolveData as group data -> add dimension to moduleRef->moduleObjectIndices
	//for refering to sync object's moduleObjectIndices
	//(moduleRef->syncOGroup->at(SOID)->moduleObjectIndices...
	// 	-> if implemented like this, size of mp_vecModuleResolveData
	uint32_t modID=(moduleID+1);
	bool create=(mp_vecModuleResolveData->size() <= modID
			||(mp_vecModuleResolveData->at(moduleID)==nullptr));
	ModuleData *moduleRef=nullptr;
	if(create){
		if(mp_vecModuleResolveData->size() <= modID)mp_vecModuleResolveData->resize(modID+1);
		moduleRef=new ModuleData();
		moduleRef->moduleIndex=moduleID;
		mp_vecModuleResolveData->data()[moduleID]=moduleRef;
		mp_vecOwningModuleSyncIDs->push_back(moduleID);
	}else{
		moduleRef=mp_vecModuleResolveData->at(moduleID);
	}
	moduleRef->moduleObjectIndices.push_back(moduleObjectIndex);
}

void MFSyncObject::enableRecycling(bool enable){
  if(enable){
    m_recycleState=RECYCLE_STATE_NONE;
  }else{
    m_recycleState=RECYCLE_STATE_RECYCLE_DISABLED;
  }
}

bool MFSyncObject::recycle(){
  //TODO recycleStateTransition(RECYCLE_STATE)
  // -> add recycleControl -> callback for recycling within game loop
  // recycleControl is MFRecycleModule which will be executed after the other modules
  //  -> it will change state for recycling from TRANS_RECYCLE to RECYCLE...
  lockRecycleState.lock();
  if(m_recycleState==RECYCLE_STATE_RECYCLED){
    lockRecycleState.unlock();
    P_ERR("already recycled!");
    return false;
  }
  m_recycleState=RECYCLE_STATE_RECYCLED;
  lockRecycleState.unlock();
  //TODO recycle callback
  return true;
}

bool MFSyncObject::recycleRecycleTransition(){
  static float
  x=-10.0f,
  y=-10.0f,
  z=15.0f;
  //TODO recycleStateTransition(RECYCLE_STATE)
  // -> add recycleControl -> callback for recycling within game loop
  // recycleControl is MFRecycleModule which will be executed after the other modules
  //  -> it will change state for recycling from TRANS_RECYCLE to RECYCLE...
  lockRecycleState.lock();
  if(m_recycleState==RECYCLE_STATE_RECYCLED){
    lockRecycleState.unlock();
    P_ERR("already recycled!");
    return false;
  }
  if(m_recycleState==RECYCLE_STATE_TRANS_RECYCLED){
    lockRecycleState.unlock();
    P_WRN("already in recycle transition!");
    return true;
  }
  m_recycleState=RECYCLE_STATE_TRANS_RECYCLED;
  lockRecycleState.unlock();
//  if(mp_syncResources->pRecycleManager!=nullptr)
//    mp_syncResources->pRecycleManager->addRecycleObject(this);
  //TODO recycle transition callback
  setModelPosition(V3(x,y,z));
  triggerModuleInputSync();
  x+=1.0f;
  if(x>10.0f){
    y+=1.0f;
    x=-10.0f;
    if(y>10.0f){
      y=-10.0f;
      x=-10.0f;
      z+=1.0f;
    }
  }
  return true;
}

bool MFSyncObject::changeRecycleState(const E_RecycleState &state){
  if(m_recycleState==RECYCLE_STATE_RECYCLE_DISABLED)
    return false;
  switch(state){
  case RECYCLE_STATE_TRANS_RECYCLED:
    return recycleRecycleTransition();
  case RECYCLE_STATE_RECYCLED:
    return recycle();
  case RECYCLE_STATE_TRANS_REUSE:
    return recycleReuseTransition();
  case RECYCLE_STATE_IN_USE:
    return recycleReuse();
  case RECYCLE_STATE_RECYCLE_DISABLED:
    m_recycleState=RECYCLE_STATE_RECYCLE_DISABLED;
    return true;
  default:
    return true;
  }
}
bool MFSyncObject::recycleReuseTransition(){
  lockRecycleState.lock();
  if(m_recycleState==RECYCLE_STATE_TRANS_REUSE || m_recycleState==RECYCLE_STATE_IN_USE){
    lockRecycleState.unlock();
    if(m_recycleState==RECYCLE_STATE_IN_USE)
      P_ERR("already in state RECYCLE_STATE_REUSED!");
    else
      P_ERR("already in state RECYCLE_STATE_TRANS_REUSE!");
    return false;
  }
  m_recycleState=RECYCLE_STATE_TRANS_REUSE;
  lockRecycleState.unlock();
  return true;
}
bool MFSyncObject::recycleReuse(){
  lockRecycleState.lock();
  if(m_recycleState==RECYCLE_STATE_IN_USE){
    lockRecycleState.unlock();
    P_ERR("already reused");
    return false;
  }
  m_recycleState=RECYCLE_STATE_IN_USE;
  lockRecycleState.unlock();
  return true;
}


void MFSyncObject::addModuleRecycleData(
    uint32_t moduleID,
    uint32_t objectRecycleIndex ){
  /*the mp_vecModuleResolveData is used for resolving to module objects.
   * For every module subclass object which is instantiated, the mp_vec... stores a
   * pointer to the specific module (if the sync object is added to the module!)
   * The location of the module will stay the same for all module objects, this means
   * no search is needed for syncObjcet<->moduleObject translation, if the module
   * is known (the module object must be at the position of
   * MFBaseModule::getSyncObjectIndex()). If the object was not added to the module,
   * a nullptr is stored within the mp_vec...
   * */
  //TODO test what happens if module count is large and many objects are added to many modules.
  //find solution, maybe documentation note...
  //mp_vecModuleResolveData as group data -> add dimension to moduleRef->moduleObjectIndices
  //for refering to sync object's moduleObjectIndices
  //(moduleRef->syncOGroup->at(SOID)->moduleObjectIndices...
  //  -> if implemented like this, size of mp_vecModuleResolveData
  uint32_t modID=(moduleID+1);
  bool create=(mp_vecModuleRecycleResolveData->size() <= modID
      ||(mp_vecModuleRecycleResolveData->at(modID)==nullptr));
  ModuleData *moduleRef=nullptr;
  if(create){
    mp_vecModuleRecycleResolveData->resize(modID+1);
    moduleRef=new ModuleData();
    moduleRef->moduleIndex=moduleID;
    mp_vecModuleRecycleResolveData->data()[moduleID]=moduleRef;
  }else{
    moduleRef=mp_vecModuleRecycleResolveData->at(moduleID);
  }
  if(moduleRef!=nullptr)
    moduleRef->moduleObjectIndices.push_back(objectRecycleIndex);
  else
    P_WRN("no module recycle reference!");
}
std::vector<uint32_t>* MFSyncObject::getModuleRecycleObjectIndices(uint32_t moduleID){
  if(mp_vecModuleRecycleResolveData->size()<=moduleID ||
      mp_vecModuleRecycleResolveData->at(moduleID)==nullptr){
    MFObject::printErr("MFSyncObject::getModuleObjectIndices - no indices found for"
        " given moduleID! returning nullptr");
    return nullptr;
  }
  return &(mp_vecModuleRecycleResolveData->at(moduleID)->moduleObjectIndices);
}


void MFSyncObject::triggerModuleInputSync(){
	//TODO mutex* for both vec's to ensure thread safety if addModuleData is called!
	for(uint32_t moduleSyncID:*mp_vecOwningModuleSyncIDs){
		//notify related module objects (one syncO may be connected to many modO's)
	  if(mp_vecModuleResolveData->size()<=moduleSyncID)continue;
	  if(mp_vecModuleResolveData->at(moduleSyncID)==nullptr)continue;
		for(uint32_t objectIndex:
				mp_vecModuleResolveData->at(moduleSyncID)->moduleObjectIndices){
			mp_moduleNotifiers->at(moduleSyncID)->addSyncNotification(objectIndex);
		}
	}
}

void MFSyncObject::triggerModuleStateUpdate(){
//	for(MFBaseModule* pModuleIterator:*mp_vecOwningModules){
//		pModuleIterator->addStateUpdate(this);
//	}
}

std::vector<uint32_t>* MFSyncObject::getModuleObjectIndices(uint32_t moduleID){
	if(mp_vecModuleResolveData->size()<=moduleID ||
			mp_vecModuleResolveData->at(moduleID)==nullptr){
		P_ERR("no indices found for given moduleID! returning nullptr");
		return nullptr;
	}
	return &(mp_vecModuleResolveData->at(moduleID)->moduleObjectIndices);
}

//MFModuleObject* MFSyncObject::getModuleObject(uint32_t index){
//	return mp_vecModuleResolveData->at(index)->pModuleObject;
//}
uint32_t MFSyncObject::getFirstModuleObjectIndex(uint32_t moduleID){
	if(mp_vecModuleResolveData->size()<=moduleID){
		return 0xFFFFFFFF;
	}
  if(mp_vecModuleResolveData->at(moduleID)==nullptr){
    return 0xFFFFFFFF;
  }
	if(mp_vecModuleResolveData->at(moduleID)->moduleObjectIndices.size()==0){
	  return 0xFFFFFFFF;
	}
	return (mp_vecModuleResolveData->at(moduleID)->moduleObjectIndices.at(0));
}

//MFModuleObject* MFSyncObject::searchModuleObject(MFBaseModule* pModule){
//	for(ModuleData* pModuleIterator:*mp_vecModuleResolveData){
//		if(pModuleIterator->pModule==pModule){
//			return pModuleIterator->pModuleObject;
//		}
//	}
//	return nullptr;
//}

