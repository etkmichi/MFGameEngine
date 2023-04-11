/*
 * MFNetInitModulesTask.h
 *
 *  Created on: 01.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFTASKDATA_MFNETADDMODULESTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFTASKDATA_MFNETADDMODULESTASK_H_

#include "../../../MFInterfacesModules/MFIManagerControl.h"
#include "../MFNetworkBaseTask.h"

/**
 * This task is used if a client connection is made with a server. The first connection
 * which is made will lead to a package which contains the module setups for setting
 * up a module manager. If the module manager was set up successfully, this task
 * will ignore all other incoming module manager setups from other connections.
 */

/**
 * This class provides client sided connect dispatching. If a client connects to a server,
 * this task will wait for the answer and dispatches the setup provided by the answer.
 */
class MFNetAddModulesTask:
		public MFNetworkBaseTask,
		public MFNetInputPackage  {
//protected:
//	/*MFBasePackage*/
//	virtual void setupDataStructure(){
//		/*allocate a buffer which should be big enough for the module manager setup*/
//		addNBitValue((m_dataSize*8), nullptr);
//		/*Server will serialize the module setup and store in in the package*/
//		/*Client sided init module task will load data from package during processing*/
//	}
public:
	static uint32_t
		sm_maxModuleCount;
private:
	uint32_t
		m_maxModuleCount;
	MFIManagerControl
		*mp_moduleManager;
	bool
		m_isInitialized=false;
public:
	/**
	 *
	 * @param pModuleProvider - module provider which is used for network operations/manipulations
	 */
	MFNetAddModulesTask(MFIManagerControl* pModuleProvider);

	virtual ~MFNetAddModulesTask();

	MFStructurableData* createNewEmptyData();

	bool processData(MFStructurableData* pData);

	void setModuleProvider(MFIManagerControl* pModMgr){mp_moduleManager=pModMgr;};
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFTASKDATA_MFNETINITMODULESTASK_H_ */
