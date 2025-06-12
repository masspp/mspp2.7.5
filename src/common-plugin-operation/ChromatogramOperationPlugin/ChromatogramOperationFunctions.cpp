/**
 * @file ChromatogramOperationFunctions.cpp
 * @brief implements of ChromatogramOperation plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.04.10
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * <�⑫����>
 * �EVersion 2.0.0�ł̕ύX�_�@2011.05.19 <Mod> M.Izumi

 �@Spectrum �� Chromatogram
   �g�`��MC/BPC���E�h���b�O��Ƀ_�C�A���O�ŕ\�����A�I�������`���ŁA
   �I��͈͂�chromatogram��\������B
 �@
   Chromatogram �� Spectrum
   �g�`�͈́irt)��\�������_�C�A���O��\������B

   ���j���[�o�[�A�c�[���o�[�����L�̍��ڂ��폜

   add XIC�ɂ��ẮA�������c���Ă��܂���2.0.0�ł͕\�����Ȃ��B
   �I�v�V�����ŕK�v�ȃ��[�U�[�ɂ̂ݔz�z�H

    �� �ύX�ӏ����������ߕύX�ӏ��͍폜���Ă��܂��B
   �@ �K�v�����ȉӏ��ɂ́A�R�����g���c���Ă��܂��B

 */


#include "stdafx.h"
#include "ChromatogramOperationFunctions.h"
#include "ChromatogramOperationManager.h"
#include "GenelateChromatogramDialog.h"
#include "GenelateChromOperation.h"

using namespace kome::operation::chrom;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::operation::sel_range;

#define CHROMATOGRAM_FILTER_STATUS_NAME		"CHROMATOGRAM_FILTER"
#define STATUSBAR_CHROM						"ChromFilter"

// set chromatotram filter
kome::objects::Variant setChromatogramFilter( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// set filter
	kome::objects::SpectraChromatogram* gchrom
		= dynamic_cast< kome::objects::SpectraChromatogram* >( chrom );
	if( gchrom != NULL ) {
		gchrom->setFilter( ChromatogramOperationManager::getInstance().getFilter() );
	}

	return ret;
}

// set chromatogram filter "all"
kome::objects::Variant setFilterAll( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( CHROMATOGRAM_FILTER_STATUS_NAME, "all" );

	// set filter
	ChromatogramOperationManager::getInstance().setFilter( 0xff );
	// set Status Bar All
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp(STATUSBAR_CHROM, 1);
	statusBar.setStatusMsg(STATUSBAR_CHROM, "Chromatogram Filter: All");
	return ret;
}

// set chromatogram filter "ms"
kome::objects::Variant setFilterMs( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( CHROMATOGRAM_FILTER_STATUS_NAME, "ms" );

	// set filter
	ChromatogramOperationManager::getInstance().setFilter( 0x01 );
	// set Status Bar MS
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp(STATUSBAR_CHROM, 1);
	statusBar.setStatusMsg(STATUSBAR_CHROM, "Chromatogram Filter: MS");
	return ret;
}

// set chromatogram filter "msms"
kome::objects::Variant setFilterMsms( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( CHROMATOGRAM_FILTER_STATUS_NAME, "msms" );

	// set filter
	ChromatogramOperationManager::getInstance().setFilter( 0xfe );
	// set Status Bar MSn
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp(STATUSBAR_CHROM, 1);
	statusBar.setStatusMsg(STATUSBAR_CHROM, "Chromatogram Filter: MSn");
	return ret;
}

//2012/02/24 add by t.okuno -> This plugin distributes 'MC' and 'BPC'.
// show Generate Chromatogram
kome::objects::Variant showGenerateChromatogram( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get group
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {		
		return ret;
	}
	kome::objects::DataGroupNode* group = spec->getGroup();
	if( group == NULL ) {
		return ret;
	}
	
	// get range
	double startMz = double();
	double endMz = double();
	if( !kome::plugin::PluginCallTool::getRange( params, &startMz, &endMz ) ) {
		return ret;
	}
	
	// create Generate Chromatogram dialog
	ChromatogramOperationManager& mgr = ChromatogramOperationManager::getInstance();
	mgr.createGenerateChromatogram( group, startMz, endMz );
	return ret;
}
//2012/02/24 add by t.okuno <- This plugin distributes 'MC' and 'BPC'.

// open generate chromatogram dialog (���j���[����Ă΂ꂽ���̏����j
kome::objects::Variant openGenerateChromatogramDlg( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;


	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get specterum
	kome::objects::DataGroupNode* group = obj.getGroup();
	if( group == NULL ) {
		LOG_ERROR( FMT( "A group is not active." ) );
		return ret;
	}
	
	// create Generate Chromatogram dialog
	ChromatogramOperationManager& mgr = ChromatogramOperationManager::getInstance();
	mgr.createGenerateChromatogram( group, -1.0, -1.0 );

	ret.prim.boolVal = true;
	return ret;
}

// operation
kome::objects::Variant getGenelateChrom( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new GenelateChormOperaton();
	
	return ret;
}
