/*
 * MFWorldModuleObject.h
 *
 *  Created on: 13.02.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFWORLDMODULE_MFWORLDMODULEOBJECT_H_
#define MFENGINEMODULES_MFWORLDMODULE_MFWORLDMODULEOBJECT_H_
#include "../../MFSynchronisationClasses/MFSyncObject.h"
#include "../MFModuleObject.h"
#include "MFWorldClasses/MFTileCollection.h"

/**
 * A MFWorldModuleObject represents a collection of the world tiles. The
 * sync object specified in the constructor is used for the coordinates of each tile.
 * The tiles are calculated around the x,y coordinates of the sync object. At
 * initialization, the tiles will be calculated and added to the scene by the
 * object manager specified in the MFWorldModule module.
 */
class MFWorldModuleObject: public MFModuleObject,MFAbstractTask {
private:
	static MFTaskThread
		*smp_initialThread;
	MFTileCollection
		*mp_tileCollection;
	std::vector<MFWorldModuleObject*>
		*mp_vecSurroundingTiles;
	bool
		m_enabled=true;
protected:
protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject){P_WRN("no impl.!");};
  virtual void setupObjectDataStructure();

public://virtual functions of MFModuleObject:
  virtual void initData(){P_WRN("no impl.!");};
  virtual bool updateData(){P_WRN("no impl.!");return true;};
  virtual bool updateObject(){P_WRN("no impl.!");return true;};
  virtual void disable();
  virtual void enable();

  /**
   * Reads the data from the given MFSyncObject.
   * @param syncObject
   * @return
   */
  virtual bool synchronizeInput();

  /**
   * Writes the data to the given MFSyncObject.
   * @param syncObject
   * @return
   */
  virtual bool synchronizeOutput();

public:
	MFWorldModuleObject();//TODO test, MFSyncObject* pSyncObject was removed!
	virtual ~MFWorldModuleObject();
	bool initTiles(bool externalInitThread);
	MFTileCollection* getTileCollection(){return mp_tileCollection;};
	bool updateTiles();

	virtual bool doWork();

	virtual bool undoWork();

	/**
	 * Creates a surrounding tile object and returns it for editing.
	 * @return
	 */
	MFWorldModuleObject* addSurroundingTile();
};

#endif /* MFENGINEMODULES_MFWORLDMODULE_MFWORLDMODULEOBJECT_H_ */
