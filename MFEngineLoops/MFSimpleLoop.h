/*
 * MFSimpleLoop.h
 *
 *  Created on: 21.10.2019
 *      Author: michl
 */

#ifndef MFENGINELOOPS_MFSIMPLELOOP_H_
#define MFENGINELOOPS_MFSIMPLELOOP_H_

#include <vector>
#include "../MFEngineModules/MFBaseModule.h"
class MFSimpleLoop {
	std::vector<MFBaseModule*>
		*mp_vecModules;
public:
	MFSimpleLoop();
	virtual ~MFSimpleLoop();
	void addModule(MFBaseModule* module);
	bool enterLoop();
};

#endif /* MFENGINELOOPS_MFSIMPLELOOP_H_ */
