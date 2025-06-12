/**
 * @file SpectrumChangerPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author okada_h
 * @date 2010.09.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "SpectrumChangerPluginFunctions.h"

#include <wx/wx.h>

// �\���X�y�N�g����؂�ւ���v���O�C��

kome::objects::Variant openNextSpectrum( kome::objects::Parameters* params ) {
	return openNextPrevSpectrum( params, 1 );
}

kome::objects::Variant openPrevSpectrum( kome::objects::Parameters* params ) {
	return openNextPrevSpectrum( params, 0 );
}

// flgPrevNext: 0=Prev 1=Next
kome::objects::Variant openNextPrevSpectrum( kome::objects::Parameters* params, int flgPrevNext ) {
	// �߂�l (������ true, ���s�� false)
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// ���ݑI������Ă���X�y�N�g�����擾
	kome::objects::Spectrum* specSelected = NULL;
	if( var.type == var.SPECTRUM ){
		// ���j���[�Ő؂�ւ��鎞
		kome::objects::MsDataVariant obj( var );
		specSelected = obj.getSpectrum();
	}else{
		// �L�[�{�[�h�C�x���g
		specSelected = kome::plugin::PluginCallTool::getSpectrum( params );
	}
	if( specSelected == NULL ) {
		return ret;
	}

	// �X�y�N�g������group���擾
	kome::objects::DataGroupNode* group = specSelected->getGroup();

	if( group == NULL ) {
		LOG_ERROR( FMT( "A group is not active." ) );
		return ret;
	}
	
	unsigned int uNumberOfSpectrum = group->getNumberOfSpectra();
	if( uNumberOfSpectrum <= 0 ){
		// error	// �X�y�N�g�����P���Ȃ�
		return ret;	// �ُ�I��
	}else{
		long lnNextNum = -1;
		long lnPrevtNum = -1;
		kome::objects::Spectrum* spec;
		for( unsigned int i=0; i<uNumberOfSpectrum; i++ ){
			spec = group->getSpectrum( i );
			if( specSelected->getScanNumber() == spec->getScanNumber() ){
				// ��v
				// SPEC No.79890 Next Spectrum/Prev Spectrum ����Spectrum�摜��\�����Ȃ��s��C��
				// @date 2011.02.07 <Mod> M.Izumi
				lnNextNum =  i+1;
				lnPrevtNum = i-1;

				if( (long)uNumberOfSpectrum <= lnNextNum ){
					lnNextNum = (uNumberOfSpectrum-1);
				}
				if( lnPrevtNum < 0 ){
					lnPrevtNum = 0;
				}

				if( flgPrevNext == 0 ){
					spec = group->getSpectrum( lnPrevtNum );	// Prev
				}else{
					spec = group->getSpectrum( lnNextNum );		// Next
				}

				break;	// �����I��
			}
		}

		// Spectrum�\��
		if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
			kome::plugin::PluginCallTool::openSpectrum( *spec );
		}
	}

	// ����I��
	ret.prim.boolVal = true;

	return ret;
}

// @date 2012.04.19 <Add> M.Izumi ->
// enable next spectrum
kome::objects::Variant enableNextSpectrum( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// Get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected != NULL ) {
		kome::objects::DataGroupNode* group = specSelected->getGroup();
		if( group == NULL ){
			return ret;
		}

		int inum = group->getNumberOfSpectra()-1;
		// �I�𒆂̃X�y�N�g����������ǂ����`�F�b�N
		if(  specSelected != group->getSpectrum( inum ) ){
			ret.prim.boolVal = true;
		}
	}

	return ret;
}

// enable prev spectrum
kome::objects::Variant enablePrevSpectrum( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// Get the currently-selected spectrum
	kome::objects::Spectrum* specSelected = NULL;
	kome::objects::MsDataVariant obj( var );

	specSelected = obj.getSpectrum();
	if( specSelected != NULL ) {
		kome::objects::DataGroupNode* group = specSelected->getGroup();
		if( group == NULL ){
			return ret;
		}
		// �I�𒆂̃X�y�N�g�����擪���ǂ����`�F�b�N
		if( specSelected != group->getSpectrum(0) ){
			ret.prim.boolVal = true;
		}
	}

	return ret;
}
// @date 2012.04.19 <Add> M.Izumi <-

// on button down (spectrum)
kome::objects::Variant onSpecKeyDownX( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::KeyEvent* keyEvent = kome::plugin::PluginCallTool::getKeyEvent( params );
	if( keyEvent == NULL ) {
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
		
	kome::objects::DataGroupNode* group = spectrum->getGroup();
	if( group == NULL ) {
		return ret;
	}
	
	if( ( keyEvent->getCode() == kome::core::key::KEY_UP ) && ( keyEvent->ctrl() == true ) ){
			openPrevSpectrum( params );
	}else if( ( keyEvent->getCode() == kome::core::key::KEY_DOWN ) && ( keyEvent->ctrl() == true ) ){
			openNextSpectrum( params );
	}
	else{
		return ret;
	}

	return ret;
}
