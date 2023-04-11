/*
 * MFBaseTrigger.h
 *
 *  Created on: 29.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFACTIONMODULE_MFACTIONTRIGGERS_MFBASETRIGGER_H_
#define MFENGINEMODULES_MFACTIONMODULE_MFACTIONTRIGGERS_MFBASETRIGGER_H_
#include <vector>
#include "../MFConditions/MFConditionFunction.h"
/**
 * This class provides basic trigger functionality.
 */
class MFBaseTrigger {
private:
	struct S_ConditionPair {
		MFConditionFunction* pConditionFunction=nullptr;
	};
	std::vector<MFConditionFunction*>
		*mp_vecConditionFunction;
	std::vector<MFConditionFunction*>
		*mp_vecCleanupConditions;
public:
	/*MFBaseTrigger functions:*/
public:
	MFBaseTrigger();
	virtual ~MFBaseTrigger();
	void addCondition(MFConditionFunction* conditionFunction);

	/**
	 * Iterates through added condition and checks if they are triggered.
	 * @return true if triggered
	 */
	bool isTriggered();
	template <class T,class U>
		void addConditionLess(T *less,U *greater);
	template <class T,class U>
		void addConditionGreater(T *less,U *greater);
	/* < ; = ; > ; >= ; <= ; == ; function*/

};

#endif /* MFENGINEMODULES_MFACTIONMODULE_MFACTIONTRIGGERS_MFBASETRIGGER_H_ */
