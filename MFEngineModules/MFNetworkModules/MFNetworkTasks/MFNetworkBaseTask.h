/*
 * MFNetworkBaseTask.h
 *
 *  Created on: 14.05.2020
 *      Author: michl
 */
#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETWORKBASETASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETWORKBASETASK_H_

#include <MFTasks/MFAbstractDataProcessingTask.h>
#include <MFData/MFInterfacesData/MFIStructurableDataCreator.h>
#include <MFTasks/MFInterfacesTasks/MFITaskProvider.h>
#include "../MFPackageClasses/MFNetInputPackage.h"
class MFNetworkBaseTask :
	public MFAbstractDataProcessingTask{
private:
	static MFITaskProvider
		*smp_netTaskProvider;

public: /*virtual functions of MFIStructurableDataCreator*/
  virtual MFStructurableData* createNewEmptyData(){
    MFObject::printErr("MFNetworkBaseTask::createNewEmptyData - not impl.!");
    return nullptr;
  }
public: /*virtual functions of MFNetworkBaseTask*/
  virtual bool createTaskDataBuffer(){
    MFObject::printErr("MFNetworkBaseTask::createTaskDataBuffer - no impl.!");
    return false;
  }

  /**
   * This function creates a filled package. The data must be in one piece, that
   * it can be sent over network. Take care to delete the created data when its
   * not used any more (f.e. after sending process)
   * @return
   */
  virtual MFNetInputPackage* createPackage(){
    MFObject::printErr("MFNetworkBaseTask::createPackage - no impl.!");
    return nullptr;
  }

  /**
   * This function fills a package with the task related data for sending purpose.
   * The data must be in one piece, that it can be sent over network.
   * @param pPkg - data of pkg must be at one piece and large enough to hold the
   * data.
   * @return
   */
  virtual bool fillPackage(MFNetInputPackage* pPkg){
    MFObject::printErr("MFNetworkBaseTask::fillPackage - no impl.!");
    return false;
  }

  virtual uint32_t calculatePackageSize(){
    MFObject::printErr("MFNetworkBaseTask::calculatePackageSize - no impl.!");
    return 0;
  }

public: /*virtual functions of MFAbstractDataProcessingTask*/
  /**
   * This function will dispatch received data, which was added as input
   * (MFAbstractDataProcessingTask::addInputData most probably called from
   * MFEngineControlSink::processInputData).
   * @param pData
   * @return
   */
  virtual bool processData(MFStructurableData* pData){
      MFObject::printWarning("MFNetworkBaseTask::processData - no"
          "implementation!");
      return true;
  };

public:
	MFNetworkBaseTask();
	virtual ~MFNetworkBaseTask();

	static void setStaticTaskProvider(MFITaskProvider* pTProv){
		smp_netTaskProvider=pTProv;
	}

	static void updateTaskProvider(
			MFITaskProvider* pTaskProvider,
			uint32_t dataCount,
			MFNetworkBaseTask* pNetworkTask,
			MFIStructurableDataCreator* pTaskDataCreator);
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFNETWORKTASKS_MFNETWORKBASETASK_H_ */
