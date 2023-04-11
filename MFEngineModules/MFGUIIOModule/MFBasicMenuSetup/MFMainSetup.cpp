/*
 * MFMainSetup.cpp
 *
 *  Created on: 18.08.2020
 *      Author: michl
 */

#include "MFMainSetup.h"

MFMainSetup::MFMainSetup() {
  mp_vecSubSetups=new std::vector<MFIGUISetup*>();
}

MFMainSetup::~MFMainSetup() {
  // TODO Auto-generated destructor stub
}

bool MFMainSetup::setupGUI(MFIMenu *pRootMenu){
  bool ret=true;
  for(MFIGUISetup* pSubSetup:*mp_vecSubSetups){
    if(pSubSetup!=nullptr){
      ret&=pSubSetup->setupGUI(pRootMenu);
    }
  }
  return ret;
}
