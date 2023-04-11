/*
 * MFPhysicsCompoundObject.cpp
 *
 *  Created on: 28.04.2021
 *      Author: michl
 */

#include "MFPhysicsCompoundObject.h"
#include <MFPhysicsHelper.h>
#include <glm/gtc/matrix_inverse.hpp>

MFPhysicsCompoundObject::MFPhysicsCompoundObject() {
  pVecCompoundChildObjects=new std::vector<S_PhysicsCompound*>();
}

MFPhysicsCompoundObject::~MFPhysicsCompoundObject() {
  for(S_PhysicsCompound* pPC:*pVecCompoundChildObjects){
    delete pPC;
  }
  pVecCompoundChildObjects->clear();
  delete pVecCompoundChildObjects;
}

//S_PhysicsCompound* MFPhysicsCompoundObject::addChild(
//    const glm::mat4x4 &localTransform,
//    MFAbstractGeometry* pGeo,
//    MFSyncObjectResources* pRes,
//    MFInitialObjectSetup* pInitSetup){
//  MFSyncObject* pSO=nullptr;
//  if(pInitSetup){
//    pSO=new MFSyncObject(pRes,pInitSetup);
//  }else{
//    pSO=new MFSyncObject(pRes);
//  }
//  S_SubStructure* pSS=addChildGeometry(localTransform, pGeo);
//  pSS->pTransform->getOpenGLMatrix((btScalar*)pSO->getGlobalModelMatrix());
//  S_PhysicsCompound* pC=new S_PhysicsCompound();
//  pC->pSO=pSO;
//  pC->pSS=pSS;
//  pVecInternChildObjects->push_back(pC);
//  return pC;
//}

bool MFPhysicsCompoundObject::initPhysicalObject(){
  if(!initGeometries()) {
    P_ERR("failed to init geometries!");
    return false;
  }

  if(mp_syncObject==nullptr) {
    P_ERR("failed, syncObject==nullptr!");
    return false;
  }

  mp_geometry=getCompoundGeometry()->getPhysicalGeometry();
  if(mp_geometry==nullptr) {
    P_ERR("failed, mp_geometry==nullptr!");
    return false;
  }
  PhysicalObjectData *pPhysObject=(PhysicalObjectData*)mp_physicalObject;
  if(mp_geometry->getBulletCollisionShape()==nullptr){
    pPhysObject->pShape=(btCollisionShape*)mp_geometry->createCollisionShape();
  }else{
    pPhysObject->pShape=(btCollisionShape*)mp_geometry->getBulletCollisionShape();
  }

  if(pPhysObject->pShape==nullptr){
    P_ERR("failed, mp_geometry->createCollisionShape()==nullptr!");
    return false;
  }

  mp_syncObject->setGeometry(mp_geometry);
  MFInitialObjectSetup* pIniDat=mp_syncObject->getInitialSetup();

  pPhysObject->initPosition = GLMV3_TO_BTVEC3((*pIniDat->mp_initPosition));
  pPhysObject->initLinearVelocity=GLMV3_TO_BTVEC3((*pIniDat->mp_initVelocity));
  pPhysObject->mass=*pIniDat->mp_initMass;
  pPhysObject->initModelMatrix=(float*)mp_syncObject->getGlobalModelMatrix();
  pPhysObject->init();
  return true;
}

bool MFPhysicsCompoundObject::initCompoundGeometry(){
  /*init MFDynCompoundGeometry*/
  return false;
}

S_PhysicsCompound* MFPhysicsCompoundObject::addChild(MFSyncObject* pChild){
  //TODO maybe its better :
  //  S_PhysicsCompound* addChild(
  //      const glm::mat4x4 &localTransform,
  //      MFAbstractGeometry* pGeo
  //      MFSyncObjectResources* pRes,
  //      MFInitialObjectSetup* pInitSetup=nullptr);
  MFAbstractGeometry* pGEO=pChild->getGeometry();
  glm::mat4x4* pGLMatrix=pChild->getGlobalModelMatrix();
  S_SubStructure* pSS=this->addChildGeometry(*pGLMatrix,pGEO);
  S_PhysicsCompound* pC=new S_PhysicsCompound();
  pC->pSO=pChild;
  pC->pSubStructure=pSS;
  pVecCompoundChildObjects->push_back(pC);
  return pC;
}

void MFPhysicsCompoundObject::disable(){
  P_ERR("not impl.!");
}

void MFPhysicsCompoundObject::enable(){
  P_ERR("not impl.!");
}

bool MFPhysicsCompoundObject::synchronizeInput(){
  /*write changes from main sync object to main physics object*/
  glm::mat4x4 *pGLMat=mp_syncObject->getGlobalModelMatrix();
  btScalar* pOGLMatrix=(btScalar*)pGLMat;
  PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
  pPhysicalObject->pBody->getWorldTransform().setFromOpenGLMatrix(pOGLMatrix);
//  /*write all changes of sub sync objects to physics object*/
//  glm::mat4x4 *pChildMatrix=nullptr;
//  glm::mat4x4 globalInversTransposed=glm::inverseTranspose(*pGLMat);
//  for(S_PhysicsCompound* pPC:*pVecCompoundChildObjects){
//    //bullet child transform is local and MFSyncObject is global!
//    //To get local transform of MFSyncObject, the SO's model matrix must be multiplied
//    //with the global inverse transposed matrix of the parent/main transformation
//
//    btTransform localTransform=getChildTransform(pPC->pSubStructure->index);
//    pChildMatrix=pPC->pSO->getGlobalModelMatrix();
//    glm::mat4x4 childLocalTransform=globalInversTransposed*(*pChildMatrix);
//    localTransform.setFromOpenGLMatrix((btScalar*)(&childLocalTransform));
//    updateChildTransform(pPC->pSubStructure->index,localTransform);
//  }
  return true;
}

bool MFPhysicsCompoundObject::synchronizeOutput(){
  /*write changes from main physics object to main sync object*/
  PhysicalObjectData* pPhysicalObject=(PhysicalObjectData*)mp_physicalObject;
  if(pPhysicalObject->pMotionState==nullptr){
    MFObject::printErr("MFPhysicModuleObject::synchronizeOutput - "
        "pPhysicalObject->pMotionState==nullptr!");
    return false;
  }
  const btTransform& worldTrafo=pPhysicalObject->pBody->getCenterOfMassTransform();
  glm::mat4x4 *pGLMat=mp_syncObject->getGlobalModelMatrix();
  worldTrafo.getOpenGLMatrix((btScalar*)pGLMat);
  mp_syncObject->triggerModuleInputSync();

  /*write all changes from physics sub components to sub sync objects*/
  for(S_PhysicsCompound* pPC:*pVecCompoundChildObjects){
    const btTransform& localTransform=getChildTransform(pPC->pSubStructure->index);
    btTransform output=worldTrafo*localTransform;
//    P_INF(T_S_V3(B3TV3(localTransform.getOrigin())));
//    pPC->worldTransform=worldTrafo*(*localTransform);
    output.getOpenGLMatrix((btScalar*)(pPC->pSO->getGlobalModelMatrix()));
    pPC->pSO->triggerModuleInputSync();
  }
  return true;
}

