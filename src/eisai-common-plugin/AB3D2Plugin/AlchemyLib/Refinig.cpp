/*********************************************************/
/*!
 @addtoGroup AB3D
 @file Refining.cpp
 @brief select spectra in the range
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/

#pragma warning(disable:4482)
#include "../stdafx.h"
#include "Refining.h"
#include <utility>
#include <tuple>
#include "../Thoto/Standardinline.hpp"
namespace rfn {
namespace rfng { 


const unsigned int kFirst(0);

// �O���錾��
// ���̊֐��Q���Ŏg���w�b�_�ɏo���Ȃ�Private�֐����̐錾�݂̂��s���B

const unsigned int onSearchIndex (kome::objects::DataSet&, const double, const unsigned int, const unsigned int);
const unsigned int onSearchIndexFullRange (kome::objects::DataSet&, const double&);
//const unsigned int onCompareRT(kome::objects::DataSet&, const double&);
const bool onGetRTRangeIndexes(kome::objects::DataSet&, const ELM::LRP&, unsigned int&, unsigned int&);

// �ȍ~���̌n===========================================
const std::pair<double,double> get_dataSetRTRange(kome::objects::DataSet& dataSet) {
	const unsigned int fx = 0;
	const unsigned int ex = dataSet.getNumberOfSpectra() - 1;
	return std::make_pair(dataSet.getSpectrum(fx)->getRt(),dataSet.getSpectrum(ex)->getRt());
}

//// DataSet �����v����RT���擾����
//const int searchSpecIndex_FromRT (
//	kome::objects::DataSet& dataSet, 
//	const double tgtrt,
//	unsigned int& rstIdx
//) {
//	if (dataSet.getNumberOfSpectra() < 1) return -2;
//	int xflag = onCompareRT(dataSet, tgtrt);
//	if (xflag == 0) {
//		rstIdx = onSearchIndexFullRange(dataSet, tgtrt);
//	}
//	return xflag;
//}

//const bool isMatchChrom(kome::objects::Spectrum* spec, const bool haschrom, unsigned int msstage) {
const bool isMatchChrom(kome::objects::Spectrum* spec, const ALC::CHROM_FLAG& chflag, unsigned int msstage) {
	if (chflag != ALC::CHROM_FLAG::NO_USE) {
		if (spec->hasChromatogram() != (chflag & ALC::CHROM_FLAG::HAS_CHROM)) {
			return false;
		}
	}
	// msstage ���O�Ȃ� Full Range.
	if (msstage > 0 && spec->getMsStage() != msstage) {
		return false;
	}
	return true;
}

// DataSet ����Chromatogram�̏����Ɉ�v����
// Spectrum* �̔z����擾����
const bool refiningSpectra (
	kome::objects::DataSet& dataSet, // In
	const ELM::LRP& RTp,
	const ALC::CHROM_FLAG& ch_flag,
//	const bool haschrom,
	unsigned int msstage,
	std::vector<kome::objects::Spectrum*>& refSpec
){
	unsigned int lidx, ridx;
//	if (!onGetRTRangeIndexes(dataSet, minRT, maxRT, lidx, ridx)) {
	if (!onGetRTRangeIndexes(dataSet, RTp, lidx, ridx)) {
		return false;
	}
	if (ridx - lidx <= 0) {
		return false;
	}
	refSpec.reserve(ridx - lidx);
	kome::objects::Spectrum* tmpspec;
	// Index�͈̔͂�������ɍ���Spectrum��Vector�ɓ����
//	for (unsigned int i = lidx; i <= ridx; i++) {
	for (unsigned int i = lidx; i < ridx; i++) {
		tmpspec = dataSet.getSpectrum(i);
		if (isMatchChrom(tmpspec, ch_flag, msstage)) {
			refSpec.push_back(tmpspec);
		}
	}
	refSpec.shrink_to_fit();
	return true;
}

//template<typename T_n>
//const bool auditPtr(const T_n* tnA, const T_n* tnB, T_n& refA, T_n& refB) {
//	if (tnA != nullptr) refA = *tnA;
//	if (tnB != nullptr) refB = *tnB;
//	return (refA < refB) ? true : false;
//}


//---------------------------------------------------------
//  INNER FUNCTIONS
//---------------------------------------------------------

/*!
 * @breif �ߎ�RT������Index���擾
 * @param[in] dataSet : Mass++�̃f�[�^�Z�b�g
 * @param[in] tgtrt   : �����Ώۂ�RT�l
 * @param[in] lidx    : dataSet�̍���Index
 * @param[in] ridx    : dataSet�̉E��Index
 * @result : �ނ�RT�ɋ߂�Index
 * @note : ���_�I�ɂ� log(N)�̌v�Z�ʁB���S��v�ł͂Ȃ��̂Œ��ӁB
*/
const unsigned int onSearchIndex (kome::objects::DataSet& dataSet, const double tgtrt, const unsigned int lidx, const unsigned int ridx) {
	unsigned int imin(lidx), imax(ridx),imid(0);
	double tmpVal;
	while (imin < imax) {
		imid = (imin + imax) / 2;
		tmpVal = dataSet.getSpectrum(imid)->getRt();
		if (tmpVal < tgtrt) {
			imin = imid + 1;
		} else if (tmpVal == tgtrt) {
			return imid;
		} else {
			imax = imid - 1;
		} 
	}

	if (dataSet.getSpectrum(imin)->getRt() < tgtrt) imin++;
	return imin;
}

// �ߎ�RT������Index��DataSet�S�̂���擾����B
// "dataSet���ł���"�Ƃ����O��̂��ƂɎg�p���鎖�B
const unsigned int onSearchIndexFullRange (kome::objects::DataSet& dataSet, const double& tgtrt) {
	const unsigned int firstidx = 0;
	const unsigned int lastidx = dataSet.getNumberOfSpectra() - 1;
	return onSearchIndex(dataSet, tgtrt, firstidx, lastidx);
}

//const unsigned int onCompareRT(kome::objects::DataSet& dataSet, const double& tgtrt) {
//	if (dataSet.getNumberOfSpectra() < 2) return J_ERROR;
//	double lrt, rrt;
//	std::tie(lrt, rrt) = get_dataSetRTRange(dataSet);
//	return RFN::STD::compVal_d(lrt,rrt,tgtrt);
//}

const bool onGetRTRangeIndexes(
	kome::objects::DataSet& dataSet, const ELM::LRP& RTp,
	unsigned int& lidx, unsigned int& ridx
) {
	const unsigned int specNum = dataSet.getNumberOfSpectra();
	if (specNum < 1) {
		return false;
	}
	lidx = 0;
	ridx = specNum - 1;

	double tlow, thigh;
	std::tie(tlow, thigh) = get_dataSetRTRange(dataSet);

	// ���ꂩ�̃^�C�~���O�Ŋ֐�������
	//�@�T���v����̍ő�RT���@�v���͈͂̍Œ�l������
	if (RTp.first > 0.0 && RTp.first > thigh) {
		return false;
	}

	//�@�T���v����̍ŏ�RT���@�v���͈͂̍ő�l���Ⴂ
	if (RTp.second > 0.0 && RTp.second < tlow) {
		return false;
	}

	if (RFN::STD::compVal_d(tlow, thigh, RTp.first) == 0) {
		lidx = onSearchIndexFullRange(dataSet, RTp.first);
	}

	if (RFN::STD::compVal_d(tlow, thigh, RTp.second) == 0) {
		ridx = onSearchIndexFullRange(dataSet, RTp.second);
	}
	return true;
}

} //end of .....
}
