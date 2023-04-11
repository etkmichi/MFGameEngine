/*
 * MFNetModuleObjectSetup.h
 *
 *  Created on: 03.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETMODULEOBJECTSETUP_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETMODULEOBJECTSETUP_H_

#include "../MFNetworkResources.h"
#include "MFNetworkBaseTask.h"
/**
 * This client task sets up all added module objects with the received data.
 * Datagramm looks like:
 * | Count of modules | Module ID 1 | Count of modO's of ModID1 | sizeof ModO1 data |
 * serialized ModO1 data | ... | Module ID 2 | Count ... ModID2 | ....
 */
class MFNetModuleObjectSetup:
		public MFNetworkBaseTask,
		public MFNetInputPackage {
private:
	MFNetworkResources*
		mp_netRes;
//protected:
//	/*MFBasePackage*/
//	virtual void setupDataStructure(){
//		addNBitValue(m_dataSize*8,nullptr);
//	}
public:
	/*MFNetworkBaseTask*/
	virtual MFStructurableData* createNewEmptyData();
	virtual bool processData(MFStructurableData* pData);

public:
	MFNetModuleObjectSetup(MFNetworkResources* pRes);
	virtual ~MFNetModuleObjectSetup();
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETMODULEOBJECTSETUP_H_ */
