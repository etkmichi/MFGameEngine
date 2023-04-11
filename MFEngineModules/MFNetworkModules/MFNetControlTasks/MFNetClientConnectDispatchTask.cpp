/*
 * MFNetClientConnectDispatchTask.cpp
 *
 *  Created on: 02.04.2020
 *      Author: michl
 */
#include "MFNetClientConnectDispatchTask.h"

#include <MFNetClientClasses/MFNetClient.h>
#include <MFDataDeleterClasses/MFDataDeletion.h>

#include <MFData/MFDataSerializer.h>

#include "../MFNetworkTasks/MFModuleTasks/MFNetAddModulesTask.h"
#include "../MFNetworkTasks/MFNetModuleObjectSetup.h"
#include "../../MFModuleGroup.h"

uint32_t MFNetClientConnectDispatchTask::sm_moduleByteSize=9;
uint32_t MFNetClientConnectDispatchTask::sm_maxModuleCount=50;

MFNetClientConnectDispatchTask::MFNetClientConnectDispatchTask(
		MFNetworkResources* pRes){
	mp_netServRes=pRes;
	mp_sbAddModules=nullptr;
	mp_sbAddGroups=nullptr;
	mp_sbInitModules=nullptr;
	mp_sbObjectSetup=nullptr;
	mp_sbStartLoop=nullptr;
	mp_sbModObjSetup=nullptr;

	mp_addModulesData=new MFNetAddModulesTask(nullptr);
	mp_addGroupsData=new MFNetAddGroupsTask(nullptr);
	mp_initModulesData=new MFNetInitModuleGroupTask(mp_netServRes->mp_moduleProvider,mp_netServRes);
	mp_objectSetupData=new MFNetObjectSetupTask(mp_netServRes);
	mp_moduleObjectSetupData=new MFNetModuleObjectSetup(mp_netServRes);
	mp_startLoopData=new MFNetStartLoopTask(mp_netServRes);
}

MFNetClientConnectDispatchTask::~MFNetClientConnectDispatchTask() {
}

/**
 * This function sends all data which are needed by a client which wants to connect.
 * Sends data:
 * 	1. module manager's module setup
 * 	-> server: uses the MFNetAddModulesData
 * 	-> client: loads the manager setup and adds modules
 * 	2. module's setup
 * 	-> server: sends the module's setup
 * 	-> client: loads the setup and inits the modules
 * 	3. object manager's object setup (the added objects)
 * 	-> server: sends the object setup
 * 	-> client: adds the objects
 * 	4. Start synchronizing
 * 	-> server: sends the setup of the added object's
 * 	-> client: synchronizes it's objects with the received data
 * 	5. Start periodic, low traffic, synchronization
 * 	-> server: sends some checkvalues for checking if simulation is synchro
 * 	-> client: checks if checkvalues match with client's simulation, if not asks for correction
 * @param pNE
 * @return
 */
bool MFNetClientConnectDispatchTask::dispatchEvent(S_MF_NetworkEvent* pNE){
	printInfo("MFNetClientConnectDispatchTask::dispatchEvent - sending engine setup" );
	/* write setup to data package */
	//1. MFNetAddModulesTask
	//2. loop over all modules for MFNetInitModule
	//3. send all informations for adding sync objects
	//TODO create mp_moduleManagerInitData with MFNetAddModulesTask
	//TODO currently no task index for mp_moduleManagerInitData available!
	if(!sendManagerSetup(pNE))return false;

	if(!sendGroupSetup(pNE))return false;

	if(!sendModuleGroupSetup(pNE))return false;

	if(!sendObjectSetup(pNE))return false;

	if(!sendModuleObjectSetup(pNE))return false;

	if(!sendStartLoop(pNE))return false;

	/*add client as module object to network module*/
//	if(mp_netServRes->mp_clientCtl!=nullptr)
//		mp_netServRes->mp_clientCtl->addServerClient(
//				MFNetworkLibraryHelper::getClient(pNE));
	return true;
}

