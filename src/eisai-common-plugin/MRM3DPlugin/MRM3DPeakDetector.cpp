/**********************************************************/
/*! @addtogroup MRM3DPlugin 
    @file       MRM3DPeakDetector.cpp
    @brief      Filter Parser Functions 
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
MRM3D Peak Picking�̃��C���N���X
*/
/**********************************************************/
#include "stdafx.h"
#include "MRM3DPeakDetector.h"

#include <boost/bind/bind.hpp>
#include <boost/foreach.hpp>

#include "FilterParser.h"
#include "MRMChromagorams.h"
#include "AssistTools.h"
#include "SettingsParamDefine.h"

const std::string kFILTER("Filter");

/*! WindowTool�̃��b�Z�[�W���͂����ňꌳ�Ǘ����� */
const std::string kINFO_BADPARAM("Intensity Filter Range is bat parameter");
const std::string kINFO_NOMRM("This sample is not support MRM Data");
const std::string kCMT_CONVERT("Convert to MRM Data From Sample File..");
const std::string kCMT_PICKING("Smoothing AND Peak Picking .."); 

using namespace kome::mrm3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace mspp::sprt;

// constructor
MRM3DCore::MRM3DCore( kome::objects::SettingParameterValues* settings ) 
	: m_settings(settings) {
	m_parser = new ParamParser(settings);
}

// destructor
MRM3DCore::~MRM3DCore(){}

// PeakPicking ���s
void MRM3DCore::executeLabeling(
	kome::objects::DataSet& dataSet,
	kome::core::Progress& progress,
	kome::objects::Peaks2D& peaks2d
) {
	if (!m_parser->checkParams()) {
		// settings bad parameter
		kome::window::WindowTool::showInfo(kINFO_BADPARAM.c_str());
		return;
	}

	if (!isHavingMRMType(dataSet)) {
		// sample is nothing MRM Type
		kome::window::WindowTool::showInfo(kINFO_NOMRM.c_str());
		return;
	}
	// MRM Ptr�̃R���e�i
	tvMRMPtr allChroms;

	// Setup Peak Detecter Info
	mspp::sprt::PeakDetector pkDtct(m_parser->getdetectStr());
	if (!pkDtct.setUp(m_settings, true)) {
		return;
	}
	// Setup Peak Smooter Info
	PeakSmoother pkSmth(m_parser->getsmoothStr());
	pkSmth.setUp(m_settings);

	// Get MRM Datas
	progress.createSubProgresses(STEP_NUM);
	getMRMDatas(dataSet, *progress.getSubProgress(CONVERT), allChroms);

	// Smoothing & Peak Picking
	ProgressKeeper prog(*progress.getSubProgress(PEAKPICK), allChroms.size(), kCMT_PICKING);
	BOOST_FOREACH(tMRMChromPtr chromPtr, allChroms) {
		detectAndConvertPeaks(chromPtr, &pkDtct, &pkSmth, peaks2d);
		prog.showNext();
	}
	// Take Thresholds
	FilterPeak2D(peaks2d);

	prog.doFinish();
	progress.fill();
}

// Check Target Sample Is Having MRM Filter
const bool MRM3DCore::isHavingMRMType (kome::objects::DataSet& dataSet) {
	kome::objects::Spectrum* spec = dataSet.getSpectrum(0);
	if (spec == NULL) return false;
	std::string filterStr = spec->getProperties().getStringValue(kFILTER.c_str(),"");
	MRMType mrmchecker;
	return mrmchecker.checkMRMType(filterStr);
}

// get MRMDatas
const bool MRM3DCore::getMRMDatas(
	kome::objects::DataSet& dataSet,
	kome::core::Progress& progress,
	tvMRMPtr& allChroms
) {
	const int specnum = dataSet.getNumberOfSpectra();

	// work objects
	typedef std::map<std::string, MRMChromKeeper*> kFILMAP;
	/*! 
		Ex ) 934.5 [220.2-220.4,330.3-330.5] 
		�Ƃ���Filter��������Spectrum�ɑ��݂���B
		Filter���ɕ�����Q3�����\��������B
		Filter�P�ʂŏ����W�ς�����K�v������B
		�̂ɕ�����MRMChrom������������MAP��p�ӂ���B
	 */
	kFILMAP filmap;
	kFILMAP::iterator it, eit = filmap.end();
	
	kome::objects::Spectrum* spec = nullptr;
	MRMChromKeeper* stoc = nullptr;
	std::string filstr;

	/*! progrss ���Z�b�g���� */
	ProgressKeeper prog(progress, specnum, kCMT_CONVERT);

	for(int idx = 0; idx < specnum; idx++) {
		prog.showNext();
		spec = dataSet.getSpectrum(idx);
		filstr = spec->getProperties().getStringValue(kFILTER.c_str(),"");
		// �V����Filter�ł���΁A��͂��Ēǉ�����B
		if (filmap.find(filstr) == eit) {
			stoc = new MRMChromKeeper(filstr);
			// SRM�̋K���̃f�[�^�ł��邩�`�F�b�N����B
			if (stoc->doInit()) {
				filmap[filstr] = stoc;
				eit = filmap.end();
			} else {
				// no Match Rule.
				return false;
			}
		}
		// Filter�ɏ�������Stoc���擾�BSpectrum����͂���MRMData���擾����B
		stoc = filmap[filstr];
		divideXYDataPoints(spec, stoc);
	}

	// �I�u�W�F�N�g���W��������B
	for(it = filmap.begin(), eit = filmap.end(); it != eit; ++it) {
		(*it).second->margeToAll(allChroms);
	}
	return true;
}

