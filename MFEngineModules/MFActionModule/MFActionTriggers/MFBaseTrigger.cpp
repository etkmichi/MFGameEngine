/*
 * MFBaseTrigger.cpp
 *
 *  Created on: 29.07.2020
 *      Author: michl
 */

#include "MFBaseTrigger.h"
#include "../MFConditions/MFCFBasicConditionOperators.h"

MFBaseTrigger::MFBaseTrigger() {
	mp_vecConditionFunction=new std::vector<MFConditionFunction*>();
	mp_vecCleanupConditions=new std::vector<MFConditionFunction*>();
}

MFBaseTrigger::~MFBaseTrigger() {
	for(MFConditionFunction* pCF:*mp_vecCleanupConditions){
		delete pCF;
	}
	delete mp_vecConditionFunction;
	delete mp_vecCleanupConditions;
}
void MFBaseTrigger::addCondition(MFConditionFunction* conditionFunction){
	mp_vecConditionFunction->push_back(conditionFunction);
}

template <class T,class U>
void MFBaseTrigger::addConditionLess(T *less,U *greater){
	MFCFLess<T,U>* pCF=new MFCFLess<T,U>(less,greater);
	mp_vecCleanupConditions->push_back(pCF);
	mp_vecConditionFunction->push_back(pCF);
}

template <class T,class U>
void MFBaseTrigger::addConditionGreater(T *less,U *greater){
	MFCFGreater<T,U>* pCF=new MFCFGreater<T,U>(less,greater);
	mp_vecCleanupConditions->push_back(pCF);
	mp_vecConditionFunction->push_back(pCF);
}

bool MFBaseTrigger::isTriggered(){
	bool ret=true;
	for(MFConditionFunction* pCF:*mp_vecCleanupConditions){
		ret&=pCF->checkCondition();
	}
	return ret;
}
