/*
 * MFGameSetupResources.cpp
 *
 *  Created on: 11.11.2020
 *      Author: michl
 */

#include "MFGameSetupResources.h"

MFGameSetupResources::MFGameSetupResources() {
  mp_vecPlayerObjects=new std::vector<MFSyncObject*>();

}

MFGameSetupResources::~MFGameSetupResources() {
  // TODO Auto-generated destructor stub
}

double MFGameSetupResources::getPhysicsMicroSecsTimeStep(){
  return pPhysicsModule->getTimeSinceLastExecutionStart();
}

double MFGameSetupResources::getDeltaTimeFactor(){
  double val=pPhysicsModule->getLastExecutionTime();
  return val;
}
