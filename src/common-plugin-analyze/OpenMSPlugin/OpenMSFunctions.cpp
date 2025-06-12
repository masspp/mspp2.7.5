/**
 * @file OpenMSFunctions.cpp
 * @brief implements of OpenMS plug-in functions
 *
 * @author S.Tanaka
 * @date 2015.07.01
 * 
 * Copyright(C) 2015 Mass++ All rights reserved.
 */


#include "stdafx.h"
#include "OpenMSFunctions.h"

#include "OpenMS.h"


using namespace kome::thirdparty::openms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// detect peaks
kome::objects::Variant detectPeaksByWavelet( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get xy data object
	kome::core::XYData* xyData = kome::plugin::PluginCallTool::getXYData( params );
	if( xyData == NULL ) {
		return ret;
	}

	// get peaks object
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ) {
		return ret;
	}

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// peak detection
	OpenMS oms( settings );
	oms.wavelet( *xyData, *peaks );

	return ret;
}

// detect peaks
kome::objects::Variant detectPeaksByHiRes( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get xy data object
	kome::core::XYData* xyData = kome::plugin::PluginCallTool::getXYData( params );
	if( xyData == NULL ) {
		return ret;
	}

	// get peaks object
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ) {
		return ret;
	}

	// parameters
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// peak detection
	OpenMS oms( settings );
	oms.hires( *xyData, *peaks );

	return ret;
}
