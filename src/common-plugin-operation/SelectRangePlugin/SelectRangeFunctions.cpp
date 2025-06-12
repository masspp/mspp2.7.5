/**
 * @file SelectRangeFunctions.cpp
 * @brief implements of SelectRange plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 
 *  <�⑫����>

 * �EVersion 2.0.0 �ł̕ύX�_  2011.05.19 <Mod> M.Izumi
  View > Operation > Profile Range �����j���[�o�[����폜
   status�Ɋ֌W�Ȃ��͈͑I���������s�Ȃ��悤�ɕύX

   �X�e�[�^�X�o�[�̐ݒ�E�\���̌Ăяo���֐��ǉ�

    �� �ύX�ӏ����������ߕύX�ӏ��͍폜���Ă��܂��B
   �@  �K�v�����ȉӏ��Eɂ́A�R�����g���c���Ă��܂��B	
 */

#include "stdafx.h"
#include "SelectRangeFunctions.h"
#include "SelectRangeManager.h"


using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// on close spectrum
kome::objects::Variant onCloseSpec( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	// close
	SelectRangeManager::getInstance().close( *spectrum );

	return ret;
}

// on close chromatogram
kome::objects::Variant onCloseChrom( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	// close
	SelectRangeManager::getInstance().close( *chromatogram );

	return ret;
}

// on button down (spectrum)
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	if( !mgr.isPeakLabelEditMode() ){
		// on button down
		ret.prim.boolVal = mgr.downButton( *spectrum, *mouseEvent, *graphics );
	}
	return ret;
}

// on button down (chromatogram)
kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	if( !mgr.isPeakLabelEditMode() ){
		// on button down
		ret.prim.boolVal = mgr.downButton( *chromatogram, *mouseEvent, *graphics );
	}
	return ret;
}

// on mouse move (spectrum)
kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	if( !mgr.isPeakLabelEditMode() ){
		// on mouse move
		ret.prim.boolVal = mgr.moveMouse( *spectrum, *mouseEvent, *graphics );
	}
	return ret;
}

// on mouse move (chromatogram)
kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	if( !mgr.isPeakLabelEditMode() ){
		// on mouse move
		ret.prim.boolVal = mgr.moveMouse( *chromatogram, *mouseEvent, *graphics );
	}
	return ret;
}

// on button up (spectrum)
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	if( !mgr.isPeakLabelEditMode() ){
		// on button up
		ret.prim.boolVal = mgr.upButton( *spectrum, *mouseEvent, *graphics, canvas );
	}
	return ret;
}

// on button up (chromatogram)
kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	if( !mgr.isPeakLabelEditMode() ){
		// on button up
		ret.prim.boolVal = mgr.upButton( *chromatogram, *mouseEvent, *graphics, canvas );
	}
	return ret;
}

// on double click (spectrum)
kome::objects::Variant onSpecDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// on double click
	ret.prim.boolVal
		=  SelectRangeManager::getInstance().dblClick( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// on double click (chromatogram)
kome::objects::Variant onChromDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// on double click
	ret.prim.boolVal
		= SelectRangeManager::getInstance().dblClick( *chromatogram, *mouseEvent, *graphics, canvas );

	return ret;
}

// draw spectrum foreground
kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011.09.07 <Add> M.Izumi
	// draw
	SelectRangeManager::getInstance().drawForeground( *spectrum, *graphics, canvas  ); // wxWindow* �ǉ��@@date 2011.09.07 <Mod> M.Izumi

	return ret;
}

// draw chromatogram foreground
kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );	// @date 2011.09.07 <Add> M.Izumi
	// draw
	SelectRangeManager::getInstance().drawForeground( *chromatogram, *graphics, canvas ); // wxWindow* �ǉ��@@date 2011.09.07 <Mod> M.Izumi

	return ret;
}

// draw spectrum background
kome::objects::Variant onDrawSpecBg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	
	// draw
	SelectRangeManager::getInstance().drawBackground( *spectrum, *graphics );

	return ret;
}

// draw chromatogram background
kome::objects::Variant onDrawChromBg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	// draw
	SelectRangeManager::getInstance().drawBackground( *chromatogram, *graphics );	

	return ret;
}

// �X�e�[�^�X�o�[�̐ݒ� (Spectrum)
kome::objects::Variant SetValStatusBarSpec( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
		
	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ){
		return ret;
	}
	
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// on mouse move
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	
	// �����ǉ� ( Spectrum*, Chromatogram* ) @date 2011.09.07 <Mod> M.Izumi
	ret.prim.boolVal = mgr.setValStatusBar( *mouseEvent, *graphics, canvas, spectrum, NULL );	
	
	return ret;
}

// �X�e�[�^�X�o�[�̐ݒ� (Chromatogram)
kome::objects::Variant SetValStatusBarChrom( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
		
	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ){
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// on mouse move
	SelectRangeManager& mgr = SelectRangeManager::getInstance();
	
	// �����ǉ� ( Spectrum*, Chromatogram* ) @date 2011.09.07 <Mod> M.Izumi
	ret.prim.boolVal = mgr.setValStatusBar( *mouseEvent, *graphics, canvas, NULL, chromatogram );	


	return ret;
}

// on mouse leave
kome::objects::Variant onMouseLeave( kome::objects::Parameters* params ){
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
			
	statusBar.setStatusMsg( "range", "" );
	statusBar.setStatusMsg( "coordinate", "" );
	statusBar.setFlgDisp( "range", 0 );			// �X�e�[�^�X�o�[�𖳌���
	statusBar.setFlgDisp( "coordinate", 0 );	// �X�e�[�^�X�o�[�𖳌���
	ret.prim.boolVal = true;
	
	
	return ret;
}


