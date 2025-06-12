/**
* @file StatusBar.cpp
* @brief Status Bar / �X�e�[�^�X�o�[��\�����A�C�ӂ̕������\�����܂�
* @author OKADA, H.
* @date 2011/01/07
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <�⑫����>
*   �g�����G
*     0. StatusBar.h ��include���܂��B
*     1. �X�e�[�^�X�o�[�̃C���X�^���X���擾���܂��B
*        kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
*     2. �X�e�[�^�X�o�[�̃����o�֐��A setStatusMsg()��p���ĕ\������������������w�肵�܂�
*        statusBar.setStatusMsg( "common", "TEST" ); �i��j
*     3. �X�e�[�^�X�o�[�̈�ԍ����̗̈�̖��O��"common"�ƌ��܂��Ă���̂ŁA�����̗̈��"TEST"�ƕ\������܂�
*     4. ��q����悤��xml�t�@�C����status-bar�^�O���w�肵���ꍇ�A�����̗̈�ȊO�ɓƎ��̗̈���m�ۂ��ĕ������\���ł��܂�
*     5. �X�e�[�^�X�o�[�̗̈�𖳌��ɂ��邱�Ƃ��o���܂��B�ȉ��̂悤�ɂ���ƁA"VerticalBox"�Œ�`�����̈悪�����Ȃ�܂��B
*        statusBar.setFlgDisp( "VerticalBox", 0 );
*        �ēx�L���ɂ������ꍇ�́A��L���0��1�ɕύX���܂��B���Ȃ��A"common"�𖳌��ɂ��邱�Ƃ͂ł��܂���B
*   
*       "common"�ȊO�̗̈��ݒ肷��ɂ́A�v���O�C����xml�t�@�C��plugin.xml�Ɉȉ��̂悤�ɋL�q���܂��B
*   
*	  <gui>
*		<status-bar target="spectrum"  order="127">
*		  <status-bar-name>VerticalBox</status-bar-name>
*		</status-bar>
*	  </gui>
*   
*	    target�ɂ� "none", "spectrum", "chromatogram", "datamap" �̂��������ꂩ����w�肵�܂��B���Ƃ���
*     target��"spectrum"���w�肵���ꍇ�A�A�N�e�B�u�ȃE�C���h�E���X�y�N�g���̏ꍇ�ɂ��̃X�e�[�^�X�o�[���\������܂��B
*     �X�y�N�g���ƃN���}�g�O���������ŕ\�����������Ƃ������ꍇ�́A��q����悤��status-bar�^�O�𕡐������܂��B
*       order�ɂ́A�\���̗D��x���w�肵�܂��B�X�e�[�^�X�o�[����������ꍇ�A�����琔�������Ȃ����ɕ\������܂��B
*     1�`255��ڈ��ɐݒ肵�ĉ������B�D��x�̒l�����Əd�Ȃ����ꍇ�̕\�������͖���`�ł��B�܂��A"common"�̗D��x��0�ł��B
*       status-bar-name�^�O�ɂ́A�X�e�[�^�X�o�[�̖��O���w�肵�܂��B�����̃v���O�C���œ������O���w�肷��ƁA
*     �����̈�ɕ\�����s�����Ƃ��o���܂��B
*       status-bar�^�O�́A�����L�q�ł��܂��Bstatus-bar-name�^�O�������^�O�𕡐��L�q�����ꍇ�A2�ڈȍ~��order�͖�������܂��B
*     �ȉ��̗�ł́A�uVerticalBox�v�Ƃ������O�̃X�e�[�^�X�o�[��ݒ肵�Aspectrum��chromatogram���A�N�e�B�u�ȂƂ��ɁA
*     setStatusMsg()�֐��Ŏw�肵�������񂪕\�������悤�ɂȂ�܂��B
*	  <gui>
*		<status-bar target="spectrum"  order="127">
*		  <status-bar-name>VerticalBox</status-bar-name>
*		</status-bar>
*		<status-bar target="chromatogram"  order="100">   ���uorder="100"�v��2�ڂȂ̂Ŗ�������܂�
*		  <status-bar-name>VerticalBox</status-bar-name>
*		</status-bar>
*	  </gui>
*   
*   
*   
*/

#include "Stdafx.h"
#include "StatusBar.h"

#include "ChildFrame.h"

using kome::window::StatusBar;


#define MAX_SIZE 10			// @date 2011.10.19 <Add> M.Izumi

// �z��̗v�f���𓾂�_countof()�}�N���̒�`(�|�C���^���w�肵�Ă͂Ȃ�Ȃ�)
#ifndef _countof
	#define _countof(array)	sizeof(array)/sizeof((array)[0])
