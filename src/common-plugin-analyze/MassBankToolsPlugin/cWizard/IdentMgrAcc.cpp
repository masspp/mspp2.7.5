/**
 * @file IdentMgrAcc.cpp
 * @brief Identification Manager Wrapper
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#include "rel_stdafx.h"
#include "IdentMgrAcc.h"

#include "ErrorCheck.h"
#include "calcFunc\StrFunc.h"

typedef kome::ident::IdentificationManager kIdentMgr;

namespace Ident {
std::string getPrefixNum_Str(
	const std::string& preval
) {
	int basenum = getPrefixNum(preval);
	return (basenum >= 0) ? ext::toStr(basenum) : "";
}

const int getPrefixNum(
	const std::string& preval
){
	kIdentMgr& identMgr = kIdentMgr::getInstance();
	int basenum = -1;
	if(CHK::checkPrefix(preval)) {
		basenum = identMgr.getAccessionNumber(preval.c_str());
		// �VPrefix : 0 �ƁA��x�����g����Prefix : 0�@�̌����������Ȃ��ׁA
		// �擾���ɉ��Z��p�~�B�L�^���ɉ��Z���āA�擾���͐G��Ȃ����Ƃɂ���B
		// ver 2.6.0
		//if (basenum > 0) {
		//	basenum += 1;
		//}
	}
	return basenum;
}

// �ŏINUM�����ĕۑ�����B
// �VPrefix : 0 �ƁA��x�����g����Prefix : 0�@�̌����������Ȃ���,
// �L�^ - �o�͎��ɐ��l�̒����͂��Ȃ����ƂƂ���
void setPrefixAndNum(
	const std::string& prefix,
	const int lastnum
){
	kIdentMgr& identMgr = kIdentMgr::getInstance();
	identMgr.setAccessionNumber(prefix.c_str(), lastnum);
	identMgr.commit();
}


}
