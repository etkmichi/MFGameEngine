/*
 * MFIRecycleObject.h
 *
 *  Created on: 15.04.2021
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFINTERFACESMODULES_MFIRECYCLEOBJECT_H_
#define MFENGINEMODULES_MFINTERFACESMODULES_MFIRECYCLEOBJECT_H_

class MFIRecycleObject {
public: /*virtual functions of MFIRecycleObject*/
  virtual void enqueueRecycleObject(void* recycleObject) = 0;
public:
  MFIRecycleObject(){};
  virtual ~MFIRecycleObject(){};
};

#endif /* MFENGINEMODULES_MFINTERFACESMODULES_MFIRECYCLEOBJECT_H_ */