#else
#endif

// constructor
StatusBar::StatusBar() {
	m_pStatbar = NULL;
	m_pFrame = NULL;
	getMainWindow();	// @date 2011/01/26 <Add> OKADA
	getStatusBarInfo();
}

// destructor
StatusBar::~StatusBar() {
}

// get refer to PluginManager object
StatusBar& StatusBar::getInstance( void ) {
	// create the only object
	static StatusBar statusBar;

	return statusBar;
}

// xml�t�@�C������ǂݍ��܂ꂽ�X�e�[�^�X�o�[����S�Ď擾����
void StatusBar::getStatusBarInfo( void )
{
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	int iCountOfPlugin = plgMgr.getNumberOfPlugins();
	int iCountOfSourceStatusBarInfos;
	kome::plugin::StatusBarInfo* p_tempStatusBarInfo; 

	for( int i=0; i<iCountOfPlugin; i++ ){
		iCountOfSourceStatusBarInfos = plgMgr.getPlugin( i )->getNumberOfStatusBarInfos();
		if( iCountOfSourceStatusBarInfos != 0 ){
			for( int j=0; j<iCountOfSourceStatusBarInfos; j++ ){
				p_tempStatusBarInfo = plgMgr.getPlugin( i )->getStatusBarInfo( j );

				// StatusBarInfo�P���擾

				int iCountOfTotal = m_totalStatusBarInfo.size();
				int flgIcchi = 0;	// ��v������(���ɓo�^����Ă����)�u�P�v

				for( int k=0; k<iCountOfTotal; k++ ){
					if (strcmp( m_totalStatusBarInfo[k].getName(), p_tempStatusBarInfo->getName() ) ){
						// �s��v
					}else{
						// ��v
						flgIcchi = 1;
						m_totalStatusBarInfo[k].setTargetVal( p_tempStatusBarInfo->getTargetVal() );
						m_totalStatusBarInfo[k].setPlugin( p_tempStatusBarInfo->getPlugin() );
						m_totalStatusBarInfo[k].setWidth( p_tempStatusBarInfo->getWidth() ); // @date 2011.10.19 <Add> M.Izuimi
						break;	// �����I��
					}
				}
	
				if( flgIcchi == 0 ){
					// ��x����v���Ȃ�����->�V���ɒǉ�
					m_totalStatusBarInfo.push_back( *p_tempStatusBarInfo );
				}
			}
		}
	}

	// sort statusbar vector / �擾�����X�e�[�^�X�o�[����order�̏��ɕ��ёւ���
	// m_totalStatusBarInfo��order�̏�����sort���܂�
	int iCountOfTotalStatusBarInfo = m_totalStatusBarInfo.size();

	// sort���������C�u�����֐��ōs���ꍇ
	std::stable_sort( m_totalStatusBarInfo.begin(), m_totalStatusBarInfo.end() );

	int iiii = 0;
}


void StatusBar::getMainWindow( void )
{
	m_pFrame = (MainFrame*)kome::window::WindowTool::getMainWindow();

	if( m_pFrame != NULL ){
		if( m_pStatbar == NULL ){
			m_pStatbar = m_pFrame->CreateStatusBar();
		}
	}
}

// "hoge" �Ƃ������L�[���w�肵�āA�����ɕ\�������鏈��
int StatusBar::setStatusMsg( const char* szPos, const char* szMessage )
{
	// �L�[������
	int nPos = getPositionByName( szPos );
	if( nPos < 0 || m_pStatbar == NULL ){	// �s��v
		return -1;	// err
	}else{
		// �\����������w��
		m_totalStatusBarInfo[nPos].setMessage( szMessage );
		// �\��
		dispAuto();

		// @date 2011.10.19 <Add> M.Izumi
		int aryWidth[MAX_SIZE];
		for (int i=0; i < _countof( aryWidth ); i++) {
			aryWidth[i] = 0;
		}

		for( unsigned int i=0; i < m_totalStatusBarInfo.size(); i++ ){
			if( strcmp( m_totalStatusBarInfo[i].getName() , "common" ) == 0 ){
				aryWidth[i] = 540;
			}else{
				aryWidth[i] = m_totalStatusBarInfo[i].getWidth();
			}
		}
		
		m_pStatbar->SetStatusWidths( m_totalStatusBarInfo.size(), aryWidth );
		
	}

	return 0;
}

