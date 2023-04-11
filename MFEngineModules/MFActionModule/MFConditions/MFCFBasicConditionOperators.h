/*
 * MFCFBasicConditionOperators.h.h
 * This header file contains basic (< <= == > >=) compare function classes.
 *  Created on: 29.07.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFACTIONMODULE_MFCONDITIONS_MFCFBASICCONDITIONOPERATORS_H_
#define MFENGINEMODULES_MFACTIONMODULE_MFCONDITIONS_MFCFBASICCONDITIONOPERATORS_H_
/**
 * This
 */
#include "MFConditionFunction.h"
template<class T,class U>
class MFCFEqual: public MFConditionFunction {
private:
	T
		*mp_eq1;
	U
		*mp_eq2;
protected:
	/*functions of MFConditionFunction*/
	virtual bool checkFunction(){
		return ((*mp_eq1)==(*mp_eq2));
	};
public:
	/*functions of MFConditionFunction*/
	virtual uint32_t getParameterCount(){
		return 0;
	}
public:
	MFCFEqual(T *val1,U *val2){
		mp_eq1=val1;
		mp_eq2=val2;
	}
	virtual ~MFCFEqual(){};
};

template<class T,class U>
class MFCFLess: public MFConditionFunction {
private:
	T
		*mp_less;
	U
		*mp_greater;
protected:
	/*functions of MFConditionFunction*/
	virtual bool checkFunction(){
		return ((*mp_less)<(*mp_greater));
	};
public:
	/*functions of MFConditionFunction*/
	virtual uint32_t getParameterCount(){
		return 0;
	}
public:
	MFCFLess(T *less,U *greater){
		mp_less=less;
		mp_greater=greater;
	}
	virtual ~MFCFLess(){};
};

template<class T,class U>
class MFCFLessEqual: public MFConditionFunction {
private:
	T
		*mp_less;
	U
		*mp_greater;
protected:
	/*functions of MFConditionFunction*/
	virtual bool checkFunction(){
		return ((*mp_less)<=(*mp_greater));
	};
public:
	/*functions of MFConditionFunction*/
	virtual uint32_t getParameterCount(){
		return 0;
	}
public:
	MFCFLessEqual(T *lessEq,U *greaterEq){
		mp_less=lessEq;
		mp_greater=greaterEq;
	}
	virtual ~MFCFLessEqual(){};
};

template<class T,class U>
class MFCFGreater: public MFConditionFunction {
private:
	T
		*mp_less;
	U
		*mp_greater;
protected:
	/*functions of MFConditionFunction*/
	virtual bool checkFunction(){
		return ((*mp_less)<(*mp_greater));
	};
public:
	/*functions of MFConditionFunction*/
	virtual uint32_t getParameterCount(){
		return 0;
	}
public:
	MFCFGreater(T *less,U *greater){
		mp_less=less;
		mp_greater=greater;
	}
	virtual ~MFCFGreater(){};
};

template<class T,class U>
class MFCFGreaterEqual: public MFConditionFunction {
private:
	T
		*mp_less;
	U
		*mp_greater;
protected:
	/*functions of MFConditionFunction*/
	virtual bool checkFunction(){
		return ((*mp_less)<=(*mp_greater));
	};
public:
	/*functions of MFConditionFunction*/
	virtual uint32_t getParameterCount(){
		return 0;
	}
public:
	MFCFGreaterEqual(T *lessEq,U *greaterEq){
		mp_less=lessEq;
		mp_greater=greaterEq;
	}
	virtual ~MFCFGreaterEqual(){};
};

#endif /* MFENGINEMODULES_MFACTIONMODULE_MFCONDITIONS_MFCFBASICCONDITIONOPERATORS_H_ */
