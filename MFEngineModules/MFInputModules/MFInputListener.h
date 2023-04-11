/*
 * MFInputListener.h
 *
 *  Created on: 02.12.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTLISTENER_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTLISTENER_H_

#include "MFInputModuleObject.h"
#include "MFInputTasks/MFInputTask.h"
#include "MFJoystickConfiguration.h"
/**
 * Class for more specific input listening and with virtual functions for dispatching input actions.
 */
class MFInputListener:
    public MFInputTask,
    public MFJoystickConfiguration {
public:/*virtual functions of MFInputTask*/
  virtual bool executeKeyAction(KeyDispatchData* dispatchData){return true;};
  virtual bool executeJoystickButtonAction(JoystickButtonDispatchData* dispatchData){return true;};
  virtual bool executeJoystickHatAction(JoystickHatDispatchData* dispatchData){return true;};
  virtual bool executeCommonInputUpdate(){return true;};

public:/*virtual functions of MFInputListener*/
  virtual bool dispatchMouseLeftClick();
  virtual bool dispatchMouseRightClick();
  virtual bool dispatchMouseMovement(double deltaX,double deltaY);

  /*joystick*/
  virtual bool dispatchLeftStFront(float value);
  virtual bool dispatchLeftStBack(float value);
  virtual bool dispatchLeftStLeft(float value);
  virtual bool dispatchLeftStRight(float value);

  virtual bool dispatchRightStFront(float value);
  virtual bool dispatchRightStBack(float value);
  virtual bool dispatchRightStLeft(float value);
  virtual bool dispatchRightStRight(float value);

  virtual void dispatchJoystickLeftTriggerAxe(float value);
  virtual void dispatchJoystickRightTriggerAxe(float value);

public: /*implemented virutal functions of MFInputTask:*/
  bool executeMouseAction(MouseDispatchData* pDispatchData);
  bool executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData);
private:
  bool
  mouseClickActive=true,
  mouseMovementActive=true,
  joystickRigthStickActive=true,
  joystickLeftStickActive=true,
  joystickLeftTriggerAxeActive=true,
  joystickRightTriggerAxeActive=true;
public:
  MFInputListener();
  virtual ~MFInputListener();
  /**
   * Checks if the given parameters are valid
   * @param pAction
   * @param axeIndex
   * @return
   */
  bool validateInputResources(JoystickAxisAction* pAction,int axeIndex){
    return ((pAction->axesArray!=nullptr)&&(pAction->axesCount>=axeIndex));
  }

  bool isJoystickLeftStickActive() const {
    return joystickLeftStickActive;
  }

  void setJoystickLeftStickActive(bool joystickLeftStickActive = true) {
    this->joystickLeftStickActive = joystickLeftStickActive;
  }

  bool isJoystickRigthStickActive() const {
    return joystickRigthStickActive;
  }

  void setJoystickRightStickActive(bool joystickRigthStickActive = true) {
    this->joystickRigthStickActive = joystickRigthStickActive;
  }

  bool isMouseClickActive() const {
    return mouseClickActive;
  }

  void setMouseClickActive(bool mouseClickActive = true) {
    this->mouseClickActive = mouseClickActive;
  }

  bool isMouseMovementActive() const {
    return mouseMovementActive;
  }

  void setMouseMovementActive(bool mouseMovementActive = true) {
    this->mouseMovementActive = mouseMovementActive;
  }

  bool isJoystickLeftTriggerAxeActive() const {
    return joystickLeftTriggerAxeActive;
  }

  void setJoystickLeftTriggerAxeActive(
      bool joystickLeftTriggerAxeActive = true) {
    this->joystickLeftTriggerAxeActive = joystickLeftTriggerAxeActive;
  }

  bool isJoystickRightTriggerAxeActive() const {
    return joystickRightTriggerAxeActive;
  }

  void setJoystickRightTriggerAxeActive(bool joystickRightTriggerAxeActive =
      true) {
    this->joystickRightTriggerAxeActive = joystickRightTriggerAxeActive;
  }

private:
  inline void joystickLeftStick(JoystickAxeDispatchData* dispatchData);
  inline void joystickRightStick(JoystickAxeDispatchData* dispatchData);
};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTLISTENER_H_ */
