/**********************************************************/
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       SettingsParamDefine.cpp
    @brief      Settings Parameter Value Support Class
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
Setting Parameter Values������ŕ������ĊǗ�����N���X�B
*/
/**********************************************************/
#include "SettingsParamDefine.h"
#include "stdafx.h"

namespace kome{
namespace mrm3d{

/*!
 *  PARAMERTER XML�̒�`��ꗗ�������ɏ����o���B
 *  �⏕�֐��i�����������ȂǕK�v�Ȃ�N���X��ǉ��Œ�`����
 */
const std::string kXMLPARAM_DETECTOR("chrom_peak_detector");
const std::string kXMLPARAM_SMOOTER("chrom_smoothing");
const std::string kXMLPARAM_USENOISEFILTER("use_noise_filter");
const std::string kXMLPARAM_INTNSITYFILTYPE("intensity_filter_type");
const std::string kXMLPARAM_INTENSITY_MIN_VALUE("minimum_range_value");
const std::string kXMLPARAM_INTENSITY_MAX_VALUE("maximum_range_value");
const std::string kENUM_NOUSE("nouse");
const std::string kENUM_RELATIVE("rel");
const std::string kENUM_ABSOLUTE("abs");

//constructor
ParamParser::ParamParser(kome::objects::SettingParameterValues* s)
	: m_settings(s) {}

//destructor
ParamParser::~ParamParser(){}

//getter
const std::string& ParamParser::getdetectStr(void) const { return kXMLPARAM_DETECTOR; }
const std::string& ParamParser::getsmoothStr(void) const { return kXMLPARAM_SMOOTER; }

//Settings Paramter Values��XML�̏�񂩂�p�[�X����B
//�ߕs���E�ᔽ�������False��Ԃ��B
const bool ParamParser::checkParams(void) {
	if (m_settings == NULL) return false;
	// �K�v�ł���Βǉ�����
	//	m_noiseFlag = m_settings->getBoolValue(kXMLPARAM_USENOISEFILTER.c_str(), false);
	m_noiseFlag = false;

	// Intensity Filter Type����t���O��ݒ�
	std::string stype = m_settings->getParameterValue(kXMLPARAM_INTNSITYFILTYPE.c_str());
	if (stype.compare(kENUM_RELATIVE) == 0) {
		m_intfilFlag = true;
		m_intfilABS = false;
	} else if (stype.compare(kENUM_ABSOLUTE) == 0) {
		m_intfilFlag = true;
		m_intfilABS = true;
	} else {
		m_intfilFlag = false;
	}

	if (m_intfilFlag) {
		double dl,dr;
		dl = m_settings->getDoubleValue(kXMLPARAM_INTENSITY_MIN_VALUE.c_str(),-1.0);
		dr = m_settings->getDoubleValue(kXMLPARAM_INTENSITY_MAX_VALUE.c_str(),DBL_MAX);
		// ��Βl�̏ꍇ
		// ���E�l�͋�(0.0�ȉ�)�Ȃ� -1.0�ɌŒ肷��B
		// ��E�l�͋�(0.0�ȉ�)�Ȃ� DBL_MAX�Ƃ���B
		if (m_intfilABS) {
			m_filL = (dl < 0.0) ? -1.0 : dl;
			m_filR = (dr < 0.0) ? DBL_MAX : dr;
		// ���Βl�̏ꍇ
		// ���MaxIntensity���󂯎��A�Čv�Z����̂Ŕ䗦�݂̂��ێ�����B
		// ���E�l�͋�(0.0�ȉ�)�Ȃ� 0.0�ɂ���B
		// ��E�l�͋�(0.0�ȉ�)�Ȃ� 1.0�Ƃ���B
		} else {
			m_filL = (dl < 0.0) ? 0.0 : dl * 0.01;
			m_filR = (dr > 100.0) ? 1.0 : dr * 0.01;
		}
		// �͈͂̒�`�G���[(���E>��E)�ɂȂ�ꍇ��False
		if (m_filR <= m_filL) return false;
	}
	return true;
}

// Noise Filter���g�����ۂ�
bool ParamParser::IsUseNoiseFilter(void) const {
	return m_noiseFlag;
}

// Intensity Filter���g���Ȃ�True
bool ParamParser::IsUseIntensityFilter(void) const {
	return m_intfilFlag;
}

// Intensity Filter��ABS�Ȃ�True
bool ParamParser::IsTypeABS(void) const {
	return m_intfilABS;
}

// Intensity Filter��Relative�Ȃ�True
bool ParamParser::IsTypeREL(void) const {
	return !(m_intfilABS);
}

// MaxIntensity���擾����B
// Relative�ł���Γ����I�ɍČv�Z����B
void ParamParser::setMaxIntensity(const double maxIntensity) {
	if (!m_intfilABS) {
		m_maxIntensity = maxIntensity;
		m_filL *= maxIntensity;
		m_filR *= maxIntensity;
	}
}

// �͈͈ȉ��Ȃ� -1.�͈͈ȏ�Ȃ� 1.�͈͓��Ȃ� 0.(Strcomp�Ɠ���)
int ParamParser::compInRange(const double tgtIntensity) {
	if (tgtIntensity < m_filL) return -1;
	if (m_filR < tgtIntensity) return 1;
	return 0;
}

// Intensity Filter Range�@���g�p�����ꍇ�Ɍ��ʂ��o���B
// �ז����Ɣ��f���ꂽ�ꍇ�̓R�����g������B
void ParamParser::showIntensityRangeInfo(const unsigned int removenum) const {
	std::string tmpStr = "-- Filtering Intensity Range --\n";
	tmpStr += (m_filL <= 0.0) ? "0.0" : FMT("%f", m_filL);
	tmpStr += " to ";
	if (m_intfilABS) {
		tmpStr += (m_filR == DBL_MAX) ? "Infinity" : FMT("%f", m_filR);
	} else {
		tmpStr += FMT("%f",m_filR);
		if (m_filR == m_maxIntensity) { tmpStr += "(maxIntensity)";}
	}
	tmpStr += FMT("\nFiltered Peaks %d", removenum);
	kome::window::WindowTool::showInfo(tmpStr.c_str());
}

}
}
