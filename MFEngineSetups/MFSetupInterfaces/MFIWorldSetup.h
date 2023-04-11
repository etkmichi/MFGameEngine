/*
 * MFIWorldSetup.h
 *
 *  Created on: 27.10.2020
 *      Author: michl
 */

#ifndef MFENGINESETUPS_MFSETUPINTERFACES_MFIWORLDSETUP_H_
#define MFENGINESETUPS_MFSETUPINTERFACES_MFIWORLDSETUP_H_

class MFIWorldSetup {
public:/*virtual functions MFIWorldSetup*/
  virtual void setupWorld(){};
public:
  MFIWorldSetup(){};
  virtual ~MFIWorldSetup(){};
};

#endif /* MFENGINESETUPS_MFSETUPINTERFACES_MFIWORLDSETUP_H_ */