bool MFNetClientConnectDispatchTask::sendManagerSetup(S_MF_NetworkEvent* pNE){
  printInfo("MFNetClientConnectDispatchTask::sendManagerSetup - sending "
      "manager setup");
	uint32_t pkgDataSize=MFNetAddModulesTask::sm_maxModuleCount*sm_moduleByteSize;
	mp_sbAddModules=new MFDataObjectBuffer(pkgDataSize+MFNetInputPackage::getHeaderSize());
	if(!mp_sbAddModules->createBuffer()){
		printErr("MFNetClientConnectDispatchTask::sendManagerSetup - "
				"Failed to create shared buffer!");
		return false;
	}
	mp_addModulesData->setPackageDataSize(pkgDataSize);
	mp_addModulesData->useSharedBuffer(mp_sbAddModules);
	mp_addModulesData->initData();
	uint32_t
		taskIndex=mp_netServRes->mp_netTaskManager->getTaskIndices().add_modules_task;
	mp_addModulesData->getTypeIDObject()->writeData(&taskIndex);
	mp_addModulesData->getPackageCounter()->writeData(&mp_netServRes->m_packageCounter);

	/*this will copy the modules setup to the data structure*/
	mp_addModulesData->dispatchPackageData(mp_netServRes->mp_moduleSetup);

	return sendData(
			(uint8_t*)mp_sbAddModules->getBuffer(),
			mp_sbAddModules->getBufferByteSize(),
			pNE);
}

bool MFNetClientConnectDispatchTask::sendModuleGroupSetup(S_MF_NetworkEvent* pNE){
//	MFIManagerControl* pMP=mp_netServRes->mp_moduleProvider;
	/*TODO remove commented after testing!*/
	/*send group setup for each module*/
//	uint32_t moduleGroupSetupCount=0;
//	uint32_t pkgDataSize=0;
//	pkgDataSize+=sizeof(uint32_t);//moduleGroupSetupCount
//	for(MFBaseModule* pM:*pMP->getVecModules()){
//		uint32_t groupCount=pM->getVecGroupIDs()->size();//count of groups module belongs too
//		uint32_t groupIdsSize=groupCount*sizeof(uint32_t);
//		pkgDataSize+=sizeof(uint32_t);//groupCount
//		pkgDataSize+=groupIdsSize;//groupID's
//		moduleGroupSetupCount++;
//	}

//	uint32_t sharedBufferSize=pkgDataSize+MFNetInputPackage::getHeaderSize();
//	mp_sbInitModules=new MFDataObjectBuffer(sharedBufferSize);
//	if(!mp_sbInitModules->createBuffer()){
//		printErr("MFNetClientConnectDispatchTask::sendGroupSetup - failed"
//				"to create shared buffer!");
//		return false;
//	}
//	mp_initModulesData->setPackageDataSize(pkgDataSize);
//	mp_initModulesData->useSharedBuffer(mp_sbInitModules);
//	mp_initModulesData->initData();

//	uint16_t taskIndex=
//			mp_netServRes->mp_netTaskManager->getTaskIndices().module_init_task;
//	mp_initModulesData->getTypeID()->writeData(&taskIndex);
//	mp_initModulesData->getPackageCounter()->writeData(&mp_netServRes->m_packageCounter);
//
//	uint8_t* dst=(uint8_t*)mp_initModulesData->getStructureData()+MFNetInputPackage::getHeaderSize();
//
//	memcpy(dst,&moduleGroupSetupCount,sizeof(uint32_t));
//	dst+=sizeof(uint32_t);
//
//	for(MFBaseModule* pM:*pMP->getVecModules()){
//		uint32_t groupCount=pM->getVecGroupIDs()->size();
//		memcpy(dst,&groupCount,sizeof(uint32_t));//count of groups
//		dst+=sizeof(uint32_t);
//
//		for(uint32_t i=0;i<groupCount;i++){
//			uint32_t groupID=pM->getVecGroupIDs()->at(i);
//			memcpy(dst,&groupID,sizeof(uint32_t));//count of groups
//			dst+=sizeof(uint32_t);
//		}
//	}
  if(!mp_initModulesData->initOutputPackage()){
    MFObject::printErr("MFNetClientConnectDispatchTask::sendModuleGroupSetup - "
        "failed to init output package");
    return false;
  }
  if(!mp_initModulesData->prepareOutputPackage()){
    MFObject::printErr("MFNetClientConnectDispatchTask::sendModuleGroupSetup - "
        "failed to init output package");
    return false;
  }

//	bool ret=MFNetworkLibraryHelper::getClient(pNE)->sendData(
//			(uint8_t*)mp_sbInitModules->getBuffer(),
//			mp_sbInitModules->getBufferByteSize(),
//			0,
//			true,
//			&mp_netServRes->m_packageCounter);
	if(!mp_initModulesData->sendPackage(MFNetworkLibraryHelper::getClient(pNE))){
		printErr("MFNetClientConnectDispatchTask::sendManagerSetup - "
				"sending module setup failed!");
		return false;
	}
	return true;
}

