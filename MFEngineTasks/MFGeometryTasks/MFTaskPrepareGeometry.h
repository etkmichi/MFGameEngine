/*
 * MFTaskPrepareGeometry.h
 *
 *  Created on: 28.01.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFSPAWNACTIONS_MFSPAWNTASKS_MFTASKPREPAREGEOMETRY_H_
#define MFSCENEACTIONS_MFSPAWNACTIONS_MFSPAWNTASKS_MFTASKPREPAREGEOMETRY_H_

#include <MFTasks/MFAbstractTask.h>
#include <MFBaseGeometries/MFAbstractGeometry.h>

class MFTaskPrepareGeometry: public MFAbstractTask {
private:
	MFAbstractGeometry
		*mp_geometry;
	void
		*mp_collisionShape;
	bool
		calcGeo=true,
		calcPhys=true;
public:
	MFTaskPrepareGeometry(MFAbstractGeometry *pGeo);
	virtual ~MFTaskPrepareGeometry();
	bool undoWork() override;
	bool doWork() override;
	MFAbstractGeometry* getGeometry();
//	btCollisionShape* getCollisionShape(){return mp_collisionShape;};
};

#endif /* MFSCENEACTIONS_MFSPAWNACTIONS_MFSPAWNTASKS_MFTASKPREPAREGEOMETRY_H_ */
