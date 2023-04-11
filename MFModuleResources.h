/*
 * MFStaticEngineResources.h
 *
 *  Created on: 27.04.2020
 *      Author: michl
 */

#ifndef MFMODULERESOURCES_H_
#define MFMODULERESOURCES_H_
#include <GLFW/glfw3.h>
#include <vector>
//#include <mfqtprintermainwindow.h>
//#include <mfqtprinter.h>
//#include <mfqtguimanager.h>
#include <MFIGUIManager.h>
#include "MFSynchronisationClasses/MFSyncObject.h"
/**
 * This class provides data for modules and other engine related classes.
 * The modules and the module manager place their data into a shared object of this
 * class. The data can be used by the modules and the module manager.
 * Example: The MFRenderModule add's it's GLFWwindow* to m_vecWindows which is
 * needed by the MFInptuModule which uses the window for polling key/controled/mouse
 * input.
 * TODO create thread safe add and get functions and make data private!
 */
class MFModuleResources {
private:
  uint32_t
  m_userCount;//count of mod mgr which r using this
public:
  std::vector<GLFWwindow*>
  m_vecWindows;

  std::vector<MFSyncObject*>
  *mp_vecCamObjects=nullptr;

  std::mutex
  lockRes;

  uint32_t
  m_deletionCounter;

//  static MFIGUISetup*
//  smp_guiManager;

  void addUser(){m_userCount++;};

};

#endif /* MFMODULERESOURCES_H_ */