bool MFNetClientConnectDispatchTask::sendGroupSetup(S_MF_NetworkEvent* pNE){
  printInfo("MFNetClientConnectDispatchTask::sendGroupSetup - preparing group setup "
      "data for sending");
	uint32_t groupCount=mp_netServRes->mp_groupProvider->getGroups()->size();
	uint32_t size=0;
	size+=sizeof(uint32_t);//group count
	for(MFModuleGroup* pMG:*mp_netServRes->mp_groupProvider->getGroups()){
		size+=sizeof(uint32_t);//size of the groupName size
		size+=pMG->groupName.size();//size of the groupName
	}

	uint32_t sharedBufferSize=size+MFNetInputPackage::getHeaderSize();
	mp_sbAddGroups=new MFDataObjectBuffer(sharedBufferSize);
	if(!mp_sbAddGroups->createBuffer()){
		printErr("MFNetClientConnectDispatchTask::sendGroupSetup - failed"
				"to create shared buffer!");
		return false;
	}
	mp_addGroupsData->setPackageDataSize(size);
	mp_addGroupsData->useSharedBuffer(mp_sbAddGroups);
	mp_addGroupsData->initData();
	uint32_t taskIndex=mp_netServRes->mp_netTaskManager->getTaskIndices().add_groups_task;
	mp_addGroupsData->getTypeIDObject()->writeData(&taskIndex);
	mp_addGroupsData->getPackageCounter()->writeData(&mp_netServRes->m_packageCounter);

	uint8_t* dst=static_cast<uint8_t*>(mp_addGroupsData->getStructureData());
	dst+=MFNetInputPackage::getHeaderSize();//add header size as offset

	memcpy(dst,&groupCount,sizeof(uint32_t));//copy count of groups to the package
	dst+=sizeof(uint32_t);

	uint32_t stringLength=0;
	for(MFModuleGroup* pMG:*mp_netServRes->mp_groupProvider->getGroups()){
		//TODO check size vs length! size calculation was done with .size() of string!
		stringLength=pMG->groupName.length();//copy size of group name to the package
		memcpy(dst,&stringLength,sizeof(uint32_t));
		dst+=sizeof(uint32_t);

		memcpy(dst,pMG->groupName.data(),stringLength);//copy groupName to package
		dst+=stringLength;
	}

	bool ret=MFNetworkLibraryHelper::getClient(pNE)->sendData(
			(uint8_t*)mp_sbAddGroups->getBuffer(),
			mp_sbAddGroups->getBufferByteSize(),
			0,
			true,
			&mp_netServRes->m_packageCounter);
	if(!ret){
		printErr("MFNetClientConnectDispatchTask::sendManagerSetup - "
				"sending module setup failed!");
	}

	return ret;
}

