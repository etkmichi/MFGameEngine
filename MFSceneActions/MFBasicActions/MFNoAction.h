/*
 * MFNoAction.h
 *
 *  Created on: 21.09.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFNOACTION_H_
#define MFSCENEACTIONS_MFNOACTION_H_

#include "MFBaseActionTask.h"

class MFNoAction: public MFBaseActionTask {

public:/*virtual functions of MFAbstractDataProcessingTask*/
  virtual bool processData(MFStructurableData* pData){
    MFObject::printInfo("MFNoAction::processData - no action!");
    //TODO check pData and print meta infos
    return true;
  };
public:
  MFNoAction();
  virtual ~MFNoAction();
};

#endif /* MFSCENEACTIONS_MFNOACTION_H_ */
