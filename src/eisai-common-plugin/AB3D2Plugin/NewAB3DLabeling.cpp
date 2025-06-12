/*********************************************************/
/*!
 @addtoGroup AB3D
 @file NewAB3DLabeling.cpp
 @brief AB3DLabeling Functions.
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/
#include "StdAfx.h"
#include "NewAB3DLabeling.h"

#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/timer.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/algorithm/string/split.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Algo_LWA.h"

// memo 2013.08.01
// DB�֘A���R���g���[�����܂��B
// PeakPosition(Matrix��)�ׂ̈̃f�[�^���m�ۂ��Ă��܂��B
#include "DBTools\DbController.h"

// memo 2013.08.01
// Mass++�̃v���O���X�����b�v����N���X�ł��B
// �Ăяo���葱�����ώG�Ȃ̂ō���Ă݂܂����B
#include "progress\progressAB3D.h"

// �� Trading poist�̃v���W�F�N�g�Ƃ��ĊO�ɂ��������̂�
// �N���X������ ���̃v���W�F�N�g�̒��ɓ���Ă���܂��B
#include "./AlchemyLib/TragingPost.h"

#include "Thoto/Peaks/Peak3DBase.hpp"
//#include "Thoto/Points/XYPoint.hpp"
//#include "Thoto/Points/XYZPoint.hpp"

const double kPPM(0.000001);

// param(Range)�̕����֐��B
const ELM::LRP splitSettingVal(const std::string& tgtStr){
	ELM::LRP rst;
	if (tgtStr.empty()) return rst;

	std::vector<std::string> tmp;
	boost::algorithm::split(tmp, tgtStr, std::bind2nd(std::equal_to<char>(), ':'));
	if (tmp.empty()) {
		return rst;
	}
	rst.first = (tmp[0].size() > 0) ? boost::lexical_cast<double>(tmp[0]) : -1.0;
	if (tmp.size() > 1) {
		rst.second = (tmp[1].size() > 0) ? boost::lexical_cast<double>(tmp[1]) : -1.0;
	}
	return rst;
}

bool getDataPointsByRT(ELM::vXYZP& vxyzp, double rt, RFN::XYP::vXYP_& vxyp) {
	for (ELM::vXYZP::iterator it = vxyzp.begin(); it != vxyzp.end(); ++it) {
		if (it->z == rt) {
			vxyp = *it->vxyp;
			return true;
		}
	}
	return false;
}

vPeak3D::iterator NewAB3DLabeling::searchMZRange(
	vPeak3D::iterator it, 
	vPeak3D::iterator& endIt, 
	const double& rtTol, 
	const bool& direction, 
	ELM::LCRP& mzRange
) {
	const double currRt = (*it)->rt.c;
	vPeak3D::iterator retIt = it;
	while (it != endIt) {
		if (direction) {
			++it;
		} else {
			--it;
		}

		BS::Peak3D* pk3d = (*it);
		if (abs(pk3d->rt.c - currRt) > rtTol) {
			break;
		}

		// m/z�͈̔͂��擾����B
		mzRange.l = std::min(mzRange.l, pk3d->mz.c);
		mzRange.r = std::max(mzRange.r, pk3d->mz.c);
		retIt = it;
	}
	return retIt;
}


bool NewAB3DLabeling::detectMZRangeByPeakPositions(
		Ptr& peak, vPeak3D& peaks,
		const double& rtTol, const double& intensityThreshold,
		ELM::LCRP& mzRange) {

	// Intensity�̏����𖞂����͈͓��̃s�[�N���擾���āART�Ń\�[�g����B
	vPeak3D peaksSortedByRT;
	Cnum::getPeaksAboveIntensity(
		peaks, intensityThreshold, peaksSortedByRT);
	Cnum::sortByRTcenter(peaksSortedByRT);

	vPeak3D::iterator currItr, lItr, rItr;
	// RT�̑o�����Ɍ������邽�߂ɁA��ƂȂ�s�[�N��Iterator���擾����B
	currItr = std::find(peaksSortedByRT.begin(), peaksSortedByRT.end(), peak);
	// RT�̐������͈̔͌���
	rItr = searchMZRange(currItr, peaksSortedByRT.end()-1, rtTol, true, mzRange);
	// RT�̕������͈̔͌���
	lItr = searchMZRange(currItr, peaksSortedByRT.begin(), rtTol, false, mzRange);

	return true;
}

bool NewAB3DLabeling::getXICPeak(ELM::vXYP& xic, const double& rt, ELM::PKPT& xicPeak) {
	const double XIC_LWA_RT_MERGE_DISTANCE = 0;
	const double XIC_LWA_FWHM_RATIO = getSettingMapDouble("MergeDataPointsWeightOfLocalMaximum");
	const ELM::LRP XIC_LWA_FWHM_WIDTH(RFN::LR::NG_LRP);

	double minRTDiff = 100.0;
	double rtDiff;

	// LWA�ɂ��s�[�N�s�b�L���O���s���B
	ELM::vPKPT xicPeaks;
	EMS::LWA::onLWALabeling(xic, XIC_LWA_RT_MERGE_DISTANCE, XIC_LWA_FWHM_RATIO, XIC_LWA_FWHM_WIDTH, xicPeaks);

	// LWA�ɂ��擾�����s�[�N�̒��ŁA�Ώۂ̃s�[�N��RT�ƍł��߂��s�[�N���擾����B
	for (ELM::vPKPT::iterator xicPIt = xicPeaks.begin(); xicPIt != xicPeaks.end(); ++xicPIt) {
		rtDiff = abs(xicPIt->Apex.first - rt);
		if (rtDiff < minRTDiff && xicPIt->Lp.first <= rt && rt <= xicPIt->Rp.first) {
			xicPeak = *xicPIt;
			minRTDiff = rtDiff;
		}
		if (rtDiff == 0.0) break;
	}
	return (minRTDiff == 0);
}

inline double NewAB3DLabeling::calcAreaByXIC(
		ELM::vXYP& xic, const double& lowerRT, const double& upperRT) {
	double area = 0.0;
	for (ELM::vXYP::iterator xicItr = xic.begin(); xicItr != xic.end(); ++xicItr) {
		if (lowerRT <= (*xicItr).first && (*xicItr).first <= upperRT) {
			area += (*xicItr).second;
		}
	}
	return area;
}

void NewAB3DLabeling::deleteShoulderPeaks(vPeak3D& detectedPeaks) {

	const double INTENSITY_RATIO = getSettingMapDouble("RemoveShoulderPeaksIntRatio");
	const double RT_DIFF = getSettingMapDouble("RemoveShoulderPeaksRtTol");
	const double MZ_DIFF_PPM = getSettingMapDouble("RemoveShoulderPeaksMzTol");
	//const double MZ_DIFF = MZ_DIFF_PPM * 1/1000000;
	const double MZ_DIFF = MZ_DIFF_PPM * kPPM;

	Cnum::sortByIntensity_DESC(detectedPeaks);

	// �s�[�N�̃��X�g���R�s�[���ART�Ń\�[�g����B
	vPeak3D pksSortedByMZ = detectedPeaks;
	//vPeak3D tgtPks;
	vPeak3D rejectPks;

	Cnum::sortByMZcenter(pksSortedByMZ);

	for (vPeak3D::iterator pkItr = detectedPeaks.begin(); pkItr != detectedPeaks.end(); ++pkItr) {
		const double intThr = (*pkItr)->intensity / INTENSITY_RATIO;
		const double mzDiff = (*pkItr)->mz.c * MZ_DIFF;
		const double rtLower = (*pkItr)->rt.l - RT_DIFF;
		const double rtUpper = (*pkItr)->rt.r + RT_DIFF;

		vPeak3D tgtPks;
		if (!Cnum::getPeaksBelowIntensityFromSortedByMz(
				pksSortedByMZ, intThr,
				(*pkItr)->mz.l - mzDiff, (*pkItr)->mz.r + mzDiff,
				rtLower, rtUpper, tgtPks)) {
			continue;
		}

		Cnum::setFlagToPeaks(tgtPks, false);
	}
	Cnum::removePeaksFromList(detectedPeaks);
}

// �A������s�[�N�̏���
void NewAB3DLabeling::removeContinuousPeaks(vPeak3D& detectedPeaks, vPeak3D& clusterCandidatePeaks) {
	
	const double REMOVE_CONTINUOUS_PEAKS_MZ_PPM_TOLERANCE = getSettingMapDouble("RemoveContinuousPeaksMzTol");
	const double REMOVE_CONTINUOUS_PEAKS_MZ_RATIO = REMOVE_CONTINUOUS_PEAKS_MZ_PPM_TOLERANCE * kPPM;
	const double REMOVE_CONTINUOUS_PEAKS_RT_TOLERANCE = getSettingMapDouble("RemoveContinuousPeaksRtTol");
	const double REMOVE_CONTINUOUS_PEAKS_INTENSITY_RATIO = getSettingMapDouble("RemoveContinuousPeaksIntRatio");
	
	vPeak3D::iterator tmpItr;
	vPeak3D nearMZPks;
	double currMZ, lowerMZ, upperMZ;
	double upperRT, lowerRT;
	double intThreshold;
	double threshold;

	// �|�����t���O�����ɖ߂��B
	Cnum::setFlagToPeaks(detectedPeaks, true);

	vPeak3D pksSortedByMZ = detectedPeaks;
	Cnum::sortByMZcenter(pksSortedByMZ);

	Cnum::sortByIntensity_DESC(detectedPeaks);
	for (vPeak3D::iterator pkItr = detectedPeaks.begin(); pkItr != detectedPeaks.end(); ++pkItr) {
		if (!(*pkItr)->sflag) {
			continue;
		}

		intThreshold = (*pkItr)->intensity * REMOVE_CONTINUOUS_PEAKS_INTENSITY_RATIO;
		currMZ = (*pkItr)->mz.c;
		lowerMZ = currMZ - REMOVE_CONTINUOUS_PEAKS_MZ_RATIO * currMZ;
		upperMZ = currMZ + REMOVE_CONTINUOUS_PEAKS_MZ_RATIO * currMZ;

		// MZ���ߖT�̃s�[�N���X�g���N���A����B
		nearMZPks.clear();
		// MZ���ߖT�̃s�[�N���擾����B
		// Intensity��+1����̂́A���ꎩ�g�̃s�[�N���擾���������߁B
		if (!Cnum::getPeaksBelowIntensityFromSortedByMz(
			pksSortedByMZ, (*pkItr)->intensity,
			lowerMZ, upperMZ, -1, -1, nearMZPks)) {
			// m/z�̋ߖT�ɑ��݂���s�[�N�����݂��Ȃ��ꍇ
			continue;
		}
		// �������g�̃s�[�N��ǉ�����B 
		nearMZPks.push_back(*pkItr);
		// MZ���ߖT�̃s�[�N���X�g��Retention Time�Ń\�[�g����B
		Cnum::sortByRTcenter(nearMZPks);

		// �Ώۂ̃s�[�N�����������̏���
		upperRT = (*pkItr)->rt.c;
		lowerRT = (*pkItr)->rt.c;
		for (tmpItr = nearMZPks.begin(); tmpItr != nearMZPks.end(); ++tmpItr) {
			if ((*tmpItr)->rt.c < lowerRT) continue;
			if ((*tmpItr)->rt.l <= upperRT) {
				// �I�[�o�[���b�v���Ă���ꍇ
				threshold = (*pkItr)->intensity;
			} else if ((*tmpItr)->rt.l <= upperRT + REMOVE_CONTINUOUS_PEAKS_RT_TOLERANCE) {
				// �I�[�o�[���b�v���Ă��Ȃ����A�K��l�͈͓̔��̏ꍇ
				threshold =  intThreshold;
			} else {
				// RT��center�Ń\�[�g���Ă���A�s�[�N�͈͂̏���̏��ԂƂ͈قȂ�\�������邽��
				// break�ł͂Ȃ�continue�Ƃ���B
				continue;
			}
			// �s�[�N�͈͂̍ő��ێ�����B
			upperRT = std::max(upperRT, (*tmpItr)->rt.r);
			if ((*tmpItr)->sflag && (*tmpItr)->intensity < threshold) {
				if ((*tmpItr)->sflag) {
					clusterCandidatePeaks.push_back(*tmpItr);
				}
				(*tmpItr)->sflag = false;
			}
		}

		// �Ώۂ̃s�[�N�����������̏���
		std::reverse(nearMZPks.begin(), nearMZPks.end());
		upperRT = (*pkItr)->rt.c;
		lowerRT = (*pkItr)->rt.c;
		for (tmpItr = nearMZPks.begin(); tmpItr != nearMZPks.end(); ++tmpItr) {
			if ((*tmpItr)->rt.c > upperRT) continue;
			if ((*tmpItr)->rt.r >= lowerRT) {
				// �I�[�o�[���b�v���Ă���ꍇ
				threshold = (*pkItr)->intensity;
			} else if ((*tmpItr)->rt.r >= lowerRT - REMOVE_CONTINUOUS_PEAKS_RT_TOLERANCE) {
				// �I�[�o�[���b�v���Ă��Ȃ����A�K��l�͈͓̔��̏ꍇ
				threshold = intThreshold;
			} else {
				// RT��center�Ń\�[�g���Ă���A�s�[�N�͈͂̉����̏��ԂƂ͈قȂ�\�������邽��
				// break�ł͂Ȃ�continue�Ƃ���B
				continue;
			}
			// �s�[�N�͈͂̍ŏ���ێ�����B
			lowerRT = std::min(lowerRT, (*tmpItr)->rt.l);
			if ((*tmpItr)->sflag && (*tmpItr)->intensity < threshold) {
				if ((*tmpItr)->sflag) {
					clusterCandidatePeaks.push_back(*tmpItr);
				}
				(*tmpItr)->sflag = false;
			}
		}
	}

	Cnum::deleteFalseFlagObject(detectedPeaks);
	Cnum::sortByIntensity_DESC(detectedPeaks);
}


//Cluster�̍쐬
void NewAB3DLabeling::createClusters(
	vPeak3D& detectedPeaks, std::map<Ptr, std::pair<int,Ptr>>& clusterMap, vPeak3D& clusterCandidatePeaks, ELM::vXYZP& vxyzp
){
	const double clusterRtTol = getSettingMapDouble("ClusteringRtTol");

	const double clusterMzTolRatio = getSettingMapDouble("ClusteringMzTol") * kPPM;

	// ���ʑ̃N���X�^�[���쐬����ہA���ʑ̂̃s�[�N��Intensity��Threahold
	const double minIntensity = getSettingMapDouble("MergeMinIntensity");

	ELM::LRP tmpDbl = splitSettingVal(settingMap_["ClusteringIntRatio"]);
	const double clusterIntensityRateLowerLimit = tmpDbl.first;
	const double clusterIntensityRateUpperLimit = tmpDbl.second;

	tmpDbl = splitSettingVal(settingMap_["ClusteringCharge"]);
	const int clusterMinCharge = (int)tmpDbl.first;
	const int clusterMaxCharge = (int)tmpDbl.second;

	const int minNumOfPeaks = getSettingMapInteger("ClusteringNumberOfPeaks");

	// �|�����t���O�����ɖ߂��B
	Cnum::setFlagToPeaks(detectedPeaks, true);
	Cnum::setFlagToPeaks(clusterCandidatePeaks, true);

	// �s�[�N�̃��X�g���R�s�[���ART�Ń\�[�g����B
	vPeak3D pksSortedByRT = detectedPeaks;
	pksSortedByRT.reserve(pksSortedByRT.size() + clusterCandidatePeaks.size());
	pksSortedByRT.insert(pksSortedByRT.end(), clusterCandidatePeaks.begin(), clusterCandidatePeaks.end());
	Cnum::sortByRTcenter(pksSortedByRT);

	std::map<Ptr, Ptr> pk2MonoIsoPk;

	// �s�[�N�̍��������珈�������Ă����B
	// �s�[�N�~���ɕ���ł���O��Ƃ���B
	vPeak3D additionalPks;
	for (vPeak3D::iterator pkItr = detectedPeaks.begin(); pkItr != detectedPeaks.end(); ++pkItr) {

		if (clusterMap.find(*pkItr) != clusterMap.end()) {
			continue;
		}

		RFN::XYP::vXYP_ vxyp;
		if (!getDataPointsByRT(vxyzp, (*pkItr)->rt.c, vxyp)) {
			// �Y����Scan��DataPoint���擾�ł��Ȃ������Ƃ�
			continue;
		}

		// RT��Tolerance�͈͓��̃s�[�N���擾����B
		// m/z�͈̔͂ɂ��ẮA�Ώۂ̃��m�A�C�\�s�[�N������Da�m�ۂ���Ă���΂悢�B
		// ���ۂɂ͂���ȍ~�̏����ōX�ɍi�荞�܂��B
		vPeak3D neighborPeaksSortedByIntensity;
		if (!Cnum::getPeaksFromSortedByRT(
				pksSortedByRT, 
				(*pkItr)->mz.c - clusterMzTolRatio,	
				(*pkItr)->mz.c + 10.0,				
				(*pkItr)->rt.c - clusterRtTol,
				(*pkItr)->rt.c + clusterRtTol, 
				neighborPeaksSortedByIntensity)) {
			continue;
		}
		Cnum::sortByIntensity_DESC(neighborPeaksSortedByIntensity);

		vPeak3D clusteredPks;
		bool foundExistingCluster = false;
		std::pair<int, Ptr> clusterPair;

		const double mzTol = (*pkItr)->mz.c * clusterMzTolRatio;
		double diffMZ;
		int foundCharge = -1;

		// �����̃��[�v
		// ���[�v�̏I������
		// �ݒ肳��Ă��鉿���͈̔͂𒴂����Ƃ��A�܂��͊����̃N���X�^�̃��m�A�C�\�ƂȂ����Ƃ�
		for (int charge = clusterMaxCharge;
				!foundExistingCluster && charge >= clusterMinCharge; charge--) {

			diffMZ = 1.0 / (double)charge;
			vPeak3D tmpClusteredPks;

			double latestMZ = (*pkItr)->mz.c;
			double latestIntensity = (*pkItr)->intensity;
			int c = 1;

			// ���鉿���̓��ʑ̃s�[�N���������̃��[�v
			// ���[�v�̏I������
			// �Y���ʒu�Ƀs�[�N��������Ȃ��Ȃ����Ƃ��A
			// �܂��͊Y���ʒu�ɉ�������v��������̃N���X�^�̃��m�A�C�\�s�[�N�����������Ƃ�
			while (1) {
				double calcMz = latestMZ + diffMZ;
				const double upperIntensity = latestIntensity * clusterIntensityRateUpperLimit;
				const double lowerIntensity = latestIntensity * clusterIntensityRateLowerLimit;

				// m/z��Intensity�͈̔͂Ō����i�荞�ށB
				vPeak3D tmpCandidatePks;
				Cnum::getPeaksFromSortedByIntensityDesc(
						neighborPeaksSortedByIntensity,
						lowerIntensity, upperIntensity,
						calcMz - mzTol, calcMz + mzTol,
						tmpCandidatePks);

				// RT�̃s�[�N�̂����Tolerance�𖞂����Ă��邪�A 
				// �ł�����Intensity�̃s�[�N��RT���ႢIntensity�̃G���A����O��Ă���s�[�N�����O����B
				vPeak3D rtOverlappedPeaks;
				if (!Cnum::getRTOverlappedPeaks(
						tmpCandidatePks, (*pkItr)->rt.c, rtOverlappedPeaks)) {
					break;
				}

				// m/z�̗��_�l�Ƃ̍����ł��������s�[�N��I������B
				Ptr foundPk;
				Cnum::getNearestMzPeak(rtOverlappedPeaks, calcMz, foundPk);
				if (clusterMap.find(foundPk) != clusterMap.end()) {
					// ���ɃN���X�^�[������Ă����ꍇ
					if (clusterMap[foundPk].first == charge) {
						// ��������v���Ă���ꍇ�́A���̃N���X�^�[�Ɋ܂߂�B
						clusterPair = clusterMap[foundPk]; 
						foundExistingCluster = true;
					}
					break;
				} else {
					// �N���X�^�[������ĂȂ��s�[�N�����݂���ꍇ
					tmpClusteredPks.push_back(foundPk);
					latestMZ = foundPk->mz.c;
					latestIntensity = foundPk->intensity;
				}
				++c;
			}

			if (tmpClusteredPks.size() >= (minNumOfPeaks - 1)) {
				if (clusteredPks.size() < tmpClusteredPks.size()) {
					// ���Ɍ��������N���X�^�[�̌������\���s�[�N�̐��������ꍇ
					clusteredPks = tmpClusteredPks;
					foundCharge = charge;
				}
			}
			if (foundExistingCluster) {
				// ���ɑ��݂���N���X�^�[�Ɋ܂߂�ꍇ
				foundCharge = charge;
				break;
			}
		}

		if (foundCharge > 0) {
			if (foundExistingCluster) {
				// ���ɑ��݂���N���X�^�[�Ɋ܂߂�ꍇ�ɂ́A 
				// ���̃s�[�N�����m�A�C�\�ɂȂ邽�߂ɁA���ɑ��݂��铯�ʑ̃s�[�N�̃}�b�v��u��������B
				vPeak3D tmpPks;
				std::map<Ptr, Ptr>::const_iterator foundItr;
				for (foundItr = pk2MonoIsoPk.begin(); foundItr != pk2MonoIsoPk.end(); ++foundItr) {
					if (clusterPair.second != foundItr->second) continue;
					tmpPks.push_back(foundItr->first);
				}
				for (vPeak3D::iterator tmpItr = tmpPks.begin(); tmpItr != tmpPks.end(); ++tmpItr) {
					clusterMap[*tmpItr] = std::make_pair(foundCharge, *pkItr);
					pk2MonoIsoPk[*tmpItr] = *pkItr;
				}
			}
			// Cluster �̍쐬
			clusterPair = std::make_pair(foundCharge, *pkItr);
			clusterMap[*pkItr] = clusterPair;
			pk2MonoIsoPk[*pkItr] = *pkItr;
			for (vPeak3D::iterator clsPkItr = clusteredPks.begin(); clsPkItr != clusteredPks.end(); ++clsPkItr) {
				clusterMap[*clsPkItr] = clusterPair;
				vPeak3D::iterator delItr = std::remove(pksSortedByRT.begin(), pksSortedByRT.end(), *clsPkItr);
				pksSortedByRT.erase(delItr, pksSortedByRT.end());
				pk2MonoIsoPk[*clsPkItr] = *pkItr;

				if (std::find(clusterCandidatePeaks.begin(), clusterCandidatePeaks.end(), *clsPkItr) != clusterCandidatePeaks.end()) {
					additionalPks.push_back(*clsPkItr);
				}
			}
		}
	}

	detectedPeaks.reserve(detectedPeaks.size() + additionalPks.size());
	detectedPeaks.insert(detectedPeaks.end(), additionalPks.begin(), additionalPks.end());

	for (vPeak3D::iterator pkItr = additionalPks.begin(); pkItr != additionalPks.end(); ++pkItr) {
		vPeak3D::iterator endItr = std::remove(clusterCandidatePeaks.begin(), clusterCandidatePeaks.end(), *pkItr);
		clusterCandidatePeaks.erase(endItr, clusterCandidatePeaks.end());
	}
}

// settingMap (params���擾�����l)�@����@Key�@-> Val(Integer)���󂯎��
// �Ȃ���� -1 ��Ԃ�
int NewAB3DLabeling::getSettingMapInteger(const std::string& pname) const 
{
	std::map<std::string, std::string>::const_iterator itr = settingMap_.find(pname);
	if (itr != settingMap_.end()) {
		return boost::lexical_cast<int>(itr->second);
	}
	return -1;
}

double NewAB3DLabeling::getSettingMapDouble(const std::string& pname) const 
{
	std::map<std::string, std::string>::const_iterator itr = settingMap_.find(pname);
	if (itr != settingMap_.end()) {
		return boost::lexical_cast<double>(itr->second);
	}
	return -1.0;
};

// For AB3D Single
// �T���v�����J����AB3D�����s�����ꍇ�ɌĂ΂�܂��B
// [in]  dataSet (mass++�̃T���v���̃I�u�W�F�N�g)
// [out] peaks2d (mass++��peaks2D�̃I�u�W�F�N�g�i����))
void NewAB3DLabeling::doAB3D (
	kome::objects::DataSet& dataSet,
	kome::objects::Peaks2D* peaks2d
) {
	onDoAB3D(dataSet, peaks2d, nullptr);
}

// For AB3D with peakPositions
// DB Wizard�o�R�Ł@�T���v�����J����AB3D�����s�����ꍇ�ɌĂ΂�܂��B
// [in]  dataSet (mass++�̃T���v���̃I�u�W�F�N�g)
// [out] dbac    (mass++��peaks2D�̌���)
void NewAB3DLabeling::doAB3D (kome::objects::DataSet& dataSet, tDBAC* dbac) {
	onDoAB3D(dataSet, nullptr, dbac);
}

// �v���O���X�o�[��params(�{��)����󂯎���āA�����n���܂��B
// ...Top.cpp���ŏ������Ă��܂��B
// �R���X�g���N�^�ɓ��ꂽ�ق����ǂ���������܂���B
void NewAB3DLabeling::bindProgress(kome::core::Progress* progress)
{
	myProgress_ = progress;
}

const bool NewAB3DLabeling::isSelected(const char* key) {
	return (std::strcmp(settingMap_[key].c_str(), "true") == 0);
}

// ����̊g�������˂� on �ŌĂ΂������ɕύX�B
bool NewAB3DLabeling::onDoAB3D (
	kome::objects::DataSet& dataSet,
	kome::objects::Peaks2D* peaks2d,
	tDBAC* dbac
) {

	ALC::TradingPost tdPost;

	// ���ؗpDB�擾�t���O
	const bool needSingleXICDB = (std::strcmp(settingMap_["OutPutForDataBase"].c_str(),"true") == 0);
	// Data Point�iXIC�Ɏg����Spectrum�l�̏W��)
	ELM::vXYZP main_vxyzp;
	// �v�Z����͈�
	ELM::MZRTP mainMZRT =
		ELM::MZRTP(splitSettingVal(settingMap_["MzRange"]), splitSettingVal(settingMap_["RtRange"]));
	// �����̌v�Z���̔z��
	vPeak3D peaksSoterdByIntensity;

	// PPM -> Ratio�Ɍv�Z
	const double mzPPMTol = getSettingMapDouble("MergeDataPointsMzTol"); // 10.0;
	const double mzTolRatio = mzPPMTol * kPPM;

	ELM::LRP tmpDbl = splitSettingVal(settingMap_["ClusteringCharge"]);
	const int minCharge = (int)tmpDbl.first;
	const int maxCharge = (int)tmpDbl.second;


	const bool removeIsotopicDataPoints =
			isSelected("MergeDataPointsRemoveNonIsotopicDataPoints");

	if (mainMZRT.mz.first == 0 && mainMZRT.mz.second == 0) {
		mainMZRT.mz.first = -1;
		mainMZRT.mz.second = -1;
	}
	if (mainMZRT.rt.first == 0 && mainMZRT.rt.second == 0) {
		mainMZRT.rt.first = -1;
		mainMZRT.rt.second = -1;
	}
	
	tdPost.getSpectrumDataPoints(
		dataSet, mainMZRT, 1, ALC::HAS_CHROM, main_vxyzp, peaksSoterdByIntensity,
		removeIsotopicDataPoints, minCharge, maxCharge, mzTolRatio);

	Cnum::sortByIntensity_DESC(peaksSoterdByIntensity);

	const double availableIntensityRatio = getSettingMapDouble("MergeDataPointsIntRatio");

	const double rtTol = getSettingMapDouble("MergeDataPointsRtTol");//0.5; 
	const double peakendhighprecent = 0.1;
	const double minIntensity = getSettingMapDouble("MergeMinIntensity");

	// ���ʑ̃N���X�^�[���쐬����ہA���ʑ̂̃s�[�N��Intensity��Threahold
	const double lowerDatapointIntensityLimit = minIntensity * availableIntensityRatio;

	// m/z�͈̔͂����肷��s�[�N��m/z�����̂܂ܔ͈͂ɓK�p���Ă��܂��Ƃ��肬��XIC���擾�ł��Ȃ��ꍇ������̂ŁA
	// �����L�߂ɔ͈͂�ݒ肷��K�v������B���̗]���̕�(�P��:PPM)�B
	const double XIC_MZ_MARGIN_PPM = 5.0;
	const double XIC_MZ_MARGIN_RATE_LOWER = (1.0 - XIC_MZ_MARGIN_PPM * kPPM);
	const double XIC_MZ_MARGIN_RATE_UPPER = (1.0 + XIC_MZ_MARGIN_PPM * kPPM);

	// �ꎟ��Ɨp�I�u�W�F�N�g
	ELM::vPKPT xicLWAPeaks;
	vPeak3D	detectedVPK3D;
	vPeak3D clusterCandidateVPK3D;
	ELM::vXYP rstXIC;

	//�Ⴂ�s�[�N����
	Cnum::eraseRemoveFalseFlagObject(peaksSoterdByIntensity, lowerDatapointIntensityLimit);

	vPeak3D peaksSortedByMz = peaksSoterdByIntensity;
	Cnum::sortByMZcenter(peaksSortedByMz);

	vPeak3D::iterator currItr = peaksSoterdByIntensity.begin();
	detectedVPK3D.reserve(peaksSoterdByIntensity.size());

	// Flag�����ȏ㗭�܂�����@�����Ď��t���O
	long falseFlagCount = 0;
	long refleshcycle = static_cast<long>(peaksSoterdByIntensity.size() * 0.04);

	// DataBase�p��MAP
	std::map<Ptr, int> dbmap_Prof;

	// Progress��ǉ�
	// Progress�֘A�̊֐��Ăяo�����ώG�Ȃ̂ŁA�N���X�ł܂Ƃ߂Ă���܂��B
	// ���؎��Ƀv���O���X���ז��ȏꍇ�́A������nullptr�ɂ��Ă�������
	// �ȍ~�� prog�͋L�q������܂����A�S�����������悤�ɂȂ�܂��B
	AB3DProg prog(myProgress_);

	prog.nextStep("Detecting peaks...");

	while ( !myProgress_->isStopped() ) {
		// ����l�𒴂����̂Ń��Z�b�g�B
		if (refleshcycle < falseFlagCount) {

			// �t���O��false�ȃs�[�N����菜���B
			// Intensity�ł̃\�[�g���͈ێ�����Ă���̂Ń\�[�g�������Ȃ��B
			Cnum::removePeaksFromList(peaksSoterdByIntensity);

			peaksSortedByMz = peaksSoterdByIntensity;
			Cnum::sortByMZcenter(peaksSortedByMz);

			currItr = peaksSoterdByIntensity.begin();
			falseFlagCount = 0;
			prog.nextStep();
		}

		// Intensity�~���̃��X�g�̖������̐擪�Ɉړ�����B
		Ptr currPeak;
		if (!Cnum::setForeFront(peaksSoterdByIntensity, currPeak, currItr)) {
			break;
		}

		if (currPeak->intensity < minIntensity) {
			// Intensity�~���ɏ������s�����߁A
			// �Ώۂ̃s�[�N��Intensity��臒l�������ΏI������B
			break;
		}

		// Intensity�̑��؂������ɁA�͈͓��̑S�Ẵs�[�N��Intensity�̍~���Ŏ擾����B
		// RT�Ɋւ��ẮA�s�[�N�s�b�L���O�����̓s����ART�̗]����Tolerance��2�{�m�ۂ���B
		const double lowerXICRt = currPeak->rt.c - rtTol;
		const double upperXICRt = currPeak->rt.c + rtTol;
		vPeak3D peaksInArea;
		peaksInArea.clear();
		Cnum::getPeaksFromSortedByMz(peaksSortedByMz,
			currPeak->mz.c * (1.0 - mzTolRatio), currPeak->mz.c * (1.0 + mzTolRatio),
			lowerXICRt, upperXICRt, peaksInArea);

		// ���o�����s�[�N��m/z�̒��S�Ɣ͈�
		// m/z�͈̔͂ɂ��ẮA���̌�̏����Ō��肷��̂ŁA�Ƃ肠�����Ώۃs�[�N�̒��S���Z�b�g����B
		ELM::LCRP peakMzRange(currPeak->mz.c, currPeak->mz.c, currPeak->mz.c);

		// ���݂���s�[�N����Am/z�͈͂����肷��B
		if (!detectMZRangeByPeakPositions(
				currPeak, peaksInArea,
				rtTol, currPeak->intensity * availableIntensityRatio, peakMzRange)) {
			// �Ώ۔͈͂���L���ȃs�[�N���擾�ł��Ȃ��ꍇ
			currPeak->sflag = false;
			++falseFlagCount;
			continue;
		}

		// �擾�����s�[�N��m/z�����኱�L�߂ɔ͈͂�ݒ肷��B
		// ��������Ȃ���΁Am/z���s�[�N���ꂷ��ɂȂ��Ă��܂��A���҂���XIC���擾�ł��Ȃ��ꍇ����B
		peakMzRange.l *= XIC_MZ_MARGIN_RATE_LOWER;
		peakMzRange.r *= XIC_MZ_MARGIN_RATE_UPPER;

		rstXIC.clear();
		if (!tdPost.createXIC(
				main_vxyzp, peakMzRange.l, peakMzRange.r, lowerXICRt, upperXICRt, rstXIC)) {
			// �w��͈͂�XIC�̎擾�Ɏ��s�����ꍇ�A
			// �Ώۂ̃X�y�N�g�����s�[�N�𖳌��ɂ���B
			currPeak->sflag = false;
			++falseFlagCount;
			continue;
		}

		ELM::PKPT xicPeak;
		if (!getXICPeak(rstXIC, currPeak->rt.c, xicPeak)) {
			// XIC����s�[�N�̎擾�Ɏ��s�����ꍇ�A
			// �Ώۂ̃X�y�N�g�����s�[�N�𖳌��ɂ���B
			currPeak->sflag = false;
			++falseFlagCount;
			continue;
		}

		// ���o�����s�[�N��RT�̒��S�Ɣ͈�
		ELM::LCRP peakRtRange(xicPeak.Lp.first, currPeak->rt.c, xicPeak.Rp.first);

		// �͈͓��ɑ��݂���f�[�^�|�C���g�𖳌��ɂ���B
		falseFlagCount += setFalseFlagToUsedPeak3D(
			peaksInArea,
			peakMzRange.l, peakMzRange.r, peakRtRange.l, peakRtRange.r);

		// �s�[�N�͈͓��ɑ��݂���f�[�^�|�C���g��Intensity���擾���āA�擾���̒l��0�ɂ���B
		std::vector<double> intensities;
		tdPost.clearIntensity(
			main_vxyzp,
			peakMzRange.l, peakMzRange.r,
			peakRtRange.l, peakRtRange.r, intensities);

		// S/N��Ȃǂ̊�Ńs�[�N�̗L��������s���B
		// XIC�̎擾�����ōs��Ȃ��̂́A�s�[�N�������ł������ꍇ�ł�
		// �͈͓��̑S�Ẵf�[�^�|�C���g�̃t���O�𖳌��ɂ��������߁B
		//if (!verifyPeak(rstXIC, xicPeak, intensities)) {
		const int peakStatus = verifyPeak(rstXIC, xicPeak, intensities);
		if (peakStatus == 0) {
			// �����ȃs�[�N�Ƃ��Ĕ��肳�ꂽ�ꍇ
			continue;
		}

		// �͈͓���XIC�����v����Area���擾����B
		double peakArea = calcAreaByXIC(rstXIC, peakRtRange.l, peakRtRange.r);


		// �s�[�N��ǉ�����B
		ELM::PK3D_ptr pk =
			new ELM::Peak3D(currPeak->intensity, peakMzRange, peakRtRange, peakArea,
					0.0, currPeak->logintensity, xicPeak.fwhmVal, intensities.size());
		pk->sflag = true;

		// �K�v�ł����DB�ɏ�������
		if (!writeToDB(dbac, pk, xicPeak, rstXIC, dbmap_Prof)) {
			return false;
		}

		if (peakStatus == 2 && currPeak->intensity >= minIntensity) {
			// ���Ƃ��ēo�^
			detectedVPK3D.push_back(pk);
		} else {
			// �N���X�^�����O���̕⏕���Ƃ��ēo�^
			clusterCandidateVPK3D.push_back(pk);
		}
	}

	unsigned int numOfPeaks = detectedVPK3D.size();
	LOG_DEBUG(FMT("Detected %d peak(s).", numOfPeaks));

	// �v�Z���Ɏg����3D�s�[�N��S�ď���
	// RTvPK3D �����Ă΂Ȃ��悤�ɒ��ӁB
	Cnum::deleteAllElm(peaksSoterdByIntensity);

	if( !myProgress_->isStopped() ){

		// RT�����ɘA������s�[�N���폜����B
		prog.nextStep("Removing continous peaks...");

		removeContinuousPeaks(detectedVPK3D, clusterCandidateVPK3D);
	
		// Shoulder�s�[�N���폜����B
		prog.nextStep("Removing shoulder peaks...");

		deleteShoulderPeaks(detectedVPK3D);

		// Cluster���쐬����B
		prog.nextStep("Creating isotopic clusters...");

		std::map<Ptr, std::pair<int,Ptr>> clusterChargeMap;
		createClusters(detectedVPK3D, clusterChargeMap, clusterCandidateVPK3D, main_vxyzp);

		if (peaks2d != nullptr) {
			toMassppPeak2D(peaks2d, clusterChargeMap, detectedVPK3D);
		} else {
			// �K�v�ł����DB�ɃN���X�^�����܂߂ď�������
			writeClusterInfotoDB(dbac,clusterChargeMap,detectedVPK3D, dbmap_Prof);
		}

		// �|�C���^�m�ې��Delete����B
		BOOST_FOREACH(const ELM::XYZP& tmpxyzp, main_vxyzp) {
			delete tmpxyzp.vxyp;
		}

		// �S�~������
		Cnum::deleteAllElm(detectedVPK3D);
		Cnum::deleteAllElm(clusterCandidateVPK3D);
	}
	prog.closeMe();
	return true;
}

// �\�[�g�ƁAFalseFlag�ɂȂ����I�u�W�F�N�g��������������B
// �󋵂ɂ���Ă͕s�v�ɂȂ邩������Ȃ�(�\�[�g�̕����d�������j
//void NewAB3DLabeling::deleteFalseFlagAndSort(vPeak3D& byIntArr, vPeak3D& byMZArr) {
//	Cnum::sortByMZcenter(byMZArr);
//}

// �͈͓��ɑ��݂���s�[�N�̃t���O��|���B
const unsigned int NewAB3DLabeling::setFalseFlagToUsedPeak3D (
	vPeak3D& peaks,
	const double& lowerMz, const double& upperMz,
	const double& lowerRt, const double& upperRt
){
	vPeak3D peaksInArea;
	peaksInArea.reserve(peaks.size());

	// �w�肵��m/z��RT�͈͓̔��ɑ��݂���s�[�N���擾���邽�߂ɁA
	// ��x�ʂ̃��X�g�ɃR�s�[���āA�����RT�Ń\�[�g���Ă���
	// �Y���͈͓̔��̃s�[�N���擾����B
	vPeak3D& peaksSortedByRt = peaks;
	Cnum::sortByRTcenter(peaksSortedByRt);
	Cnum::getPeaksFromSortedByRT(peaksSortedByRt, lowerMz, upperMz, lowerRt, upperRt, peaksInArea);

	Cnum::setFlagToPeaks(peaksInArea, false);
	return peaksInArea.size();
}

// �Y���֐��͉����ׂ̈Ɂ@���L�ɐ؂肾�����B���e�͈ꏏ�B
// Mass++�t�H�[�}�b�g�ɕϊ�����(Peak2D & Cluster) 
void NewAB3DLabeling::toMassppPeak2D (
	kome::objects::Peaks2D* peaks2d, 
	std::map<Ptr, std::pair<int,Ptr>>& clusterChargeMap, 
	vPeak3D& detectedVPK3D
) {
	std::map<Ptr, std::pair<int,Ptr>>::iterator cCmapItr;
	std::map<Ptr, kome::objects::PeaksCluster2D*> clusterMap;
	std::map<Ptr, kome::objects::PeaksCluster2D*>::iterator cmapItr;
	std::pair<int, Ptr> clusterPair;

	// �N���X�^�[���\�����Ȃ��s�[�N�����O���邩�ǂ���
	const bool removeNonClustered = isSelected("ClusteringRemoveNonClustered");
	// ���m�A�C�\�݂̂ɍi�荞�ނ��ǂ���
	const bool onlyMonoIsoPeaks = isSelected("ClusteringOutputOnlyMonoisotopic");

	Cnum::sortByMZcenter(detectedVPK3D);

	BOOST_FOREACH(const Ptr pk, detectedVPK3D) {
		cCmapItr = clusterChargeMap.find(pk);

		// �N���X�^�[���\������s�[�N���ǂ���
		bool isClustered = (cCmapItr != clusterChargeMap.end());
		// ���m�A�C�\�̃s�[�N���ǂ���
		bool isMonoIsotopic = false;
		if (isClustered) {
			clusterPair = cCmapItr->second;
			cmapItr = clusterMap.find(clusterPair.second);
			isMonoIsotopic = (cmapItr == clusterMap.end());
		}

		if (!removeNonClustered || isClustered) {
			// �ݒ��ʂŁA"Exclude Non-clustered Peaks"���I�����ꂽ�ꍇ�́A
			// �N���X�^�[���\������s�[�N�݂̂��s�[�N�Ƃ��č̗p����B 

			if (isMonoIsotopic || !onlyMonoIsoPeaks || (!removeNonClustered && !isClustered)) {

				// �ݒ��ʂŁA"Monoisotopic Peaks Only"���I�����ꂽ�ꍇ�́A
				// ���m�A�C�\�̃s�[�N�݂̂��̗p����B
				kome::objects::Peak2DElement* pe =
					peaks2d->createPeak(
							pk->rt.c, pk->rt.l, pk->rt.r,
							pk->mz.c, pk->mz.l, pk->mz.r, pk->intensity);
				pe->setArea(pk->area);
				if (isClustered) {
					pe->setCharge(clusterPair.first);
				}

				if (isMonoIsotopic) {
					kome::objects::PeaksCluster2D* cluster = peaks2d->createCluster();
					cluster->setCharge(clusterPair.first);
					clusterMap[clusterPair.second] = cluster;
				}
				if (isClustered) {
					pe->setCluster(clusterMap[clusterPair.second]);
				}
			}
		}
	}
}

// DB�֏������݂̔���
// Single& DBWrite�������Ă���ꍇ�ATDPost�o�R��DB�ɏ������ށB
// Multi�̏ꍇ�́A�ꎞ�e�[�u����Profile�ɕK�v�ȏ��������Ă����B
const bool NewAB3DLabeling::writeToDB(
	tDBAC* dbac,
	ELM::PK3D_ptr pk, 
	ELM::PKPT& foundpeak,
	const ELM::vXYP& rstXIC, 
	std::map<Ptr, int>& dbmapProf
){
	if (dbac != nullptr) {
		return onWriteToDB(dbac, pk, foundpeak, rstXIC, dbmapProf);
	} 
	return true;
}

// ���P
// memo 2013.08.01
//
// dbmap_prof(map) �ɂ���
// PK3D�̃|�C���^�@�� DB�ɕۑ������@PeakId��R�Â��Ă��m�ۂ��Ă���܂��B

// ���Q
// memo 2013.08.02
// dbac->setXIC �̃C���^�t�F�[�X��ύX���܂����B
// Thoto�n�̎�����S�Ĕr�����Ă���܂��B
// 1. �Ώۂ�XIC�́@vXYP(XYData)
// �@�@rstXIC�̎󂯑��� const std::vector<std::pair<double,double>>& �Ŏ󂯂Ă܂��B
// �@�@ELM::vXYP�@�� �����typedef�������̂Ȃ̂œ����ł��B

// 2. �s�[�N�� RT, M/Z, Matrix���Value
// �@ Peak2D�@�Ƃ��Ắ@�s�[�N�̒��_�������Ă��������B
//    M/Z, RT �́AMatrix�@ROW�̑�\�l�@�Ƃ��Ďg���܂��B
//    Value�̒l�́@Matrix�̊i�q�̒l�Ƃ��ĕ\������܂��B
//    �i�����ɒ�ʐ��������l������悤�ɂȂ�Ǝv���܂��j

// 3. �Ώۂ�XIC�́@�s�[�N���(�����_(X,Y), Left(X,Y),Right(X,Y))
//    Matrxi�̌��ʂ̔g�`��`�悳���ۂɁA�s�[�N�Ƃ��ĔF���E�\�������l�ł��B
//    ����� LWA�̒��ŉ��d���ς����Ă��Ȃ��̂ŁAApex��Center�̍��W�͓������Ƒ����܂����A
//    �v�Z��ł͂Ȃ��A�����_�̍��W��n���Ă��������B

const bool NewAB3DLabeling::onWriteToDB(
	tDBAC* dbac,
	ELM::PK3D_ptr pk, 
	ELM::PKPT& foundpeak,
	const ELM::vXYP& rstXIC, 
	std::map<Ptr, int>& dbmapProf
){
	// ���Q
	int dbPeakId = dbac->setXICData(
		rstXIC,
		pk->rt.c,
		pk->mz.c,
		pk->intensity,
		foundpeak.Apex.first,
		foundpeak.Apex.second,
		foundpeak.Lp.first,
		foundpeak.Lp.second,
		foundpeak.Rp.first,
		foundpeak.Rp.second
	);

	// ���P
	if (dbPeakId >= 0) {
		dbmapProf[pk] = dbPeakId;
		return true;
	}				
	return false;
}


// Cluster & Peak�̏���DB�ɏ����o���K�v������ꍇ
// �`���[�W�N���X�^�̏�񂪓���B
//
// memo 2013.08.01
//
// [�L����Peak�ł���]�Ɣ��肳�ꂽ Peak�́@Charge ��
// dbac->updateCharge(peakId, z); �Ƃ��ĕύX���܂��B
// z = -1 or z >= 2 �����邱�Ƃ����҂��Ă��܂��B
// �����ύX����Ȃ��l�͎̂Ă܂��B
// �L���Ƃ���(Matrix��ɏo��Peak�j�́@�K����Charge��^���Ă��������B

// �����ύX�̂��m�点
// "Exclude Non-clustered Peaks" ���@��I���̎���
// �����Ă��܂��p�^�[�����������̂ŁAIF�̕���������ύX���Ă���܂��B

void NewAB3DLabeling::writeClusterInfotoDB(
	tDBAC* dbac, 
	std::map<Ptr, std::pair<int,Ptr>>& clusterChargeMap, 
	vPeak3D& detectedVPK3D,
	std::map<Ptr, int>& dbmap_Prof
){
	// DB���s���Ȃ�܂�Ԃ�
	if (dbac == nullptr) {
		return;
	}
	std::map<Ptr, std::pair<int,Ptr>>::iterator mapItr;
	std::set<Ptr> usedCluster;

	// �N���X�^�[���\�����Ȃ��s�[�N�����O���邩�ǂ���
	const bool excludeNonClusteredPeaks = isSelected("ClusteringRemoveNonClustered");
	// ���m�A�C�\�݂̂ɍi�荞�ނ��ǂ���
	const bool monoisotopicPeaksOnly = isSelected("ClusteringOutputOnlyMonoisotopic");
	// Mz �Ń\�[�g
	Cnum::sortByMZcenter(detectedVPK3D);

	BOOST_FOREACH(const Ptr pk, detectedVPK3D) {
		int peakId = dbmap_Prof[pk];
		mapItr = clusterChargeMap.find(pk);

		// �N���X�^�}�b�v��ɑ��݂��Ȃ����̂������Ă���B
		if(mapItr == clusterChargeMap.end()) {
			if (!excludeNonClusteredPeaks) {
				// "Exclude Non-clustered Peaks" ���@��I���̎��� -1 �Ƃ��ĕ⊮
				// Peak2d�Ƃ��ē����邪�A�N���X�^�̖������̂ɂȂ�B
				dbac->updateCharge(peakId, -1);
			}
			continue;
		}

		Ptr monoIsoPtr = mapItr->second.second;
		int charge = mapItr->second.first;

		if(monoisotopicPeaksOnly) {
			// �ݒ��ʂŁA"Monoisotopic Peaks Only"���I�����ꂽ�ꍇ�́A
			// ���m�A�C�\�̃s�[�N�݂̂��̗p����B
			if(usedCluster.count(monoIsoPtr) == 0) {
				usedCluster.insert(monoIsoPtr);
				dbac->updateCharge(peakId, charge);
			}
		} else {
			dbac->updateCharge(peakId, charge);
		}
	}
}

int NewAB3DLabeling::verifyPeak(
	ELM::vXYP& xic, ELM::PKPT& foundPeak, std::vector<double>& intensities) {

	// XIC��Retention Time�̃s�[�N��
	const double peakMinRtSpan = getSettingMapDouble("VerificationPeakSpan");
	if (foundPeak.Rp.first - foundPeak.Lp.first < peakMinRtSpan) {
		// �s�[�N��Retention Time�����ݒ�l�ȉ��̏ꍇ
		return 0;
	}

	// XIC�s�[�N�̔��l���̍ŏ��l
	const double peakMinFWHM = getSettingMapDouble("VerificationPeakFWHM");
	if (foundPeak.fwhmVal < peakMinFWHM) {
		return 0;
	}

	// XIC�s�[�N�ɑ��݂���N���̍ő吔
	const int maxUndulations = getSettingMapInteger("VerificationPeakUndulations");
	int lInd = 0, rInd = 0;
	int numOfUndulations = 0;
	double xicInt = -1;
	bool isAscent = false;
	for (ELM::vXYP::iterator xicItr = xic.begin(); xicItr != xic.end(); ++xicItr) {
		if ((*xicItr).first == foundPeak.Lp.first) {
			lInd = xicItr - xic.begin();
		}
		if (xicInt == -1) {
			xicInt = (*xicItr).second;
		} else {
			if (lInd > 0 && rInd == 0 && xicInt > (*xicItr).second && isAscent) {
				// �N�������݂���ꍇ
				++numOfUndulations;
				if (numOfUndulations > maxUndulations) {
					return 0;
				}
			}
			isAscent = (xicInt < (*xicItr).second);
			xicInt = (*xicItr).second;
		}
		if ((*xicItr).first == foundPeak.Rp.first) {
			rInd = xicItr - xic.begin();
			break;
		}
	}

	const double snRatio = getSettingMapDouble("VerificationPeakSNRatio");
	const int dataPoints = getSettingMapInteger("VerificationNumOfDataPoints");
	const double baselineRatio = getSettingMapDouble("VerificationBaselineIntRatio");
	const bool baselineMethodMedian = (settingMap_["VerificationBaselineMethod"] != "baseline_mean");

	std::vector<double> tmpIntensities = std::vector<double>();
	double sum = 0.0;
	for (std::vector<double>::iterator itr = intensities.begin(); itr != intensities.end(); ++itr) {
		double intensity = *itr;
		if (intensity > 0) {
			tmpIntensities.push_back(intensity);
			if (!baselineMethodMedian) {
				sum += log10(intensity);
			}
		}
	}
	std::sort(tmpIntensities.begin(), tmpIntensities.end(), std::greater<double>());
	int lSize = tmpIntensities.size();
	double baseline;
	if (baselineMethodMedian) {
		if (lSize % 2 == 0) {
			baseline = (tmpIntensities[lSize/2-1] + tmpIntensities[lSize/2]) / 2;
		} else {
			baseline = tmpIntensities[lSize/2];
		}
	} else {
		baseline = pow(10, (sum/lSize));
	}

	double s = 0.0;
	for (std::vector<double>::iterator itr = tmpIntensities.begin(); itr != tmpIntensities.end(); ++itr) {
		s += (*itr - baseline) * (*itr - baseline);
	}

	double maxIntensity = *(tmpIntensities.begin());
	double stdev = std::sqrt(s/lSize);
	double sn;
	if (stdev > 0) {
		sn = (maxIntensity - baseline) / stdev;
	} else {
		sn = 0.0;
	}

	// �e�p�����[�^�̏����𖞂����Ă��邩�ǂ����̔���
	bool rightSn = (sn >= snRatio);
	bool rightDps = (lSize >= dataPoints);
	bool rightBaseline = (baseline / maxIntensity) <= baselineRatio;

	// �S�Ă̏����𖞂����ꍇ�́A2��Ԃ��B
	// �������AS/N�ȊO�̏����𖞂����āA����S/N�̏�����1/2�𖞂����Ă���ꍇ�́A1��Ԃ��B
	if (rightDps && rightBaseline) {
		if (rightSn) return 2;
		if (sn >= snRatio / 2) return 1;
	}
	return 0;
}
