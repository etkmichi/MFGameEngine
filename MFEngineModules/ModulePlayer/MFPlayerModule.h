/*
 * MFPlayerModule.h
 *
 *  Created on: 02.12.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULEPLAYER_MFPLAYERMODULE_H_
#define MFENGINEMODULES_MODULEPLAYER_MFPLAYERMODULE_H_
#include "../MFBaseModule.h"
#include "../MFInterfacesModules/MFIModuleCreator.h"
#include "../ModulePlayer/MFPlayerObject.h"

class MFPlayerModule:
    public MFBaseModule,
    public MFIModuleCreator {
protected:/*virtual functions of MFBaseModule*/
  //TODO create MFPhysicalObjectInitSetup out of the sync objects init setup data.
  //TODO implement usage of MFPhysicalObjectInitSetup object in physics library
  virtual bool initModuleObject(MFSyncObject* syncObject,MFModuleObject* pObject){return true;};

public:/*virtual functions of MFBaseModule*/
  virtual MFModuleObject* createModuleObject(){
    MFPlayerObject* pModuleObject=new MFPlayerObject();
    return pModuleObject;
  };
  virtual void updateResources(MFModuleResources* pRes);
  virtual MFBaseModule* createModule();
  virtual bool initModule();
  virtual bool executeModule();

public:
  MFPlayerModule(MFSyncObjectResources* pSyncRes);
  virtual ~MFPlayerModule();
};

#endif /* MFENGINEMODULES_MODULEPLAYER_MFPLAYERMODULE_H_ */
