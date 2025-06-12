/**
 * @file BatchProcessingMethodConfigurationDialog.cpp
 * @brief implements of BatchProcessingMethodConfigurationDialog class
 *
 * @author okada_h
 * @date 2012/02/15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BatchProcessingMethodConfigurationDialog.h"
#include "BatchGrid.h"
#include "BatchActionSelectPanel.h"
#include "BatchButtonDef.h"
#include "BatchServicePluginManager.h"
#include "BatchJobListDlg.h"
#include "BatchProcessingSubmitJobDlg.h"
#include "NewBatchMethodDlg.h"
#include "BatchEditNameDlg.h"	// @date 2012/04/19 <Add> OKADA

#include <wx/event.h>
#include <wx/notebook.h>

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




#define BORDER_SIZE	3
#define GRID_GAP	3

#define BATCH_SERVICE_FUNC_TYPE	"GET_BATCH_SERVICE"

#define LABELING_NAME			"Peak Detector"
#define SECTION					"Auto Peak Picking"
#define PEAK_ALGORITHM_KEY		"SPEC_PEAK_ALGORITHM"


BEGIN_EVENT_TABLE( BatchProcessingMethodConfigurationDialog, kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	EVT_BUTTON( ID_MATRIX_ADD_BUTTON,		BatchProcessingMethodConfigurationDialog::onAdd )
	EVT_BUTTON( ID_MATRIX_EDIT_BUTTON,		BatchProcessingMethodConfigurationDialog::onEdit )
	EVT_BUTTON( ID_MATRIX_SAVE_BUTTON,		BatchProcessingMethodConfigurationDialog::onSave )
	EVT_BUTTON( ID_MATRIX_SAVEAS_BUTTON,	BatchProcessingMethodConfigurationDialog::onSaveAs )
	EVT_BUTTON( ID_MATRIX_DELETE_BUTTON,	BatchProcessingMethodConfigurationDialog::onDelete )
	EVT_BUTTON( ID_BAT_JOB_LIST_BUTTON,		BatchProcessingMethodConfigurationDialog::onBatJobList )
	EVT_BUTTON( wxOK,						BatchProcessingMethodConfigurationDialog::onOkButton )
	
	EVT_BUTTON( wxCANCEL,					BatchProcessingMethodConfigurationDialog::onCancelButton )	// SPEC 85856

	EVT_COLLAPSIBLEPANE_CHANGED( ID_BAT_CONF_CPANE, BatchProcessingMethodConfigurationDialog::onToggleBatConfPane)
	EVT_NOTEBOOK_PAGE_CHANGED( wxID_ANY, onChangePage )
//	EVT_TEXT(ID_COMBOBOX_METHOD, BatchProcessingMethodConfigurationDialog::changeMethodCombobox )	// @date 2013/07/02 <Del> OKADA �s�v
	EVT_COMBOBOX(ID_COMBOBOX_METHOD, BatchProcessingMethodConfigurationDialog::changeMethodCombobox )

	EVT_UPDATE_UI( ID_MATRIX_ADD_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_EDIT_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_SAVE_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_SAVEAS_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_DELETE_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( wxID_OK,  BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_COMBOBOX_METHOD,	BatchProcessingMethodConfigurationDialog::onUpdateUI )	// SPEC 85673
END_EVENT_TABLE()


// constructor
BatchProcessingMethodConfigurationDialog::BatchProcessingMethodConfigurationDialog( wxWindow* parent )
: kome::window::ParametersAndHelpDialog( parent,	"Batch Processing - Method Configuration"), m_parameters( NULL, false )	// @Date:2014/01/24	<extend help>	A.Ozaki
{
	m_pBatConfAddButton		= NULL;
	m_pBatConfEditButton	= NULL;
	m_pBatConfSaveButton	= NULL;
	m_pBatConfSaveAsButton	= NULL;
	m_pBatConfDeleteButton	= NULL;

	m_pBatchList = new BatchGrid( this );

	m_pApplyAllButton = NULL;
	m_pCloseButton = NULL;
	m_pBatchProcessingSubmitJobDlg = NULL;
	m_vect_pBatchActionSelectPanel.clear();

	m_pComboMethod = NULL;
	m_stringsMethod.clear();
	m_bChangeAction = false;	// SPEC 85856

	getInitData();
}

// destructor
BatchProcessingMethodConfigurationDialog::~BatchProcessingMethodConfigurationDialog() {
}

// create top sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createMainSizer() {

	// field sizer
	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 4, 1, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableRow( 1 );

	// top sizer
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 0 );


	// create peak sizer
	topSizer->Add(
		createBatchMethodSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);

	topSizer->Add(
		createBatchButtonSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);
	fieldSizer->Add( topSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	if( m_pBatchList != NULL ) {
		m_pBatchList->create();
	}
	fieldSizer->Add(m_pBatchList, 1, wxEXPAND);
	
	// �B���y�C��
	wxSizer* sizer = NULL;
	m_pBatConfPane = new kome::window::CollapsiblePane(this, wxT("Batch Method Stage Settings"), ID_BAT_CONF_CPANE);
	fieldSizer->Add(m_pBatConfPane , 0, wxGROW|wxALL, 5);

	// �^�u
    wxWindow *win = m_pBatConfPane->GetPane();
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);

	// note book
	m_pNoteBook = new wxNotebook(
		win,
		wxID_ANY,
		wxDefaultPosition,
		wxSize(638, 312),
		wxNB_TOP
	);
	
	paneSz->Add( m_pNoteBook, 1, wxALIGN_CENTER | wxALIGN_CENTRE_VERTICAL | wxALL | wxGROW | wxSHAPED, 5 ); // SPEC No.91863 @date 2013.04.01 <Mod> M.Izumi

	BatchActionSelectPanel * winBatchActionSelectPanel0 = new BatchActionSelectPanel( m_pNoteBook, this, 0 );
	BatchActionSelectPanel * winBatchActionSelectPanel1 = new BatchActionSelectPanel( m_pNoteBook, this, 1 );
	BatchActionSelectPanel * winBatchActionSelectPanel2 = new BatchActionSelectPanel( m_pNoteBook, this, 2 );
	BatchActionSelectPanel * winBatchActionSelectPanel3 = new BatchActionSelectPanel( m_pNoteBook, this, 3 );
	BatchActionSelectPanel * winBatchActionSelectPanel4 = new BatchActionSelectPanel( m_pNoteBook, this, 4 );
	BatchActionSelectPanel * winBatchActionSelectPanel5 = new BatchActionSelectPanel( m_pNoteBook, this, 5 );

	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel0 );	// �ۑ����Ă���
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel1 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel2 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel3 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel4 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel5 );

	m_pNoteBook->AddPage( winBatchActionSelectPanel0, wxT( g_aszStageString[0] ));	
	m_panels.push_back( winBatchActionSelectPanel0 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel1, wxT( g_aszStageString[1] ));	
	m_panels.push_back( winBatchActionSelectPanel1 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel2, wxT( g_aszStageString[2] ));	
	m_panels.push_back( winBatchActionSelectPanel2 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel3, wxT( g_aszStageString[3] ));	
	m_panels.push_back( winBatchActionSelectPanel3 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel4, wxT( g_aszStageString[4] ));	
	m_panels.push_back( winBatchActionSelectPanel4 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel5, wxT( g_aszStageString[5] ));	
	m_panels.push_back( winBatchActionSelectPanel5 );

	// Init Dialog
	for( unsigned int i = 0; i < m_panels.size(); i++ ) {
// >>>>>>	@Date:2014/02/04	<Modify>	A.Ozaki
// WinXP�ł́AIniDialog(OnInitDialog)�̏������ɗ�O���������āAMass++���̂������Ă��܂�
// ���̉����@�Ƃ��āAOnInitDialog�𓮂������ɁAWidget�𐶐�������悤�ɕύX���܂���
//
//		m_panels[ i ]->InitDialog();
		m_panels[ i ]->createWidgets();
//
// <<<<<<	@Date:2014/02/04	<Modify>	A.Ozaki
	}

    win->SetSizer(paneSz);
    paneSz->SetSizeHints(win);

	// XML�t�@�C������ǂݍ��񂾃f�[�^���e�^�u�ɏ�������
	{
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();

		std::vector<Action>	vectActions = mgr.getVectXmlActions();
		int nCnt = vectActions.size();
		for( int i=0; i<nCnt; i++ ){
			Action actTemp = vectActions[i];	
			m_pBatchList->addAction( actTemp );

			int nnn=m_vect_pBatchActionSelectPanel.size();

			BatchActionGrid* bag;
			switch( actTemp.nStage ){
			case SN_INITIALIZE:		// �@Initialize
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[0]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER:			// �A
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[1]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_MANIPULATE:		// �B
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[2]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_ANALYZE:		// �C
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[3]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_IDENTIFY:		// �D
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[4]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER_RESULTS:	// �EFilter Results
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[5]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			default:
				break;
			}
		}

		// ��ʂɔ��f
		m_pBatchList->updateList();
		m_pBatchList->RefreshBatchGrid();

	}

	return fieldSizer;
}


// create button sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createButtonSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 4, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableRow( 2 );
	sizer->AddGrowableRow( 3 );

	// default button
	wxButton* button = new wxButton( this, ID_BAT_JOB_LIST_BUTTON, wxT( "Batch Job List..." ) );
	sizer->Add( button, 0, wxALIGN_BOTTOM | wxALIGN_LEFT | wxLEFT , BORDER_SIZE );

	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 2, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}


// create top sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createBatchMethodSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );

	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableRow( 0 );

	// "Batch Method"
	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Batch Method" ), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( descrName, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// XML�t�@�C���̈ꗗ���擾
	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	std::vector<std::string> vect_strFiles = mgr.getSettingXmlFiles();

	wxArrayString stringsMethod;
	for( unsigned int i=0; i<vect_strFiles.size(); i++ ){
		// �g���q��������4�������폜
		std::string strTemp = vect_strFiles[i];
		int nLen = vect_strFiles[i].length();
		if( 4 <= nLen ){
			strTemp = vect_strFiles[i].substr( 0, nLen-4 );
		}

		stringsMethod.Add(wxT( strTemp.c_str() ));
		m_strMethodName = strTemp;
	}

	// �Ō�ɗ��p����xml�t�@�C����ini�t�@�C������ǂݏo��
	std::string strXmlFileNameLast = "";
	if( vect_strFiles.size() == 0 ){
		strXmlFileNameLast = "";
	}else{
		strXmlFileNameLast = vect_strFiles[0];	// �����l
	
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getParameters();

		std::string strXmlFileNameWithoutExt = ini->getString( SECTION_XML, DEFAULT_XML_KEY, DEFAULT_VALUEY );
		std::string strXmlFileName = strXmlFileNameWithoutExt;

		strXmlFileName += ".xml";	// �g���q��ǉ�
		if( strcmp( DEFAULT_VALUEY, strXmlFileName.c_str() ) == 0 ){
			// ��v�A�ǂݎ�莸�s
		}else{
			// �ǂݎ�萬���B�����݂���t�@�C�����`�F�b�N
			for( unsigned int i=0; i<vect_strFiles.size(); i++ ){
				if( compareignorecase( vect_strFiles[i].c_str(), strXmlFileName.c_str() ) == 0 ){
					// ��v
					strXmlFileNameLast = strXmlFileNameWithoutExt;
					break;
				}
				if( (i+1) == vect_strFiles.size() ){
					// �Ō�Ȃ̂ŕs��v
					strXmlFileNameLast = "";
				}
			}
		}
	}

	wxSize wxsSize = wxDefaultSize;	// SPEC 85682
	wxsSize.SetWidth( 320 );		// SPEC 85682
	m_pComboMethod = new wxComboBox(this, ID_COMBOBOX_METHOD, strXmlFileNameLast, wxDefaultPosition, wxsSize, stringsMethod, wxCB_SIMPLE | wxCB_READONLY );	 //�@�R���{�{�b�N�X�ɒǉ�
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_pComboMethod, "Saved batch methods appears in this drop down and you can select it." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki
	m_stringsMethod = stringsMethod;

	// ���݂̃��\�b�h�I��l���X�V
	m_strMethodName = m_pComboMethod->GetValue().c_str();	// SPEC 85953	�I���ς݃��\�b�h���̒l���X�V	//�@@date 2012/05/16 <Add> OKADA

	sizer->Add( m_pComboMethod, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// Ini �t�@�C���`�F�b�N
	if( strXmlFileNameLast == "" ){
		;	// Ini�t�@�C���ݒ薳��
	}else{

		// XML
		std::string strXmlFileNameWithExt;
		strXmlFileNameWithExt = strXmlFileNameLast;
		strXmlFileNameWithExt += ".xml";

		if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
			;
		}else{
			strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
		}

		mgr.readXML( strXmlFileNameWithExt.c_str() );	// XML�t�@�C������ǂݍ���

	}
	// ��ʂɔ��f
	m_pBatchList->updateList();
	m_pBatchList->RefreshBatchGrid();

	return sizer;
}


// create top sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createBatchButtonSizer() {
	// create sizer
	wxSizer* sizer = NULL;
	sizer = new wxBoxSizer(wxHORIZONTAL);

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pBatConfAddButton = new wxBitmapButton( this, ID_MATRIX_ADD_BUTTON, *iconMgr.getIcon("new") );
	m_pBatConfAddButton->SetToolTip("Add new batch method");
	buttonSizer->Add(m_pBatConfAddButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfEditButton = new wxBitmapButton( this, ID_MATRIX_EDIT_BUTTON, *iconMgr.getIcon("edit") );
	m_pBatConfEditButton->SetToolTip("Edit batch method name");
	buttonSizer->Add(m_pBatConfEditButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfSaveButton = new wxBitmapButton( this, ID_MATRIX_SAVE_BUTTON, *iconMgr.getIcon("item_save") );
	m_pBatConfSaveButton->SetToolTip("Save batch method");
	buttonSizer->Add(m_pBatConfSaveButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfSaveAsButton = new wxBitmapButton( this, ID_MATRIX_SAVEAS_BUTTON, *iconMgr.getIcon("item_save_new") );
	m_pBatConfSaveAsButton->SetToolTip("Save as new batch method");
	buttonSizer->Add(m_pBatConfSaveAsButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfDeleteButton = new wxBitmapButton( this, ID_MATRIX_DELETE_BUTTON, *iconMgr.getIcon("item_del") );
	m_pBatConfDeleteButton->SetToolTip("Delete batch method");
	buttonSizer->Add(m_pBatConfDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	return sizer;
}


wxSizer* BatchProcessingMethodConfigurationDialog::createBatchPaneSizer() {

	wxSizer* sizer = NULL;
	
	m_pBatConfPane = new kome::window::CollapsiblePane(this, wxT("Peak Matrix Settings"), ID_BAT_CONF_CPANE);

	sizer->Add(m_pBatConfPane , 0, wxGROW|wxALL, 5);

	return sizer;
}


// on toggle the batch configuration collapsible pane
void BatchProcessingMethodConfigurationDialog::onToggleBatConfPane( wxCollapsiblePaneEvent& evt )
{
}

// on add
void BatchProcessingMethodConfigurationDialog::onAdd( wxCommandEvent& evt ) {
	// ���݂̃��\�b�h�I��l���X�V
	m_strMethodName = m_pComboMethod->GetValue().c_str();	// @date 2012/04/19 <Add> OKADA

	// open dialog
	NewBatchMethodDlg dlg(
		kome::window::WindowTool::getMainWindow(), this
	);

	int nRet = dlg.ShowModal();
	if( nRet == wxID_OK )
	{
		// ����
		int iii = 0;
		NewMethodSetting stNewMethodSetting = dlg.getNewMethodSetting();

		// ���W�I�{�^�����uMethod�v�Ȃ�A
		// �E�R���{�{�b�N�X�őI�����ꂽMethod��ǂݍ���
		// �E�G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ�V�������\�b�h���ŏ㏑��
		// ���W�I�{�^�����uTemplate�v�Ȃ�A
		// ���W�I�{�^�����uNone�v�Ȃ�A
		// �E�S�폜
		// �E�G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ�V�������\�b�h���ŏ㏑��

		if( stNewMethodSetting.bMethod ){
			// ���W�I�{�^�����uMethod�v�Ȃ�A
			// �E�R���{�{�b�N�X�őI�����ꂽMethod��ǂݍ���
			std::string strMethod = stNewMethodSetting.wxstrMethod.c_str();

			// �����œ������O���Ȃ����`�F�b�N
			if( checkSameNameConbobox(stNewMethodSetting.strNewMethodName.c_str()) != 0 ){
				// �������O��������
				return;
			}

			changeMethod( strMethod.c_str() );
			// �E�G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ�V�������\�b�h���ŏ㏑��
			m_pComboMethod->Append( stNewMethodSetting.strNewMethodName );
			m_pComboMethod->SetValue( stNewMethodSetting.strNewMethodName );

			m_stringsMethod.push_back( stNewMethodSetting.strNewMethodName );	// @date 2012/04/19 <Add> OKADA		// SPEC 85666

			copyDefaultOutputFormat( strMethod.c_str(),  stNewMethodSetting.strNewMethodName.c_str() );	//@date 2013/05/23 <Add> OKADA
		}
		else if( stNewMethodSetting.bTemplate ){
			// ���W�I�{�^�����uTemplate�v�Ȃ�A


			// @date 2013/08/19 <Add> OKADA ------->

			// managers
			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

			std::string strTemplate = stNewMethodSetting.wxstrTemplate;

			const char* TEMPLATE_TYPE = "batch_set";
			std::string strTemplateDefault;

			unsigned int uNum = plgMgr.getNumberOfFunctionItems( TEMPLATE_TYPE );
			kome::plugin::PluginFunctionItem* item = NULL;
			for( unsigned int i=0; i<uNum; i++ ){
				 item = plgMgr.getFunctionItem( TEMPLATE_TYPE, i );
				if( item != NULL ){
					const char* szLongName = item->getLongName();
					if( strTemplate.compare( szLongName ) ){
						// �s��v
					}else{
						// ��v

						kome::objects::ActiveObjectsManager& actMgr = kome::objects::ActiveObjectsManager::getInstance();
						kome::objects::Sample* acSample = actMgr.getActiveSample();

						kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

						if( page != NULL ){
							
						}

						kome::objects::SettingParameterValues settings;

						// create parameters
						kome::objects::Parameters params;
						kome::plugin::PluginCallTool::setSample( params, *acSample );

						kome::objects::Variant ret;

						ret = item->getCall()->invoke( &params );

						kome::plugin::BatchInfo* batchInfo = (kome::plugin::BatchInfo*)ret.prim.pt;

						if( batchInfo == NULL ){
							continue;
						}

						// �e�^�u���N���A
						{
							int nCnt = m_vect_pBatchActionSelectPanel.size();
							for( int i=0; i<nCnt; i++ ){
								BatchActionGrid* bag;
								bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[i]))->getSelectedActions();
								bag->init();	// @date 2013/07/02 <Add> OKADA	// SPEC 93492
							}
						}

						if( batchInfo != NULL ){
							int nNum = batchInfo->getNumberOfBatchFunctions();
							for( int j=0; j<nNum; j++){

								std::string strFunction;
								batchInfo->getBatchProcessingFunction( j, strFunction );
								// strFunction = "chrom_peak_detect" �Ƃ����������񂪕Ԃ�

								kome::plugin::PluginFunctionItem* item = batchInfo->getBatchFunction( j );
								if( item != NULL ){
									
									// step �擾
									const char* stepStr = item->getCall()->getProperties().getStringValue( "step", "" );
									// stepStr = 0x02ba912c "identify" �Ƃ����������񂪎擾�����

									unsigned short uPrecedence = item->getCall()->getOrder();

									Action actTemp;

									// �X�e�[�W�ԍ��B�@Initialize�`�EFilterResults
									actTemp.nStage = SN_SENTINEL;
									if( compareignorecase( stepStr, "initialize" ) == 0 ) {
										actTemp.nStage = SN_INITIALIZE;
									}
									else if( compareignorecase( stepStr, "input" ) == 0 ) {
										// Input�́Abatch�ł͈���Ȃ�
									//	actTemp.nStage = STEP_INPUT;
									}
									else if( compareignorecase( stepStr, "data_set" ) == 0 ) {
										// data_set��Filter�ɂȂ�܂���
										actTemp.nStage = SN_FILTER;
									}
									else if( compareignorecase( stepStr, "operation" ) == 0 ) {
										// operation�́Amanipulate�ɂȂ�܂���
										actTemp.nStage = SN_MANIPULATE;
									}
									else if( compareignorecase( stepStr, "analyze" ) == 0 ) {
										actTemp.nStage = SN_ANALYZE;
									}
									else if( compareignorecase( stepStr, "identify" ) == 0 ) {
										actTemp.nStage = SN_IDENTIFY;
									}
									else if( compareignorecase( stepStr, "filter" ) == 0 ) {
										actTemp.nStage = SN_FILTER;
									}
									else if( compareignorecase( stepStr, "output" ) == 0 ) {
										// Output�́Abatch�ł͈���Ȃ�
									//	actTemp.nStage = STEP_OUTPUT;
									}
									else if( compareignorecase( stepStr, "finalize" ) == 0 ) {
										// finalize�́Abatch�ł͈���Ȃ�
									//	actTemp.nStage = STEP_FINALIZE;
									}
									// @date 2013/08/02 <Add> OKADA ------->
									else if( compareignorecase( stepStr, "manipulate" ) == 0 ) {
										actTemp.nStage = SN_MANIPULATE;
									}
									else if( compareignorecase( stepStr, "results" ) == 0 ) {
										actTemp.nStage = SN_FILTER_RESULTS;
									}
									if( actTemp.nStage == SN_SENTINEL ){
										// error
										int nnnn = 0;
										continue;	// @date 2013/08/21 <Add> OKADA
									}

									// nPrecedence
									actTemp.nPrecedence = uPrecedence;

									// long name
									actTemp.strName = item->getLongName();

									// short name
									actTemp.strNameShort = strFunction;

									// Plugin Manager����getFunctionItem()�œǂݎ��ۂ̔ԍ�
									// plgMgr.getFunctionItem( "batch", i );���́ui�v�ɑ���
									actTemp.nBatchIndex = -1;
									kome::plugin::PluginFunctionItem* itemTemp = NULL;
									unsigned int uNumTemp = plgMgr.getNumberOfFunctionItems( "batch" );
									for( unsigned int l=0; l<uNumTemp; l++ ){
										itemTemp = plgMgr.getFunctionItem( "batch" , l );
										const char* szLongName = itemTemp->getLongName();
										if( compareignorecase( szLongName, actTemp.strName.c_str() ) == 0 ){
											// ��v
											actTemp.nBatchIndex = l;
											break;
										}
									}
									if( actTemp.nBatchIndex < 0 ){
										// error
										int nnnn = 0;
										continue;	// @date 2013/08/21 <Add> OKADA
									}

									// 1=settings ok
									actTemp.flgSettings = 1;

									// // kome::plugin::SettingsPage * page->getParameterString( settings );	�̕Ԃ�l��ۑ�
									std::string strPatameter;
									batchInfo->getParameterOfBatchProcessingFunction( j, strPatameter );

									actTemp.strSettings = strPatameter;
									
									// �p�����[�^�ݒ�(xml�t�@�C���p) // @date 2013/03/28 <Add> OKADA
									std::vector<std::string> vecParam;
									param2vect( strPatameter.c_str(), vecParam );
									actTemp.vec_strSettings = vecParam;


									// ����ŁAactTemp�ɒl���������

									// �^�u�֒ǉ�����
									BatchActionGrid* bag;
									int nTabNo=0;
									switch( actTemp.nStage ){
									case SN_INITIALIZE:		nTabNo=0;	break;	// �@Initialize
									case SN_FILTER:			nTabNo=1;	break;	// �A
									case SN_MANIPULATE:		nTabNo=2;	break;	// �B
									case SN_ANALYZE:		nTabNo=3;	break;	// �C
									case SN_IDENTIFY:		nTabNo=4;	break;	// �D
									case SN_FILTER_RESULTS:	nTabNo=5;	break;	// �EFilter Results
									default:
										// error
										// �ʏ�Ȃ�
										continue;	// @date 2013/08/21 <Add> OKADA
										break;
									}

									bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[nTabNo]))->getSelectedActions();
									bag->addAction( actTemp );
									bag->updateList();

								}
							}
						}
						// @date 2013/08/21 <Add> OKADA batchInfo�폜 ------->
						if( batchInfo != NULL ){
							delete batchInfo;
							batchInfo = NULL;
						}
						// @date 2013/08/21 <Add> OKADA <-------

					}
				}
			}
			m_strMethodName = stNewMethodSetting.strNewMethodName;

			// ��ʂɔ��f
			m_pBatchList->updateList();
			m_pBatchList->RefreshBatchGrid();

			// ��ʂɔ��f
			updateBatchList();
			m_pNoteBook->Refresh();

			// �E�G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ�V�������\�b�h���ŏ㏑��
			m_pComboMethod->Append( stNewMethodSetting.strNewMethodName );
			m_pComboMethod->SetValue( stNewMethodSetting.strNewMethodName );

			m_stringsMethod.push_back( stNewMethodSetting.strNewMethodName );	// @date 2012/04/19 <Add> OKADA		// SPEC 85666

			// @date 2013/08/19 <Add> OKADA <-------
		}
		else if( stNewMethodSetting.bBlank ){

			// �����œ������O���Ȃ����`�F�b�N
			if( checkSameNameConbobox(stNewMethodSetting.strNewMethodName.c_str()) != 0 ){
				// �������O��������
				return;
			}

			// ���W�I�{�^�����uNone�v�Ȃ�A
			// �E�S�폜
			changeMethod( "" );
			// �E�G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ�V�������\�b�h���ŏ㏑��
			m_pComboMethod->Append( stNewMethodSetting.strNewMethodName );
			m_pComboMethod->SetValue( stNewMethodSetting.strNewMethodName );

			m_stringsMethod.push_back( stNewMethodSetting.strNewMethodName );	// @date 2012/04/19 <Add> OKADA		// SPEC 85666
		}
		m_strMethodName = stNewMethodSetting.strNewMethodName;

		dlg.Destroy();
	}

	return;
}


// on Edit
void BatchProcessingMethodConfigurationDialog::onEdit( wxCommandEvent& evt ) {

	// ���݂̃��\�b�h�I��l���X�V
	m_strMethodName = m_pComboMethod->GetValue().c_str();	// @date 2012/04/19 <Add> OKADA
		
	BatchEditNameDlg dlgBatch( kome::window::WindowTool::getMainWindow(), this );
	if( dlgBatch.ShowModal() == wxID_OK ){
		std::string strName;
		strName = dlgBatch.getNewMethodName();

		if( strName.length() <= 0 ){
			return;	// cancel
		}

		// �����œ������O���Ȃ����`�F�b�N
		if( checkSameNameConbobox(strName.c_str()) != 0 ){
			// �������O��������
			return;
		}

		if( strName == m_strMethodName ){
			return;	// �ύX������[OK]����
		}
	
		// SPEC 85934
		{
			std::string strXmlFileNameWithExt;
			strXmlFileNameWithExt = m_strMethodName.c_str();
			strXmlFileNameWithExt += ".xml";

			// New
			std::string strXmlFileNameWithExtNew;
			strXmlFileNameWithExtNew = strName.c_str();
			strXmlFileNameWithExtNew += ".xml";

			if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
				;
			}else{
				// m_strMethodName��XML�t�@�C���̃t���p�X�𓾂�
				BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
				strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
				strXmlFileNameWithExtNew = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExtNew.c_str() );
			}
			if( copyfile( strXmlFileNameWithExt.c_str(), strXmlFileNameWithExtNew.c_str() ) ){
				remove( strXmlFileNameWithExt.c_str() );	// �폜
			}

			copyDefaultOutputFormat( m_strMethodName.c_str(), strName.c_str(), true );	// @date 2013/05/23�@<Add>�@OKADA
		}

		// SPEC 85951
		{
			m_stringsMethod.Remove( m_strMethodName.c_str() );	// m_strMethodName���폜���폜
			m_stringsMethod.push_back( strName );		// 
		}


		m_strMethodName = strName;

		if( m_pComboMethod != NULL ){
			int n = m_pComboMethod->GetCurrentSelection();
			m_pComboMethod->Delete( n );
			m_pComboMethod->Insert( strName, n );
			m_pComboMethod->SetValue( strName );
			int jjj = 0;
		}
	}

	return;
}

// on save
void BatchProcessingMethodConfigurationDialog::onSave( wxCommandEvent& evt ){
	
	exportBatchXML( m_strMethodName.c_str() );

	m_bChangeAction = false;	// �ۑ�����̂Ń��Z�b�g	// SPEC 85856

	return;
}

// on save as
void BatchProcessingMethodConfigurationDialog::onSaveAs( wxCommandEvent& evt ){

	wxString strName = ::wxGetTextFromUser( wxT("Name"), wxT("Save As New Batch Method"),  getCurrentMethod().c_str(), this );  // SPEC 85718	@date 2012/05/15 <Mod> OKADA �f�t�H���g�������݂̃��\�b�h����

	if( strName != "" ){
		// OK
		// @date 2012/05/15 <Add> OKADA ------->
		// �������\�b�h�����������m�F
		if( checkSameNameConbobox( strName.c_str() ) != 0 ){	
			// �������\�b�h����������
			kome::window::WindowTool::showInfo( "A method with that name already exists." );
			return;
		}
		// @date 2012/05/15 <Add> OKADA <-------

		//�@SPEC 85953
		m_stringsMethod.push_back( strName );		// �V�������O��ǉ�

		// [OK]�Ȃ�A���\�b�h��(m_strMethodName)�X�V
		m_strMethodName = strName;

		// [OK]�Ȃ�AexportBatchXML()�ŕۑ�
		exportBatchXML( m_strMethodName.c_str() );

		// [OK]�Ȃ�A�R���{�{�b�N�X�֕ۑ�
		m_pComboMethod->Append( strName );
		m_pComboMethod->SetValue( strName );

		m_bChangeAction = false;	// �ۑ�����̂Ń��Z�b�g	// SPEC 85856	//�@@date 2012/05/15 <Add> OKADA
	}

	return;
}

// on delete
void BatchProcessingMethodConfigurationDialog::onDelete( wxCommandEvent& evt ){

	bool bRet = kome::window::WindowTool::showYesNo( wxT("Are you sure you want to delete the selected batch method?"), "Delete", "Don't delete" );

	if( bRet ){

		wxString wxstrTemp = m_pComboMethod->GetValue();

		int nFindedNo = m_pComboMethod->FindString( wxstrTemp );
		if( wxNOT_FOUND != nFindedNo ){

			// wxstrTemp��XML�t�@�C�����폜����
			{
				std::string strXmlFileNameWithExt;
				strXmlFileNameWithExt = wxstrTemp.c_str();
				strXmlFileNameWithExt += ".xml";

				if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
					;
				}else{
					// wxstrTemp��XML�t�@�C���̃t���p�X�𓾂�
					BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
					strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
				}

				remove( strXmlFileNameWithExt.c_str() );	// �폜

				deleteDefaultOutputFormat( wxstrTemp.c_str() );	// @date 2013/05/23 <Add> OKADA
			}

			m_pComboMethod->Delete( nFindedNo );


			m_stringsMethod.RemoveAt( nFindedNo );	// @date 2012/04/19 <Add> OKADA	// SPEC 85666
			

			// ���̃f�[�^��S������
			int nActSizeTotal = m_pBatchList->getActionSize();
			for( int i=nActSizeTotal-1; 0<=i; i-- ){
				m_pBatchList->deleteAction( i );
			}
			m_pBatchList->clearData( true );

			// �e�^�u���N���A
			{
				int nCnt = m_vect_pBatchActionSelectPanel.size();
				for( int i=0; i<nCnt; i++ ){
					BatchActionGrid* bag;
					bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[i]))->getSelectedActions();
				}
			}

			// ��ʂɔ��f
			updateBatchList();
			m_pNoteBook->Refresh();
			m_strMethodName = "";

			// SPEC 85674
			m_pComboMethod->Select(0);	// �폜���A�擪��I��
			changeMethod( m_pComboMethod->GetValue().c_str() );
		}
	}

	return;
}


// onBatJobList
void BatchProcessingMethodConfigurationDialog::onBatJobList( wxCommandEvent& evt ) {

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	mgr.dispBatchJobWindow();

	return;
}


// onOkButton
void BatchProcessingMethodConfigurationDialog::onOkButton( wxCommandEvent& evt ) {

	// ������XML�t�@�C���֐ݒ�f�[�^��ۑ�

//	if( m_strMethodName.size() != 0 ){
//		exportBatchXML( m_strMethodName.c_str() );
//	}
	if( m_strMethodName.size() != 0 ){
		int check = exportBatchXML( m_strMethodName.c_str() );  // �o�͂ɐ�������ƁA0���Ԃ��Ă���
		if (check != 0){
			return;    // �o�͂Ɏ��s���Ă���΁A�����ŏI��
		}
	}
 
	// Batch Job List �N��

	//�@BatchProcessingSubmitJobDlg::onSubmitButton()�֐��Ɠ��e����

	if( m_pBatchProcessingSubmitJobDlg != NULL ){
		delete m_pBatchProcessingSubmitJobDlg;
		m_pBatchProcessingSubmitJobDlg = NULL;
	}

	m_pBatchProcessingSubmitJobDlg = new BatchProcessingSubmitJobDlg( kome::window::WindowTool::getMainWindow() );

	int nRet = m_pBatchProcessingSubmitJobDlg->ShowModal();

	if( m_pBatchProcessingSubmitJobDlg != NULL ){
		delete m_pBatchProcessingSubmitJobDlg;
		m_pBatchProcessingSubmitJobDlg = NULL;
	}
	if( nRet == wxID_CANCEL ) {
		// ����
		int iii = 0;
	}
	else{
		EndDialog( 0 );
	}
	
	return;
}

// on cancel button
void BatchProcessingMethodConfigurationDialog::onCancelButton( wxCommandEvent& evt ) {

	bool bRet = true;	// [OK]=true [Cancel]=false
	if( m_bChangeAction == true ){
		bRet = kome::window::WindowTool::showOkCancelWarning( "There are unsaved changes. If the dialog is canceled, all changes will be lost." );
	}

	if( bRet == true ){
		EndDialog( 0 );
		m_bChangeAction = false;
	}

	return;
}

// get init data
void BatchProcessingMethodConfigurationDialog::getInitData( void ){

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	mgr.initBatchServicePluginManager();

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	unsigned int uiNum = plgMgr.getNumberOfFunctionItems( "batch" );
	for( unsigned int i=0; i<uiNum; i++ ){
		kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", i );
		unsigned short wOrder = pItem->getCall()->getOrder();

		std::string step = pItem->getCall()->getProperties().getStringValue( "step", "HOGE" );

		std::string strLongName = pItem->getLongName();
		std::string strShortName = pItem->getShortName();


		stage_no nStage = mgr.getStageNoFromStepString( step.c_str() );

		if( nStage == SN_SENTINEL ){
			continue;
		}

		Action actTemp;
		actTemp.nStage = nStage;
		actTemp.nPrecedence = wOrder;
		actTemp.strName = strLongName;
		actTemp.strNameShort = strShortName;
		actTemp.nBatchIndex = i;
		actTemp.flgSettings = 0;

		mgr.setAvailableActions( actTemp );
	}

	return;
}

// on change page
void BatchProcessingMethodConfigurationDialog::onChangePage( wxNotebookEvent& evt ){

	int nSelection = evt.GetSelection();

	return;

};

// update batch list
int BatchProcessingMethodConfigurationDialog::updateBatchList( void ){

	m_bChangeAction = true;	// ����������ꂽ�̂ŃZ�b�g	// SPEC 85856

	BatchActionSelectPanel* pTemp;

	// ���̃f�[�^��S������
	int nActSizeTotal = m_pBatchList->getActionSize();
	for( int i=nActSizeTotal-1; 0<=i; i-- ){
		m_pBatchList->deleteAction( i );
	}
	m_pBatchList->clearData( true );
	
	int nSize = m_vect_pBatchActionSelectPanel.size();	// �^�u�̖���
	for( int i=0; i<nSize; i++ ){
		pTemp = (BatchActionSelectPanel*)m_vect_pBatchActionSelectPanel[i];
		int nActSize = pTemp->getSelectedActions()->getActionSize();
		for( int j=0; j<nActSize; j++ ){
			Action actTemp = pTemp->getSelectedActions()->getAction( j );	// BatchActionGrid����m_vectActions[index]��Ԃ�
			m_pBatchList->addAction( actTemp );	// BatchGrid����m_vectActions�ɒl��ۑ�
		}
	}

	m_pBatchList->updateList();
	m_pBatchList->RefreshBatchGrid();

	return 0;
}

// export batch XML
int BatchProcessingMethodConfigurationDialog::exportBatchXML( const char* szMethodName ){

	// ������XML�t�@�C���֐ݒ�f�[�^��ۑ�
	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	updateBatchList();                              
	int nSize = m_pBatchList->getActionSize();  

	// XML�ɏo�͂��ׂ��A�N�V����������
	std::vector<Action> vectActions;
	for( int i=0; i<nSize; i++ ){
		Action actTemp;
		actTemp = m_pBatchList->getAction( i );
		if (actTemp.strNameShort == "norm_standard"){       // "norm_standard"�ɋ󗓂������Ă��܂��s��΍�(�Č������Ȃ��̂őΏ��Ö@�I�ɍs��)
			std::string settings = actTemp.strSettings.c_str();
			if (settings == "[file,]"){   // �󗓂������Ă���ꍇ
				kome::window::WindowTool::showInfo( "normalization standard sample setting is empty. " );
				return 1;
			}
		}

		vectActions.push_back( actTemp );

	}

	mgr.exportBatchXML( szMethodName, vectActions );
	mgr.saveLastRecordXmlFilename( szMethodName );	// Ini�t�@�C���֕ۑ�	

	return 0;
}

// change method
void BatchProcessingMethodConfigurationDialog::changeMethod( const char * szMethodName ){
	
	m_strMethodName = szMethodName;

	// XML�̓ǂݒ��������ɔ����A�e�^�u���N���A
	{
		int nCnt = m_vect_pBatchActionSelectPanel.size();
		for( int i=0; i<nCnt; i++ ){
			BatchActionGrid* bag;
			bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[i]))->getSelectedActions();
			bag->init();	// @date 2013/07/02 <Add> OKADA	// SPEC 93492
		}
	}

	// XML�t�@�C������ǂݒ�������
	std::string strXmlFileNameWithExt;
	strXmlFileNameWithExt = szMethodName;
	strXmlFileNameWithExt += ".xml";

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
		;
	}else{
		strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
	}

	mgr.readXML( strXmlFileNameWithExt.c_str() );	// XML�t�@�C������ǂݍ���

	// XML�t�@�C������ǂݍ��񂾃f�[�^���e�^�u�ɏ�������
	{
		
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();

		std::vector<Action>	vectActions = mgr.getVectXmlActions();
		int nCnt = vectActions.size();
		for( int i=0; i<nCnt; i++ ){
			Action actTemp = vectActions[i];	
			m_pBatchList->addAction( actTemp );


			BatchActionGrid* bag;
			switch( actTemp.nStage ){
			case SN_INITIALIZE:		// �@Initialize
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[0]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER:			// �A
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[1]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_MANIPULATE:		// �B
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[2]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_ANALYZE:		// �C
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[3]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_IDENTIFY:		// �D
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[4]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER_RESULTS:	// �EFilter Results
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[5]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			default:
				break;
			}
		}

		// ��ʂɔ��f
		m_pBatchList->updateList();
		m_pBatchList->RefreshBatchGrid();

	}

	// ��ʂɔ��f
	updateBatchList();
	m_pNoteBook->Refresh();

	m_bChangeAction = false;	// xml��ǂݒ����̂Ń��Z�b�g
 // SPEC 85856

}

// change method combobox
void BatchProcessingMethodConfigurationDialog::changeMethodCombobox( wxCommandEvent& evt )
{

	std::string strMethodName = evt.GetString();	// �R���{�{�b�N�X�̃��\�b�h��������

	changeMethod( strMethodName.c_str() );

	return;
}

// remake method combobox
void BatchProcessingMethodConfigurationDialog::remakeMethodCombobox( std::string strSelectMethod ){
	wxArrayString stringsMethod;
	stringsMethod = m_stringsMethod;
}

// get strings method
wxArrayString BatchProcessingMethodConfigurationDialog::getStringsMethod( void ){
	return m_stringsMethod;
}

// get current method
std::string BatchProcessingMethodConfigurationDialog::getCurrentMethod( void ){
	return m_strMethodName;
}

// get combo method text
std::string BatchProcessingMethodConfigurationDialog::getComboMethodText( void ){
	wxString wxstrTemp = m_pComboMethod->GetValue();

	std::string strTemp = wxstrTemp.c_str();

	return strTemp;
}

// on update UI
void BatchProcessingMethodConfigurationDialog::onUpdateUI( wxUpdateUIEvent& evt ){
	
	int nId = evt.GetId();
	switch( nId ){
	case ID_MATRIX_ADD_BUTTON:
		evt.Enable( true );
		break;
	case ID_MATRIX_EDIT_BUTTON:		// fall-through
	case ID_MATRIX_SAVE_BUTTON:		// fall-through
	case ID_MATRIX_SAVEAS_BUTTON:	// fall-through
	case ID_MATRIX_DELETE_BUTTON:	
		// Batch Method���󗓂łȂ��ꍇ��enable
		if( m_pComboMethod != NULL ){
			wxString wxstrBatchMethod = m_pComboMethod->GetValue();
			std::string strBatchMethod = wxstrBatchMethod.c_str();
			if( 1 <= strBatchMethod.length() ){
				// Batch Method �R���{�{�b�N�X���󗓂łȂ�->enable
				evt.Enable( true );
			}else{
				// Batch Method �R���{�{�b�N�X����
				evt.Enable( false );
			}
		}
		break;
	case ID_COMBOBOX_METHOD:		// SPEC 85673
		if( m_pComboMethod != NULL ){
			wxArrayString wxasStrings = m_pComboMethod->GetStrings();
			if( wxasStrings.size() <= 0 ){
				// Batch Method �R���{�{�b�N�X����
				evt.Enable( false );
			}else{
				// Batch Method �R���{�{�b�N�X����łȂ�->enable
				evt.Enable( true );
			}
		}
		break;
	case wxID_OK:
		evt.Enable( true );
		break;
	default:
		break;
	}

	return;
}


// for SPEC 85672
int BatchProcessingMethodConfigurationDialog::checkSameNameConbobox( const char * szNewMethodName )
{
	wxArrayString wxasStrings = m_pComboMethod->GetStrings();


	for( unsigned int i=0; i<wxasStrings.size(); i++){	// @date 2013/06/10 <Mod> OKADA
		if( strcmp( wxasStrings[i].c_str(), szNewMethodName ) == 0){
			// ��v
			return 1;
		}
	}
	
	return 0;
}

/** �^�u��onEdit���Ăяo���܂� */
void BatchProcessingMethodConfigurationDialog::editBatchActionSelectPanel( int index, int line_no ){
	getBatchActionSelectPanel( index )->editByIndex( line_no );
}

