/*
 * MFIModuleObjectCreator.h
 *
 *  Created on: 30.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFIMODULEOBJECTCREATOR_H_
#define MFENGINEMODULES_MFIMODULEOBJECTCREATOR_H_
#include "../MFModuleObject.h"
/**
 * This interface shall be used if a module can't provide the necessary informations
 * for initializing a module object. With an implementation of this interface
 * advanced objects can be created for a module. After creating an module object
 * it can be added by MFBaseModule::addSyncObject(MFSyncObject*,MFModuleObject*).
 */
class MFIModuleObjectSetup {
public:/*virtual functions of MFIModuleObjectSetup*/
  /**
   * If multipe module objects are created or objects were added to module already,
   * nullptr may be returned.
   * @param pSO - sync object which shall be used for creation.
   * @return the created module object or nullptr.
   */
  virtual MFModuleObject* createModuleObject(MFSyncObject* pSO=nullptr){return nullptr;};
  virtual bool preObjectCreation(MFSyncObject* pSO){return true;};
  /**
   *
   * @param pSO - sync object which was created for this creator.
   * @param pMO - corresponding module object, may be nullptr!
   * @return
   */
  virtual bool postObjectCreation(MFSyncObject* pSO,MFModuleObject* pMO){return true;};

public:
	MFIModuleObjectSetup(){};
	virtual ~MFIModuleObjectSetup(){};
	//TODO maybe return dummy which has its own class which is extended by all
	//existing module object classes -> seems to be dirty...
};

#endif /* MFENGINEMODULES_MFIMODULEOBJECTCREATOR_H_ */