// ���� �X�e�[�^�X�o�[�́A���ݕ\�����ׂ����ۂ����肷��֐� �Ԓl�F0=�\�����ׂ��łȂ� 1=�\�����ׂ�
int StatusBar::isDispNow( kome::plugin::StatusBarInfo& statusBarInfo )
{
	kome::plugin::StatusBarInfo::StatusBarTargetValue targetVal;

	if( statusBarInfo.getFlgDisp() == 0 ){
		return 0;
	}

	targetVal = statusBarInfo.getTargetVal();
	if( targetVal == kome::plugin::StatusBarInfo::VAL_SB_TGT_NONE ){
		return 1;
	}else{
		kome::plugin::StatusBarInfo::StatusBarTargetValue activeWindowKind;
		activeWindowKind = getActiveWindowKind();

		if( activeWindowKind & targetVal ){
			return 1;
		}
	}

	return 0;
}

// ���݃A�N�e�B�u�ȃE�C���h�E�̎�ނ��擾����
// �Ԓl�G
//   VAL_SB_TGT_OTHER			�A�N�e�B�u�ȃE�C���h�E���擾�ł��Ȃ�
//   VAL_SB_TGT_NONE			�A�N�e�B�u�ȃE�C���h�E�́Aspectrum, chromatogram, datamap�̂ǂ�ł��Ȃ�
//   VAL_SB_TGT_SPECTRUM		�A�N�e�B�u�ȃE�C���h�E�́Aspectrum
//   VAL_SB_TGT_CHROMATOGRAM	�A�N�e�B�u�ȃE�C���h�E�́Achromatogram
//   VAL_SB_TGT_DATAMAP			�A�N�e�B�u�ȃE�C���h�E�́Adatamap
kome::plugin::StatusBarInfo::StatusBarTargetValue StatusBar::getActiveWindowKind( void )
{
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	unsigned int specNum = 0;
	unsigned int chromNum = 0;
	unsigned int mapNum = 0;

	if( frame != NULL ) {
		specNum = frame->getNumberOfSpectra();
		chromNum = frame->getNumberOfChromatograms();
		mapNum = frame->getNumberOfDataMaps();
	}else{
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_OTHER;
	}

	if( (0 < specNum) && (chromNum == 0) && (mapNum == 0) ) {
		// Spectrum �A�N�e�B�u
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_SPECTRUM;
	}
	else if( (specNum == 0) && (0 < chromNum) && (mapNum == 0) ) {
		// Chromatogram �A�N�e�B�u
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_CHROMATOGRAM;
	}
	else if( (specNum == 0) && (chromNum == 0) && (0 < mapNum) ) {
		// Data Map �A�N�e�B�u
		return kome::plugin::StatusBarInfo::VAL_SB_TGT_DATAMAP;
	}
	else {
		// None
	}

	return kome::plugin::StatusBarInfo::VAL_SB_TGT_NONE;
}

// �S�ẴX�e�[�^�X�o�[���`�F�b�N���āA�\�����ׂ����̂�\��
void StatusBar::dispAuto( void )
{
	int iCountOfTotalStatusBarInfo = m_totalStatusBarInfo.size();	// ���ۂɂ́A�\�����ׂ��X�e�[�^�X�o�[�̌����𐔂��� iCountOfTotalStatusBarInfo �ɑ������
	int iFieldsCount = 0;

	if( m_pStatbar == NULL ){
		return;
	}
	
	// �\�����ׂ��t�B�[���h�̐���ݒ肷��
	m_pStatbar->SetFieldsCount( iCountOfTotalStatusBarInfo );	

	// �\�����ׂ��t�B�[���h�̂ݕ\������	
	// Spec No.86227 When dragging on an area in the Heatmap, the end position RT value is not shown in the status bar @date 2012.07.03 <Mod> M.Izumi
	for( int i=0; i<iCountOfTotalStatusBarInfo; i++ ){
		if( m_pStatbar != NULL ){
			if( 0 < isDispNow( m_totalStatusBarInfo[i] ) ){				
				m_pStatbar->SetStatusText( m_totalStatusBarInfo[i].getMessage(), i );				
			}
		}
	}

}


int StatusBar::getPositionByName( const char* szName )
{
	// �L�[������
	int iCountOfTotalStatusBarInfo = m_totalStatusBarInfo.size();

	for( int i=0; i<iCountOfTotalStatusBarInfo; i++ ){
		if( strcmp( m_totalStatusBarInfo[i].getName(), szName ) == 0 ){
			// ��v
			return i;
		}
	}

	return -1;	// err
}


int StatusBar::setFlgDisp(  const char* szPos, int flgDisp )
{
	// �L�[������
	int nPos = getPositionByName( szPos );
	if( nPos < 0 ){	// �s��v
		return -1;	// err
	}else{
		// �\����������w��
		m_totalStatusBarInfo[nPos].setFlgDisp( flgDisp );
		// �\��
		dispAuto();
	}

	return 0;
}
