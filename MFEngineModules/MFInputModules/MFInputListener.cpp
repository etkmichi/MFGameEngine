/*
 * MFInputListener.cpp
 *
 *  Created on: 02.12.2020
 *      Author: michl
 */

#include "MFInputListener.h"
#include <MFListenerInterfaces/MFInterfaceJoystickListener.h>
MFInputListener::MFInputListener() {
}

MFInputListener::~MFInputListener() {
  // TODO Auto-generated destructor stub
}


bool MFInputListener::executeMouseAction(MouseDispatchData* dispatchData){
  bool ret2=true;
  if(mouseClickActive){
    if(dispatchData->pMouseListener->pollButtonState(GLFW_MOUSE_BUTTON_LEFT)
        ==GLFW_PRESS){
      /*look left/right update*/
      bool ret=dispatchMouseLeftClick();
      if(!ret){
        printWarning("MFInputListener::executeMouseAction - right click dispatch failure!");
        ret2=false;
      }
    }
    if(dispatchData->pMouseListener->
        pollButtonState(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
      bool ret=dispatchMouseRightClick();
      if(!ret){
        printWarning("MFInputListener::executeMouseAction - right click dispatch failure!");
        ret2=false;
      }
    }
  }
  if(!mouseMovementActive)return ret2;
  bool ret=dispatchMouseMovement(dispatchData->deltaX, dispatchData->deltaY);
  if(!ret){
    printWarning("MFInputListener::executeMouseAction -dispatch mouse movement failure!");
  }
  ret2&=ret;
  return ret2;
}

bool MFInputListener::dispatchMouseLeftClick(){
  printInfo("MFInputListener::dispatchMouseLeftClick() - no impl.!");
  return true;
}

bool MFInputListener::dispatchMouseRightClick(){
  printInfo("MFInputListener::dispatchMouseRightClick() - no impl.!");
  return true;
}

bool MFInputListener::dispatchMouseMovement(double deltaX,double deltaY){
  printInfo("MFInputListener::dispatchMouseMovement() - no impl.!\n "
      "deltaX="+std::to_string(deltaX)+" | deltaY="+std::to_string(deltaY));
  return true;
}

void MFInputListener::joystickLeftStick(JoystickAxeDispatchData* dispatchData){
  int index=m_axisIndexLeftStickFront;
  JoystickAxisAction* pJAA=dispatchData->pAction;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<.0f) && checkLStTresholdFront(val))
      dispatchLeftStFront(-val);
  }

  index=m_axisIndexLeftStickBack;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>.0f) && checkLStTresholdBack(val))
      dispatchLeftStBack(val);
  }

  index=m_axisIndexLeftStickLeft;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<.0f) && checkLStTresholdLeft(val))
      dispatchLeftStLeft(-val);
  }

  index=m_axisIndexLeftStickRight;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>(0.0f)) && checkLStTresholdRight(val))
      dispatchLeftStRight(val);
  }
}

void MFInputListener::joystickRightStick(JoystickAxeDispatchData* dispatchData){
  int index=m_axisIndexRightStickFront;
  JoystickAxisAction* pJAA=dispatchData->pAction;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<(0.0f)) && checkRStTresholdFront(val))
      dispatchRightStFront(-val);
  }

  index=m_axisIndexRightStickBack;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>(0.0f)) && checkRStTresholdBack(val))
      dispatchRightStBack(val);
  }

  index=m_axisIndexRightStickLeft;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<(0.0f)) && checkRStTresholdLeft(val))
      dispatchRightStLeft(-val);
  }

  index=m_axisIndexRightStickRight;
  if(validateInputResources(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>(0.0f)) && checkRStTresholdRight(val))
      dispatchRightStRight(val);
  }
}

bool MFInputListener::executeJoystickAxeAction(
    JoystickAxeDispatchData* dispatchData){
  if(joystickLeftStickActive){
    joystickLeftStick(dispatchData);
  }

  if(joystickRigthStickActive){
    joystickRightStick(dispatchData);
  }

  int index=m_axisIndexLeftTriggerAxe;
  JoystickAxisAction* pJAA=dispatchData->pAction;
//  printJoystickAxes(0);
  if(joystickLeftTriggerAxeActive){
    if(validateInputResources(pJAA,index)){
      float val=pJAA->axesArray[index];
      if((val!=.0f))
        dispatchJoystickLeftTriggerAxe(val);
    }
  }
  index=m_axisIndexRightTriggerAxe;
  if(joystickRightTriggerAxeActive){
    if(validateInputResources(pJAA,index)){
      float val=pJAA->axesArray[index];
      if((val!=.0f))
        dispatchJoystickRightTriggerAxe(val);
    }
  }
  return true;
}

/*joystick*/
bool MFInputListener::dispatchLeftStFront(float value){
  printInfo("MFInputListener::dispatchLeftStFront - no impl.! val="+std::to_string(value));
  return true;
}
bool MFInputListener::dispatchLeftStBack(float value){
  printInfo("MFInputListener::dispatchLeftStBack - no impl.! val="+std::to_string(value));
  return true;
}
bool MFInputListener::dispatchLeftStLeft(float value){
  printInfo("MFInputListener::dispatchLeftStLeft - no impl.! val="+std::to_string(value));
  return true;
}
bool MFInputListener::dispatchLeftStRight(float value){
  printInfo("MFInputListener::dispatchLeftStRight - no impl.! val="+std::to_string(value));
  return true;
}

bool MFInputListener::dispatchRightStFront(float value){
  printInfo("MFInputListener::dispatchRightStFront - no impl.! val="+std::to_string(value));
  return true;
}
bool MFInputListener::dispatchRightStBack(float value){
  printInfo("MFInputListener::dispatchRightStBack - no impl.! val="+std::to_string(value));
  return true;
}
bool MFInputListener::dispatchRightStLeft(float value){
  printInfo("MFInputListener::dispatchRightStLeft - no impl.! val="+std::to_string(value));
  return true;
}
bool MFInputListener::dispatchRightStRight(float value){
  printInfo("MFInputListener::dispatchRightStRight - no impl.! val="+std::to_string(value));
  return true;
}
void MFInputListener::dispatchJoystickRightTriggerAxe(float value){
  printInfo("MFInputListener::dispatchJoystickRightTriggerAxe - no impl.! val="+std::to_string(value));
  return;
}
void MFInputListener::dispatchJoystickLeftTriggerAxe(float value){
  printInfo("MFInputListener::dispatchJoystickLeftTriggerAxe - no impl.! val="+std::to_string(value));
  return;
}