// xydata ��MRMChrom�̏��ɏ]���ĐU�蕪����
void MRM3DCore::divideXYDataPoints(
	kome::objects::Spectrum* spectrum,
	MRMChromKeeper*& stocker
) {
	if (spectrum == nullptr) return;

	const double rt = spectrum->getRt();
	kome::core::XYData* xydata = spectrum->getXYData();

	if (xydata == nullptr) return;

	// RT�̔������B�O�ł����B
	stocker->addPointsBase(rt);

	const int xyNum = xydata->getLength();
	double mz, tmpInt;

	for (int idx = 0; idx < xyNum; idx++) {
		mz = xydata->getX(idx);
		tmpInt = xydata->getY(idx);
		// �Y����Ԃ�����Ή��Z���Ă���
		stocker->findAndsetPoints(mz, tmpInt);
	}
	// ���^�f�[�^�͏���
	spectrum->deleteXYData();
}

// Peak Picking�@����
void MRM3DCore::detectAndConvertPeaks (
	tMRMChromPtr& chromPtr,
	mspp::sprt::PeakDetector* pkFunc,
	mspp::sprt::PeakSmoother* pksmth,
	kome::objects::Peaks2D& peaks2d
) {
	// Convert Data
	kome::core::XYData* xydata = new kome::core::DataPoints();
	if (! convertToXYData(chromPtr, &*xydata)) return;
	
	// Peak Picking
	pksmth->invokeMe(xydata);
	kome::objects::Peaks tmpPeaks;
	pkFunc->invokeMe(*xydata, tmpPeaks);
	
	// convert 2DPeaks
	convertTo2DPeaks(chromPtr, tmpPeaks, peaks2d);
}

// Reverce Convert Mass++'s XYData Objects
const bool MRM3DCore::convertToXYData (
	tMRMChromPtr& chromPtr,
	kome::core::XYData* xydata
) {
	// assing XYData from mrmChrom Class
	chromPtr->setReserveFunc(boost::bind(&kome::core::XYData::reserve, &*xydata, _1));
	chromPtr->setAddFunc(boost::bind(&kome::core::XYData::addPoint, &*xydata, _1, _2));
	chromPtr->doAssignXYData();
	// check no xyData.
	if (xydata->getLength() <= 0) {
		return false;
	}
	return true;
}

// Peaks ���K���ɑ���A2DPeaks�ɕς���B
const bool MRM3DCore::convertTo2DPeaks (
	tMRMChromPtr& chromPtr,
	kome::objects::Peaks& tmpPeaks,
	kome::objects::Peaks2D& peaks2d
) {
	unsigned int nums = tmpPeaks.getLength();
	double mzf,mz,mze,prc;
	std::tie(mzf, mz, mze) = chromPtr->getQ3s();
	prc = chromPtr->getQ1();
	for (unsigned int i = 0; i < nums; i++) {
		kome::objects::PeakElement* pk = tmpPeaks.getPeak(i);
		kome::objects::Peak2DElement* peak2Delm = peaks2d.createPeak(
			pk->getX(), pk->getLeftX(), pk->getRightX(), mzf, mz, mze, pk->getY());
		peak2Delm->setPrecursorMass(prc);
	}
	return true;
}

// Filtering���s��
const bool MRM3DCore::FilterPeak2D (
	kome::objects::Peaks2D& peaks2d
) {
	if (!m_parser->IsUseIntensityFilter()) {
		return false;
	}
	// sort
	peaks2d.sortByIntensity(true);

	unsigned int peaknum = peaks2d.getNumberOfPeaks();
	typedef kome::objects::Peak2DElement tP2D;

	std::vector<tP2D*> vp2d;
	tP2D* tmpP2d;

	if (m_parser->IsTypeREL()) {
		tmpP2d = peaks2d.getPeak(0);
		m_parser->setMaxIntensity(tmpP2d->getIntensity());
	}


	for (unsigned int i = 0; i < peaknum; i++) {
		tmpP2d = peaks2d.getPeak(i);
		if (m_parser->compInRange(tmpP2d->getIntensity()) != 0) {
			vp2d.push_back(tmpP2d);
		}
	}
	m_parser->showIntensityRangeInfo(vp2d.size());
	BOOST_FOREACH(tP2D* t, vp2d) {
		peaks2d.removePeak(t);
	}
	return true;
}
