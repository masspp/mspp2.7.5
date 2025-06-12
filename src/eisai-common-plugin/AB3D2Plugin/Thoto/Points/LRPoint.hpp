/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       LRPoint.hpp
	@brief      LRPoint �� �\��
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

#include "LRPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
const RFN::LR::LRP_ NG_LRP(-1.0,-1.0);
-- RFN::LR --
ELM  LRP        --- std::pair<double,double>
     MZRTP      --- struct{LRP mz, LRP rt}
/ -------------------------------------------------------*/

#include "../Standardinline.hpp"

namespace RFN {
namespace LR {

// L <= TGT <= R �𔻒�
inline bool withinLimits(const double& tgt, const RFN::LR::LRP_& lrp) {
	return RFN::STD::withinLimits(tgt, lrp.first, lrp.second);
}

// MZRT �́@MZ �́@�͈͓��ɂ��邩�ۂ�
inline bool withinMZLimits(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
	return RFN::LR::withinLimits(tgt, mzrt.mz);
}

// MZRT �́@RT �́@�͈͓��ɂ��邩�ۂ�
inline bool withinRTLimits(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
	return RFN::LR::withinLimits(tgt, mzrt.rt);
}

//// L-R �͈̔͂ɑ΂��Atgt�@���@������ : 0 ������ -1,�@�傫�� :  1
//inline const int compLRP(const double& tgt, const RFN::LR::LRP_& lrp) {
//	return RFN::STD::compVal_d(lrp.first, lrp.second, tgt);
//}

//// MZRT �� MZ �͈͂ɑ΂��Atgt�@���@������ : 0 ������ -1,�@�傫�� :  1
//inline const int compMZRT_MZ(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
//	return RFN::LR::compLRP(tgt, mzrt.mz);
//}

//// MZRT �� RT �͈͂ɑ΂��Atgt�@���@������ : 0 ������ -1,�@�傫�� :  1
//inline const int compMZRT_RT(const double& tgt, const RFN::LR::MZRTP_& mzrt) {
//	return RFN::LR::compLRP(tgt, mzrt.mz);
//}

//// tgt - tol, tgt + tol�@��Ԃ��iABS)
//inline ELM::LRP createLRP_withABSTol(const double& tgt, const double& tol) {
//	return ELM::LRP(tgt - tol, tgt + tol);
//}
//
//// tol = (tgt * PPM), tgt - tol, tgt + tol�@��Ԃ��iPPM)
//// PPM��Ratio�����Ȃ��œn���� (10ppm�Ȃ�10�œn��)
//inline ELM::LRP createLRP_withPPMTol(const double& tgt, const double& ppmtol) {
//	const double tol = RFN::STD::getPPMTol(tgt, ppmtol);
//	return ELM::LRP(tgt - tol, tgt + tol);
//}

//// LRP * 2 (MZ,RT) to MZRT
//inline void setMZRT(const ELM::LRP& mzp, const ELM::LRP& rtp, ELM::MZRTP& tgt) {
//	tgt.mz = mzp;
//	tgt.rt = rtp;
//}

//// �}�C�i�X�̒l���n�����ꍇ�͍ő�A�ŏ��l�������ĕԂ�
//inline ELM::LRP createLRP_NagaToInfinity(const double& dlow, const double& dmax) {
//	double l = (dlow < 0.0) ? -1.0 : dlow;
//	double r = (dmax < 0.0) ? DBL_MAX : dmax;
//	return ELM::LRP(l, r);
//}

} // end of namespace LR
} // end of namespace RFN
