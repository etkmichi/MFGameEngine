/*
 * MFSharedData.h
 *
 *  Created on: 22.10.2019
 *      Author: michl
 */

#ifndef MFSYNCHRONISATIONCLASSES_MFSHAREDDATA_H_
#define MFSYNCHRONISATIONCLASSES_MFSHAREDDATA_H_
#include <glm/glm.hpp>
class MFSharedData {

	glm::mat4x4
		*mp_viewMatrix,
		*mp_projectionMatrix;
public:
	MFSharedData();
	virtual ~MFSharedData();
};

#endif /* MFSYNCHRONISATIONCLASSES_MFSHAREDDATA_H_ */
