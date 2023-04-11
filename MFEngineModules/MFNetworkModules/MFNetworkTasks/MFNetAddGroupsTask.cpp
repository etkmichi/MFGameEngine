/*
 * MFNetAddGroupTask.cpp
 *
 *  Created on: 28.05.2020
 *      Author: michl
 */

#include "MFNetAddGroupsTask.h"


MFNetAddGroupsTask::MFNetAddGroupsTask(MFIModuleGroupControl* pGroupSetup){
	mp_groupProvider=pGroupSetup;
  m_surpressSetupWarning=true;
	setPackageDataSize(5000);
}

MFStructurableData* MFNetAddGroupsTask::createNewEmptyData(){
	MFNetAddGroupsTask *pDat=new MFNetAddGroupsTask(mp_groupProvider);
	pDat->setPackageDataSize(5000);
	return pDat;
}

bool MFNetAddGroupsTask::processData(MFStructurableData* pData){
	uint8_t* src=static_cast<uint8_t*>(pData->getVecData()->at(2)->getData());
	MFObject::printInfo("MFNetAddGroupsTask::processData - adding groups");
//	pData->printMetaData();
	uint32_t groupCount=0;
	memcpy(&groupCount,src,sizeof(uint32_t));
	src+=sizeof(uint32_t);

	uint32_t stringSize=0;
	for(uint32_t i=0;i<groupCount;i++){
		memcpy(&stringSize,src,sizeof(uint32_t));
		src+=sizeof(uint32_t);
		char* stringStart=(char*)(src);
		std::string groupName(stringStart,stringSize);
		if(i!=0)//0 is default group, which should be already initialized!
			mp_groupProvider->addModuleGroup(groupName);
		else{
			if(groupName!=mp_groupProvider->getGroups()->at(0)->groupName){
				MFObject::printInfo("MFNetAddGroupsTask::processData - missmatch in "
						"default group! received group name:" +groupName);
			}
		}
		src+=stringSize;
	}
	return true;
}
