/**********************************************************
Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
   @addtogroup C++ CommonFunctions
	@file       Standardinline.h
	@brief      �g�p�p�x�̍����t���[�Y��inline�Ƃ��ēZ�߂�B
	@date       2012.11.16
	@author     M.Fukuda
	@version    1.0.0
    @note       Boost���A�O�����C�u�����Ɋւ�鏈���͓���Ȃ��B
**********************************************************/
#pragma once
#include <cmath>

namespace RFN {
namespace STD {

const double kDiffLimit = 1.0E-10;
const double kPPMRatio = 1.0E-6;

// Double�̌덷(1.0E-10) ���z������ dl <= dr �̔���
inline const bool eq_less(const double dl, const double dr) {
	return dl < (dr + 1.0E-10);
}

//// $$$ �g�p�ӏ�����#2014.01.22
//// Double�̌덷(1.0E-10) ���z������ dl == dr �̔���
//inline const bool eq(const double& dl, const double& dr) {
//	return (std::fabs(dl-dr) < 1.0E-10);
//}

// l <= tgt <= R �� T/F �𔻒�
inline bool withinLimits(const double& tgt, const double& l, const double& r) {
	return eq_less(l, tgt) && eq_less(tgt, r);
}

//// PPM * Tol �̌v�Z�����ĕԂ��B
//inline const double getPPMTol(const double& tgt, const double& ppmVal) {
//	return tgt * (ppmVal * kPPMRatio);
//}

//// $$$ �g�p�ӏ�����#2014.01.22
///* @fn xxx */
//template<typename T_n>
//inline const int compVal(const T_n& l, const T_n& r, const T_n& tgt) {
//	if (tgt < l) return -1;
//	else if(tgt > r) return 1;
//	else return 0;
//}

// Refing.hpp�̂ݎg�p�B
// ��肪������΁ARefing.hpp�Ɉړ�����B
inline const int compVal_d(const double& l, const double& r, const double& tgt) {
	if (tgt < l) return -1;
	else if(tgt > r) return 1;
	else return 0;
}

//// $$$ �g�p�ӏ�����#2014.01.22
//// �Ώۂ̒l�� -1.0(�����l)�Ȃ� 0 �ɒu��
//inline const double fixZero(const double& tgt) {
//	return (tgt < 0.0) ? 0.0 : tgt;
//}

//// $$$ �g�p�ӏ�����#2014.01.22
//// �Ώۂ̒l�� -1.0(�����l)�Ȃ� DBL_MAX �ɒu��
//inline const double fixMaxDbl(const double& tgt) {
//	return (tgt < 0.0) ? DBL_MAX : tgt;
//}

//// $$$ �g�p�ӏ����� #2014.01.22
//// �Ώۂ̒l�� -1.0(�����l)�Ȃ� -DBL_MAX �ɒu��
//inline const double fixNegaMaxDbl(const double& tgt) {
//	return (tgt < 0.0) ? DBL_MAX * (-1.0) : tgt;
//}

//// $$$ �Y���ӏ���g�p #2014.01.22
//// $$$ �Y���ӏ� XYZPoints.hpp : searchZ_MinMax
//
//// dmin, dmax�@�Ԃ̐������̃`�F�b�N������B
//// isFreeRng
//inline const bool checkRangeConsistency(
//	const double& dmin, const double& dmax, const bool minus_is_Infinity) {
//	if (!minus_is_Infinity) return (dmin < dmax);
//	// use minus is infinity
//	if (dmin > 0.0 && dmax > 0.0) return (dmin < dmax);
//	return true;
//}

}// end of namespace
}