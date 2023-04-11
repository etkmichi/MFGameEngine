/*
 * MFConditionFunction.h
 *
 *  Created on: 29.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFACTIONMODULE_MFCONDITIONS_MFCONDITIONFUNCTION_H_
#define MFENGINEMODULES_MFACTIONMODULE_MFCONDITIONS_MFCONDITIONFUNCTION_H_
#include <MFBasicDefines.h>
#include <stdint.h>
/**
 * This class shall provide conditions as an object. It provides functionality for
 * creating a specified condition check.
 * TODO implement container of same class with boolean operators which connect other
 * MFConditionFunction objects: Shall make it possible to create a condition check
 * like ((a&b)|c) where a,b,c are sub type of MFConditionFunction.
 */
class MFConditionFunction {
private:
	bool
		m_invert=false;
protected:
	/*functions of MFConditionFunction*/
	virtual bool checkFunction(){return true;};
public:
	/*functions of MFConditionFunction*/
	virtual uint32_t getParameterCount(){
		return 0;
	}

public:
	MFConditionFunction();
	virtual ~MFConditionFunction();

	/**
	 *
	 * @return
	 */
	bool checkCondition(){
		if(m_invert){
		  MFObject::printErr("not impl.!");
//			return !checkCondition();
		}
		MFObject::printErr("not impl.!");
//		return checkCondition();
    return false;
	};

	void invertResults(bool invert){
		m_invert=invert;
	}
};

#endif /* MFENGINEMODULES_MFACTIONMODULE_MFCONDITIONS_MFCONDITIONFUNCTION_H_ */
