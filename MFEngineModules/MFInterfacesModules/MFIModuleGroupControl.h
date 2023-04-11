/*
 * MFIModuleGroupProvider.h
 *
 *  Created on: 27.05.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFIMODULEGROUPCONTROL_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFIMODULEGROUPCONTROL_H_
#include <vector>
#include "../MFBaseModule.h"
#include "../../MFEngineStructs.h"
#include "../MFModuleGroup.h"
class MFIModuleGroupControl {
public:
	virtual ~MFIModuleGroupControl(){};

	virtual MFModuleGroup* getModuleGroup(uint32_t groupIndex){
		MFObject::printErr("MFIModuleGroupProvider::getModuleGroup - no impl.!");
		return nullptr;
	};

	virtual std::vector<MFModuleGroup*>* getGroups(){
		MFObject::printErr("MFIModuleGroupProvider::getGroups - not impl.!");
		return nullptr;
	};

	/**
	 * Adds a module group and returns a S_ModuleGroup which contains the index of the
	 * group, the name, the group of added MFBaseModules and the group of
	 * added sync objects.
	 * @param groupName - used to descibe the group
	 * @return the index of the added group. The index should be stored for further usage.
	 */
	virtual MFModuleGroup* addModuleGroup(std::string groupName){
		MFObject::printErr("MFIModuleGroupProvider::addModuleGroup - not impl.!");
		return nullptr;
	};

	/**
	 * Adds a module to the specified group. If group is not existent, this function
	 * will return false!
	 * The specified group index must be a returned value of addModuleGroup()!!!
	 * @param groupIndex
	 * @param pModule
	 * @return returns false if groupIndex is out of bounce. If this happens,
	 * the module group wasn't created by addModuleGroup()!
	 */
	virtual bool addModuleToGroup(uint32_t groupIndex,MFBaseModule *pModule){
		MFObject::printErr("MFIModuleGroupProvider::addModuleGroup - not impl.!");
		return false;
	}
};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFIMODULEGROUPCONTROL_H_ */
