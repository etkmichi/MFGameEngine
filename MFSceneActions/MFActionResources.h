/*
 * MFActionResources.h
 *
 *  Created on: 21.10.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFACTIONRESOURCES_H_
#define MFSCENEACTIONS_MFACTIONRESOURCES_H_
#include <MFGeometryManager.h>
#include "../MFEngineModules/MFInterfacesModules/MFIEngineLoopControl.h"
#include "../MFEngineModules/MFInterfacesModules/MFIModuleCreator.h"
#include "../MFEngineModules/MFInterfacesModules/MFIModuleGroupControl.h"
#include "../MFEngineModules/MFInterfacesModules/MFInputSyncNotifier.h"
#include "../MFEngineModules/MFModuleObjectManager.h"

class MFActionTaskManager;
class MFActionResources {
public:
  uint32_t
  m_printIndex=0;

  MFSyncObjectResources
  *mp_syncObjectRes;
  MFIModuleGroupControl
  *mp_groupProvider;
  MFIEngineLoopControl
  *mp_loopCtl;

  MFActionTaskManager
  *mp_actionManager;
  MFModuleObjectManager
  *mp_objectManager;

  MFGeometryManager
  *mp_geometryManager;

public:
  MFActionResources();
  virtual ~MFActionResources();
};

#endif /* MFSCENEACTIONS_MFACTIONRESOURCES_H_ */
