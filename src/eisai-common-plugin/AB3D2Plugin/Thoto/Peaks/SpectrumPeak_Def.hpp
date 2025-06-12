///****************************************************************
//  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
//-----------------------------------------------------------------
//    @addtogroup Refine_Thoto_Peaks
//    @file       PeakBase_Def.hpp
//	@brief      spectrum Peaks �̍\����`
//	@date       2012.12.13
//	@author     M.Fukuda
//	@version    1.2.0
//	@note
//		spectrum�P�ʂł̃s�[�N���Ǘ�����\���́B
//		�����ꎞ�I�u�W�F�N�g�B
//****************************************************************/
//#pragma once
//
//#include "PeakBase_Def.hpp"
///* --- RFN::PEAKS --------------------------------------- /
//ELM  PKPT      --- struct {ELM::XYP Cp(Center), Lp(Left), Rp(Right), Apex(���_���W)}
//     vPKPT     --- std::vector<PKPT>
//ITR  vPKPT_Itr --- vPKPT::iterator
//SPTR vPKPT_Ptr --- vPKPT*
/// -------------------------------------------------------*/
//
//namespace RFN  {
//namespace PEAK {
//
//// spectrum peaks value struct
//struct specPeakVal {
//	double rt;              // Retention time
//	SPTR::vPKPT_Ptr peaks_; // spectram's peaks values
//	// constructor
//	specPeakVal(const double& _rt) 
//		: rt(_rt), peaks_(new ELM::vPKPT()) {}
//	specPeakVal(const double& _rt, const SPTR::vPKPT_Ptr pkpt) 
//		: rt(_rt), peaks_(new ELM::vPKPT()){peaks_ = pkpt;}
//	// destrctor
//	~specPeakVal(void) {
//		if (peaks_ != nullptr) {delete peaks_;}
//	}
//};
//
//} // end of namespace PEAK
//} // end of namespace RFN
//
//// Typedef For Point Objects ELEMENT
//namespace ELM {
//	typedef RFN::PEAK::specPeakVal SPEC_PK;
//	typedef std::vector<SPEC_PK> vSPEC_PKs;
//}
