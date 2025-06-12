/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       LRCPoint.hpp
	@brief      Left - Center - Right
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/
#pragma once

#include "LCRPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
NG_LCRP(-1.0);
-- RFN::LCR --
ELM  LCRP      --- struct{double l(left), c(center), r(right)
/ -------------------------------------------------------*/

#include "../Standardinline.hpp"

namespace RFN {
namespace LCR {

//// $$$ �g�p�ӏ�����#2014.01.23
//// tgt���@LCRP���ɂ��邩�ۂ�
//inline bool withinLCRP(const RFN::LCR::LCRP_& lcrp, const double& tgt) {
//	return RFN::STD::eq_less(lcrp.l, tgt) && RFN::STD::eq_less(tgt, lcrp.r);
//}

//// $$$ �g�p�ӏ�����#2014.01.23
//// LCRP��Tol���ō쐬���ĕԂ�
//inline ELM::LCRP createLCRPwithTol(const double& val, const double& tol) {
//	return ELM::LCRP(val - tol, val, val + tol);
//}

// LCRP��Tol���ōđ������
inline void setLCRP(const double& val, const double& tol, ELM::LCRP& tgt_lcrp) {
	tgt_lcrp.c = val;
	tgt_lcrp.l = val - tol;
	tgt_lcrp.r = val + tol;
}

inline ELM::LCRP setLCRP(const double& val, const double& tol) {
	ELM::LCRP rst_lcrp; 
	rst_lcrp.c = val;
	rst_lcrp.l = val - tol;
	rst_lcrp.r = val + tol;
	return rst_lcrp;
}

//// $$$ �g�p�ӏ�����#2014.01.23
//// LCRP���쐬����B
//inline ELM::LCRP toLCRP(const double& val) {
//	return ELM::LCRP(val);
//}
//
//// LCRP���쐬����B
//inline ELM::LCRP toLCRP_tol(const double& val, const double& tol) {
//	return ELM::LCRP(val - tol, val, val + tol);
//}
//
//// LCRP���쐬����B
//inline ELM::LCRP toLCRP(const double& l, const double& c, const double& r) {
//	return ELM::LCRP(l,c,r);
//}

}
}