/*
 * MFModuleGroup.h
 *
 *  Created on: 23.03.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULEGROUP_H_
#define MFENGINEMODULES_MFMODULEGROUP_H_

#include "ModuleRecycling/MFRecycleObjectSink.h"
#include "../MFSynchronisationClasses/MFSyncObject.h"
#include "MFBaseModule.h"
#include "ModuleRecycling/MFRecyclingModule.h"
/**
 * This class provides grouping for all specific objects which uses a MFModuleObject <-> MFSyncObject
 * relation. The grouping is essential for recycling. Group all objects with needed classifications
 * together f.e. bullets of a shooting tower (classifications:same bullet geometry).
 * This class provides parental settings for a better sorting. If a parent is added a new object
 * will be added to this groups sub modules and all parents (incl. paranets of parents...) sub
 * modules! If a module is specified multiple times in the parental structure, a error may occure
 * (not tested)!
 */
class MFModuleGroup: public MFRecycleObjectSink {
public:
  uint32_t
  groupIndex;

  std::string
  groupName;

  std::vector<MFSyncObject*>
  vecSyncObjects;

  std::vector<MFBaseModule*>
  vecModules;

  std::vector<MFModuleGroup*>
  vecParentGroups;

  std::mutex
  lockVecSyncObjects,
  lockVecModules;
protected: /*virtual funcitons MFRecycleObjectSink:*/
  /**
   * Default implementation disables the object in parent group/s and this group
   * @param pSO
   */
  virtual void disableModuleObject(MFSyncObject* pSO);
public:
  MFModuleGroup(
      MFSyncObjectResources *pSyncRes,
      std::string groupName="group",
      uint32_t groupIndex=0);
  virtual ~MFModuleGroup();

  /**
   *
   * @param pSO
   * @param forceAdd
   * @param mainGroup - true for group which is the main group of the object (not a parent)
   * @return
   */
  bool addSyncObject(MFSyncObject* pSO,bool forceAdd=false, bool mainGroup=true);

  /**
   * Adds a paren group. An added sync object will be added to all parent groups!
   * Try not to add a parent group after an object was added by addSyncObject...!2
   *
   * TODO what if parent group is added after at least one addSyncObject call?
   * TODO what if a module was added in this group and parent group?
   * At the moment better do only add modules to the last parent!
   * @param pMG
   */
  void addParentGroup(MFModuleGroup* pMG);
  void addModule(MFBaseModule* pBM);
  void printGroupNames(){
    std::string info="";
    info="\ngroupName | modules: "+groupName+" ";
    for(MFBaseModule* pM:vecModules){
      info+=" | "+pM->getSetupName();
    }
    P_INF(info);
    for(MFModuleGroup* pG:vecParentGroups){
      pG->printGroupNames();
    }
  }
private:
  inline bool recycleReuseTransition(
      MFBaseModule* pM,
      MFSyncObject* pSO,
      bool forceAdd);
};

#endif /* MFENGINEMODULES_MFMODULEGROUP_H_ */
