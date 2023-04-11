/*
 * MFPhysiscsModuleSync.cpp
 *
 *  Created on: 08.07.2020
 *      Author: michl
 */

#include "MFPhysiscsModuleSync.h"

MFPhysiscsModuleSync::MFPhysiscsModuleSync(
		MFPhysicsModule* pModule,
		uint32_t sharedTaskIndex,
		uint32_t syncCycleCount) :
	MFModuleSyncTask(sharedTaskIndex,syncCycleCount){
	mp_module=pModule;
}

MFPhysiscsModuleSync::~MFPhysiscsModuleSync() {
}

void MFPhysiscsModuleSync::updateSyncData() {
	/*little bit shitty but it will do... maybe...
	 * if all buffers are returned we can clear the output and refill it*/
	if(mp_updateDataBuffers->getAvailableBufferCount() !=
		mp_updateDataBuffers->getMaxBufferCount()){//TODO do it better...
		return;
	}

	if((mp_module->getCurrentModuleCycle()-m_lastUpdateCycleStep) < m_syncCycle)
		return;//Its not time to update sync data.

	if(mp_updateDataBuffers==nullptr){
		MFObject::printWarning("MFPhysiscsModuleSync::updateSyncData - MFModuleSyncTask"
				"was not initlialized!");
	}
	m_lastUpdateCycleStep=mp_module->getCurrentModuleCycle();
	mp_vecOutputBuffers->clear();
	/* data stream datagramm:
	 * TODO describe the usage of all values!
	 * TODO subdivide the task with switch case index to provide hash compatiblity
	 * |uint32_t sharedTaskIndex |uint32_t update cycle step |uint16_t firstObjectIndex |
	 * uint16_t object count | object1 data (4x4 model mat | 3D Vec linVel | 3D vec rotVel | 3dVec grav)) | object2 data ...
	 * */
	MFStructurableData *pNext=mp_updateDataBuffers->takeData();
	int32_t remainingSize=m_maxBufferSize;
	uint16_t firstObjectIndex=0;
	uint16_t* objectCount=nullptr;
	uint8_t* pDst=(uint8_t*)pNext->getStructureData();

	if(pDst==nullptr){
		MFObject::printErr("MFPhysiscsModuleSync::updateSyncData - pDst==nullptr!");
	}

	memcpy(pDst,&m_sharedTaskIndex,sizeof(uint32_t));
	pDst+=sizeof(uint32_t);
	remainingSize-=sizeof(uint32_t);

	memcpy(pDst,&m_lastUpdateCycleStep,sizeof(uint32_t));
	pDst+=sizeof(uint32_t);

	memcpy(pDst,&firstObjectIndex,sizeof(uint16_t));
	pDst+=sizeof(uint16_t);

	objectCount=(uint16_t*)pDst;
	*objectCount=0;
	pDst+=sizeof(uint16_t);

	remainingSize-=sizeof(uint32_t);
	remainingSize-=sizeof(uint32_t);
	remainingSize-=sizeof(uint16_t);

	glm::vec3
		linVel,
		rotVel,
		grav;

//	std::string positions="cycle: "+std::to_string(m_lastUpdateCycleStep)+
//			"\npositions:\n";

	for(MFBaseModule::ObjectData* pMO:*mp_module->getVecModuleObjects()){
		MFPhysicModuleObject* pPMO=(MFPhysicModuleObject*)pMO->pModuleData;

		if(pPMO==nullptr){
			MFObject::printWarning("MFPhysiscsModuleSync::updateSyncData - "
					"nullptr within module objects!");
			continue;
		}
		linVel=pPMO->getLinearVelocity();
		rotVel=pPMO->getAngularVelocity();
		grav=pPMO->getGravity();

		remainingSize-=sizeof(glm::mat4x4);
		remainingSize-=sizeof(glm::vec3);
		remainingSize-=sizeof(glm::vec3);
		remainingSize-=sizeof(glm::vec3);
		if(remainingSize<=0){
			MFObject::printWarning("MFPhysiscsModuleSync::updateSyncData() - sync"
					" buffer overflow, no impl. till now!");
			break;//TODO take new pNext if possibru and add old pNext to output queue
		}

		/*position, orientation, basis*/
		pPMO->get4x4ModelMatrix((float*)pDst);
		pDst+=sizeof(glm::mat4x4);

//		positions+="x="+std::to_string((*pModelMat)[3][0])+" y="+
//				std::to_string((*pModelMat)[3][1])+" z="+
//				std::to_string((*pModelMat)[3][2])+"\n";


		memcpy(pDst,&linVel,sizeof(glm::vec3));
		pDst+=sizeof(glm::vec3);

		memcpy(pDst,&rotVel,sizeof(glm::vec3));
		pDst+=sizeof(glm::vec3);

		memcpy(pDst,&grav,sizeof(glm::vec3));
		pDst+=sizeof(glm::vec3);

		(*objectCount)++;
	}
//	MFObject::printInfo("MFPhysiscsModuleSync::updateSyncData - data\n"+positions);
	lockOutputQueue.lock();
	mp_vecOutputBuffers->push_back(pNext);
	lockOutputQueue.unlock();
	return;
}

