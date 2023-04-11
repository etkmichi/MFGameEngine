/*
 * MMFGUIIOModule.h
 *
 *  Created on: 06.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFGUIIOMODULE_MFGUIIOMODULE_H_
#define MFENGINEMODULES_MFGUIIOMODULE_MFGUIIOMODULE_H_

#include "MFGUIModuleObject.h"
#include "../MFBaseModule.h"
#include "../MFInterfacesModules/MFIModuleCreator.h"
#include <MFIGUIManager.h>
#include <MFMenuClasses/MFIMenu.h>
#include <MFMenuClasses/MFIMenuEntry.h>

class MFGUIIOModule:
		public MFBaseModule,
		public MFIModuleCreator{
private:
	MFIGUIManager
		*mp_guiManager;
	std::vector<MFDataObject*>
		*mp_vecDataObjects;
	std::vector<glm::mat4x4*>
		*mp_vecMatrices;


protected:
	bool initModuleObject(
			MFSyncObject* syncObject,
			MFModuleObject* pObject);

	virtual MFBaseModule* createModule(){
	  return new MFGUIIOModule(mp_syncRes);
	};

public:
	MFModuleObject* createModuleObject();
	void updateResources(MFModuleResources* pRes);

public:
	MFGUIIOModule(MFSyncObjectResources* pSyncRes);
	virtual ~MFGUIIOModule();
	bool initModule();

	bool syncOutputData();

	MFIGUIManager* getGuiManager(){return mp_guiManager;};
	bool executeModule();
};

#endif /* MFENGINEMODULES_MFGUIIOMODULE_MFGUIIOMODULE_H_ */
