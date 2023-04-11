/*
 * MFThirdPersonCam.h
 *
 *  Created on: 29.10.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFCAMMOVEMENTS_MFTHIRDPERSONCAM_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFCAMMOVEMENTS_MFTHIRDPERSONCAM_H_
#include "../../../MFSynchronisationClasses/MFSyncObject.h"

/**
 * Third person cam, which is by default moving on a sphere which was cut in
 * vertival direction -> ( ). Default upper limit 3.0f-0.6f.
 * Default lower limit 0.6f. The upper and lower limit are used as offset to
 * the lookAt object.
 *
 */
class MFThirdPersonCam {
public:/*virtual functions MFThirdPersonCam*/
  /**
   * This function must implement all needed actions for specific keys.
   * @param dispatchData - data of the pressed key and the synchronisation object which
   * should be manipulated.
   * @return
   */
  virtual bool updateGlobalCamPosition();

  virtual bool moveFront(float value);
  virtual bool moveBack(float value);
  virtual bool moveLeft(float value);
  virtual bool moveRight(float value);

  virtual bool rotateUp(float value);
  virtual bool rotateDown(float value);
  virtual bool rotateClockwise(float value);
  virtual bool rotateCounterClockwise(float value);
public:
  float/*offsets to the object which the cam shall look at*/
  upperZCamPosLimit=15.0f,
  lowerZCamPosLimit=2.0f,
  upperXCamPosLimit=10.0f,
  lowerXCamPosLimit=-10.0f,
  upperYCamPosLimit=15.0f,
  lowerYCamPosLimit=-15.0,
  maxCamDistance=6.0f,
  minCamDistance=3.0f;

  MFSyncObject
  *mp_lookAt=nullptr,
  *mp_playerCamObject=nullptr;

  glm::vec3
  m_horizontalRot;

  glm::vec4
  m_currentNormalizedCamPos;

  glm::mat4
  m_camMatrix;

  float
  m_zScale=.001f,
  m_rotationScale,
  m_camDistanceScale=6.0f,
  m_moveScale=0.001f;

public:
  MFThirdPersonCam(MFSyncObject* pLookAtThis,MFSyncObject* pCamObject);
  virtual ~MFThirdPersonCam();
  void setLookAtObject(MFSyncObject* pLookAt){mp_lookAt=pLookAt;};
  MFSyncObject* getCamObject(){return mp_playerCamObject;};

};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFCAMMOVEMENTS_MFTHIRDPERSONCAM_H_ */
