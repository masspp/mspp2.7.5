/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       XYPoint.hpp
	@brief      X,Y �֐��̎�����
	@date       2012.12.12
	@author     M.Fukuda@Eisai
	@version    1.2.0
	@note
****************************************************************/
#pragma once

#include "XYPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
const XYP_ NG_XYP(-1.0,-1.0);

-- RFN::XYP --
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*
/ -------------------------------------------------------*/
#include <algorithm>
#include <tuple>     // for std::tie

#include "../Standardinline.hpp"

namespace RFN {
namespace XYP {

// ---------------------------------------------------------
// �֐��I�u�W�F�N�g
// --------------------------------------------------------

//// $$$ �g�p�ӏ�����#2014.01.22
//// Y�����̃\�[�g�֐��I�u�W�F�N�g
//struct lessY : public std::binary_function<const RFN::XYP::XYP_&, const RFN::XYP::XYP_&, bool> {
//	bool operator()(const RFN::XYP::XYP_& lxyp, const RFN::XYP::XYP_& rxyp) const {
//		return lxyp.second < rxyp.second;
//	}
//};

// binary sort �p��X�����֐��I�u�W�F�N�g
struct compX { 
	bool operator()(const RFN::XYP::XYP_& lxy, double rx)  { return lxy.first < rx; }
	bool operator()(double lx, const RFN::XYP::XYP_& rxy) { return lx < rxy.first; }
	bool operator()(const RFN::XYP::XYP_& lxy, const RFN::XYP::XYP_& rxy) { return lxy.first < rxy.first; }
};

//// $$$ �g�p�ӏ�����#2014.01.22
//// Zero�ȉ��̃|�C���g��T��
struct underZeroY : public std::unary_function<const RFN::XYP::XYP_&, bool> {
	bool operator()(const RFN::XYP::XYP_& tgt) const {
		return tgt.second < 0.0;
	}
};

// ---------------------------------------------------------
// ������
// ---------------------------------------------------------

//// $$$ �g�p�ӏ�����#2014.01.22
//// vXYP �̗]��(Capacity����)�����
//inline void shrinkToFit(RFN::XYP::vXYP_& vxyp) {
//	ELM::vXYP(vxyp).swap(vxyp);
//}

//// $$$ �g�p�ӏ�����#2014.01.22
//// vXYP �����S�ɃN���A����
//inline void fullClear(RFN::XYP::vXYP_& vxyp) {
//	vxyp.clear();
//	shrinkToFit(vxyp);
//}

// vxyp �Ɂ@�|�C���g��ǉ�����
inline void addXYPoint(const double& x, const double& y, RFN::XYP::vXYP_& vxyp) {
	vxyp.push_back(std::make_pair(x, y));
}

//// $$$ �g�p�ӏ�����#2014.01.22
//// �ő�Intensity������ XYP�@�ʒu�̃C�e���[�^��Ԃ�
//inline RFN::XYP::vXYP_::iterator getXYP_MaxIntensity(RFN::XYP::vXYP_& vxyp){
//	return std::max_element(vxyp.begin(), vxyp.end(), RFN::XYP::lessY());
//}

// vxyp�̒�����ߎ��l x <= result.x �ł���l��T���B�����ꍇ�� vxyp
inline const RFN::XYP::vXYP_::iterator nearSearchX(
	const double& x, RFN::XYP::vXYP_& vxyp
) {
	return std::lower_bound(vxyp.begin(), vxyp.end(), x, RFN::XYP::compX());
}

// �C�e���[�^�̒T������Œ肵�āi�{���͌v�Z���Ċm�肵�Ă����̂��]�܂����j
inline const RFN::XYP::vXYP_::iterator nearSearchX(const double& x, const ITR::vXYP_Itr& bit, RFN::XYP::vXYP_& vxyp) {
	return std::lower_bound(bit, vxyp.end(), x, RFN::XYP::compX());
}

//// $$$ �g�p�ӏ�����#2014.01.22
//inline const bool onInDistance(
//	const ITR::vXYP_Itr& bgnit,
//	const ITR::vXYP_Itr& endit,
//	const unsigned int lenRange
//){
//	if(lenRange == 0) return false;
//	return (std::distance(bgnit,endit) > static_cast<int>(lenRange));
//}

//// $$$ �g�p�ӏ�����#2014.01.22
//// �C�e���[�^�̒T������Œ肵�āi�{���͌v�Z���Ċm�肵�Ă����̂��]�܂����j
//inline const RFN::XYP::vXYP_::iterator nearSearchX_withABSRange(
//	const double& x, 
//	const ITR::vXYP_Itr& bit,
//	const unsigned int lenRange,
//	RFN::XYP::vXYP_& vxyp
//) {
//	if (onInDistance(bit, vxyp.end(), lenRange)) {
//		return std::lower_bound(bit, bit + lenRange, x, RFN::XYP::compX());
//	}
//	return std::lower_bound(bit, vxyp.end(), x, RFN::XYP::compX());
//}

//// $$$ �g�p�ӏ�����#2014.01.22
//// �ő��Intensity��Ԃ�
//inline double getMaxIntensity(RFN::XYP::vXYP_& vxyp) {
//	return (*RFN::XYP::getXYP_MaxIntensity(vxyp)).second;
//}

// X, Y �̗v�f�̏�Z��Ԃ� (product)
inline double productXY(const RFN::XYP::XYP_& src) {
	return src.first * src.second;
}

// Sort X 
inline void sortX(RFN::XYP::vXYP_& vxyp, const bool desc) {
	if (!desc) {
		std::sort(vxyp.begin(), vxyp.end());
	} else { // �~���Ȃ̂Ńl�Q�[�^�Ŕ��]
		std::sort(vxyp.begin(), vxyp.end(), std::greater<RFN::XYP::XYP_>());
	}
}

//// $$$ �g�p�ӏ�����#2014.01.22
//// Sort Y
//inline void sortY(RFN::XYP::vXYP_& vxyp, const bool desc) {
//	if (!desc) {
//		std::sort(vxyp.begin(), vxyp.end(), RFN::XYP::lessY());
//	} else { // �~���Ȃ̂Ńl�Q�[�^�Ŕ��]
//		std::sort(vxyp.begin(), vxyp.end(), std::not2(RFN::XYP::lessY()));
//	}
//}

// vxyp ���� minX, maxX�͈̔͂����C�e���[�^��T���B
// [Error] : litr = vxyp.end()
// Ex) minX < vxyp[0].mz �ł���΁Avxyp.begin()��Ԃ��B 
inline const std::pair<ITR::vXYP_Itr, ITR::vXYP_Itr>
	searchX_MinMax(
	const double& minX, 
	const double& maxX, 
	RFN::XYP::vXYP_& vxyp
) {
	// �f�t�H���g��ݒ肷��
	ITR::vXYP_Itr lowit = vxyp.end(); // ��O�ׁ̈Alowit��End�ɌŒ�B
	ITR::vXYP_Itr highit = vxyp.end();

	if (! vxyp.empty() && vxyp[0].first <= maxX) {
		// minX, maxX�̐������𔻒肷��
		size_t num = vxyp.size();
		const double arrlow(vxyp[0].first), arrhigh(vxyp[num-1].first);

		if (minX < arrlow) { // �z��̍ŏ��l�ł���ΐ擪��Ԃ��B
			lowit = vxyp.begin();
		} else if (RFN::STD::eq_less(minX, arrhigh)) {
			lowit = nearSearchX(minX, vxyp);
		}
		if (maxX < 0.0 || arrhigh < maxX) { // �����w��A�z����傫���ꍇ�͖��[��Ԃ�
			highit = vxyp.end();
		} else if (RFN::STD::eq_less(arrlow, maxX)) {
			highit = nearSearchX(maxX, lowit, vxyp);
		}
	}
	return std::make_pair(lowit, highit);
}

//// $$$ �g�p�ӏ�����#2014.01.22
//// ���x�e�X�g���Bmake_pair�́E�E
//inline const bool
//	searchX_MinMaxF(const double& minX, const double& maxX, RFN::XYP::vXYP_& vxyp, ITR::vXYP_Itr& lowit, ITR::vXYP_Itr& highit) {
//	// �f�t�H���g��ݒ肷��
//	lowit = vxyp.end(); // ��O�ׁ̈Alowit��End�ɌŒ�B
//	highit = vxyp.end();
//
//	size_t num = vxyp.size();
//	const double arrlow(vxyp[0].first), arrhigh(vxyp[num-1].first);
//
//	if (minX < arrlow) { // �z��̍ŏ��l�ł���ΐ擪��Ԃ��B
//		lowit = vxyp.begin();
//	} else if (RFN::STD::eq_less(minX, arrhigh)) {
//		lowit = nearSearchX(minX, vxyp);
//	}
//	if (maxX < 0.0 || arrhigh < maxX) { // �����w��A�z����傫���ꍇ�͖��[��Ԃ�
//		highit = vxyp.end();
//	} else if (RFN::STD::eq_less(arrlow, maxX)) {
//		highit = nearSearchX_withABSRange(maxX, lowit, 63, vxyp);
//	}
//	return lowit != vxyp.end();
//}


// src ���́@lx - rx �́@�f�[�^�� dst �ɃR�s�[����
// [in]  lx  : �ŏ��l (minMz)
// [in]  rx  : �ő�l (maxMz)
// [in]  src : ���ƂȂ� vxyp
// [out] dst : ���ʂ̏o��
// �l�������Ȃ�u��v�ŋA��
inline bool getInRangeElements(
	const double& lx, const double& rx, 
	RFN::XYP::vXYP_& src, RFN::XYP::vXYP_& dst
){
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);

