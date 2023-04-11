/*
 * MFPlayerObject.h
 *
 *  Created on: 02.12.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULEPLAYER_MFPLAYEROBJECT_H_
#define MFENGINEMODULES_MODULEPLAYER_MFPLAYEROBJECT_H_
#include "../MFModuleObject.h"
#include "../MFInputModules/MFInputModuleObject.h"
#include "../ModulePhysics/MFPhysicModuleObject.h"
#include "../MFInputModules/MFInputModule.h"
#include "../ModulePhysics/MFPhysicsModule.h"
#include "../ModuleRender/MFRenderModuleObject.h"
#include <MFEngineModules/MFModuleGroup.h>
class MFPlayerObject: public MFModuleObject {
protected:
  static uint32_t s_playerCounter;
  uint32_t playerID=0;
  //TODO static player counter for key/joystick setup (default keys wasd for p1 ijkl for p2...)
public:
  MFInputModuleObject
  *pPlayerInputObject=nullptr;
  MFPhysicModuleObject
  *pPlayerPhysicsObject=nullptr;
  MFRenderModuleObject
  *pPlayerRenderObject=nullptr;
  //todo add int joystickID -1 if no joystick
  /*TODO implement and add a class for player input keys (wasd-fire-reload-use-...) */
  MFModuleGroup
  *pGroupPlayers=nullptr;

protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject){};
  virtual void setupObjectDataStructure(){};
public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){return true;};
  virtual bool updateObject(){return true;};
  virtual void disable(){};
  virtual void enable(){};
  /**
   * Reads the data from the given MFSyncObject.
   * @param syncObject
   * @return
   */
  virtual bool synchronizeInput(){
    return true;
  };
  /**
   * Writes the data to the given MFSyncObject.
   * @param syncObject
   * @return
   */
  virtual bool synchronizeOutput(){
    return true;
  };
public:/*virtual functions of MFPlayerObject*/
  virtual bool initPlayerGeometries(){return true;};
  virtual bool initPlayerControls(){return true;};
  virtual bool executePlayer(){return true;};
public:
  MFPlayerObject(){
    playerID=s_playerCounter;
    s_playerCounter++;
  };
  MFPlayerObject(MFSyncObject* pPlayerObject);
  MFPlayerObject(MFModuleGroup* pGroupPlayers);
  virtual ~MFPlayerObject();

  bool initPlayer(){return true;};
  static glm::vec4 getPlayerColor(uint32_t index);
  static glm::vec4 getPlayerColor();
//This is more player module related:
//  static MFModuleGroup* createPlayerGroup(
//      MFModuleGroup* pModMgr,
//      MFInputModule* pIMod,
//      MFPhysicsModule* pPhysMod){
//    return nullptr;
//  };
  /**
   * initInputObject()
   */
//  void initInputObject();
//  void initPlayerObject();

//   MFInputModuleObject* getPPlayerInputObject()  {
//    return pPlayerInputObject;
//  }
//
//  void setPPlayerInputObject( MFInputModuleObject *pPlayerInputObject = nullptr) {
//    this->pPlayerInputObject = pPlayerInputObject;
//  }
//
//   MFPhysicModuleObject* getPPlayerPhysicsObject()  {
//    return pPlayerPhysicsObject;
//  }
//
//  void setPPlayerPhysicsObject(MFPhysicModuleObject *pPlayerPhysicsObject = nullptr) {
//    this->pPlayerPhysicsObject = pPlayerPhysicsObject;
//  }
//
//   MFRenderModuleObject* getPPlayerRenderObject()  {
//    return pPlayerRenderObject;
//  }
//
//  void setPPlayerRenderObject(MFRenderModuleObject *pPlayerRenderObject = nullptr) {
//    this->pPlayerRenderObject = pPlayerRenderObject;
//  }
};

#endif /* MFENGINEMODULES_MODULEPLAYER_MFPLAYEROBJECT_H_ */
