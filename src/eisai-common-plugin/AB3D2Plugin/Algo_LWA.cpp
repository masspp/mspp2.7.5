/*********************************************************/
/*!
 @addtoGroup AB3D
 @file Algo_LWA.cpp
 @brief LWA peak detection(use only AB3D)
 
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

#include "Algo_LWA.h"
#include <tuple>
#include <boost/foreach.hpp>
#include "Thoto\Peaks\PeakBase.hpp"
#include "Thoto\Points\XYPoint.hpp"

//#include "Thoto/Mspp/ConvertXYDatas.hpp"
//#include "Thoto/Mspp/ConvertPeakData.hpp"

namespace EMS {
namespace LWA {

/*
 * Like Private Functions
 * ���l(thrval)�ȉ��̏��o�_��T��(tgtIt)
 * ���̌�A����ȍ~�e�[�����O���鉺�E�l(acutualIt)��T��
 * @breif search FWHM Point
 * @param[in]  coreIt  : start Point iterator
 * @param[in]  endIt   : end of vXYP_ iterator(v.end())
 * @param[in]  thrval  : FWHM High Point (FWHM�̍���)
 * @param[in]  toRight : T search Right / F search Left
 * @param[out] tgtIt   : lessFWHM first Point
 * @param[out] acutualIt : ���܂ŉ�����؂����|�C���g
 */
const bool getBorderPoints(
	const RFN::XYP::vXYP_::iterator& coreIt,
	const RFN::XYP::vXYP_::iterator& endIt,
	const double thrval,
	const bool toRight,
	RFN::XYP::vXYP_::iterator& ref_tgtIt,
	RFN::XYP::vXYP_::iterator& ref_actualIt);


void LWA_FindBasePeaks (
	RFN::XYP::vXYP_& vxyp,
	const double fwhmRatio,
	const double mergeDistance,
	RFN::PEAK::vPKPT_& vPeaks
);

void LWA_Merge (
	RFN::XYP::vXYP_& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const ELM::LRP& fwhm_width,
	RFN::PEAK::vPKPT_& srcPeaks,
	RFN::PEAK::vPKPT_& rstPeaks
);

//---------------------------------------------------------

//ArithmeticGeometricMean
class AriGeoMean {
private:
	const double& intThr_;
	RFN::XYP::XYP_& xyp_;
public : 
	AriGeoMean (const double& intthr, RFN::XYP::XYP_& xyp) : intThr_(intthr), xyp_(xyp) {
		xyp_.first = 0.0;
		xyp_.second = 0.0;
	}
	void operator()(const RFN::XYP::XYP_& src) {
		xyp_.first += src.first * (src.second - intThr_);
		xyp_.second += (src.second - intThr_);
	}
};

//LWALabeling

void onLWALabeling (
	RFN::XYP::vXYP_& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const RFN::LR::LRP_& fwhm_width,
	RFN::PEAK::vPKPT_& rstPeaks
) {
	RFN::PEAK::vPKPT_ vPeaks;
	LWA_FindBasePeaks(vxyp, fwhm_ratio, merge_distance, vPeaks);
	LWA_Merge(vxyp, merge_distance, fwhm_ratio, fwhm_width, vPeaks, rstPeaks);
}

void LWA_FindBasePeaks (
	RFN::XYP::vXYP_& vxyp,
	const double fwhmRatio,
	const double mergeDistance,
	RFN::PEAK::vPKPT_& vPeaks
) {

	typedef RFN::XYP::XYP_ tXYP;
	typedef RFN::XYP::vXYP_::iterator tvXYP_It;

	if (vxyp.empty()) { return;}
	RFN::XYP::sortX(vxyp, false);

	// for work while Itr
	tvXYP_It leftIt, rightIt;
	tvXYP_It leftEndIt, rightEndIt;
	const tvXYP_It beginIt = vxyp.begin();
	const tvXYP_It endIt = vxyp.end();

	// ��_
	tXYP refP, centroidP;
	double intThr(0.0);

	// �x�[�X�̃C�e���[�^
	tvXYP_It baseIt = vxyp.begin();
	++baseIt;

	while(baseIt != endIt) {
		// �N�_�̏��A臒l�̐ݒ�
		if (baseIt->second <= 0.0) {
			++baseIt;
			continue;
		}

		refP = *baseIt;
		intThr = refP.second * fwhmRatio;

		// search Borders
		if (getBorderPoints(baseIt, endIt,   intThr, true,  rightIt, rightEndIt) &&
			getBorderPoints(baseIt, beginIt, intThr, false, leftIt,  leftEndIt)) {

			// Left,Right�̗��[��FWHM�ȉ��̏��o�_�Ȃ̂�
			// �̌v�Z�ɂ͍̗p���Ȃ�
//			AriGeoMean agm(intThr);
			std::for_each(leftIt + 1, rightIt, AriGeoMean(intThr, centroidP));
			centroidP.first /= centroidP.second;
			centroidP.second = refP.second;
			vPeaks.push_back(RFN::PEAK::PKPT_(centroidP, *leftEndIt, *rightEndIt, refP));

			// get FWHM
			// 
			while(leftIt->second < intThr) {++leftIt;}
			while(rightIt->second < intThr) {--rightIt;}
			// ���l���́AintThr�Ƃ�����܂����ʒu����`�⊮�ŋ��߂�B
			vPeaks.back().fwhmVal =
				(leftIt->first - (leftIt-1)->first) * (leftIt->second - intThr) / (leftIt->second - (leftIt-1)->second) +
				(rightIt->first - leftIt->first) +
				((rightIt+1)->first - rightIt->first) * (rightIt->second - intThr) / (rightIt->second - (rightIt+1)->second);
		}
		baseIt = rightEndIt;
	}
	if (vPeaks.empty()) { return; } // no peaks;
}