	dst.clear();
	// error. Break.
	if (lit == src.end()) { return false; }

	dst.reserve(std::distance(lit, rit));
	while(lit != rit) {
		dst.push_back(*lit);
		++lit;
	}
	return true;
}

// src�@�́@lx-rx�@�́@total Intensity (TIC) ��Ԃ�
// [in]  lx  : �ŏ��l (minMz)
// [in]  rx  : �ő�l (maxMz)
// [in]  src : ���ƂȂ� vxyp
// [rst] TIC or 0 (�����A�G���[���͑S�� 0 )
inline const double getInRangeTotalIntensity(
	const double lx, const double rx, RFN::XYP::vXYP_& src
){
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//	searchX_MinMaxF(lx, rx, src, lit, rit);
	//ITR::vXYP_Itr lit = nearSearchX(lx, src);
	//ITR::vXYP_Itr rit = nearSearchX(rx, src);

	register double rstVal(0.0);
	if (lit != src.end()) {
		while(lit != rit) {
			rstVal += lit->second;
			++lit;
		}
	}
	return rstVal;
}

//// $$$ �g�p�ӏ�����#2014.01.22
inline const double getInRangeTotalIntensity(
	const double lx, const double rx, RFN::XYP::vXYP_& src, double rt, std::vector<std::tuple<double, double, double>>& vPnt
) {
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//	searchX_MinMaxF(lx, rx, src, lit, rit);
	//ITR::vXYP_Itr lit = nearSearchX(lx, src);
	//ITR::vXYP_Itr rit = nearSearchX(rx, src);

	register double rstVal(0.0);
	if (lit != src.end()) {
		while(lit != rit) {
			rstVal += lit->second;
			vPnt.push_back(std::tuple<double, double, double>(rt, lit->first, lit->second));
			++lit;
		}
	}
	return rstVal;
}

//// $$$ �g�p�ӏ�����#2014.01.22
//// src�@�́@lx-rx�@�́@Max Intensity ��Ԃ�
//// [in]  lx  : �ŏ��l (minMz)
//// [in]  rx  : �ő�l (maxMz)
//// [in]  src : ���ƂȂ� vxyp
//// [rst] Max Intensity or 0 (�����A�G���[���͑S�� 0 )
//inline const double getInRangeMaxIntensity(
//	const double& lx, const double& rx, RFN::XYP::vXYP_& src
//){
//	ITR::vXYP_Itr lit, rit;
//	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//
//	double rstVal(0.0);
//	if (lit != src.end()) {
//		while(lit != rit) {
//			rstVal = (RFN::STD::eq_less(rstVal, lit->second)) ? lit->second : rstVal;
//			++lit;
//		}
//	}
//	return rstVal;
//}

//// $$$ �g�p�ӏ�����#2014.01.22
//// src�̎w���Ԃ�Y(Intensity)��S�ĂO�ɂ���B
//inline void clearY(
//	const double& lx, const double& rx, RFN::XYP::vXYP_& src
//) {
//	ITR::vXYP_Itr lit, rit;
//	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);
//
//	if (lit == src.end()) {	
//		return;
//	}
//	for (; lit != rit; ++lit) {
//		lit->second = 0.0;
//	}
//}

// src�̎w���Ԃ�Y(Intensity)��S��0�ɂ���B
// �Ō�̈����̃��X�g��0�ɂ���O��Intensity���i�[����B
inline void clearY(
	const double& lx, const double& rx, RFN::XYP::vXYP_& src, std::vector<double>& intensities) {
	ITR::vXYP_Itr lit, rit;
	std::tie(lit, rit) = searchX_MinMax(lx, rx, src);

	if (lit == src.end()) {	
		return;
	}
	for (; lit != rit; ++lit) {
		if (lit->second > 0) {
			intensities.push_back(lit->second);
			lit->second = 0.0;
		}
	}
}


//// $$$ �g�p�ӏ�����#2014.01.22
// �ȉ��A�����܂Ŗ��g�p
//---------------------------------------------------------
// �s�v��Ԃ����֐��Q�B�I�u�W�F�N�g�����߂���
// ���������[�N���N���������Ȏ��̑΍�Q�B
// �O���A�������Ԃ͊m�ۂ��鉿�l���Ȃ���΃f�[�^�����铙�B
//---------------------------------------------------------

//// underPoint �ȉ��̘A����Ԃ�a�𕉂ŁA����ȊO�̘a�𐳂ŕԂ��B
//// EX) 0,0,4,5,2,0,0,1,0,9,9,9,.... => -2,3,-2,1,-1,3,...
inline const bool tranceZeroPoints (
	RFN::XYP::vXYP_& src,
	std::vector<int>& fgs
) {
	fgs.resize(src.size());
	std::vector<int>::iterator it = fgs.begin();
	bool gg, fg = false;
	ITR::vXYP_Itr itr = src.begin(), edItr = src.end();
	while(itr != edItr) {
		gg = (itr->second > 0.0);
		// ���O�̒l�Ɛ������قȂ�ꍇ
		if (fg != gg) {
			fg = !fg;
			++it;
			*(it) = 0;
		}
		(*it) += (fg) ? 1 : -1;
		++itr;
	}
	// �s�v�ȋ�Ԃ��폜
	fgs.erase(std::remove(fgs.begin(),fgs.end(),0),fgs.end());
	return true;
}
//
// �R�[�h���Ŗ��g�p�B
//// src������ N�_�����\������Ȃ��|�C���g��0�ɂ���B
//// [in] lessnum : ���̐��l�ȉ��̃|�C���g��Ώۂɂ���B
//// [in] zeroPts : �����Ԃ̑O��� "0"�@����������O�񂩁H
//// ex ) less : 1 / zeroPts 2 �Ȃ�
//// * * ! * * * * * * * * ! * * (! �������_)
//// 0 0 3 0 0 2 4 0 9 0 0 9 0 0
//inline const int toZeroLonlyOnlyPoint (
//	RFN::XYP::vXYP_& src, 
//	const unsigned int lessnum, 
//	const unsigned int zeroPts
//){
//	std::vector<int> fgs;
//	tranceZeroPoints(src, fgs);
//	return 0;
//}
//
// underPoint�ȉ��̋�Ԃ�,�u�L���v�ȓ_����݂�
// keepZeroNum �ȏ㑱���ꍇ�͍��B
// �s�[�N�̍\���̊֌W��A�Œ���A�P�_�͕ۏ؂���B
// EX) underPoint = 0, keepZeroNum=2
// ####00000000### �Ȃ� -> (####00)0000(00###) ()�̋�Ԃ��c����
// ####0000### �̗l�ɂ���B
inline const bool deleteSereisOfZeroPoints(
	RFN::XYP::vXYP_&src,
	const unsigned int keepZeroNum
) {
	std::vector<int> fgs;
	unsigned int msCnt(0), deleteNum(0);
	unsigned int zeroNum = (keepZeroNum < 1) ?  1 : keepZeroNum;
	int limitVal = zeroNum * -2;
	// �O�̘A����Ԃ��擾����
	tranceZeroPoints(src, fgs);
	std::vector<int>::iterator it = fgs.begin() , eit = fgs.end();

	while(it != eit) {
		// ����Ԃ���������
		if (*it < limitVal) {
			deleteNum = limitVal - *it;
			msCnt += zeroNum;
			for (unsigned int i = 0; i < deleteNum; i++) {
				src[msCnt].second = -1.0;
				msCnt++;
			}
			msCnt += zeroNum;
		} else {
			msCnt += std::abs(*it);
		}
		++it;
	}
	src.erase(std::remove_if(src.begin(),src.end(), underZeroY()), src.end());

	return true;
}

}// end of namespace
}

