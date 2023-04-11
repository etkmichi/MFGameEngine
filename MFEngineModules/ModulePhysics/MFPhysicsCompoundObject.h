/*
 * MFPhysicsCompoundObject.h
 *
 *  Created on: 28.04.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULEPHYSICS_MFPHYSICSCOMPOUNDOBJECT_H_
#define MFENGINEMODULES_MODULEPHYSICS_MFPHYSICSCOMPOUNDOBJECT_H_

#include <MFBaseGeometries/MFDynCompoundGeometry.h>

#include "MFPhysicModuleObject.h"

struct S_PhysicsCompound {
  MFSyncObject* pSO=nullptr;
  S_SubStructure* pSubStructure=nullptr;
  btTransform worldTransform;
};

class MFPhysicsCompoundObject:
    public MFPhysicModuleObject,
    protected MFDynCompoundGeometry {
protected:
  std::vector<S_PhysicsCompound*>
  *pVecCompoundChildObjects;
public:/*virtual functions of MFModuleObject:*/
  virtual void disable();
  virtual void enable();
  virtual bool synchronizeInput();
  virtual bool synchronizeOutput();

public:/*virtual functions of MFDynCompoundGeometry*/
  virtual bool initChildStructures(){return true;};

public: /*virtual functions of MFPhysicsModuleObject*/
  virtual bool initPhysicalObject();
public:
  MFPhysicsCompoundObject();
  virtual ~MFPhysicsCompoundObject();

  bool initCompoundGeometry();
//  S_PhysicsCompound* addChild(
//      const glm::mat4x4 &localTransform,
//      MFAbstractGeometry* pGeo
//      MFSyncObjectResources* pRes,
//      MFInitialObjectSetup* pInitSetup=nullptr);
  /**
   *
   * @param pChild - sync object which must provide a valid geometry (including a physics shape)
   *  and model matrix. The model matrix must be the offsets of the child component
   *  (local transformation of the child)!
   * @return
   */
  S_PhysicsCompound* addChild(MFSyncObject* pChild);

  std::vector<MFAbstractGeometry*>* getVecRenderGeometries(){return getVecSubGeometries();};
  MFAbstractGeometry* getCompoundPhysicsGeometry(){return pPhysicsGeometry;};
  MFDynCompoundGeometry* getCompoundGeometry(){
    return static_cast<MFDynCompoundGeometry*>(this);
  }
};

#endif /* MFENGINEMODULES_MODULEPHYSICS_MFPHYSICSCOMPOUNDOBJECT_H_ */