bool MFNetClientConnectDispatchTask::sendObjectSetup(S_MF_NetworkEvent* pNE){
	//MFNetObjectSetupTask - sync added objects with geometries and groups
	//(no coords mass or sth else)
	//TODO enqueue add object calls during sendObjectSetup call -> objects which are
	//TODO mach ma halb lang
	//
	//added during this call are not added to the object setup
	if(mp_netServRes->mp_objectManager==nullptr){
		printErr("MFNetClientConnectDispatchTask::sendObjectSetup - "
				"mp_netServRes->mp_objectManager==nullptr");
		return false;
	}
	std::vector<MFSyncObject*> *pVO=mp_netServRes->mp_objectManager->getSyncObjects();
	/*	p1 = parameter setup of first sync object
	 *	g1 = group setup of first sync object */
	/* package setup: objectCount (uint32_t) | size p1 | p1 |... | size g1 | g1 ...*/

	/*calculate size of geometry id and geometry parameters*/
	uint32_t objectParameterSize=0;//object count
	/*calculate size of group setups*/
	uint32_t objGrpSize=0;
	for(MFSyncObject *pSO:*pVO){//TODO what if one pSO==nullptr?
		MFStructurableDataSetup* pParamSetup=pSO->getGeometry()->getGeometrySetup();
		objectParameterSize+=sizeof(uint32_t);//size of size of param setup(min 0bytes)
		objectParameterSize+=pParamSetup->getStructureSize();//parameter setup

		objGrpSize+=sizeof(uint32_t);
		objGrpSize+=sizeof(uint32_t)*pSO->getVecModuleGroups()->size();
	}

	/*create package and fill it*/
	uint32_t size=
			sizeof(uint32_t)+//size of object count
			objectParameterSize+//size of object parameters
			objGrpSize;//size of object's group setups

	uint32_t sharedBufferSize=size+MFNetInputPackage::getHeaderSize();
	mp_sbObjectSetup=new MFDataObjectBuffer(sharedBufferSize);
	if(!mp_sbObjectSetup->createBuffer()){
		printErr("MFNetClientConnectDispatchTask::sendObjectSetup - failed"
				"to create shared buffer!");
		return false;
	}
	mp_objectSetupData->setPackageDataSize(size);
	mp_objectSetupData->useSharedBuffer(mp_sbObjectSetup);
	printInfo("MFNetClientConnectDispatchTask::sendObjectSetup - "
			"size of object setup: "+std::to_string(size));
	mp_objectSetupData->initData();
	uint16_t taskIndex=mp_netServRes->mp_netTaskManager->getTaskIndices().object_setup_task;
	mp_objectSetupData->getTypeIDObject()->writeData(&taskIndex);
	mp_objectSetupData->getPackageCounter()->writeData(&mp_netServRes->m_packageCounter);

	uint8_t* dst=static_cast<uint8_t*>(mp_objectSetupData->getStructureData());
	dst+=MFNetInputPackage::getHeaderSize();

	/*cpy count of objects*/
	uint32_t objectCount=pVO->size();
	memcpy(dst,&objectCount,sizeof(uint32_t));
	dst+=sizeof(uint32_t);

	/*cpy geometry*/
	for(MFSyncObject *pSO:*pVO){//TODO what if one pSO==nullptr?
		uint32_t serializedSize=0;
		if(!pSO->getGeometry()->serializeGeometrySetup(serializedSize,dst)){
			printErr("MFNetClientConnectDispatchTask::sendObjectSetup - "
					"failed to copy object init data!");
		}
		dst+=serializedSize;
	}

	/*cpy object's group setups*/
	std::string info="serialized group setup:\n";
	for(MFSyncObject *pSO:*pVO){//TODO what if one pSO==nullptr?
		uint32_t gsSize=pSO->getVecModuleGroups()->size()*sizeof(uint32_t);
		info+="group count: "+std::to_string(gsSize/4)+" | GroupNames={";
		for(uint32_t pGroup:*pSO->getVecModuleGroups())
		{
		  info+=mp_netServRes->mp_groupProvider->getModuleGroup(pGroup)->groupName+="; ";
		}
		info+="}\n";
		memcpy(dst,&gsSize,sizeof(uint32_t));
		dst+=sizeof(uint32_t);
		memcpy(dst,pSO->getVecModuleGroups()->data(),gsSize);
		dst+=gsSize;
	}

  printInfo("MFNetClientConnectDispatchTask::sendObjectSetup - "+info);

	/*send collected data*/
	return sendData(
			(uint8_t*)mp_sbObjectSetup->getBuffer(),
			mp_sbObjectSetup->getBufferByteSize(),
			pNE);
}

