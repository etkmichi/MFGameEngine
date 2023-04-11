/*
 * MFNetInitModulesTask.h
 *
 *  Created on: 13.05.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETINITMODULEGROUPTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETINITMODULEGROUPTASK_H_

#include <MFTasks/MFAbstractDataProcessingTask.h>
#include <MFObjects/MFObject.h>
#include <vector>

#include "../../MFPackageClasses/MFNetOutputPackage.h"
#include "../../MFNetworkResources.h"
#include "../../../MFBaseModule.h"
#include "../../../MFInterfacesModules/MFIManagerControl.h"
#include "../../../MFInterfacesModules/MFIModuleGroupControl.h"
#include "../MFNetworkBaseTask.h"
#include "../../MFPackageClasses/MFNetInputPackage.h"
/**
 */
/**
 * TODO this is a valid example for using MFNetOutputPackage
 *TODO create base class and MFITaskProvider -> init by calling MFITaskProvider functions (currently
 *TODO all tasks are init within MFNetworkTaskManger.
 * This class inits module-group setup on client side. On server side it serializes the module group setup.
 * The module setup and module-group setup on client and server module managers should be the same!
 *
 * Client sided init module task. Uses data to load module setups and
 * initialize modules. This class will add modules to already created module groups.
 * MFBasePackage:
 * Data part of MFNetInitModulesTask. The data provides setup data for initializing
 * modules for the engine loop.
 *
 * Datagram:
 * Pkg counter - uint16_t - header; counter for every pkg
 * taskIndex - uint16_t - header; index of the network task
 * Count of modules - uint32_t - header extended; count of modules for grouping
 * loop over all modules:
 *  group count of module0 - uint32_t - count of groups module0 belongs too
 *  loop over groops of module0:
 *    index group0 - uint32_t - index of first module
 *    ...
 *  group count of module1
 *  ...
 *
 */
class MFNetInitModuleGroupTask :
		public MFNetworkBaseTask,
		public MFNetOutputPackage,
		public MFNetInputPackage
		 {
//protected:
//	virtual void setupDataStructure(){
//		addNBitValue(m_dataSize*8, nullptr);
//	};
private:
	MFIManagerControl
		*mp_moduleProvider;
	MFNetworkResources
    *mp_netServRes;

protected:/*virtual functions MFNetOutputPackage*/
  virtual bool initOutputPackages(
      MFStructurableData* p_preparePackage,
      MFStructurableData* p_outputPackage){
    return true;
  }
  /**
   * Prepares the output package (fills it with data).
   * @param pDst - destination to write the data too
   * @param writtenBytes - count of written bytes
   * @param maxByteSize - maximal bytes pDst can take
   * @return
   */
	virtual bool prepOutputPackage(void* pDst,uint64_t &writtenBytes,const uint64_t &maxByteSize);
  /**
   * If not set by setMaxOutputSize, this function will calculate the maximal size during initialization.
   * Output buffer will be allocated with the returned max size.
   * @return maximal count of bytes for the output buffer.
   */
  virtual uint64_t calculateMaxDataSize();

public:
	MFNetInitModuleGroupTask(
			MFIManagerControl* pModuleProvider,
			MFNetworkResources *pNetServRes);

	virtual ~MFNetInitModuleGroupTask();
//  bool prepareOutputPackage(){
//    MFNetOutputPackage* pWhatDFk=(MFNetOutputPackage*)(this);
//    return pWhatDFk->prepareOutputPackage();//TODO why does compiler complain if the prepareOutputPackage() is
//    //called directly, its public within MFNetOutp.. and this class derives it public!
	//this happens if virtual bool prepOutputPackage...is named prepareOutputPackage...
//  }

	MFStructurableData* createNewEmptyData();

	void setModuleProvider(MFIManagerControl* pModuleProvider){
		mp_moduleProvider=pModuleProvider;
	}

	bool processData(MFStructurableData* pData);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFMODULETASKS_MFNETINITMODULEGROUPTASK_H_ */
