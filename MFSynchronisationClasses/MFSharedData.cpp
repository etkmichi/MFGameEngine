/*
 * MFSharedData.cpp
 *
 *  Created on: 22.10.2019
 *      Author: michl
 */

#include "MFSharedData.h"

MFSharedData::MFSharedData() {
	mp_projectionMatrix=new glm::mat4x4(1.0f);
	mp_viewMatrix=new glm::mat4x4(1.0f);
}

MFSharedData::~MFSharedData() {
	// TODO Auto-generated destructor stub
}

