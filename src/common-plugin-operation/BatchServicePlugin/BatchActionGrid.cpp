/**
 * @file BatchActionGrid.cpp
 * @brief interfaces of BatchActionGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchActionGrid.h"
#include "BatchActionSelectPanel.h"

#include <vector>

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( BatchActionGrid,  kome::window::EditGridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, BatchActionGrid::onColClick )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, BatchActionGrid::onDoubleClick )
END_EVENT_TABLE()


// �R���X�g���N�^
BatchActionGrid::BatchActionGrid( wxWindow* parent, int nPanelNo, int nActionKind )
  : kome::window::EditGridListCtrl( parent, false ) {

	m_pParent = parent;
	m_nPanelNo = nPanelNo;
	m_nActionKind = nActionKind;

	init();
	
	if( nActionKind == 0 ){
		setEditMenuName( "Edit" );
		setUpDown( true );
		setPopupMenuFlag( POPUP_EDIT | POPUP_UP | POPUP_DOWN | POPUP_SELECT | POPUP_DESELECT | POPUP_DELETE );
	}	

	setAddMenuName("");
}


// �f�X�g���N�^
BatchActionGrid::~BatchActionGrid() { 
}


// �����o�ϐ��̏�����
void BatchActionGrid::init( void ) {
	/** �Z���̍s�� */
	int m_nRows = 0;

	/** �Z���̗� */
	m_nCols =2;

	/** �A�N�V�������X�g������ */
	m_vectActions.clear();

	clearData( true ); // SPEC No.89645 Batch Prosessing�ŃR���{�{�b�N�X��؂�ւ������ɋ�̗񂪎c�� @date 2012.12.20 <Add> M.Izumi
}

// �e�[�u���쐬
void BatchActionGrid::onCreate() {
    // ��
	addColumn( "Name", TYPE_STRING, 150 );		// add width @date 2013.04.01 <Add> M.Izumi
	addColumn( "Precedence", TYPE_STRING, 70 );	// add width @date 2013.04.01 <Add> M.Izumi

	if( m_nActionKind == 1 ){	// Available Actions
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();

		int nCntAction = mgr.getCountOfAvailableActions();

		for( int i=0; i<nCntAction; i++ ){
			Action acTmpAction;
			mgr.getAvailableAction( i, &acTmpAction );

			stage_no anEnum2StageNo[] = { 
				SN_INITIALIZE,		// �@Initialize
				SN_FILTER,			// �A
				SN_MANIPULATE,		// �B
				SN_ANALYZE,			// �C
				SN_IDENTIFY,		// �D
				SN_FILTER_RESULTS,	// �EFilter Results
				SN_SENTINEL			// �ԕ�				
			};
			if( acTmpAction.nStage == SN_SENTINEL ){
				continue;
			}

			if( acTmpAction.nStage ==  anEnum2StageNo[m_nPanelNo] ){
				acTmpAction.nStage = acTmpAction.nStage;
				acTmpAction.strName = acTmpAction.strName;
				acTmpAction.nPrecedence = acTmpAction.nPrecedence;

				m_vectActions.push_back( acTmpAction );
			}
		}
	}

	RefreshBatchActionGrid();
}


//int BatchActionGrid::addAction( Action act )
int BatchActionGrid::addAction( Action& act )
{
	m_vectActions.push_back( act );

	clearData( true );

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	mgr.sortActionsByPrecedenceRef( m_vectActions );

	RefreshBatchActionGrid();

	return 0;
}

// �w��s���X�V
int BatchActionGrid::updateAction(  int index, Action act ){

	if( index < 0 ){
		return -1;
	}

	if( m_vectActions.size() <= (unsigned int)index ){
		return -1;
	}

	m_vectActions[index] = act;

	return 0;

}

// Action��ǉ�
int BatchActionGrid::deleteAction( int index ){

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


// Grid���ĕ`��
int BatchActionGrid::RefreshBatchActionGrid( void ){

    // �s
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vectActions.size(); i++ ) {
		long long llTempVal = (long long)plgMgr.getPlugin( i );
		addData( (long long)( i ), false );
    }

	return 0;
}


