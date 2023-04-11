/*
 * MFCreatorRandomGeometry.cpp
 *
 *  Created on: 20.11.2020
 *      Author: michl
 */

#include "MFCreatorRandomGeometry.h"
#include <glm/gtc/random.hpp>
MFCreatorRandomGeometry::MFCreatorRandomGeometry(
    MFModuleObjectManager* pMOMgr,
    MFGeometryManager* pGeoMgr,
    MFModuleGroup* pGroupSimulation) {
  mp_objectManager=pMOMgr;
  mp_geometryManager=pGeoMgr;
  mp_groupSimulation=pGroupSimulation;
  mp_vecCreationMasks=new std::vector<S_GeometryCreationMask*>();
}

MFCreatorRandomGeometry::~MFCreatorRandomGeometry() {
  for(S_GeometryCreationMask* pGeoMask:*mp_vecCreationMasks){
    if(pGeoMask!=nullptr)
      if(pGeoMask->destroyInDestructor==true && pGeoMask->pGeoSetup!=nullptr){
        delete pGeoMask->pGeoSetup;
      }
    delete pGeoMask;
  }
  delete mp_vecCreationMasks;
}

MFModuleObject* MFCreatorRandomGeometry::createModuleObject(MFSyncObject* pSO){
  if(mp_vecCreationMasks->size()==0){
    MFObject::printWarning("MFCreatorRandomGeometry::createModuleObject - "
        "no valid index added (for geometry creation with MFGeoManager)!");
    return nullptr;
  }
  uint32_t min=0;
  uint32_t max=mp_vecCreationMasks->size()-1;
  uint32_t randomIndex=glm::linearRand(min, max);
  *pSO->getInitialSetup()->mp_initMass=glm::linearRand(1,50);
  MFAbstractGeometry* pCreator=mp_vecCreationMasks->at(randomIndex)->pGeoSetup;
  MFAbstractGeometry* pGeo=mp_geometryManager->createGeometry(
      pCreator->getGeometrySetup());
  pGeo->initGeometry();
  pGeo->createCollisionShape();
  pSO->setGeometry(pGeo);
  mp_objectManager->addCustomObject(
      pSO,
      mp_groupSimulation);
  for(MFIModuleObjectSetup* pSubC:vecSubCreators){
      pSubC->createModuleObject(pSO);
  }
  //TODO if reusage of geometry within render and physics modules is implemented,
  //just return pCreator!
  //
  return nullptr;
}

bool MFCreatorRandomGeometry::preObjectCreation(MFSyncObject* pSO){
  return true;
}

bool MFCreatorRandomGeometry::postObjectCreation(MFSyncObject* pSO,MFModuleObject* pMO){
  return true;
}

void MFCreatorRandomGeometry::initSampleGeometries(){
  MFAbstractGeometry* pSphere=mp_geometryManager->createSphere(0.5f);
  addCreationGeometry(pSphere,true);
  MFAbstractGeometry* pBox=mp_geometryManager->createBox(glm::vec3(0.3f,0.3f,0.3f));
  addCreationGeometry(pBox,true);
  MFAbstractGeometry* pCylinder=mp_geometryManager->createCylinder(2.0f,0.5f);
  addCreationGeometry(pCylinder,true);
  MFAbstractGeometry* pCone=mp_geometryManager->createCone(3.0f,0.8f);
  addCreationGeometry(pCone,true);
  MFAbstractGeometry* pCapsule=mp_geometryManager->createCapsule(3.0f,0.6f);
  addCreationGeometry(pCapsule,true);
//  MFAbstractGeometry* pAxis=mp_geometryManager->createAxis(glm::vec3(0,0,-1),1.7f);
//  addCreationGeometry(pAxis,true);
}

S_GeometryCreationMask* MFCreatorRandomGeometry::addCreationGeometry(
    MFAbstractGeometry* pGeo,
    bool freeInDestructor){
  S_GeometryCreationMask *pM=new S_GeometryCreationMask();
  pM->pGeoSetup=pGeo;
  pM->currentIndex=mp_vecCreationMasks->size();
  pM->destroyInDestructor=freeInDestructor;
  mp_vecCreationMasks->push_back(pM);
  return pM;
}
