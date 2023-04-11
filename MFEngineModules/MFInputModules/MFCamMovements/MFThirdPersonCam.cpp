/*
 * MFThirdPersonCam.cpp
 *
 *  Created on: 29.10.2020
 *      Author: michl
 */

#include "MFThirdPersonCam.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
MFThirdPersonCam::MFThirdPersonCam(MFSyncObject* pLookAtThis,MFSyncObject* pCamObject)
{
  mp_lookAt=pLookAtThis;
  mp_playerCamObject=pCamObject;
  m_horizontalRot=glm::vec3(0,1,0);
  m_currentNormalizedCamPos=glm::vec4(5,5,5,0);
  m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
  upperZCamPosLimit=m_camDistanceScale-m_camDistanceScale/5.0f;
  lowerZCamPosLimit=0.0f+m_camDistanceScale/5.0f;
  m_camMatrix=glm::mat4(1);
  m_camMatrix[3]=m_currentNormalizedCamPos;
  m_rotationScale=.0001f;
  m_moveScale=.001f;
}

MFThirdPersonCam::~MFThirdPersonCam(){

}

/*joystick*/
bool MFThirdPersonCam::moveFront(float value){
  glm::vec3 updateVector=*mp_playerCamObject->getLocalLookAt();
  updateVector.z=0;
  updateVector=glm::normalize(updateVector)*m_moveScale;
  mp_lookAt->addTranslationUpdate(-updateVector);
  return true;
}

bool MFThirdPersonCam::moveBack(float value){
  glm::vec3 updateVector=*mp_playerCamObject->getLocalLookAt();
  updateVector.z=0;
  updateVector=glm::normalize(updateVector)*m_moveScale;
  mp_lookAt->addTranslationUpdate(-updateVector);
  return true;
}

bool MFThirdPersonCam::moveLeft(float value){
  glm::vec3 updateVector=
      glm::cross(*mp_playerCamObject->getLocalLookAt(),glm::vec3(0,0,1));
  updateVector.z=0;
  updateVector=glm::normalize(updateVector)*m_moveScale;
  mp_lookAt->addTranslationUpdate(-updateVector);
  return true;
}

bool MFThirdPersonCam::moveRight(float value){
  glm::vec3 updateVector=
      glm::cross(*mp_playerCamObject->getLocalLookAt(),glm::vec3(0,0,1));
  updateVector.z=0;
  updateVector=glm::normalize(updateVector)*m_moveScale;
  mp_lookAt->addTranslationUpdate(updateVector);
  return true;
}

bool MFThirdPersonCam::rotateUp(float value){
  /*move cam uppwards (z)*/
  m_currentNormalizedCamPos=(m_camMatrix[3]);
  m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
  float camHeight=m_currentNormalizedCamPos.z*m_camDistanceScale;
  if(camHeight <= upperZCamPosLimit){
    glm::mat4 rotUpdate=glm::rotate(glm::mat4(1),-m_rotationScale*value,m_horizontalRot);
    m_camMatrix=rotUpdate*m_camMatrix;
    m_currentNormalizedCamPos=(m_camMatrix[3]);
    m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
    mp_playerCamObject->setModelPosition(
        ((glm::vec3)m_currentNormalizedCamPos*m_camDistanceScale)+
        (*mp_lookAt->getModelPosition()));
  }
  return true;
}

bool MFThirdPersonCam::rotateDown(float value){
  /*move cam downwards (-z)*/
  m_currentNormalizedCamPos=(m_camMatrix[3]);
  m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
  float camHeight=m_currentNormalizedCamPos.z*m_camDistanceScale;
  if(camHeight >= lowerZCamPosLimit){
    glm::mat4 rotUpdate=glm::rotate(glm::mat4(1),m_rotationScale*value,m_horizontalRot);
    m_camMatrix=rotUpdate*m_camMatrix;
    m_currentNormalizedCamPos=(m_camMatrix[3]);
    m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
    mp_playerCamObject->setModelPosition(
        ((glm::vec3)m_currentNormalizedCamPos*m_camDistanceScale)+
        (*mp_lookAt->getModelPosition()));
  }
  return true;
}

bool MFThirdPersonCam::rotateClockwise(float value){
    glm::mat4 rotUpdate=glm::rotate(glm::mat4(1),m_rotationScale*value,glm::vec3(0,0,1));
    m_camMatrix=rotUpdate*m_camMatrix;
    m_currentNormalizedCamPos=(m_camMatrix[3]);
    m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
    mp_playerCamObject->setModelPosition(
        ((glm::vec3)m_currentNormalizedCamPos*m_camDistanceScale)+
        (*mp_lookAt->getModelPosition()));
//    glm::vec3 update=glm::vec3(0,m_zScale*value,0);
//    mp_camObject->addTranslationUpdate(update);

  return true;
}

bool MFThirdPersonCam::rotateCounterClockwise(float value){
  glm::mat4 rotUpdate=glm::rotate(
      glm::mat4(1),-m_rotationScale*value,glm::vec3(0,0,1));
  m_camMatrix=rotUpdate*m_camMatrix;
  m_currentNormalizedCamPos=(m_camMatrix[3]);
  m_currentNormalizedCamPos=glm::normalize(m_currentNormalizedCamPos);
  mp_playerCamObject->setModelPosition(
      ((glm::vec3)m_currentNormalizedCamPos*m_camDistanceScale)+
      (*mp_lookAt->getModelPosition()));
//    glm::vec3 update=glm::vec3(0,-m_zScale*value,0);
//    mp_camObject->addTranslationUpdate(update);

  return true;
}

bool MFThirdPersonCam::updateGlobalCamPosition(){
  /*update the cam position depending on the pos of the look at object*/
  glm::vec3 camPos=*mp_playerCamObject->getModelPosition();
  glm::vec3 currentLAPos=*mp_lookAt->getModelPosition();
  glm::vec3 localCamPos=currentLAPos-camPos;
  m_horizontalRot=glm::cross(localCamPos, glm::vec3(0,0,1));

  float sqDist=glm::length2(localCamPos);
  sqDist=glm::sqrt(sqDist);
  sqDist=sqDist-m_camDistanceScale;
  glm::vec3 difference=glm::normalize(localCamPos)*(sqDist);
  (*mp_playerCamObject->getModelPosition())+=difference;

  glm::vec3 lookAtPosition=(*mp_lookAt->getModelPosition());
  mp_playerCamObject->setModelLookAtPosition(lookAtPosition,glm::vec3(0,0,1));

  glm::vec3 lookAtDirection=lookAtPosition-(*mp_playerCamObject->getModelPosition());
  lookAtDirection.z=0;
  mp_lookAt->setModelLookAtDirection(lookAtDirection,glm::vec3(1,0,0));
  //todO AFTER implementation of debug module (with showLookAtAxis(enabled))
  //see where look at axis shows
  //see the original rotation matrix and compare to rotation matrix after setModelLookAt...
  m_camMatrix[3][0]=(-localCamPos.x);
  m_camMatrix[3][1]=(-localCamPos.y);
  m_camMatrix[3][2]=(-localCamPos.z);
  return true;
}
