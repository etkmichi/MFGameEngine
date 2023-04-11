/*
 * MFNetAddObjectsTask.h
 *
 *  Created on: 11.09.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETADDOBJECTSTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETADDOBJECTSTASK_H_

#include "MFNetworkBaseTask.h"

/**
 * This task provides network functionality for adding objects. After objects are
 * added the server must confirm it's indices because other clients may added
 * objects at the same time (indices in the vector of objects must be synchron).
 * The first received added objects at server side will stay at their index, the others
 * will be moved.
 */
class MFNetAddObjectsTask:
    public MFNetworkBaseTask,
    public MFNetInputPackage {
public: /*virtual functions of MFNetworkBaseTask*/
//  virtual MFStructurableData* createNewEmptyData();
//
//  virtual bool createTaskDataBuffer();
//
//  /**
//   * This function creates a filled package. The data must be in one piece, that
//   * it can be sent over network. Take care to delete the created data when its
//   * not used any more (f.e. after sending process)
//   * @return
//   */
//  virtual MFNetInputPackage* createPackage();
//
//  /**
//   * This function fills a package with the task related data.
//   * The data must be in one piece, that it can be sent over network.
//   * @param pPkg - data of pkg must be at one piece and large enough to hold the
//   * data.
//   * @return
//   */
//  virtual bool fillPackage(MFNetInputPackage* pPkg);
//
//  virtual uint32_t calculatePackageSize();
public:
  MFNetAddObjectsTask();
  virtual ~MFNetAddObjectsTask();

  /**
   * The maxObjectsCount defines how many objects cann be sent by one AddObjects-Package
   * (AddObjects-Package=MFNetAddObjectsTask). The buffer of this task will be changend
   * in size, depending on the objects count.
   * @param maxObjectsCount
   */
//  void setMaxObjectsCount(uint32_t maxObjectsCount);

  /**
   * Adds one object which shall be added on the receiver side to this package.
   * @param pGroup - the group to which the object was added too. Must be the same
   * on client and server!
   * @param pObject - the object which was added -> must provide all informations,
   * needed by the object manager, to add the object.
   * @param spawnCycle - can be manipulated to compensate latency between the server and
   * client simulations.
   * @return the remaining object count for this package.
   */
//  uint32_t addObject(
//      S_ModuleGroup* pGroup,
//      MFSyncObject* pObject,
//      uint32_t spawnCycle);

//  /**
//   * Resets the object count. Needed if package was sent and this task/package shall
//   * be reused.
//   */
//  void clearObjects();
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETADDOBJECTSTASK_H_ */
