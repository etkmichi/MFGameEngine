/*
 * MFWorldModule.h
 *
 *  Created on: 12.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFWORLDMODULE_MFWORLDMODULE_H_
#define MFENGINEMODULES_MFWORLDMODULE_MFWORLDMODULE_H_
#include "../../MFSynchronisationClasses/MFSyncObject.h"
#include "MFWorldModuleObject.h"
#include "MFWorldClasses/MFTileCollection.h"
#include "../MFModuleObjectManager.h"
#include "../MFInterfacesModules/MFIModuleCreator.h"
#include "../MFInterfacesModules/MFIModuleGroupControl.h"

class MFWorldModule:
		public MFBaseModule,
		public MFIModuleCreator {
private:
	MFModuleObjectManager
		*mp_objectMgr;
	MFIModuleGroupControl
		*mp_groupProvider;

protected:
	virtual bool initModuleObject(
			MFSyncObject* pSO,
			MFModuleObject* pObject);

	virtual bool initCollection(
			MFWorldModuleObject* pWMO);

public:
	MFWorldModule(
			MFModuleObjectManager* pOM,
			MFIModuleGroupControl* pModuleGroupProvider,
			MFSyncObjectResources* pSyncRes);

	virtual ~MFWorldModule();

public:
	virtual MFBaseModule* createModule(){
		return new MFWorldModule(mp_objectMgr,mp_groupProvider,mp_syncRes);};

	void updateResources(MFModuleResources* pRes);

	MFModuleObject* createModuleObject(){
		MFWorldModuleObject* pModO=new MFWorldModuleObject();
		return pModO;
	};
	virtual bool initModule();

	virtual bool executeModule();

	virtual bool syncOutputData();

};

#endif /* MFENGINEMODULES_MFWORLDMODULE_MFWORLDMODULE_H_ */
