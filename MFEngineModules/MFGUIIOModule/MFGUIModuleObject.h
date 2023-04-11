/*
 * MFGUIModuleObject.h
 *
 *  Created on: 07.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFGUIIOMODULE_MFGUIMODULEOBJECT_H_
#define MFENGINEMODULES_MFGUIIOMODULE_MFGUIMODULEOBJECT_H_

#include <MFData/MFDataObject.h>
#include <MFIGUIManager.h>
#include "../MFModuleObject.h"
#include <glm/glm.hpp>

class MFGUIModuleObject : public MFModuleObject{
	uint64_t
		m_index=0;
	MFIGUIManager
		*mp_guiMgr;
	MFDataObject
		*mp_dataObject,
		*mp_internalDataObject;//TODO test with internal
protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject){};
  virtual void setupObjectDataStructure(){
//    mp_dataObject=addData(*mp_dataObject);TODO
  };
public://virtual functions of MFModuleObject:
  virtual void initData(){};
  virtual bool updateData(){return true;};
  virtual bool updateObject(){return true;};
  virtual void disable(){};
  virtual void enable(){};
  virtual bool synchronizeInput();
  virtual bool synchronizeOutput();
public:
	MFGUIModuleObject(MFDataObject* pData,MFIGUIManager *pMgr);
	MFGUIModuleObject(MFIGUIManager *pMgr);
	virtual ~MFGUIModuleObject();
	MFDataObject* getModelMatData(){return mp_dataObject;};
	void setIndex(uint64_t index){m_index=index;};
	MFDataObject* getGUIDataObject(){return mp_dataObject;};

};

#endif /* MFENGINEMODULES_MFGUIIOMODULE_MFGUIMODULEOBJECT_H_ */
