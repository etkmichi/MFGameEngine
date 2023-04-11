/*
 * MFIModuleCreator.h
 *
 *  Created on: 07.04.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFIMODULECREATOR_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFIMODULECREATOR_H_
#include "../MFBaseModule.h"
#include <MFBasicDefines.h>
class MFIModuleCreator {
protected:/*virtual functions MFIModuleCreator*/
	/**
	 * Creates a module
	 * @return
	 * TODO return module which shows the setup by gui.
	 * 		- if interface is not implemented, module setup can be viewed.
	 */
	virtual MFBaseModule* createModule(){return nullptr;};
private:
	uint32_t
		m_moduleTypeID=0;
	std::string
	m_name="";
public:
	MFIModuleCreator(const std::string &name=""){
	  m_name=name;
	};
	virtual ~MFIModuleCreator(){};

	/**
	 * Creates a module
	 * @return
	 * TODO return module which shows the setup by gui.
	 * 		- if interface is not implemented, module setup can be viewed.
	 *///TODO create setter setVecModContainer to provide container access without parameter! see
	//MFModuleManager::addModule(...) -> not container available -> set during initModCreators
	MFBaseModule* createNewModule(MFModuleResources* pRes,std::vector<MFBaseModule*> *pVecModContainer=nullptr){
		MFBaseModule* pModule=createModule();
		if(pModule==nullptr){
			P_ERR("failed to create module! Creator name: "+m_name);
			return nullptr;
		}
		if(pVecModContainer!=nullptr)
		  pVecModContainer->push_back(pModule);
		pModule->setEngineResources(pRes);
		if(m_moduleTypeID!=0){
			pModule->setModuleTypeID(m_moduleTypeID);
		}else{
			MFObject::printWarning("MFIModuleCreator::createNewModule - "
					"module type id not set!");
		}
    pModule->setStructureName(m_name);
		return pModule;
	};

	void setTypeID(uint32_t typeID){m_moduleTypeID=typeID;};
	uint32_t getTypeID(){return m_moduleTypeID;};
	std::string getName(){return m_name;};
};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFIMODULECREATOR_H_ */
