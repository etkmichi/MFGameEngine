/*
 * MFRecyclingModule.h
 *
 *  Created on: 19.03.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULERECYCLING_MFRECYCLINGMODULE_H_
#define MFENGINEMODULES_MODULERECYCLING_MFRECYCLINGMODULE_H_
#include "../MFBaseModule.h"
#include "../MFInterfacesModules/MFIRecycleManager.h"
#include <vector>
/**
 * TODO this module must not be executed during execution of other modules!
 *  -> disabling of objects may occure while other module still uses the object in its execution()
 * TODO implementation of creating all module recources without the recycled objects
 *  -> vector<ObjectData>
 *  -> after new resources are created f.e. every 30seconds, this module shall replace the
 *    original data with the created ones
 */
class MFRecyclingModule :
    public MFBaseModule,
    public MFIRecycleManager{
private:
  std::mutex
  lockVecReferenceCheck;
  std::vector<S_RecycleCollection*>
  *pVecReferenceCheck;
void doReferenceCheck();
protected: /*virtual functions of MFIRecycleManager*/
  bool recycleObjects(std::vector<S_RecycleCollection*> *pVecObjects);
protected:/*virtual functions of MFBaseModule*/
  /**
   * This function will add the sync object to the modules underlying system (for example to the underlying physics engine or the render api...)
   * It will be called within the execution of MFBaseModule::addSyncObject(...)
   * @param syncObject
   * @return
   */
  virtual MFModuleObject* initNewModuleObject(MFSyncObject* syncObject){
    return nullptr;
  };

  virtual bool initModuleObject(MFSyncObject* syncObject,MFModuleObject* pObject){
    P_ERR("no impl.!");
    return false;
  };

public:/*virtual functions of MFIModuleObjectCreator*/
  /**
   * If multipe module objects are created or objects were added to module already,
   * nullptr may be returned.
   * @param pSO - sync object which shall be used for creation.
   * @return the created module object or nullptr.
   */
  virtual MFModuleObject* createModuleObject(MFSyncObject* pSO){
    return createModuleObject();
  };

public:/*virtual functions of MFBaseModule*/
  virtual MFModuleObject* createModuleObject(){
    printWarning("MFBaseModule::createModuleObject() - "
      "not implemented, returning nullptr!");
    return nullptr;
  };

  virtual void updateResources(MFModuleResources* pRes){
    P_WRN("not impl.!");
  }

  virtual bool initModule(){
    P_WRN("not impl.!");return true;};

  virtual bool disableObject(MFSyncObject *pObject){
    P_WRN("not impl.!");return true;};

  /**
   * Default implementation will iterate over all motified changes of
   * MFSyncObjects (notifyInputSync::notifyInputSync()).
   * @warning This function is not thread safe!!
   * @return
   */
  virtual bool syncInputData(){return true;};

  /**
   * Default implementation will set the ObjectData's index to nullptr. The
   * ObjectData is a struct which connects the sync object to a specific
   * module object.
   * @param pSO
   */
  virtual void removeObject(MFSyncObject* pSO){};

  /**
   * This function uses the given indices for synchronicing/updating module objects with
   * their sync objects. In default implementation this function will be called with
   * notified input sync objects (only objects which received a notification bc. the changed).
   * The notification call should be implemented at a reasonable point f.e. if physics
   * collision occures, a notification can be sent to a sound playback object.
   * @return
   */
  virtual bool syncInputData(const std::vector<uint32_t>* pSyncIndices){return true;};

  /**
   * The implementation of this function must do the work of the module.
   * For example the physics module will calculate physics and
   * the render module will render/update the scene.
   * @return
   */
  virtual bool executeModule(){
    bool ret=doRecycling();
    doReferenceCheck();
    return ret;
  };

  /**
   * This function writes data from the inner module objects to the sync objects.
   * Default implementation will iterate over all module objects and calls the
   * MFModuleObject::synchronizeOutput() function.
   *
   * @return
   */
  virtual bool syncOutputData(){return true;};

public:
  MFRecyclingModule();
  virtual ~MFRecyclingModule();
};

#endif /* MFENGINEMODULES_MODULERECYCLING_MFRECYCLINGMODULE_H_ */
