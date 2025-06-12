/**
 * @file XPeakDetector
 * @brief Spectrum PeakPicking & Filtering 
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "XPeakDetector.h"
#include <tuple>

namespace kome {
namespace massbank {
namespace wizard {

// constructor
XPeakDetector::XPeakDetector() 
	: peakDetector_(nullptr)
	, params_(nullptr)
	, m_peakfilters("")
	, procStream_("")
{
}

// destructor
XPeakDetector::~XPeakDetector(){}

// PeakDetector�ƃp�����[�^���Z�b�g����B
void XPeakDetector::setPeakInfo(
	kome::plugin::PluginFunctionItem* _peakdetector,
	kome::objects::SettingParameterValues* _params
){
	peakDetector_ = _peakdetector;
	params_ = _params;
}

// Filter �����Z�b�g����
void XPeakDetector::setFilterInto(const std::string _filter) 
{
	m_peakfilters = _filter;
}

// Peak Picking��Filter������B
// �����s���� Peak���c����̂� dstArr�Ƃ��ĕԂ����B
const bool XPeakDetector::doPeakPickAndFilter(
	std::vector<kome::objects::Spectrum*>& srcArr,
	std::vector<kome::objects::Spectrum*>& dstArr,
	const bool usingProcFg
){
	if(peakDetector_ == nullptr) {
		return false;
	}

	std::vector<kome::objects::Spectrum*> spectra;
	std::vector<kome::objects::Peaks*> peaksArr;
	std::string ErrMsg(""),ErrMsg2("");

	ErrMsg = onExecPeakPicking(srcArr, usingProcFg, spectra, peaksArr);
	if(ErrMsg.empty()) {
		ErrMsg2 = onExecPeakFilter(spectra, peaksArr, dstArr);
	}
	
	if (ErrMsg.size() > 0 || ErrMsg2.size() > 0) {
		std::string rstErrMsg = ErrMsg + ErrMsg2;
		//���X�g�Ƃ��ďo���B�x���ǂ܂�B
		kome::window::WindowTool::showWarning(rstErrMsg.c_str(), "no Peaks");
	}

	return true;
}

const bool XPeakDetector::doPeakPickAndFilter(
	kome::objects::Spectrum* spec,
	kome::core::DataPoints& dps,
	const bool usingProcFg
){
	kome::objects::Peaks* peaks;
	std::vector<kome::objects::Spectrum*> spectra;
	std::vector<kome::objects::Peaks*> peaksArr;

	if(getProcceDataPoints(spec, usingProcFg, dps)) {
		execSpectrumPeakPicking(spec, &dps, peaks);
		if (peaks->getLength() > 0) {
			spectra.push_back(spec);
			peaksArr.push_back(peaks);
		}
	}
	return true;
}


// Peak Picking�̎��s
const std::string XPeakDetector::onExecPeakPicking(
	std::vector<kome::objects::Spectrum*>& srcArr,
	const bool usingProcFg,
	std::vector<kome::objects::Spectrum*>& spectra,
	std::vector<kome::objects::Peaks*>& peaksArr
){	
	//PeakPicking�̎��s
	using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

	Peaks* peaks = NULL;
	std::vector<Spectrum*> nopeaksArr;
	typedef std::vector<Spectrum*>::iterator vITR;

	for each(Spectrum* spec in srcArr) {
	// Can not Do Peak Detectoin
		if (spec != NULL) {
			kome::core::DataPoints pts;
			if(getProcceDataPoints(spec,usingProcFg,pts)) {
				kome::core::XYData* xyData = &pts;
				if (execSpectrumPeakPicking(spec, &pts, peaks)) {
					// �v�f�̎c�������̂������X�g�A����
					if (peaks->getLength() > 0) {
						spectra.push_back(spec);
						peaksArr.push_back(peaks);
					} else {
						nopeaksArr.push_back(spec);
					}
				}
			}
		}
	}
	std::string ErrMsg = toErrorMsg(nopeaksArr,0);
	return ErrMsg;
}

// �s�[�N�s�b�L���O���s��
const bool XPeakDetector::execSpectrumPeakPicking (
	kome::objects::Spectrum* spec, 
	kome::core::XYData* xyData,
	kome::objects::Peaks*& peaks
){
	// create peaks
	using namespace kome::plugin;
	using namespace kome::objects;
	PeaksManager& pkMgr = PeaksManager::getInstance();
	peaks = pkMgr.createPeaks( spec );
	peaks->setDataPoints( *xyData );

	// create parameters
	Parameters peakParams;
	PluginCallTool::setSpectrum( peakParams, *spec );
	PluginCallTool::setXYData( peakParams, *xyData );
	PluginCallTool::setPeaks( peakParams, *peaks );

	if( params_ != NULL ) {
		PluginCallTool::setSettingValues( peakParams, *params_ );
	}
	peakDetector_->getCall()->invoke( &peakParams );
	return true;
}

// DataPoints���擾����
const bool XPeakDetector::getProcceDataPoints(
	kome::objects::Spectrum* spec,
	const bool usingProcFg,
	kome::core::DataPoints &pts
){
	// Using Processed data �ɑΉ��̈�
	if(usingProcFg) {
		// Mass++�̉�ʂœK���ς݂�op�̓��e��K������
		spec->getXYData(&pts, true);
	} else {
		// �f�̑S�̂�Spectrum���擾����B
		spec->getXYData(&pts, -1.0, -1.0, true);
		// Proceed��K��	
		takeProccessing(pts, pts, spec);
	}
	return (&pts != NULL );
}

//Processing�����s����
const bool XPeakDetector::takeProccessing(
	kome::core::DataPoints& src,
	kome::core::DataPoints& dst,
	kome::objects::Spectrum* spec
){
	kome::core::DataPoints dps1,dps2;
	dps2 = src;
	for(unsigned int i = 0; i < opts_.size(); i++) {
		dps1.clearPoints();
		dps1 = dps2;
		dps2.clearPoints();
		opts_[i]->update(dps1, dps2, *spec);
	}
	dst = dps2;
	return true;
}

// Filter�̓K��
const std::string XPeakDetector::onExecPeakFilter(
	std::vector<kome::objects::Spectrum*>& specArr,
	std::vector<kome::objects::Peaks*>& peaksArr,
	std::vector<kome::objects::Spectrum*>& dstArr
){
	std::vector<kome::objects::Spectrum*> nopeaksArr2;
	// PeakFilter�̎��s
	unsigned int specsNum = specArr.size();
	if (specsNum > 0 && m_peakfilters.size() > 0) {
		kome::plugin::PluginCallTool::filterSpecPeaks( 
			&specArr[0], &peaksArr[0], specsNum, m_peakfilters.c_str());
	}

	// makeResult
	dstArr.clear();

	for(unsigned int i = 0; i < specsNum; i++) {
		if(peaksArr[i]->getLength() > 0) {
			dstArr.push_back(specArr[i]);
		} else {
			nopeaksArr2.push_back(specArr[i]);
		}
	}
	return toErrorMsg(nopeaksArr2, 1);
}

//Peak Picking�Ńs�[�N�̖������m������
const std::string XPeakDetector::toErrorMsg(
	std::vector<kome::objects::Spectrum*>& nomatch,
	const int itype
){
	std::string ErrMsg = " Records of following spectrum/spectra is/are not exported\n ";
	if(nomatch.empty()) {
		return "";
	}
	if(itype == 0) {
		ErrMsg += "because no peaks are detected:\n";
	} else {
		ErrMsg += "because peaks are filtered:\n";
	}
	typedef std::vector<kome::objects::Spectrum*>::iterator vITR;
	ErrMsg += nomatch[0]->getName();
	int backcnt = 0;
	for (vITR it = nomatch.begin() + 1, eit = nomatch.end(); it != eit; ++it) {
		ErrMsg += ", ";
		ErrMsg += (*it)->getName();
		backcnt++;
		if (backcnt > 5) {
			ErrMsg += "\n";
		}
	}
	return ErrMsg;
}


} // end of namespace
}
}

////Processing�����s����
//const bool XPeakDetector::takeProccessing(
//	kome::core::DataPoints& src,
//	kome::core::DataPoints& dst,
//	kome::objects::Spectrum* spec
//){
//	using kome::objects::XYDataOperation;
//	unsigned int inum = items_.size();
////	unsigned int inum = itemArr_.size();
//	for(unsigned int i = 0; i < inum; i++) {
////		auto p = itemArr_[i];
//		kome::plugin::PluginFunctionItem* item = items_[i];
//
//		if(item != NULL) {
//			kome::objects::Parameters params;
////			kome::plugin::PluginCallTool::setSettingValues(params, itemArr_[i].second);
//			kome::plugin::PluginCallTool::setSettingValues(params, setVals_[i]);
//			kome::plugin::PluginCallTool::setSpectrum(params, *spec);
//
//			kome::objects::XYDataOperation* opt = NULL;
////			kome::objects::XYDataOperation* opt = NULL;
////			kome::core::DataPoints dps;
//	//		std::unique_ptr<XYDataOperation> opt(
//	/*		kome::plugin::PluginCall* pcl = item->getCall();
//			kome::objects::Variant kov = pcl->invoke(&params);
//			opt = (kome::objects::XYDataOperation*)kov.prim.pt;*/
//			opt = (XYDataOperation*)item->getCall()->invoke(&params).prim.pt;
//			if(opt!=nullptr) {
//				opt->update(src, dst, *spec);
//			}
//		}
//	}
//	return true;
//}

