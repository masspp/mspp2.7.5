/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Peaks
    @file       PeakBase.hpp
	@brief      Peak �̍\����`
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
	@note
		mass++���̃s�[�N�\�����\���̉��������́B
****************************************************************/
#pragma once

#include "PeakBase_Def.hpp"
/* ------------ RFN::PEAKS ------------------------------ /
ELM  PKPT      --- struct {ELM::XYP Cp(Center), Lp(Left), Rp(Right), Apex(���_���W)}
     vPKPT     --- std::vector<PKPT>
ITR  vPKPT_Itr --- vPKPT::iterator
/ -------------------------------------------------------*/

#include "../Points/XYPoint.hpp"
#include "../Points/LRPoint.hpp"
/* ------------ RFN::XYP   ----------------------------- /
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*
XYP NG_XYP(-1.0,-1.0)
/* ------------ RFN::LR   ----------------------------- /
ELM  LRP        --- std::pair<double,double>
     MZRTP      --- struct{LRP mz, LRP rt}
LRP NG_LRP(-1.0,-1.0)
/ -------------------------------------------------------*/
//#include <tuple> Need Me

namespace RFN {
namespace PEAK{
// ---------------------------------------------------------
// �֐��I�u�W�F�N�g
// --------------------------------------------------------
struct lessCPX : public std::binary_function<const ELM::PKPT&, const ELM::PKPT&, bool> {
	bool operator()(const ELM::PKPT& lp, const ELM::PKPT& rp) const {
		return lp.Cp.first < rp.Cp.first;
	}
};

// ---------------------------------------------------------
// ������
// --------------------------------------------------------

// product cp.x * cp.y (M/Z or RT * Intensity)
inline double productCoreXY(const ELM::PKPT& pks) {
	return RFN::XYP::productXY(pks.Cp);
}

// distance L.cp.x to R.cp.x(x as first as M/Z or RT)
inline double distanceCoreX(const ELM::PKPT& lpks, const ELM::PKPT& rpks) {
	return fabs(lpks.Cp.first - rpks.Cp.first);
}

// sort by center x (M/Z or RT)
inline void sortByX(ELM::vPKPT& vpkpt) {
	std::sort(vpkpt.begin(), vpkpt.end(), lessCPX());
}

//// 2014.01.22 ���L�̃R�[�h���폜����
//// get Peaks Edge (Left & Right) Point Iterators. 
//// #include <tuple> and recieve std::tie(Left, Right))
//inline std::pair<ITR::vXYP_Itr, ITR::vXYP_Itr>
//	getPeakEdgePoint(ELM::vXYP& src_vxyp, const ELM::PKPT& peakpt) {
//		ITR::vXYP_Itr lt = RFN::XYP::nearSearchX(peakpt.Lp.first, src_vxyp);
//		ITR::vXYP_Itr rt = RFN::XYP::nearSearchX(peakpt.Rp.first, src_vxyp);
//		return std::make_pair(lt, rt);
//}
//
//// ������ withinThrsholds�ɌĂяo����Ă������A
//// AB3D����FWHM�̔�����g�p���Ȃ��̂ŁA���̃R�[�h�͎g���Ȃ��B
//// is within Fwhm Thresholds (Min-Max Range)
//inline bool withinFwhmThrFrom_L_R_Edge (
//	const double& fwhmhigh,
//	const ELM::LRP& fwhm_width,
//	ITR::vXYP_Itr& lt,
//	ITR::vXYP_Itr& rt
//){
//	//�[���璆���Ɍ�����fwhmhigh��荂���Ȃ�ŏ��̓_���C�e���[�g����B
//	while(lt->second < fwhmhigh) { ++lt;}
//	while(rt->second < fwhmhigh) { --rt;}
//	// ���f�[�^�̔��l�����Z�o����
//	double calcfwhm_width = rt->first - lt->first;
//	// FWHM��臒l���O�𔻒�
//	if (RFN::LR::withinLimits(calcfwhm_width, fwhm_width)) {
//		return true;
//	}
//	return false;
//}
//
//// LWA�ł̔���ȊO�Ɏg���Ă��Ȃ�
//inline bool withinThrsholds(
//	ELM::vXYP& vxyp,
//	const ELM::PKPT& tgtpeak,
//	const double fwhm_ratio,
//	const ELM::LRP& fwhm_width,
//	const unsigned int limit_point
//) {
//	// ���f�[�^�����s�[�N�̗��[���W���擾�B
//	// @@@�����͍X�ɕ������Đ��K�������ق����ǂ������B
//	ITR::vXYP_Itr lt, rt;
//	std::tie(lt, rt) = RFN::PEAK::getPeakEdgePoint(vxyp, tgtpeak);
//	
//	if (std::distance(lt,rt) < limit_point) { 
//		return false;
//	}
//	if (fwhm_width != RFN::LR::NG_LRP) {
//		// ���l���̔���
//		double fwhmhigh = tgtpeak.Cp.second * fwhm_ratio;
//		if (RFN::PEAK::withinFwhmThrFrom_L_R_Edge (fwhmhigh, fwhm_width, lt, rt)) {
//			return false;
//		}
//	}
//	return true;
//}
//
//// ���l���̌v�Z�[�B
//inline const double getFWHMValue(ELM::vXYP& vxyp, const ELM::PKPT& tgtpeak, const double fwhmhigh) {
//	ITR::vXYP_Itr lt, rt;
//	std::tie(lt, rt) = RFN::PEAK::getPeakEdgePoint(vxyp, tgtpeak);
//	while(lt->second < fwhmhigh) { ++lt;}
//	while(rt->second < fwhmhigh) { --rt;}
//	// ���f�[�^�̔��l�����Z�o����
//	return rt->first - lt->first;
//}

}//end of namespace
}