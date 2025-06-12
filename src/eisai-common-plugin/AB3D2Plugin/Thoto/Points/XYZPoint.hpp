/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Refine_Thoto_Points
    @file       XYZPoint.hpp
	@brief      <X,Y> * N ,Z �̎�����
	@date       2012.12.12
	@author     M.Fukuda
	@version    1.2.0
	@note
		�v�f���|�C���^�Ŋi�[����Ă��܂��B
		�f�X�g���N�^�Ƀ|�C���^�j����
		�I�u�W�F�N�g�̃R�s�[�̍ۂ͒��ӂ��ĉ������B
****************************************************************/
#pragma once

#include "XYZPoint_Def.hpp"
/* ------ TYPEDEF Infomations --------------------------- /
const XYP_ NG_XYP(-1.0,-1.0);

-- RFN::XYP --
ELM  XYP       --- std::pair<double,double>
     vXYP      --- std::vector<XYP>
ITR  vXYP_Itr  --- vXYP::iterator
	 vXYP_cItr --- vXYP::const_iterator
SPTR vXYP_sp   --- vXYP*

-- RFN::XYZP --
ELM  XYZP      --- struct {double z, SPTR::vXYZ_sp}
     vXYZP     --- std::vector<XYZPt>
ITR  vXYZP_Itr --- vXYZP::iterator
/ -------------------------------------------------------*/

#include <algorithm>

#include "../Standardinline.hpp"

namespace RFN {
namespace XYZP {

// ---------------------------------------------------------
// �֐��I�u�W�F�N�g
// --------------------------------------------------------
struct lessZ : public std::binary_function<const XYZP_&, const XYZP_&, bool> {
	bool operator()(const RFN::XYZP::XYZP_& lxyz, const RFN::XYZP::XYZP_& rxyz) const {
		return lxyz.z < rxyz.z;
	}
};

struct compZ { 
	bool operator()(const RFN::XYZP::XYZP_& lxyz, double rx)  { return lxyz.z < rx; }
	bool operator()(double lx, const RFN::XYZP::XYZP_& rxyz) { return lx < rxyz.z; }
	bool operator()(const RFN::XYZP::XYZP_& lxyz, const RFN::XYZP::XYZP_& rxyz) { return lxyz.z < rxyz.z; }
};

// ---------------------------------------------------------
// ������
// ---------------------------------------------------------

// �|�C���g���쐬����
inline void addZPoint(const double& z, const RFN::XYP::vXYP_&src, RFN::XYZP::vXYZP_& vxyzp) {
	vxyzp.push_back(XYZP_(z, src));
}

// �|�C���g���쐬���āA���̃|�C���^��Ⴄ
inline SPTR::vXYP_sp createPoint_Sp(const double& z, RFN::XYZP::vXYZP_& vxyzp) {
	vxyzp.push_back(XYZP_(z));
	vxyzp.back().vxyp = new ELM::vXYP();
	return vxyzp.back().vxyp;
}

// �|�C���g���쐬���āA���̃I�u�W�F�N�g��Ⴄ
inline RFN::XYP::vXYP_& createPoint(const double& z, RFN::XYZP::vXYZP_& vxyzp) {
	vxyzp.push_back(XYZP_(z));
	return *vxyzp.back().vxyp;
}

// Z�Ń\�[�g����
inline void sortZ(RFN::XYZP::vXYZP_& vxyzp, const bool desc = false) {
	if (!desc) {
		std::sort(vxyzp.begin(), vxyzp.end(), lessZ());
	} else {
		std::sort(vxyzp.begin(), vxyzp.end(), std::not2(lessZ()));
	}
}

// �ߖT(<=)�̒��ߒl��T���B
inline RFN::XYZP::vXYZP_::iterator nearSearchZ(const double& z, RFN::XYZP::vXYZP_& vxyzp) {
	return std::lower_bound(vxyzp.begin(), vxyzp.end(), z, compZ());
}

inline RFN::XYZP::vXYZP_::iterator nearSearchZ(const double& z, const RFN::XYZP::vXYZP_::iterator& fst, RFN::XYZP::vXYZP_& vxyzp) {
	return std::lower_bound(fst, vxyzp.end(), z, compZ());
}

// vxyzp ���� minZ, maxZ�͈̔͂����C�e���[�^��T���B
inline const std::pair<ITR::vXYZP_Itr, ITR::vXYZP_Itr>
	searchZ_MinMax(
		const double& minZ, const double& maxZ, ELM::vXYZP& vxyzp
){
	// �f�t�H���g��ݒ肷��
	ITR::vXYZP_Itr lowit = vxyzp.end();
	ITR::vXYZP_Itr highit = vxyzp.end();

	if (!vxyzp.empty() && vxyzp[0].z <= maxZ) {	
		size_t num = vxyzp.size();
		const double arrlow(vxyzp[0].z), arrhigh(vxyzp[num-1].z);
		if (minZ < arrlow) { // �z��̍ŏ��l�ł���ΐ擪��Ԃ��B
			lowit = vxyzp.begin();
		} else if (RFN::STD::eq_less(minZ, arrhigh)) {
			lowit = nearSearchZ(minZ, vxyzp);
		}
		if (maxZ < 0.0 || arrhigh < maxZ) { // �����w��A�z����傫���ꍇ�͖��[��Ԃ�
			highit = vxyzp.end();
		} else if (RFN::STD::eq_less(arrlow, maxZ)) {
			highit = nearSearchZ(maxZ, lowit, vxyzp);
		}
	}
	return std::make_pair(lowit, highit);
}

// vxyzp ���� minZ, maxZ�͈̔͂����C�e���[�^��T���B
inline void
	searchZ_MinMaxF(
		const double& minZ, const double& maxZ, ELM::vXYZP& vxyzp, ITR::vXYZP_Itr& lowit, ITR::vXYZP_Itr& highit
){
	// �f�t�H���g��ݒ肷��
	lowit = vxyzp.end();
	highit = vxyzp.end();
	size_t num = vxyzp.size();
	const double arrlow(vxyzp[0].z), arrhigh(vxyzp[num-1].z);

	if (minZ < arrlow) { // �z��̍ŏ��l�ł���ΐ擪��Ԃ��B
		lowit = vxyzp.begin();
	} else if (RFN::STD::eq_less(minZ, arrhigh)) {
		lowit = nearSearchZ(minZ, vxyzp);
	}
	if (maxZ < 0.0 || arrhigh < maxZ) { // �����w��A�z����傫���ꍇ�͖��[��Ԃ�
		highit = vxyzp.end();
	} else if (RFN::STD::eq_less(arrlow, maxZ)) {
		highit = nearSearchZ(maxZ, lowit, vxyzp);
	}
}

} // end of namespace XYZP
}
