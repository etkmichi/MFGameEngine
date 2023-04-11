/*
 * MFNetModuleObjectSetup.cpp
 *
 *  Created on: 03.07.2020
 *      Author: michl
 */

#include "MFNetModuleObjectSetup.h"
#include <MFData/MFDataSerializer.h>

MFNetModuleObjectSetup::MFNetModuleObjectSetup(MFNetworkResources* pRes) {
	mp_netRes=pRes;
  m_surpressSetupWarning=true;
	setPackageDataSize(50000);
}

MFNetModuleObjectSetup::~MFNetModuleObjectSetup() {
}

MFStructurableData* MFNetModuleObjectSetup::createNewEmptyData(){
	MFNetInputPackage *pData=new MFNetModuleObjectSetup(mp_netRes);
	pData->setPackageDataSize(50000);
	return pData;
}

bool MFNetModuleObjectSetup::processData(MFStructurableData* pData){
	//| Count of modules | Module ID 1 | Count of modO's of ModID1 | sizeof
	//data1 | data1 | sizeof data2 | data2 |... | Module ID 2 | Count ... ModID2 | sizeof ModO2
	//data | ....
	MFObject::printInfo("MFNetModuleObjectSetup::processData - "
			"setting up module objects");
	uint32_t moduleCount=0;
	uint32_t currentModuleID=0xFFFFFFFF;
	uint32_t crtModObjtCount=0;
	uint8_t* pSrc=(uint8_t*)pData->getVecData()->at(2)->getData();

	memcpy(&moduleCount,pSrc,sizeof(uint32_t));
	pSrc+=sizeof(uint32_t);
	MFIManagerControl* pManCtl=mp_netRes->mp_moduleProvider;
	if(pManCtl->getVecModules()->size()!=moduleCount){
		MFObject::printWarning("MFNetModuleObjectSetup::processData - "
				"module count doesn't match!");
	}

	for(uint32_t i=0;i<moduleCount;i++){
		bool skip=false;
		memcpy(&currentModuleID,pSrc,sizeof(uint32_t));
		pSrc+=sizeof(uint32_t);

		memcpy(&crtModObjtCount,pSrc,sizeof(uint32_t));
		pSrc+=sizeof(uint32_t);
		if(pManCtl->getVecModules()->size()<=i){
			MFObject::printWarning("MFNetModuleObjectSetup::processData - "
					"not module at index "+std::to_string(i));
			continue;
		}
		MFBaseModule* pMod=pManCtl->getModule(i);
		if(pMod==nullptr)skip=true;
		MFObject::printInfo("MFNetModuleObjectSetup::processData - setting up module "
				"objects:\n"
				"module number: "+std::to_string(i)+"\n"+
				"module obj count: "+std::to_string(crtModObjtCount)+
				"\nskipping module objects: "+std::to_string(skip));
		for(uint32_t j=0;j<crtModObjtCount;j++){
			uint32_t crtModOSetupSize=0;
			memcpy(&crtModOSetupSize,pSrc,sizeof(uint32_t));
			pSrc+=sizeof(uint32_t);
			if(skip){
				MFObject::printWarning("MFNetModuleObjectSetup::processData - no module "
						"for currrent module!\n"
						"moduleID: "+std::to_string(currentModuleID)+"!\n"
						"object index: "+std::to_string(j));
				pSrc+=crtModOSetupSize;
				continue;
			}
			MFModuleObject *pModO=pMod->getModuleObject(j);
			if(pModO==nullptr){
				MFObject::printWarning("MFNetModuleObjectSetup::processData no "
						"module object found for index: "+
						std::to_string(j));
				pSrc+=crtModOSetupSize;
				continue;
			}
			if(!MFDataSerializer::deserialize(pSrc, pModO)){
				MFObject::printWarning("MFNetModuleObjectSetup::processData - "
						"deserualization"
						"failed for object with index index: "+
						std::to_string(j));
			}
			pSrc+=crtModOSetupSize;
			pModO->updateData();//TODO it was MFModuleObject::updateObject() (removed)!
		}

	}
	return true;
}
