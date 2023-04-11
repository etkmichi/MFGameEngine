/*
 * MFModuleGroupManager.h
 *
 *  Created on: 26.03.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULEGROUPMANAGER_H_
#define MFENGINEMODULES_MFMODULEGROUPMANAGER_H_
#include "MFInterfacesModules/MFIModuleGroupControl.h"
class MFModuleGroupManager :
    public MFIModuleGroupControl {
private:
  MFSyncObjectResources* pRes;
protected:
  std::vector<MFModuleGroup*>
  *mp_vecModuleGroups;
  std::mutex
  lockVecGroups;
public:/*MFIModuleGroupProvider functions:*/
  virtual MFModuleGroup* addModuleGroup(std::string groupName);
  virtual MFModuleGroup* getModuleGroup(uint32_t groupIndex);
  virtual bool addModuleToGroup(uint32_t groupIndex,MFBaseModule *pModule);
  virtual std::vector<MFModuleGroup*>* getGroups(){return mp_vecModuleGroups;};
public:
  MFModuleGroupManager(MFSyncObjectResources* pRes);
  MFModuleGroupManager();

  virtual ~MFModuleGroupManager();
  void setSyncResources(MFSyncObjectResources* pRes){
    this->pRes=pRes;
  };
  void initDefaultGroup(){
    P_INF("adding default group!");
    MFModuleGroup* nextModule=new MFModuleGroup(pRes);
    nextModule->groupName="DefaultGroup";
    nextModule->groupIndex=mp_vecModuleGroups->size();
    lockVecGroups.lock();
    mp_vecModuleGroups->push_back(nextModule);
    lockVecGroups.unlock();
  }
};

#endif /* MFENGINEMODULES_MFMODULEGROUPMANAGER_H_ */
