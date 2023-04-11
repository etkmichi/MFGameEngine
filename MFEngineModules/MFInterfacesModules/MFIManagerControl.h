/*
 * MFIModuleProvider.h
 *
 *  Created on: 09.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFIMANAGERCONTROL_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFIMANAGERCONTROL_H_
#include <MFObjects/MFObject.h>
#include <MFData/MFDataObject.h>
#include <MFData/MFStructurableData.h>

#include "../MFBaseModule.h"
#include "../MFModuleObjectManager.h"
#include "../../MFSceneActions/MFActionTaskManager.h"

class MFIManagerControl {
public:
	virtual ~MFIManagerControl(){};
	virtual bool initFromSetup(MFStructurableData* pData=nullptr){
		MFObject::printErr("MFIManagerControl::initFromSetup - no impl.!");return false;};
	virtual bool initFromData(MFDataObject* pData){
		MFObject::printErr("MFIManagerControl::initFromData - no impl.!");return false;};
	virtual MFBaseModule* getModule(uint32_t moduleIndex){
		MFObject::printErr("MFIModuleProvider::getVecModules - no impl.!");return nullptr;};
	virtual std::vector<MFBaseModule*>* getVecModules(){
		MFObject::printErr("MFIModuleProvider::getVecModules - no impl.!");return nullptr;};

	//TODO thats dirty -> maybe rename this class into MFIModuleManagerOperator
	virtual MFModuleObjectManager* getObjectManager(){return nullptr;};
	virtual void setObjectManager(MFModuleObjectManager* pObjectMgr){};

	virtual MFActionTaskManager* getActionManager(){return nullptr;};
	virtual void setActionManager(MFActionTaskManager* pActMgr){};

	virtual bool initModules(){
	  MFObject::printErr("MFIManagerControl::initModules - not impl.!");
	  return false;
	}

	//TODO add functions for module manager virtual

};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFIMANAGERCONTROL_H_ */
