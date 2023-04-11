/*
 * MFIEngineLoopControl.h
 *
 *  Created on: 30.06.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFIENGINELOOPCONTROL_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFIENGINELOOPCONTROL_H_
#include <MFThreadSystem/MFTaskThread.h>
class MFIEngineLoopControl {
public:/* virtual functions of MFIEngineLoopControl */
  /**
   * function for executing modules in a loop.
   * @return
   */
  virtual bool executeModuleLoop(bool* externalClose){
    MFObject::printErr("MFIEngineLoopControl::executeModuleLoop - "
        "no impl.!");
    return false;
  }

  virtual bool executeExternalModuleLoop(bool* externalClose,MFTaskThread* externalThread=nullptr){
    MFObject::printErr("MFIEngineLoopControl::executeExternalModuleLoop - "
        "no impl.!");
    return false;
  }

  virtual bool isStarted(){
    MFObject::printErr("MFIEngineLoopControl::isStarted - "
        "no impl.!");
    return false;};

  /**
   *
   * @return the thread which executes the external module loop.
   */
  virtual MFTaskThread* getExternalLooperThread(){
    MFObject::printErr("MFIEngineLoopControl::executeExternalModuleLoop - "
        "no impl.!");
    return nullptr;
  }

  /**
   * function for single step execution.
   * @return
   */
  virtual bool executeModuleStep(){
    MFObject::printErr("MFIEngineLoopControl::executeModuleStep - "
        "no impl.!");
    return false;
  }

  /**
   * Stops the module loop.
   * @return
   */
  virtual bool stopModuleLoop(){
    MFObject::printErr("MFIEngineLoopControl::stopModuleLoop - "
        "no impl.!");
    return false;
  }

  /**
   * Stops the exteranl module loop.
   * @return
   */
  virtual bool stopExternalModuleLoop(){
    MFObject::printErr("MFIEngineLoopControl::stopExternalModuleLoop - "
        "no impl.!");
    return false;
  }

  virtual void setExternalClose(bool* externalClose){
    MFObject::printErr("MFIEngineLoopControl::setExternalClose(bool*) - "
        "no impl.!");
  };

  /**
   * Returns the current cycle count. The current cycle is increased by 1 every on
   * every module loop.
   * @return
   */
  virtual uint32_t getCurrentCycle(){return 0;};

  virtual void setCurrentCycle(uint32_t cycle){};

public:
	MFIEngineLoopControl(){};

	virtual ~MFIEngineLoopControl(){};

};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFIENGINELOOPCONTROL_H_ */
