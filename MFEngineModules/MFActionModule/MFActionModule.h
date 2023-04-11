/*
 * MFActionModule.h
 *
 *  Created on: 29.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFACTIONMODULE_MFACTIONMODULE_H_
#define MFENGINEMODULES_MFACTIONMODULE_MFACTIONMODULE_H_

#include "../MFBaseModule.h"

class MFActionModule: public MFBaseModule {

protected:
	/*functions of MFBaseModule*/
	/**
	 * This function will add the sync object to the modules underlying system (for example to the underlying physics engine or the render api...)
	 * It will be called within the execution of MFBaseModule::addSyncObject(...)
	 * @param syncObject
	 * @return
	 */
	virtual MFModuleObject* initNewModuleObject(MFSyncObject* syncObject){return nullptr;};
	virtual bool initModuleObject(MFSyncObject* syncObject,MFModuleObject* pObject){
		return false;
	};

public:
	/*functions of MFBaseModule*/
	virtual MFModuleObject* createModuleObject(){
		MFObject::printWarning("MFBaseModule::createModuleObject() - "
				"not implemented, returning nullptr!");
		return nullptr;
	};

	virtual void updateResources(MFModuleResources* pRes){
		MFObject::printWarning("MFBaseModule::updateResources - not impl.!");
	}

	/**
	 * Executes the actions.
	 * @return
	 */
	virtual bool executeModule();


public:
	MFActionModule(MFSyncObjectResources *pSyncRes);
	virtual ~MFActionModule();
  void setEngineResources(MFModuleResources *res){mp_res=res;}

};

#endif /* MFENGINEMODULES_MFACTIONMODULE_MFACTIONMODULE_H_ */
