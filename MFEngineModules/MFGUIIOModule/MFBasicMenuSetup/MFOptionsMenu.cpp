/*
 * MFOptionsMenu.cpp
 *
 *  Created on: 18.08.2020
 *      Author: michl
 */

#include "MFOptionsMenu.h"

MFOptionsMenu::MFOptionsMenu() {
  mp_optionsMenu=nullptr;
  mp_screen=nullptr;
  mp_controls=nullptr;
  mp_soundMenu=nullptr;
}

MFOptionsMenu::~MFOptionsMenu() {
}

bool MFOptionsMenu::setupGUI(MFIMenu *pRootMenu){
  mp_optionsMenu=pRootMenu->addSubMenu("Options");
  mp_controls=mp_optionsMenu->addSubMenu("Controls");
  mp_controls->addEntry("Not implemented!", nullptr);
  mp_screen=mp_optionsMenu->addSubMenu("Screen");
  mp_screen->addEntry("Not implemented!", nullptr);
  mp_soundMenu=mp_optionsMenu->addSubMenu("Sound");
  mp_soundMenu->addEntry("Not implemented!", nullptr);
  bool ret=true;
  for(MFIGUISetup* pSubSetup:*mp_vecSubSetups){
    if(pSubSetup!=nullptr){
      /*Use the mp_optionsMenu for sub menus of Option-Menu*/
      ret&=pSubSetup->setupGUI(mp_optionsMenu);
    }
  }
  return ret;
}
