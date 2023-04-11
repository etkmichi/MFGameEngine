/*
 * MFMainSetup.h
 *
 *  Created on: 18.08.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFMAINSETUP_H_
#define MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFMAINSETUP_H_

#include <MFIGUISetup.h>
#include <vector>

class MFMainSetup: public MFIGUISetup {
protected:
  std::vector<MFIGUISetup*>
  *mp_vecSubSetups;

public:/* virtual functions of MFIGUISetup*/
  virtual bool setupGUI(MFIMenu *pParent);

public:
  MFMainSetup();
  virtual ~MFMainSetup();

  /*
   * Adds a sub setup which will be used during setup gui.
   */
  void addSubSetup(MFIGUISetup* pSubSetup){
    mp_vecSubSetups->push_back(pSubSetup);
  }
};

#endif /* MFENGINEMODULES_MFGUIIOMODULE_MFBASICMENUSETUP_MFMAINSETUP_H_ */
