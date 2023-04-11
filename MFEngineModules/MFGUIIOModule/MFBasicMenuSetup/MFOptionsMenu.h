/*
 * MFOptionsMenu.h
 *
 *  Created on: 18.08.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFOPTIONSMENU_H_
#define MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFOPTIONSMENU_H_

#include "MFMainSetup.h"
#include <MFMenuClasses/MFIMenu.h>
#include <MFMenuClasses/MFIMenuEntry.h>
class MFOptionsMenu: public MFMainSetup {
private:
  MFIMenu
  *mp_optionsMenu,
  *mp_screen,
  *mp_controls,
  *mp_soundMenu;

public:/* virtual functions of MFIGUISetup*/
  virtual bool setupGUI(MFIMenu *pRootMenu);

public:
  MFOptionsMenu();
  virtual ~MFOptionsMenu();
  MFIMenu* getSoundMenu(){return mp_soundMenu;};
  MFIMenu* getScreenMenu(){return mp_screen;};
  MFIMenu* getControlsMenu(){return mp_controls;};
  MFIMenu* getOptionsMenu(){return mp_optionsMenu;};
};

#endif /* MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFOPTIONSMENU_H_ */
