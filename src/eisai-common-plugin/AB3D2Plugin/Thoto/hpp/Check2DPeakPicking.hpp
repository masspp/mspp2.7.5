/**
2014/01/22 M.Fukuda
#Mass++�S�̂�Ab3DPluginTop.cpp�̂ݎg�p���m�F�B
#�R�[�h���ڐA�������߁A�s�v�ɂȂ�܂����B
#�\�����[�V��������폜�Ώۂɂ��܂�
**/

/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
    @addtogroup Mspp PluginCall helper headers
    @file       2DPeak_Picking.hpp
	@brief      2DPeak_Picking�Ƃ��ăR�[�����ꂽ�ۂ̕⏕
	@date       2012.12.13
	@author     M.Fukuda
	@version    1.0.0
****************************************************************/


#pragma once

//#include "stdafx_mininumSet.h"

namespace mspp {

// Plugin
inline const bool check2DPeakPickingParams (
	kome::objects::Parameters* params,
	kome::objects::DataSet*& dataSet,
	kome::objects::Peaks2D*& peaks2d,
	kome::objects::SettingParameterValues*& settings,
	kome::core::Progress*& progress
){
	// Plugin Call ���ꂽ���A
	// Param���珊��̎葱�������āA�e�v�f�������o���K�v�L��B
	// �r���ŃG���[�ɂȂ����ꍇ�͕K��������ł��؂�̂ŁA
	// ���ʉ��ׂ̈ɏ������B
	typedef kome::plugin::PluginCallTool tPluginCallTool;
	bool noErrorFg = true;

	// check dataSet
	dataSet = tPluginCallTool::getDataSet(params);
	if ( dataSet == nullptr ) {
		LOG_ERROR(FMT("It failed to get the dataSet"));
		noErrorFg =  false;
	}
	// check peaks2D
	peaks2d = tPluginCallTool::getPeaks2D(params);
	if ( peaks2d == nullptr ) {
		LOG_ERROR(FMT("It failed to get the peaks2D"));
		noErrorFg =  false;
	}
	// check have setting params
	settings = tPluginCallTool::getSettingValues(params);
	if (settings == nullptr) {
		LOG_ERROR(FMT("It failed to get the settings"));
		noErrorFg = false;
	}
	// set Progress
	progress = tPluginCallTool::getProgress(params);

	if( progress == nullptr ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	return noErrorFg;
}

} // end of namespace
