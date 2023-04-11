/*
 * MFCreatorRandomGeometry.h
 *
 *  Created on: 20.11.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFCREATORIMPLEMENTATIONS_MFCREATORRANDOMGEOMETRY_H_
#define MFENGINEMODULES_MFCREATORIMPLEMENTATIONS_MFCREATORRANDOMGEOMETRY_H_
#include "../MFModuleObjectManager.h"
#include <MFBaseGeometries/MFAbstractGeometry.h>
#include <MFGeometryManager.h>
#include "../MFModuleGroup.h"
#include "../MFInterfacesModules/MFIModuleObjectSetup.h"

struct S_GeometryCreationMask {
  MFAbstractGeometry* pGeoSetup=nullptr;
  int32_t currentIndex=-1;
  bool destroyInDestructor=false;
};

class MFCreatorRandomGeometry: public MFIModuleObjectSetup {
public:/*structs for MFCreatorRandomGeometry*/
private:
  MFModuleObjectManager
  *mp_objectManager;
  MFGeometryManager
  *mp_geometryManager;
  std::vector<S_GeometryCreationMask*>
  *mp_vecCreationMasks;
  MFModuleGroup
  *mp_groupSimulation;
  std::vector<MFIModuleObjectSetup*>
  vecSubCreators;

public:/*virtual functions of MFIModuleObjectCreator*/
  virtual MFModuleObject* createModuleObject(MFSyncObject* pSO=nullptr);
  virtual bool preObjectCreation(MFSyncObject* pSO);
  virtual bool postObjectCreation(MFSyncObject* pSO,MFModuleObject* pMO);

public:
  MFCreatorRandomGeometry(
      MFModuleObjectManager* pMOMgr,
      MFGeometryManager* pGeoMgr,
      MFModuleGroup* pGroupSimulation);

  virtual ~MFCreatorRandomGeometry();

  void initSampleGeometries();

  /**
   * Adds a geometry which will be used for recreating new geometries
   * @param pGeo - geometry which was created by a valid MFGeometryManager!
   * @return index for removing a gemotry from creation.
   */
  S_GeometryCreationMask* addCreationGeometry(
      MFAbstractGeometry* pGeo,
      bool freeInDestructor=true);
  void removeCreationMask(S_GeometryCreationMask* pAddedMask){return;};
  void addSubcreator(MFIModuleObjectSetup *pSubCreator){
    vecSubCreators.push_back(pSubCreator);
  }
};

#endif /* MFENGINEMODULES_MFCREATORIMPLEMENTATIONS_MFCREATORRANDOMGEOMETRY_H_ */