// clear Default Output Format
void BatchProcessingMethodConfigurationDialog::deleteDefaultOutputFormat( const char* szBatchMethod )
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();

	ini->deleteValue( SECTION_METHOD_FMT, szBatchMethod );

	std::string strKey = szBatchMethod;
	strKey.append( "_PARAM" );
	ini->deleteValue( SECTION_METHOD_FMT, strKey.c_str() );

	return;
}

// copy Default Output Format
void BatchProcessingMethodConfigurationDialog::copyDefaultOutputFormat( const char* szBatchMethod, const char* szNewBatchMethod, bool flgRename )
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();

	std::string strKey = szBatchMethod;
	strKey.append( "_PARAM" );

	const char* szSetVal = ini->getString( SECTION_METHOD_FMT, szBatchMethod, "" );
	std::string strSetVal = szSetVal;

	const char* szSetParamVal = ini->getString( SECTION_METHOD_FMT, strKey.c_str(), "" );
	std::string strSetParamVal = szSetParamVal;

	std::string strNewKey = szNewBatchMethod;
	strNewKey.append( "_PARAM" );

	ini->setString( SECTION_METHOD_FMT, szNewBatchMethod, strSetVal.c_str() );
	ini->setString( SECTION_METHOD_FMT, strNewKey.c_str(), strSetParamVal.c_str() );

	if( flgRename ){
		// delete original setting
		deleteDefaultOutputFormat( szBatchMethod );
	}

	return;
}

