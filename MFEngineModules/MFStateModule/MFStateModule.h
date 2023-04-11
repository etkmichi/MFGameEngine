/*
 * MFStateModule.h
 *
 *  Created on: 14.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFSTATEMODULE_MFSTATEMODULE_H_
#define MFENGINEMODULES_MFSTATEMODULE_MFSTATEMODULE_H_

#include "../MFBaseModule.h"
#include "../MFDummyModuleObject.h"
/**
 * This class is needed to update the sync object states before the next iteration of
 * a game loop. It must be executed after all other modules, because the other
 * modules define the transition of the objects.
 */
class MFStateModule: public MFBaseModule {
private:
	MFModuleObject* pDummy;
protected:
	virtual bool initModuleObject(
			MFSyncObject* syncObject,
			MFModuleObject* pObject);
public:
	void updateResources(MFModuleResources* pRes);

	virtual bool initModule(){return true;};

//	bool disableObject(MFSyncObject *pObject);TODO

	virtual bool syncStates(std::vector<MFSyncObject*>* pVecStateChanges);

	virtual bool syncInputData(){return true;};

	virtual bool syncInputData(std::vector<MFSyncObject*>* pVecInputData){return true;};

	virtual bool executeModule(){return true;};

	virtual bool syncOutputData(){return true;};

	virtual MFModuleObject* createModuleObject(){return new MFDummyModuleObject();};
public:
	MFStateModule(MFSyncObjectResources* pSyncRes);
	virtual ~MFStateModule();
};

#endif /* MFENGINEMODULES_MFSTATEMODULE_MFSTATEMODULE_H_ */