bool MFNetClientConnectDispatchTask::sendModuleObjectSetup(S_MF_NetworkEvent* pNE){
	//MFNetModuleObjectSetup sync module specific data like color mass coords...
	/*this vector will be filled during size calculation*/
	std::vector<MFDataObject*> *pVecSerialized=new std::vector<MFDataObject*>();

	/*calculate and collect object module setup size*/
	uint32_t ObjModSize=0;
	ObjModSize+=sizeof(uint32_t);//Count of modules

	//| Count of modules | Module ID 1 | Count of modO's of ModID1 | sizeof
	//data1 | data1 | sizeof data2 | data2 |... | Module ID 2 | Count ... ModID2 | sizeof ModO2
	//data | ....
	for(MFBaseModule *pBM : *mp_netServRes->mp_moduleProvider->getVecModules()){
		/*count of modO's | size of setup | setup 1 ... | count of sc. modO's | ...*/
		ObjModSize+=sizeof(uint32_t);//Module ID
		ObjModSize+=sizeof(uint32_t);//Count of modO's
		/*collect serialized data setups*/
		for(MFBaseModule::ObjectData* pMO:*pBM->getVecModuleObjects()){
			//should be the same for all pMO, just to be sure add it every time.
			ObjModSize+=sizeof(uint32_t);
			pMO->pModuleData->updateData();
			MFDataObject* pSerialized=MFDataSerializer::createSerializedData(
					pMO->pModuleData);
			if(pSerialized==nullptr){
				printWarning("MFNetClientConnectDispatchTask::sendModuleObjectSetup - "
						"serialization of module object failed!");
			}else{
				ObjModSize+=pSerialized->getDataSize();
			}
			pVecSerialized->push_back(pSerialized);
		}
	}

	uint32_t sharedBufferSize=ObjModSize+MFNetInputPackage::getHeaderSize();
	mp_sbModObjSetup=new MFDataObjectBuffer(sharedBufferSize);
	if(!mp_sbModObjSetup->createBuffer()){
		printErr("MFNetClientConnectDispatchTask::sendModuleObjectSetup - failed"
				"to create shared buffer!");
		return false;
	}
	mp_moduleObjectSetupData->setPackageDataSize(ObjModSize);
	mp_moduleObjectSetupData->useSharedBuffer(mp_sbModObjSetup);
	printInfo("MFNetClientConnectDispatchTask::sendModuleObjectSetup - "
			"size of object setup: "+std::to_string(ObjModSize));
	mp_moduleObjectSetupData->initData();
	uint16_t taskIndex=mp_netServRes->mp_netTaskManager->getTaskIndices().module_object_setup;
	mp_moduleObjectSetupData->getTypeIDObject()->writeData(&taskIndex);
	mp_moduleObjectSetupData->getPackageCounter()->writeData(&mp_netServRes->m_packageCounter);

	//write: Count of modules | Module ID 1 | Count of modO's of ModID1 | sizeof
	//data1 | data1 | sizeof data2 | data2 |... | Module ID 2 | Count ... ModID2 | sizeof ModO2
	//data | ....
	uint32_t
		modulesCount=mp_netServRes->mp_moduleProvider->getVecModules()->size(),
		crtModID=0,
		crtModOCount=0,
		crtDataSize=0;
	uint8_t* dst=static_cast<uint8_t*>(mp_moduleObjectSetupData->getStructureData());
	dst+=MFNetInputPackage::getHeaderSize();

	memcpy(dst,&modulesCount,sizeof(uint32_t));
	dst+=sizeof(uint32_t);
	uint32_t serializedCounter=0;
	for(MFBaseModule *pBM:*mp_netServRes->mp_moduleProvider->getVecModules()){
		crtModID=pBM->getModuleID();
		memcpy(dst,&crtModID,sizeof(uint32_t));
		dst+=sizeof(uint32_t);

		crtModOCount=pBM->getVecModuleObjects()->size();
		memcpy(dst,&crtModOCount,sizeof(uint32_t));
		dst+=sizeof(uint32_t);

		/*collect serialized data setups*/
		for(uint32_t i=serializedCounter;i<serializedCounter+crtModOCount;i++){
			MFDataObject* pSerialized=pVecSerialized->at(i);
			if(pSerialized==nullptr){
				crtDataSize=0;
			}else{
				crtDataSize=pSerialized->getDataSize();
			}
			printInfo("MFNetClientConnectDispatchTask::sendModuleObjectSetup - "
					"adding serialized data with size: "+std::to_string(crtDataSize));
			memcpy(dst,&crtDataSize,sizeof(uint32_t));
			dst+=sizeof(uint32_t);

			memcpy(dst,pSerialized->getData(),crtDataSize);
			dst+=crtDataSize;
		}
		serializedCounter+=crtModOCount;
	}

	printInfo("MFNetClientConnectDispatchTask::sendModuleObjectSetup - "
			"serialized data count: "+std::to_string(pVecSerialized->size()));
	for(MFDataObject* pD:*pVecSerialized)
		if(pD!=nullptr)delete pD;
	delete pVecSerialized;
	return sendData(
			(uint8_t*)mp_sbModObjSetup->getBuffer(),
			mp_sbModObjSetup->getBufferByteSize(),
			pNE);
}

bool MFNetClientConnectDispatchTask::sendStartLoop(S_MF_NetworkEvent* pNE){
	mp_startLoopData->setPackageDataSize(10);
	mp_sbStartLoop=new MFDataObjectBuffer(MFNetInputPackage::getHeaderSize()+10);

	if(!mp_sbStartLoop->createBuffer()){
		printErr("MFNetClientConnectDispatchTask::sendStartLoop - "
				"Failed to create shared buffer!");
		return false;
	}
	mp_startLoopData->useSharedBuffer(mp_sbStartLoop);
	mp_startLoopData->initData();
	uint32_t
		taskIndex=mp_netServRes->mp_netTaskManager->getTaskIndices().loop_start_task;
	mp_startLoopData->getTypeIDObject()->writeData(&taskIndex);
	mp_startLoopData->getPackageCounter()->writeData(&mp_netServRes->m_packageCounter);

	return sendData(
			(uint8_t*)mp_sbStartLoop->getBuffer(),
			mp_sbStartLoop->getBufferByteSize(),
			pNE);
}

bool MFNetClientConnectDispatchTask::sendData(
		uint8_t* data,
		uint32_t byteSize,
		S_MF_NetworkEvent* pNE){
	bool ret=MFNetworkLibraryHelper::getClient(pNE)->sendData(
			data,
			byteSize,
			0,
			true,
			&mp_netServRes->m_packageCounter);
	if(!ret){
		printErr("MFNetClientConnectDispatchTask::sendData - "
				"sending data failed!");
	}
	return ret;
}

