/*
 * MFNetObjectSetupTask.h
 *
 *  Created on: 25.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETOBJECTSETUPTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETOBJECTSETUPTASK_H_

#include <MFTasks/MFAbstractDataProcessingTask.h>
#include <MFData/MFStructurableData.h>

#include "../../MFInterfacesModules/MFIModuleGroupControl.h"
#include "../../MFModuleObjectManager.h"
#include "../MFNetworkResources.h"
#include "MFNetworkBaseTask.h"

/**
 * This class sets up scene objects on the client side.
 * Datagramm:
 * taskIndex | pkg counter | object count | geometry parameters | group setups
 */
class MFNetObjectSetupTask :
    public MFNetworkBaseTask,
    public MFNetInputPackage{
public:
  /*MFNetworkBaseTask*/
  virtual MFNetInputPackage* createPackage();
  virtual bool fillPackage(MFNetInputPackage* pPkg);
  virtual uint32_t calculatePackageSize();

private:
  MFNetworkResources
  *mp_netRes;
  uint32_t
  *mp_objectCount;
  MFDataObjectBuffer
  *mp_sbObjectSetup;

public:
  MFNetObjectSetupTask(MFNetworkResources *pNetRes);
  virtual ~MFNetObjectSetupTask();
  MFStructurableData* createNewEmptyData();
  bool processData(MFStructurableData* pData);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETOBJECTSETUPTASK_H_ */
