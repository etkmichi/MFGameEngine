/*
 * MFInitialObjectSetup.h
 *
 *  Created on: 29.01.2020
 *      Author: michl
 */

#ifndef MFOBJECTSETUP_MFINITIALOBJECTSETUP_H_
#define MFOBJECTSETUP_MFINITIALOBJECTSETUP_H_
#include <mutex>
#include <vector>
#include <glm/glm.hpp>
#include <MFBaseGeometries/MFAbstractGeometry.h>
/**
 * This class provides initial data for creation of module objects.
 * The initial data are pointers to static values, if needed, the pointers can be set to
 * other values. For thread safety, use public lockData to access any data of an object of this class!
 * TODO maybe implement take/return with locks for data access and move data fields to private.
 */
class MFInitialObjectSetup {
private:
  bool
    initialDataCreated=false;
  void initStaticDummies();

  static std::mutex
  lockDummies;

  static float
  sm_initMass,
  sm_initRotAngle;

  static uint32_t
  sm_initSpawnCycle;

  static glm::vec3
  *sp_dummyPosition,
  *sp_dummyVelocity,
  *sp_dummySurroundingBox,
  *sp_dummyRotationAxis,
  *sp_dummyLookAt;

  static glm::vec4
  *sp_dummyColor;

  static bool
  sm_initDone;

  static MFAbstractGeometry
  *sp_geometry;

  static uint32_t
  sm_serializedSize;

public:
  std::mutex
  lockData;

  float
  *mp_initMass,
  *mp_initGLMatrix=nullptr;//TODO nowhere used! Remove initPos and use getInitPos() which returns
  //glm::vec3* to this matrix[3]
  uint32_t
  *mp_initSpawnCycle;
  glm::vec3
  *mp_initPosition,
  *mp_initVelocity,
  *mp_initSurroundingBox,
  *mp_initLookAt,
  *mp_initUpwards;

  glm::vec4
  *mp_color;

  MFAbstractGeometry
  *mp_geometry;

public:
  MFInitialObjectSetup();
  virtual ~MFInitialObjectSetup();

  /**
   * This function will allocate all parameters (without the sub objects like mp_geometry!)
   * for this initial setup.
   */
  void initAllParameters();
  void lock(){lockData.lock();}
  void unlock(){lockData.unlock();}

  /**
   *
   * @return the serialized data size of the initial parameters:
   *initMass,32bit float
   *initRotationAngle;32bit float
   *initPosition,3*32bit float
   *initVelocity,3*32bit float
   *initRotationAxis,3*32bit float
   *initLookAt;3*32bit float
   *color;4*32bit float
   */
  static uint32_t getSerializedSize(){return sm_serializedSize;};

  /**
   * Serializes the initial parameters, using the public lockData mutex.
   * TODO doc implementation for this class for specific init values.
   * Parameters (72bytes):
   * initMass,32bit float
   * initRotationAngle;3*32bit float
   * initPosition,3*32bit float
   * initVelocity,3*32bit float
   * initRotationAxis,3*32bit float
   * initLookAt;3*32bit float
   * color;4*32bit float
   * @param dstBuffer
   * @param maxBytes
   * @param copiedBytes
   * @return
   */
  virtual bool serialize(
      uint8_t* dstBuffer,
      const uint32_t &maxBytes,
      uint32_t &copiedBytes);

  virtual void deserialize(uint8_t *pSrc);
};

#endif /* MFOBJECTSETUP_MFINITIALOBJECTSETUP_H_ */
