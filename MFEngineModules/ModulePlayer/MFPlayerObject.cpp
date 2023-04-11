/*
 * MFPlayerObject.cpp
 *
 *  Created on: 02.12.2020
 *      Author: michl
 */

#include "../ModulePlayer/MFPlayerObject.h"
uint32_t MFPlayerObject::s_playerCounter=0;

MFPlayerObject::MFPlayerObject(MFSyncObject* pPlayerObject) {
  setSetupName("MFPlayerObject");
  playerID=s_playerCounter;
  s_playerCounter++;
//  this->pPlayerInputObject=pPlayerInputObject;
//  this->pPlayerPhysicsObject=pPlayerPhysicsObject;
//  this->pPlayerRenderObject=pPlayerRenderObject;
}

MFPlayerObject::MFPlayerObject(MFModuleGroup* pGroupPlayers){
  this->pGroupPlayers=pGroupPlayers;
  playerID=s_playerCounter;
  s_playerCounter++;
};
MFPlayerObject::~MFPlayerObject() {
}

glm::vec4 MFPlayerObject::getPlayerColor(){
  if(s_playerCounter==0){
    return V4Red;
  }
  if(s_playerCounter==1){
    return V4Vlt;
  }
  if(s_playerCounter==2){
    return V4Blu;
  }
  if(s_playerCounter==3){
    return V4Tur;
  }
  if(s_playerCounter==4){
    return V4Yel;
  }
  if(s_playerCounter==5){
    return V4Gry;
  }
  if(s_playerCounter==6){
    return V4Wht;
  }
  if(s_playerCounter==7){
    return V4Grn;
  }
  return V4Wht;
}

glm::vec4 MFPlayerObject::getPlayerColor(uint32_t index){
  if(index==0){
    return V4Red;
  }
  if(index==1){
    return V4Vlt;
  }
  if(index==2){
    return V4Blu;
  }
  if(index==3){
    return V4Tur;
  }
  if(index==4){
    return V4Yel;
  }
  if(index==5){
    return V4Gry;
  }
  if(index==6){
    return V4Wht;
  }
  if(index==7){
    return V4Grn;
  }
  /*TODO static vec with rnd cols!...*/
  return V4Yel;
}