// �����Ŏw�肳�ꂽ�s�����ւ���
int BatchActionGrid::swapAction( int nIndex1, int nIndex2 ){
	
	int nCountOfVectActions = getCountOfVectActions();

	// �͈̓`�F�b�N
	if( nIndex1 < 0 ){
		return -1;
	}
	if( nIndex2 < 0 ){
		return -1;
	}
	if( nCountOfVectActions <= nIndex1 ){
		return -1;
	}
	if( nCountOfVectActions <= nIndex2 ){
		return -1;
	}
	if( nIndex1 == nIndex2 ){
		return 0;	// swap�̕K�v�Ȃ�
	}

	// Precedence �`�F�b�N
	if(	m_vectActions[nIndex1].nPrecedence == m_vectActions[nIndex2].nPrecedence ){
		// OK
	}else{
		// NG
		return -1;
	}

	// swap����
	Action actTemp;
	actTemp = m_vectActions[nIndex1];
	m_vectActions[nIndex1] = m_vectActions[nIndex2];
	m_vectActions[nIndex2] = actTemp;

	std::vector<Action>  tempVectActions;
	tempVectActions = m_vectActions;

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	m_vectActions = mgr.sortActionsByPrecedence( tempVectActions );

	return 1;	// swap
}


// BatchActionGrid�������Ă���Action��Ԃ�
Action BatchActionGrid::getAction( int index ){
	bool flgError=false;
	if( index < 0 ){
		flgError = true;
	}
	unsigned long uIndex = index;
	if( m_vectActions.size() <= uIndex ){
		flgError = true;
	}											

	if( flgError )								// @date 2013/05/17 <Add> OKADA
	{
		// err
		Action tmpAct;
		tmpAct.nStage = SN_SENTINEL;
		tmpAct.flgSettings = 0;
		tmpAct.nBatchIndex = -1;
		tmpAct.nPrecedence = -1;
		return tmpAct;
	}

	return m_vectActions[index];
}


// ������擾
std::string BatchActionGrid::getString( const long long data, const int col ) const {
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

    // �l�擾
    if( col == 0 ) {    // plugin name
		if( SN_SENTINEL <= m_vectActions[data].nStage ){
			s = "";	// error
		}else{
			s = m_vectActions[data].strName;
		}
    }
    else if( col == 1 ) {    // link
		char szTemp[256];
		sprintf( szTemp, "%d", m_vectActions[data].nPrecedence );
		s = szTemp;
    }
    return s;
}


// �����擾
int BatchActionGrid::getInt( const long long data, const int col ) const {
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
int BatchActionGrid::setRows( int nRow ){
	if( 0 <= nRow ){
		m_nRows = nRow;
	}

	return m_nRows;
}

// �^�C�g�����N���b�N���Ă������N���Ȃ��悤�ɂ���
void BatchActionGrid::onColClick( wxListEvent& evt ) {
}

void BatchActionGrid::onDoubleClick( wxListEvent& evt ) {
	
	if( m_pParent != NULL ){
		wxCommandEvent evtDummy;
		BatchActionSelectPanel* ptr = ((BatchActionSelectPanel*)(m_pParent));

		std::string strTemp = ptr->getParentDlg()->getComboMethodText();
		if( 0 < strTemp.length() ){
			;	// Batch Method���󗓂łȂ�
		}else{
			// Batch Method����
			return;	// Batch Method���󗓂̏ꍇ�́AAdd/edit�ł��Ȃ��悤�ɂ���
		}

		if( m_nActionKind == 0 ){	// Selected Actions
			ptr->onEdit( evtDummy );
		}
		if( m_nActionKind == 1 ){	// Available Actions
			ptr->onAdd( evtDummy );
		}
	}

	return;
}

// on delete data edit
bool BatchActionGrid::onDeleteData( const long long data ){
	
	wxCommandEvent evtDummy;
	BatchActionSelectPanel* ptr = ((BatchActionSelectPanel*)(m_pParent));
	if( ptr != NULL ){
		ptr->onDeleteLine( (int)data, true );	// @date 2013/06/04 <Add> OKADA
	}

	// @date2012.12.19 <Mod> M.Izumi
	// ����BatchActionSelectPanel::onDelete���\�b�h�Őe�N���X��m_data���N���A���Ă���ׁBFalse��Ԃ��悤�ɕύX�B
	return false;
}
