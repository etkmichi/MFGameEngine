/*
 * MFUsertTags.h
 *
 *  Created on: 13.03.2020
 *      Author: michl
 */

#ifndef MFSCENEACTIONS_MFUSERTAGS_H_
#define MFSCENEACTIONS_MFUSERTAGS_H_
#include <string>
class MFUserTag {
public:
	struct S_UserTag{
		uint32_t tag;
		std::string userName="";
	};
	const static uint32_t
		tag_admin=0xCAFEBABE,
		tag_all=0xFFFFFFFF;
};


#endif /* MFSCENEACTIONS_MFUSERTAGS_H_ */
