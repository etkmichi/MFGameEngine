/*
 * MFIActionConfirm.h
 *
 *  Created on: 15.09.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFINTERFACESACTION_MFIACTIONCONFIRM_H_
#define MFSCENEACTIONS_MFINTERFACESACTION_MFIACTIONCONFIRM_H_

class MFIActionConfirm {
public:/*virutal functions of MFIActionConfirm*/
  virtual void confirmAction(MFActionConfirm* pConfirm){
    MFObject::printErr("MFIActionConfirm::confirmAction - no impl.!");
  }
public:
  MFIActionConfirm(){};
  virtual ~MFIActionConfirm(){};
};

#endif /* MFSCENEACTIONS_MFINTERFACESACTION_MFIACTIONCONFIRM_H_ */
