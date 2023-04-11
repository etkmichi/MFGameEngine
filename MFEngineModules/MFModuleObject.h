/*
 * MFModuleObject.h
 *
 *  Created on: 22.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFMODULEOBJECT_H_
#define MFENGINEMODULES_MFMODULEOBJECT_H_

class MFModuleObject;

#include <math.h>
#include <MFData/MFStructurableDataSetup.h>
#include "../MFSynchronisationClasses/MFSyncObject.h"
#include <MFBasicDefines.h>

class MFModuleObject : public MFStructurableDataSetup {
private:
	uint32_t
  m_moduleObjectIndex=0xFFFF,
  m_moduleIndex=0xFFFF;
	uint64_t
  recycleIndex=-1;
	MFDataObject
  *mp_moduleObjectIndex,
  *mp_moduleIndex;
	bool
	structureInitialized=false;
protected:
  MFSyncObject
  *mp_syncObject;
  /** copy paste virtual functions:
protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject) = 0;
  virtual void setupObjectDataStructure() = 0;

public://virtual functions of MFModuleObject:
  virtual void initData() = 0;
  virtual bool updateData() = 0;
  virtual bool updateObject() = 0;
  virtual void disable() = 0;
  virtual void enable() = 0;
  virtual bool synchronizeInput() = 0;
  virtual bool synchronizeOutput() = 0;
  */
protected:/*virtual functions MFStructurableDataSetup*/
	/**
	 * This function updates the relevant module object data. If the module uses
	 * a pointer to the sync objects data, this pointer must be set to the new
	 * sync object!
	 * @param syncObject
	 */
	virtual void updateSyncObject(MFSyncObject* syncObject) = 0;

	/**
	 * This function sets up the module object specific data structure.
	 */
	virtual void setupObjectDataStructure() = 0;

public:/*virtual functions of MFModuleObject:*/
	virtual void initData() = 0;
	/**
	 * This function shall update the data of this objects structurable setup with related data of
	 * this module object.
	 * @return
	 */
	virtual bool updateData() = 0;

  /**
   * Updates the module object with data provided by this objects structurable data setup.
   * @return
   */
  virtual bool updateObject() = 0;

	/**
	 * Disables the module object
	 */
	virtual void disable() = 0;
	/**
	 * Enables the module object
	 */
	virtual void enable() = 0;

	/**
	 * Reads the data from the given MFSyncObject.
	 * @param syncObject
	 * @return
	 */
	virtual bool synchronizeInput() = 0;

	/**
	 * Writes the data to the given MFSyncObject.
	 * @param syncObject
	 * @return
	 */
	virtual bool synchronizeOutput() = 0;
public:
	MFModuleObject();
	virtual ~MFModuleObject();
	MFSyncObject* getSyncObject(){return mp_syncObject;};

  /*MFStructurableDataSetup*/ /**
   * This function sets up the MFModuleObject's structure and then the implementaiton
   * specific structure.
   //TODO used pre-allocated shared buffer for all module objects.
   //TODO create pre-allocated unsused objects in MFBaseModule.
   */
  void setupDataStructure();

	void setSyncObject(MFSyncObject *pSyncObject)
	{
		mp_syncObject=pSyncObject;
		updateSyncObject(pSyncObject);
	};

	/**
	 * Sets the index of this object's position of its modlue.
	 * @param index
	 */
	void setModuleIndex(uint32_t index){
		*((uint32_t*)mp_moduleObjectIndex->getData())=index;};

	/**
	 * Sets the module id, which can be used for module identification.
	 * @param moduleID
	 */
	void setModuleID(uint32_t moduleID){
		*((uint32_t*)mp_moduleIndex->getData())=moduleID;};

	uint32_t getModuleIndex(){return *((uint32_t*)mp_moduleObjectIndex->getData());};
  uint32_t getModuleID() {
    return *((uint32_t*) mp_moduleIndex->getData());
  }
  uint64_t getRecycleIndex() const {
    return recycleIndex;
  }

  void setRecycleIndex(uint64_t recycleIndex) {
    this->recycleIndex = recycleIndex;
  }
};

#endif /* MFSYNCHRONISATIONCLASSES_MFMODULEOBJECT_H_ */
