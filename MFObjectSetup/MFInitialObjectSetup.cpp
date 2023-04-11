/*
 * MFInitialObjectSetup.cpp
 *
 *  Created on: 29.01.2020
 *      Author: michl
 */

#include "MFInitialObjectSetup.h"
#include <MFBaseGeometries/MFSphere.h>
#include <cstring>
bool
MFInitialObjectSetup::sm_initDone=false;

float
MFInitialObjectSetup::sm_initMass=1.0f,
MFInitialObjectSetup::sm_initRotAngle=0.0f;

uint32_t
MFInitialObjectSetup::sm_serializedSize=84;

glm::vec3
*MFInitialObjectSetup::sp_dummyPosition,
*MFInitialObjectSetup::sp_dummyLookAt,
*MFInitialObjectSetup::sp_dummyVelocity,
*MFInitialObjectSetup::sp_dummyRotationAxis,
*MFInitialObjectSetup::sp_dummySurroundingBox;

glm::vec4
*MFInitialObjectSetup::sp_dummyColor;

std::mutex
MFInitialObjectSetup::lockDummies;

MFAbstractGeometry*
MFInitialObjectSetup::sp_geometry;
uint32_t
MFInitialObjectSetup::sm_initSpawnCycle;
glm::vec3 up=glm::vec3(0,0,1);
MFInitialObjectSetup::MFInitialObjectSetup() {
  lockData.lock();
  initStaticDummies();
  mp_initMass=&sm_initMass;
  mp_initPosition=sp_dummyPosition;
  mp_initVelocity=sp_dummyVelocity;
  mp_initSurroundingBox=sp_dummySurroundingBox;
  mp_color=sp_dummyColor;
  mp_initLookAt=sp_dummyLookAt;
  mp_initUpwards=&up;
  mp_geometry=nullptr;
  mp_initSpawnCycle=&sm_initSpawnCycle;
  lockData.unlock();
}

MFInitialObjectSetup::~MFInitialObjectSetup() {
}

void MFInitialObjectSetup::initStaticDummies(){
  MFInitialObjectSetup::lockDummies.lock();
  if(sm_initDone){
    lockDummies.unlock();
    return;
  }
  MFInitialObjectSetup::sm_initDone=true;
  sm_initSpawnCycle=0;
  MFInitialObjectSetup::sp_dummyPosition=new glm::vec3(.0f,.0f,.0f);
  MFInitialObjectSetup::sp_dummyVelocity=new glm::vec3(.0f,.0f,.0f);
  MFInitialObjectSetup::sp_dummyRotationAxis=new glm::vec3(0,0,1);
  MFInitialObjectSetup::sp_dummyColor=new glm::vec4(.0f,1.0f,0.2f,0.5f);
  MFInitialObjectSetup::sp_dummySurroundingBox=new glm::vec3(1.0f,1.0f,1.0f);
  MFInitialObjectSetup::sp_dummyLookAt=new glm::vec3(1.0f,.0f,.0f);
  MFInitialObjectSetup::sp_geometry=new MFSphere(1.0f);
  sp_geometry->initGeometry();
  MFInitialObjectSetup::lockDummies.unlock();
  return;
}

void MFInitialObjectSetup::initAllParameters(){
  lockData.lock();
  if(initialDataCreated){
    lockData.unlock();
    return;
  }
  initialDataCreated=true;
  mp_initMass=new float();
  *mp_initMass=0;
  mp_initSpawnCycle=new uint32_t();
  *mp_initSpawnCycle=0;

  mp_initPosition=new glm::vec3(0,0,0);
  mp_initVelocity=new glm::vec3(0,0,0);
  mp_initSurroundingBox=new glm::vec3(1,1,1);
  mp_initLookAt=new glm::vec3(1,0,0);

  mp_color=new glm::vec4(0,1,1,1);
  mp_initUpwards=new glm::vec3(0,0,1);
  lockData.unlock();
}

bool MFInitialObjectSetup::serialize(
    uint8_t* dstBuffer,
    const uint32_t &maxBytes,
    uint32_t &copiedBytes){//TODO add spawn cycle
  lockData.lock();
  if(maxBytes<sm_serializedSize){
    copiedBytes=0;
    lockData.unlock();
    return false;
  }
  memcpy(dstBuffer,mp_initMass,sizeof(float));
  dstBuffer+=sizeof(float);

  memcpy(dstBuffer,mp_initPosition,sizeof(glm::vec3));
  dstBuffer+=sizeof(glm::vec3);
  memcpy(dstBuffer,mp_initVelocity,sizeof(glm::vec3));
  dstBuffer+=sizeof(glm::vec3);
  memcpy(dstBuffer,mp_initLookAt,sizeof(glm::vec3));
  dstBuffer+=sizeof(glm::vec3);
  memcpy(dstBuffer,mp_color,sizeof(glm::vec4));
  dstBuffer+=sizeof(glm::vec4);
  memcpy(dstBuffer,mp_initUpwards,sizeof(glm::vec3));
  dstBuffer+=sizeof(glm::vec3);
  copiedBytes=sm_serializedSize;
  lockData.unlock();
  return true;
}

void MFInitialObjectSetup:: deserialize(uint8_t *pSrc){
  initAllParameters();
  lockData.lock();
  memcpy(mp_initMass,pSrc,sizeof(float));
  pSrc+=sizeof(float);

  memcpy(mp_initPosition,pSrc,sizeof(glm::vec3));
  pSrc+=sizeof(glm::vec3);
  memcpy(mp_initVelocity,pSrc,sizeof(glm::vec3));
  pSrc+=sizeof(glm::vec3);
  memcpy(mp_initLookAt,pSrc,sizeof(glm::vec3));
  pSrc+=sizeof(glm::vec3);
  memcpy(mp_color,pSrc,sizeof(glm::vec4));
  pSrc+=sizeof(glm::vec4);
  memcpy(mp_initUpwards,pSrc,sizeof(glm::vec3));
  pSrc+=sizeof(glm::vec3);
  lockData.unlock();
}