// param to vect
int BatchProcessingMethodConfigurationDialog::param2vect( const char* szParam, std::vector<std::string>& vecParam )
{
	int nLen = strlen( szParam );

	if( nLen <= 1 ){
		// error
	}

	int nCutStart = 0;
	int nCutEnd = nLen-1;

	if( szParam[0] =='[' ){
		nCutStart=1;
	}
	if( szParam[nCutEnd] ==']' ){
		nCutEnd--;
	}

	if( nCutStart >=  nCutEnd ){
		// error
	}

	char szDestParam[4096];
	int nLenDestParam = nCutEnd-nCutStart+1;
	if( sizeof(szDestParam) < nLenDestParam ){
		// error
	}
	strncpy( szDestParam, &szParam[nCutStart], nLenDestParam );
	
	char szTemp[10] = "";
	std::string strTemp = "";
	std::string strTemp2;
	std::vector<std::string> vecParamLocal;
	int nKakkoDepth = 0;

	for( int i=0; i<nLenDestParam; i++ ){
		if( szDestParam[i] == ',' ){
			if( nKakkoDepth == 0 ){
				vecParamLocal.push_back( strTemp );
				strTemp.clear();
			}else{
				szTemp[0] = szDestParam[i];
				szTemp[1] = '\0';
				strTemp2 = strTemp + szTemp;
				strTemp = strTemp2;
			}
		}else if( szDestParam[i] == '[' ){
			nKakkoDepth++;
			szTemp[0] = szDestParam[i];
			szTemp[1] = '\0';
			strTemp2 = strTemp + szTemp;
			strTemp = strTemp2;
		}else if( szDestParam[i] == ']' ) {
			nKakkoDepth--;
			szTemp[0] = szDestParam[i];
			szTemp[1] = '\0';
			strTemp2 = strTemp + szTemp;
			strTemp = strTemp2;
		}else{
			szTemp[0] = szDestParam[i];
			szTemp[1] = '\0';
			strTemp2 = strTemp + szTemp;
			strTemp = strTemp2;
		}
	}
	vecParamLocal.push_back( strTemp );
	strTemp.clear();

	vecParam = vecParamLocal;

	return 0;
}
