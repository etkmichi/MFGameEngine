/*
 * MFSyncObjectResources.h
 *
 *  Created on: 08.06.2020
 *      Author: michl
 */

#ifndef MFSYNCHRONISATIONCLASSES_MFSYNCOBJECTRESOURCES_H_
#define MFSYNCHRONISATIONCLASSES_MFSYNCOBJECTRESOURCES_H_
#include <vector>
#include <mutex>
#include <MFGeometryManager.h>
#include <MFPrinters/MFPrintTarget.h>
#include "../MFEngineModules/MFInterfacesModules/MFIEngineLoopControl.h"
#include "../MFEngineModules/MFInterfacesModules/MFIRecycleManager.h"


#include "../MFEngineModules/MFModuleSyncTasks/MFModuleSyncTask.h"
#include "../MFEngineModules/MFInterfacesModules/MFInputSyncNotifier.h"
class MFSyncObjectResources {
private:
	static uint32_t idCounter;
	static std::mutex lockID;

	/*network sync tasks for modules*/
	std::vector<MFModuleSyncTask*>*
		mp_vecModuleSyncTasks;
public:
	uint32_t
	m_logIndex=0;
	MFPrintTarget*
  mp_logWindow;

	MFIEngineLoopControl
  *mp_loopCtl=nullptr;
  MFGeometryManager
  *mp_geometryManager=nullptr;


	uint32_t
		m_moduleIndexCounter,
		m_objectIDCounter,
		m_syncResourceID=0;
	std::vector<MFInputSyncNotifier*>*
		mp_moduleNotifiers;
	//TODO what if its a large world with a lot of objects?
		// optimization tasks must synchronize objectID's from server with clients
	std::mutex
		lockModuleIndexCounter,
		lockObjectIDCounter;
	MFIRecycleManager
	*pRecycleManager=nullptr;
public:
	MFSyncObjectResources();
	virtual ~MFSyncObjectResources();

	/**
	 * For a new module this function will be called. A sync object will store the modules specific
	 * data at the returned index.
	 * @param pInputNotifier - for every module index a input notifier must be given. The
	 * notifier is used for adding input sync notifications to the module with the returned
	 * moduleID
	 * @return unique module id
	 */
	uint32_t getNextModuleID(MFInputSyncNotifier* pInputNotifier);
	uint32_t getCurrentCycle(){
		if(mp_loopCtl){
			return mp_loopCtl->getCurrentCycle();
		}
		MFObject::printWarning("MFSyncObjectResources::getCurrentCycle - "
				"returning 0, no loop ctl set!");
		return 0;
	}
	void addSyncTask(MFModuleSyncTask* pSyncTask){
		mp_vecModuleSyncTasks->push_back(pSyncTask);
	}
	std::vector<MFModuleSyncTask*>* getVecSyncTasks(){return mp_vecModuleSyncTasks;};
};

#endif /* MFSYNCHRONISATIONCLASSES_MFSYNCOBJECTRESOURCES_H_ */