bool MFPhysiscsModuleSync::updateModule(MFStructurableData* inputData) {
	/* data stream datagramm:
	 * TODO subdivide the task with switch case index to provide hash compatiblity
	 * |uint32_t sharedTaskIndex |uint32_t update cycle step |uint16_t firstObjectIndex |
	 * uint16_t object count | object1 data (4x4 model mat | 3D Vec linVel | 3D vec rotVel | 3dVec grav)) | object2 data ...
	 * */
	uint8_t *pSrc=(uint8_t *)inputData->getStructureData();
	if(pSrc==nullptr){
		MFObject::printErr("MFPhysiscsModuleSync::updateModule - input==nullptr!");
		return false;
	}
	pSrc+=sizeof(uint32_t);//skip the task index
	/*cycle of engine loop (not used yet)*/
	uint32_t updateCycleStep=*((uint32_t*)pSrc);
	if(m_latestUpdatedStep >= updateCycleStep){
		MFObject::printWarning("MFPhysiscsModuleSync::updateModule - update data "
				"is outdated! (More recent package received!)");
		return true;//happens when a package was lost.
	}
	pSrc+=sizeof(uint32_t);

	uint16_t firstObjectIndex=*((uint16_t*)pSrc);
	pSrc+=sizeof(uint16_t);

	uint16_t objectCount=*((uint16_t*)pSrc);
	pSrc+=sizeof(uint16_t);

	glm::vec3
		linVel,
		rotVel,
		grav;
//	std::string positions="cycle: "+std::to_string(updateCycleStep)+"\npositions:\n";
	for(uint32_t i=firstObjectIndex;i<firstObjectIndex+objectCount;i++){
		MFPhysicModuleObject *pPMO=(MFPhysicModuleObject*)mp_module->getModuleObject(i);
		if(pPMO==nullptr){
			//TODO add dummy and send missing module object notation.
//			MFObject::printWarning("MFPhysiscsModuleSync::updateModule - "
//					"nullptr within module objects!");
			continue;
		}
		/*position, orientation, basis*/
		glm::mat4x4* pModelMat=(glm::mat4x4*)pSrc;
		pPMO->set4x4ModelMatrix((float*)pModelMat);
		pSrc+=sizeof(glm::mat4x4);
//		positions+="x="+std::to_string((*pModelMat)[3][0])+ " y="+
//				std::to_string((*pModelMat)[3][1])+" z="+
//				std::to_string((*pModelMat)[3][2])+"\n";

		memcpy(&linVel,pSrc,sizeof(glm::vec3));
		pSrc+=sizeof(glm::vec3);

		memcpy(&rotVel,pSrc,sizeof(glm::vec3));
		pSrc+=sizeof(glm::vec3);

		memcpy(&grav,pSrc,sizeof(glm::vec3));
		pSrc+=sizeof(glm::vec3);

		pPMO->setLinearVelocity(linVel);
		pPMO->setAngularVelocity(rotVel);
		pPMO->setGravity(grav);
	}
//	MFObject::printInfo("MFPhysiscsModuleSync::updateModule - data\n"+positions);
	return true;
}