// back wsAve, totalInt
//std::pair<double,double> setBaseVal(const PKP& src) {
//	return std::make_pair(productCoreXY(src), src.coreP.second);
//}

// get CurrentPeak, Total Base Intensity, wait shift average.
std::tuple<RFN::PEAK::PKPT_, double, double> setBaseVal(const RFN::PEAK::PKPT_& src) {
	return std::make_tuple(src, src.Cp.second, RFN::PEAK::productCoreXY(src));
}


void LWA_Merge (
	RFN::XYP::vXYP_& vxyp,
	const double merge_distance,
	const double fwhm_ratio,
	const RFN::LR::LRP_& fwhm_width,
	RFN::PEAK::vPKPT_& srcPeaks,
	RFN::PEAK::vPKPT_& rstPeaks
) {
	const bool dorevisezero(false);
	if (srcPeaks.empty()) {
		return;
	}
	RFN::PEAK::vPKPT_::iterator peakIt = srcPeaks.begin();
	RFN::PEAK::vPKPT_::iterator peakItEnd = srcPeaks.end();

	rstPeaks.reserve(srcPeaks.size());
	RFN::PEAK::PKPT_ curPk;
	double totalbaseInt, wsAve;

	//PKP curPk = (*peakIt);
	//double totalbaseInt = curPk.coreP.second;
	//double wsAve = productCoreXY(curPk);
	std::tie(curPk, totalbaseInt, wsAve) = setBaseVal(*peakIt);

	for (; peakIt != peakItEnd; ++peakIt) {
		// �}�[�W����臒l�ȓ����𔻒�
		if (distanceCoreX(*peakIt, curPk) > merge_distance) {

			curPk.Cp.first = wsAve / totalbaseInt;
			curPk.Cp.second = curPk.Apex.second;

			// 臒l��Z�߂Ĕ��� FWHM���g��Ȃ��̂őS�� True
			//if (withinThrsholds(vxyp, curPk, fwhm_ratio, fwhm_width, 0)) {
			//	rstPeaks.push_back(curPk);
			//}
			rstPeaks.push_back(curPk);
			std::tie(curPk, totalbaseInt, wsAve) = setBaseVal(*peakIt);
		} else {
			// ���d���ςŌv�Z
			totalbaseInt += (*peakIt).Cp.second;
			wsAve += productCoreXY(*peakIt);
			// �E�[���X�V
			curPk.Rp = (*peakIt).Rp;
			// ���_�����ő�l�ɕύX
			if ((*peakIt).Apex.second > curPk.Apex.second) {
				curPk.Apex = (*peakIt).Apex;
			}
		}
	}
	// Add End Point;
	rstPeaks.push_back(curPk);


	// �s�[�N�[���O�l�␳����ꍇ
	//if (dorevisezero) {
	//	BOOST_FOREACH(PKP& tmp, rstPeaks) {
	//		tmp.leftP.second = 0.0;
	//		tmp.rightP.second = 0.0;
	//	}
	//}
}

//---------------------------------------------------------
// Inner Only
//---------------------------------------------------------

const bool getBorderPoints(
	const RFN::XYP::vXYP_::iterator& coreIt,
	const RFN::XYP::vXYP_::iterator& endIt,
	const double intensityThrshold,
	const bool toRight,
	RFN::XYP::vXYP_::iterator& ref_tgtIt,
	RFN::XYP::vXYP_::iterator& ref_actualIt
){
	// checkPoints
	bool foundPointFg(false);
	bool loopEndFg(false);
	const double refInt = coreIt->second;
	ref_tgtIt = coreIt;

	// found under FWHM's Y Point
	while (!loopEndFg && !foundPointFg) {
		(toRight) ? ++ref_tgtIt : --ref_tgtIt;
		if (ref_tgtIt == endIt) {
			loopEndFg = true;
		} else {
			if (refInt < ref_tgtIt->second) { //ref Point's Y Overed.
				loopEndFg = true;
			} else if (intensityThrshold > ref_tgtIt->second) { //found Points
				foundPointFg = true;
			}
		}
	}
	
	ref_actualIt = ref_tgtIt;
	if (loopEndFg) {return false;}

	if (foundPointFg) {
		double prevVal = ref_actualIt->second;
		loopEndFg = false;
		while (!loopEndFg) {
			(toRight) ? ++ref_actualIt : --ref_actualIt;
			if (ref_actualIt == endIt) {
				loopEndFg = true;
			} else if (ref_actualIt->second < prevVal) {
				prevVal = ref_actualIt->second;
			} else {
				loopEndFg = true;
			}
		}
		// ref_actualIt is overstepped. one step undo.
		(!toRight) ? ++ref_actualIt : --ref_actualIt;
	}
	
	return true;
}

} // end of namespace
}


//inline size_t PointDistance(P2DA_ptr ptr, const PKP& p) {
//	return distance(ptr->searchX(p.leftP.x), ptr->searchX(p.rightP.x));
//}
//
