/*
 * MFGameSetupManager.cpp
 *
 *  Created on: 11.11.2020
 *      Author: michl
 */

#include "MFGameSetupManager.h"

MFGameSetupManager::MFGameSetupManager() {
}

MFGameSetupManager::~MFGameSetupManager() {

}


void MFGameSetupManager::initMainGroups(
    MFIModuleGroupControl* pGroupManager,
    MFGameSetupResources* pGameSetupRes){
  pGameSetupRes->mp_groupRenderer=pGroupManager->addModuleGroup("GroupRenderer");
  pGameSetupRes->mp_groupPhysics=pGroupManager->addModuleGroup("GroupPhysics");
  pGameSetupRes->mp_groupInput=pGroupManager->addModuleGroup("GroupInput");
  pGameSetupRes->mp_groupSimulationWorld=pGroupManager->addModuleGroup("GroupPhysicsWorld");
}
