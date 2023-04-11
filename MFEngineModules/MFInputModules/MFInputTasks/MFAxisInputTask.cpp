/*
 * MFAxisInputTask.cpp
 *
 *  Created on: 30.10.2020
 *      Author: michl
 */

#include "MFAxisInputTask.h"

MFAxisInputTask::MFAxisInputTask() {
  // TODO Auto-generated constructor stub

}

MFAxisInputTask::~MFAxisInputTask() {
  // TODO Auto-generated destructor stub
}

bool MFAxisInputTask::executeMouseAction(MouseDispatchData* dispatchData){
  bool ret2=true;
  if(dispatchData->pMouseListener->pollButtonState(GLFW_MOUSE_BUTTON_LEFT)
      ==GLFW_PRESS){
    /*look left/right update*/
    bool ret=dispatchMouseLeftClick();
    if(!ret){
      printWarning("MFAxisInputTask::executeMouseAction - right click dispatch failure!");
      ret2=false;
    }
  }
  if(dispatchData->pMouseListener->
      pollButtonState(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
    bool ret=dispatchMouseRightClick();
    if(!ret){
      printWarning("MFAxisInputTask::executeMouseAction - right click dispatch failure!");
      ret2=false;
    }
  }
  bool ret=dispatchMouseMovement(dispatchData->deltaX, dispatchData->deltaY);
  if(!ret){
    printWarning("MFAxisInputTask::executeMouseAction -dispatch mouse movement failure!");
  }
  ret2&=ret;
  return ret2;
}

bool MFAxisInputTask::dispatchMouseLeftClick(){
  //printInfo("MFAxisInputTask::dispatchMouseLeftClick() - no impl.!");
  return true;
}

bool MFAxisInputTask::dispatchMouseRightClick(){
  //printInfo("MFAxisInputTask::dispatchMouseRightClick() - no impl.!");
  return true;
}

bool MFAxisInputTask::dispatchMouseMovement(double deltaX,double deltaY){
  //printInfo("MFAxisInputTask::dispatchMouseMovement() - no impl.!\n "
//      "deltaX="+std::to_string(deltaX)+" | deltaY="+std::to_string(deltaY));
  return true;
}

bool MFAxisInputTask::executeJoystickAxeAction(
    JoystickAxeDispatchData* dispatchData){
  JoystickAxisAction* pJAA=dispatchData->pAction;

  int index=m_axisIndexLeftStickFront;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<.0f) && checkLStTresholdFront(val))
      dispatchLeftStFront(-val);
  }

  index=m_axisIndexLeftStickBack;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>.0f) && checkLStTresholdBack(val))
      dispatchLeftStBack(val);
  }

  index=m_axisIndexLeftStickLeft;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<.0f) && checkLStTresholdLeft(val))
      dispatchLeftStLeft(-val);
  }

  index=m_axisIndexLeftStickRight;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>(0.0f)) && checkLStTresholdRight(val))
      dispatchLeftStRight(val);
  }


  index=m_axisIndexRightStickFront;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<(0.0f)) && checkRStTresholdFront(val))
      dispatchRightStFront(-val);
  }

  index=m_axisIndexRightStickBack;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>(0.0f)) && checkRStTresholdBack(val))
      dispatchRightStBack(val);
  }

  index=m_axisIndexRightStickLeft;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val<(0.0f)) && checkRStTresholdLeft(val))
      dispatchRightStLeft(-val);
  }

  index=m_axisIndexRightStickRight;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val>(0.0f)) && checkRStTresholdRight(val))
      dispatchRightStRight(val);
  }
  index=m_axisIndexLeftTriggerAxe;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val!=.0f))
      dispatchJoystickLeftTriggerAxe(val);
    }

  index=m_axisIndexRightTriggerAxe;
  if(validateInputData(pJAA,index)){
    float val=pJAA->axesArray[index];
    if((val!=.0f))
      dispatchJoystickRightTriggerAxe(val);
  }

  return true;
}

/*joystick*/
void MFAxisInputTask::dispatchJoystickLeftTriggerAxe(float value){
//  printInfo("MFAxisInputTask::dispatchJoystickLeftTriggerAxe - no impl.! val="+std::to_string(value));
  return ;
};
void MFAxisInputTask::dispatchJoystickRightTriggerAxe(float value){
  //printInfo("MFAxisInputTask::dispatchJoystickRightTriggerAxe - no impl.! val="+std::to_string(value));
  return ;
};
bool MFAxisInputTask::dispatchLeftStFront(float value){
  //printInfo("MFAxisInputTask::dispatchLeftStFront - no impl.! val="+std::to_string(value));
  return true;
};
bool MFAxisInputTask::dispatchLeftStBack(float value){
  //printInfo("MFAxisInputTask::dispatchLeftStBack - no impl.! val="+std::to_string(value));
  return true;
};
bool MFAxisInputTask::dispatchLeftStLeft(float value){
  //printInfo("MFAxisInputTask::dispatchLeftStLeft - no impl.! val="+std::to_string(value));
  return true;
};
bool MFAxisInputTask::dispatchLeftStRight(float value){
  //printInfo("MFAxisInputTask::dispatchLeftStRight - no impl.! val="+std::to_string(value));
  return true;
};

bool MFAxisInputTask::dispatchRightStFront(float value){
  //printInfo("MFAxisInputTask::dispatchRightStFront - no impl.! val="+std::to_string(value));
  return true;
};
bool MFAxisInputTask::dispatchRightStBack(float value){
  //printInfo("MFAxisInputTask::dispatchRightStBack - no impl.! val="+std::to_string(value));
  return true;
};
bool MFAxisInputTask::dispatchRightStLeft(float value){
  //printInfo("MFAxisInputTask::dispatchRightStLeft - no impl.! val="+std::to_string(value));
  return true;
};
bool MFAxisInputTask::dispatchRightStRight(float value){
  //printInfo("MFAxisInputTask::dispatchRightStRight - no impl.! val="+std::to_string(value));
  return true;
};
