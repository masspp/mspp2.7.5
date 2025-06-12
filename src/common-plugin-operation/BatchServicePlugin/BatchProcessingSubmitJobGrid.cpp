/**
 * @file BatchProcessingSubmitJobGrid.cpp
 * @brief implementsation of BatchProcessingSubmitJobGrid class
 *
 * @author OKADA, H
 * @date 2012-02-29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


// BatchProcessingSubmitJobGrid.cpp



#include "stdafx.h"
#include "BatchProcessingSubmitJobGrid.h"
#include "BatchServicePluginManager.h"

namespace
{
	enum col_no_tag{
		COL_TYPE,	// 0
		COL_INPUT,	// 1
		COL_OUTPUT	// 2-
	} col_no;
};


using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( BatchProcessingSubmitJobGrid,  kome::window::GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, BatchProcessingSubmitJobGrid::onColClick )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, BatchProcessingSubmitJobGrid::onDoubleClick )
END_EVENT_TABLE()

// �R���X�g���N�^
BatchProcessingSubmitJobGrid::BatchProcessingSubmitJobGrid( wxWindow* parent )
    : kome::window::GridListCtrl( parent, wxID_ANY, 400, 300, false ) {

	m_pParent = parent;
	m_selectFormatExt = "";
}

// �f�X�g���N�^
BatchProcessingSubmitJobGrid::~BatchProcessingSubmitJobGrid() { 
}

void BatchProcessingSubmitJobGrid::onCreate() {	 

	doCreate();
}


void BatchProcessingSubmitJobGrid::doCreate( void ){

	ClearAll();

    // ��

    addColumn( "Type", TYPE_STRING );	// Type ��		// @date 2012/08/07 <Add> OKADA
	
	addColumn( "Input", TYPE_STRING );	// Input ��

	addColumn( "Output File", TYPE_STRING );	

	m_vecIOSettingsList.clear();
	IOSettingsList tempIOSettingsList;

	/** ���̓t�@�C�� */
	for( unsigned int i=0; i<m_vectstrPaths.size(); i++ ){
	
		std::string strNowTime = m_vectstrOutputFilenameInfo[i];

		tempIOSettingsList.strTypeName.clear();	// @date 2012/08/07 <Add> OKADA
		tempIOSettingsList.strInput.clear();
		tempIOSettingsList.strOutput.clear();
		
		tempIOSettingsList.strTypeName = m_vectstrTypes[i];	// @date 2012/08/07 <Add> OKADA

		tempIOSettingsList.strInput = getfilename( m_vectstrPaths[i].c_str() );	// filename
		
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
		
		// @date 2013.09.30 <Mod> M.Izumi
		std::string strOutput = mgr.getOutputFineName( 
		m_vectstrPaths[i], 
		m_vectstrOutputFilenameInfo[i], 
		m_selectFormatExt.c_str(), 
		i,	// @date 2012/08/09 <Add> OKADA
		0 );

		std::string strFinename = getfilename( strOutput.c_str() );
			
		tempIOSettingsList.strOutput.push_back( strFinename );

		m_vecIOSettingsList.push_back( tempIOSettingsList );	// getString()����Q�Ƃ��܂�
	}

	clearData( false );
	
    // �s
    kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < m_vecIOSettingsList.size(); i++ ) {
		addData( (long long)( i ), false );
    }
}

// get string
std::string BatchProcessingSubmitJobGrid::getString( const long long data, const int col ) const {
    // ������
    std::string s;

    // �v���O�C�����
    kome::plugin::PluginInfo* plugin = (kome::plugin::PluginInfo*)data;

	if( data < m_vecIOSettingsList.size() ){
		;	// OK
	}else{
		// error
		return "";
	}

	IOSettingsList tempIOSettingsList;
	tempIOSettingsList = m_vecIOSettingsList[data];

	// column���񐔂��z���Ă��Ȃ����OK
	if( (unsigned long)col < (tempIOSettingsList.strOutput.size()+COL_OUTPUT) ){	// @date 2012/08/08 <Add> OKADA
		;	// OK
	}else{
		// error
		return "";	// col�̒l���傫������
	}

    // �l�擾
	if( col == COL_TYPE ){	// Type		// @date 2012/08/07 <Add> OKADA
		s = tempIOSettingsList.strTypeName;	// @date 2012/08/07 <Add> OKADA
	}										// @date 2012/08/07 <Add> OKADA

    if( col == COL_INPUT ) {    // Input
		s = tempIOSettingsList.strInput;
    }
    else if( COL_OUTPUT <= col ) {    // Output
		s = tempIOSettingsList.strOutput[col-COL_OUTPUT];	// @date 2012/08/08 <Add> OKADA
    }

    return s;
}

// �����擾
int BatchProcessingSubmitJobGrid::getInt( const long long data, const int col ) const {
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

// �^�C�g�����N���b�N���Ă������N���Ȃ��悤�ɂ���
void BatchProcessingSubmitJobGrid::onColClick( wxListEvent& evt ) {
}

// �_�u���N���b�N
void BatchProcessingSubmitJobGrid::onDoubleClick( wxListEvent& evt ) {
}

void BatchProcessingSubmitJobGrid::setVectStrPaths( stIOInf stIoInf )
{
	m_vectstrTypes.clear();	//�@@date 2012/08/07 <Add> OKADA
	m_vectstrPaths.clear();
	m_vectstrOutputFilenameInfo.clear();

	for( unsigned int i=0; i<stIoInf.vectstrPaths.size(); i++ ){	// @date 2013/06/10 <Mod> OKADA
		m_vectstrTypes.push_back( stIoInf.vectstrTypeName[i] );	//�@@date 2012/08/08 <Add> OKADA
		m_vectstrPaths.push_back( stIoInf.vectstrPaths[i] );
		m_vectstrOutputFilenameInfo.push_back( stIoInf.vectstrOutputFilenameInfo[i] );
	}
}


