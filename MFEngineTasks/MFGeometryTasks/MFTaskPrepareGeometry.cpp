/*
 * MFTaskPrepareGeometry.cpp
 *
 *  Created on: 28.01.2020
 *      Author: michl
 */

#include "MFTaskPrepareGeometry.h"

MFTaskPrepareGeometry::MFTaskPrepareGeometry(MFAbstractGeometry *pGeo) {
}

MFTaskPrepareGeometry::~MFTaskPrepareGeometry() {
	// TODO Auto-generated destructor stub
}

bool MFTaskPrepareGeometry::undoWork() {
	MFObject::printErr("MFTaskPrepareGeometry::undoWork() - "
			"No undo implemented!");
	return false;
}

bool MFTaskPrepareGeometry::doWork() {
	if(mp_geometry==nullptr){
		MFObject::printErr("MFTaskPrepareGeometry::doWork - "
				"mp_geometry==nullptr");
		return false;
	}
	bool ret=true;
	if(calcGeo && mp_geometry!=nullptr)
		ret=mp_geometry->initGeometry();
	if(calcPhys && mp_collisionShape!=nullptr){
		mp_collisionShape=mp_geometry->createCollisionShape();
		ret&=(nullptr!=mp_collisionShape);
	}
	return true;
}
