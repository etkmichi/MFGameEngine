/*
 * MFAxisInputTask.h
 *
 *  Created on: 30.10.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFAXISINPUTTASK_H_
#define MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFAXISINPUTTASK_H_

#include "MFInputTask.h"
#include <MFJoystickConfiguration.h>
/**
 * This class provides functionality for axis input like mouse movement and controler axis.
 *
 */
class MFAxisInputTask:
    public MFInputTask,
    public MFJoystickConfiguration {
public:/*virtual functions of MFInputTask*/
  /**
   * This function must implement all needed actions for specific keys.
   * @param dispatchData - data of the pressed key and the synchronisation object which
   * should be manipulated.
   * @return
   */
  virtual bool executeKeyAction(KeyDispatchData* dispatchData){return true;};
  virtual bool executeJoystickButtonAction(JoystickButtonDispatchData* dispatchData){return true;};
  virtual bool executeJoystickHatAction(JoystickHatDispatchData* dispatchData){return true;};
  virtual bool executeCommonInputUpdate(){return true;};

public:/*virtual functions of MFAxisInputTask*/
  virtual bool dispatchMouseLeftClick();//TODO isRightClicked & isLeftClicked functions
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

public:/*virtual functions of MFAxisInputTask*/
  void initInputSetup(uint32_t playerNumber){
    switch(playerNumber){
    case 0:
      initInput0();
      return;
    case 1:
      initInput1();
      return;
    case 2:
      initInput2();
      return;
    case 3:
      initInput3();
      return;
    default:
      P_WRN("no input setup for player number: "+T_S(playerNumber));
      return;
    }
  }
  virtual void initInput0(){P_WRN("no impl.!");};
  virtual void initInput1(){P_WRN("no impl.!");};
  virtual void initInput2(){P_WRN("no impl.!");};
  virtual void initInput3(){P_WRN("no impl.!");};
public: /*implemented virutal functions of MFInputTask:*/
  bool executeMouseAction(MouseDispatchData* pDispatchData);
  bool executeJoystickAxeAction(JoystickAxeDispatchData* dispatchData);

public:
  MFAxisInputTask();
  virtual ~MFAxisInputTask();
  bool validateInputData(JoystickAxisAction* pAction,int axeIndex){
    return ((pAction->axesArray!=nullptr)&&(pAction->axesCount>=axeIndex));
  }

};

#endif /* MFENGINEMODULES_MFINPUTMODULES_MFINPUTTASKS_MFAXISINPUTTASK_H_ */
