/**
 * @file BatchGrid.cpp
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchGrid.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( BatchGrid,  kome::window::GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, BatchGrid::onColClick )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, BatchGrid::onItemActivated )	// @date 2012/10/17 <Add> OKADA
END_EVENT_TABLE()


// �R���X�g���N�^
BatchGrid::BatchGrid( wxWindow* parent )
    : kome::window::GridListCtrl( parent, wxID_ANY, 400, 300, false ) {

	m_pParentDlg = (kome::batchservice::BatchProcessingMethodConfigurationDialog*)parent;	// @date 2012/10/17 <Add> OKADA

	init();
}

// �f�X�g���N�^
BatchGrid::~BatchGrid() { 
}


// �����o�ϐ��̏�����
void BatchGrid::init( void ) {

	/** �Z���̍s�� */
	int m_nRows = 0;

	/** �Z���̗� */
	m_nCols =2;

	/** �A�N�V�������X�g������ */
	m_vectActions.clear();
}


// �e�[�u���쐬
void BatchGrid::onCreate() {
    // ��
    addColumn( "Stage", TYPE_STRING );
    addColumn( "Action", TYPE_STRING );

	// Dummy
	Action tempAct;

	// �s
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vectActions.size(); i++ ) {
		long long llTempVal = (long long)plgMgr.getPlugin( i );
		addData( (long long)( i ), false );	// TEST
    }
}


int BatchGrid::addAction( Action act ){
	m_vectActions.push_back( act );

	clearData( true );

	RefreshBatchGrid();

	return 0;
}


int BatchGrid::updateAction(  int index, Action act ){

	if( index < 0 ){
		return -1;
	}

	if( m_vectActions.size() <= (unsigned int)index ){
		return -1;
	}

	m_vectActions[index] = act;

	return 0;

}


int BatchGrid::deleteAction( int index ){

	if( index < 0 ){
		return -1;
	}

	if( m_vectActions.size() <= (unsigned int)index ){
		return -1;
	}

	std::vector<Action>::iterator it = m_vectActions.begin();
	it += index;

	m_vectActions.erase( it );      // it�̈ʒu�̗v�f���폜
	
	return 0;
}


int BatchGrid::RefreshBatchGrid( void ){

    // �s
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vectActions.size(); i++ ) {
		long long llTempVal = (long long)plgMgr.getPlugin( i );
		addData( (long long)( i ), false );	// TEST
    }

	return 0;
}


Action& BatchGrid::getAction( int index ){
	return m_vectActions[index];
}


// ������擾
std::string BatchGrid::getString( const long long data, const int col ) const {
    // ������
    std::string s;

    // �v���O�C�����
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;

	if( data < m_vectActions.size() ){
		;	// OK
	}else{
		// error
		return "";
	}
	if( data < 0 ){
		// error
		return "";
	}

	Action tempAct;
	tempAct = m_vectActions[data];

    // �l�擾
    if( col == 0 ) {    // plugin name
		if( SN_SENTINEL <= tempAct.nStage ){
			s = "";	// error
		}else{
			
			int nFlgAddStr = 0;
			if( data == 0 ){
				// �擪�s�͕K���ǉ�
				nFlgAddStr = 1;
			}else if( tempAct.nStage == m_vectActions[data-1].nStage ){
				// Stage��1�s�O�̕��Ɠ����Ȃ�󔒂�ǉ�
				nFlgAddStr = 0;
			}else{
				nFlgAddStr = 1;
			}

			if( nFlgAddStr == 1 ){
				s = g_aszStageString[tempAct.nStage];
			}else{
				s = "";
			}
		}
    }
    else if( col == 1 ) {    // link
		s = tempAct.strName;
    }

    return s;
}


// �����擾
int BatchGrid::getInt( const long long data, const int col ) const {
    // ����
    int v = -1;

    // �v���O�C�����
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;
    if( data == NULL ) {
        return v;
    }

    // �l�擾
    if( col == 2 ) {    // call count
        v = plugin->getNumberOfCalls();
    }

    return v;
}


// �s���ݒ�
int BatchGrid::setRows( int nRow ){
	if( 0 <= nRow ){
		m_nRows = nRow;
	}

	return m_nRows;
}

// �^�C�g�����N���b�N���Ă������N���Ȃ��悤�ɂ���
void BatchGrid::onColClick( wxListEvent& evt ) {
	int ddd=0;
}

// �Z�����_�u���N���b�N
void BatchGrid::onItemActivated( wxListEvent& evt ) 
{
	int nIndex = evt.m_itemIndex;	// �I���s��\���B��ԏ�̍s��0�ŕԂ�

	int nLineNo=0;
	for( int i=0; i<=nIndex; i++ ){
		if( m_vectActions[nIndex].nStage == m_vectActions[i].nStage ){
			nLineNo++;
		}
	}
	if( nLineNo <= 0 ){
		return;
	}
	int nLineNoBaseZero = nLineNo-1;	// �s�ԍ���0�`�ɂ���

	// nStage �^�u�� nLineNo���ږځi���ڂ�1����n�܂�j�̍��ڂ�ҏW

	int nTabNo = m_vectActions[nIndex].nStage;

	switch( nTabNo ){
	case SN_INITIALIZE:		// �@Initialize	// fall-through
	case SN_FILTER:			// �A
	case SN_MANIPULATE:		// �B
	case SN_ANALYZE:		// �C
	case SN_IDENTIFY:		// �D
	case SN_FILTER_RESULTS:	// �EFilter Results
		m_pParentDlg->editBatchActionSelectPanel( nTabNo, nLineNoBaseZero );
		break;

	default:
		break;
	}

	return;
}
