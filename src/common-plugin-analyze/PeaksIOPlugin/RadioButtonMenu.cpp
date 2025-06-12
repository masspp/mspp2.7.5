/**
 * @file RadioButtonMenu.cpp
 * @brief radio button menu dialog box class
 *
 * @author OKADA, H
 * @date 2011/05/31
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "RadioButtonMenu.h"

#include <wx/radiobut.h>

using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE	3
#define GRID_GAP	3

#if USE_RADIO_BUTTON
BEGIN_EVENT_TABLE( RadioButtonMenuDialog, kome::window::ParametersAndHelpDialog )
	EVT_RADIOBUTTON( wxID_ANY,	RadioButtonMenuDialog::OnSelectRadioButton )
END_EVENT_TABLE()
#endif

// constructor
RadioButtonMenuDialog::RadioButtonMenuDialog( wxWindow* parent, kome::objects::Variant acObj ) 
	: kome::window::ParametersAndHelpDialog(	parent, "Import Peaks" ) {
  m_obj = acObj;

	// initialize
#if USE_RADIO_BUTTON

	m_selectedRadioButtonNo = -1;

	// radio button menu �̏�����
	strMenu[IMPORT_PEAKS_IMPORT			-IMPORT_PEAKS_IMPORT]	= _T( STR_IMPORT_PEAKS_IMPORT );
	strMenu[IMPORT_PEAKS_OVERLAP		-IMPORT_PEAKS_IMPORT]	= _T( STR_IMPORT_PEAKS_OVERLAP );
	strMenu[IMPORT_PEAKS_DELETE_OVERLAP	-IMPORT_PEAKS_IMPORT]	= _T( STR_IMPORT_PEAKS_DELETE_OVERLAP );

	// radio button menu ��Window ID ��������
	for( int i=0; i<_countof(idMenu); i++ ){
		idMenu[i] = 0;
	}
	
	m_tempRadioButton = NULL;
#else
	m_targetBox = NULL;
#endif

}

// destructor
RadioButtonMenuDialog::~RadioButtonMenuDialog() {
}

// create top sizer
wxSizer* RadioButtonMenuDialog::createMainSizer() {
	// field sizer
	wxBoxSizer* fieldSizer = new wxBoxSizer( wxVERTICAL );

	// create target box
#if USE_RADIO_BUTTON
	

	wxWindowID wxId=-100;

	for( int i=IMPORT_PEAKS_IMPORT; i<(IMPORT_PEAKS_NUMS); i++ ){

		// ���W�I�{�^���ݒ�
		if( i == IMPORT_PEAKS_IMPORT ){
			// ����̓O���[�v�w��
			m_tempRadioButton = new wxRadioButton( this, wxID_ANY, strMenu[i-IMPORT_PEAKS_IMPORT], wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
			m_tempRadioButton->SetValue( true );
			m_selectedRadioButtonNo = IMPORT_PEAKS_IMPORT;
			wxId = m_tempRadioButton->GetId();	// �V�X�e���������I��ID������U��̂ŁA�ۑ����Ă���
			idMenu[i-IMPORT_PEAKS_IMPORT] = wxId;
		}else{
			
			m_tempRadioButton = new wxRadioButton( this, wxID_ANY, strMenu[i-IMPORT_PEAKS_IMPORT], wxDefaultPosition, wxDefaultSize );
			wxId = m_tempRadioButton->GetId();
			idMenu[i-IMPORT_PEAKS_IMPORT] = wxId;	// �V�X�e���������I��ID������U��̂ŁA�ۑ����Ă���
			// Heat Map �ȊO�I��s��
			if( m_obj.type == kome::objects::Variant::CHROMATOGRAM || m_obj.type == kome::objects::Variant::SPECTRUM ){
				m_tempRadioButton->Enable( false );
			}
		}
	
		setDescription( m_tempRadioButton, "Select the action when importing peaks.\n"
"Import: import peaks.\n"
"Overlap: Overlapping peaks. (Available for 2D peaks)\n"
"Delete Overlap: Delete overlapping pekas. (Available for 2D peaks)" );
		// wxFlexGridSizer��add
		fieldSizer->Add(
			m_tempRadioButton,
			1,
			wxGROW | wxALL,
			BORDER_SIZE
		);
	}
#else
	fieldSizer->Add(
		createTargetBox(),
		1,				
		wxGROW | wxALL,
		BORDER_SIZE
	);
#endif
	return fieldSizer;
}

#if USE_RADIO_BUTTON
#else
// creates target radio box
wxRadioBox* RadioButtonMenuDialog::createTargetBox() {
	// create the array
	std::vector< wxString > items;

	items.push_back( wxT( STR_IMPORT_PEAKS_IMPORT ) );
	items.push_back( wxT( STR_IMPORT_PEAKS_OVERLAP ) );
	items.push_back( wxT( STR_IMPORT_PEAKS_DELETE_OVERLAP ) );
	
	// create radio box
	m_targetBox = new wxRadioBox( this, wxID_ANY, wxT( "Select Pattern(TEST:wxRadioBox)" ), wxDefaultPosition, wxDefaultSize, items.size(), 	&( items[ 0 ] ),
		0,
		wxRA_SPECIFY_ROWS
	);
	m_targetBox->SetSelection( 0 );
	setDescription( m_targetBox, "Select the action when importing peaks.\n"
"Import: import peaks.\n"
"Overlap: Overlapping peaks. (Available for 2D peaks)\n"
"Delete Overlap: Delete overlapping pekas. (Available for 2D peaks)" );
	return m_targetBox;
}
#endif

// return selected button ID ( IMPORT_PEAKS_IMPORT �` IMPORT_PEAKS_DELETE_OVERLAP )
int RadioButtonMenuDialog::getSelection( void ){

#if USE_RADIO_BUTTON
	return m_selectedRadioButtonNo;
#else
	if( m_targetBox ){
		return m_targetBox->GetSelection();
	}
#endif

	return -1;
}

#if USE_RADIO_BUTTON

void RadioButtonMenuDialog::OnSelectRadioButton(wxCommandEvent& event){

	int iEventId = event.GetId();	// iEventId ���ǂ̃��j���[�ɑΉ����邩�� idMenu[] �Ɋi�[����Ă���

	for( int i=IMPORT_PEAKS_IMPORT; i<(IMPORT_PEAKS_NUMS); i++ ){
		if( iEventId == idMenu[i-IMPORT_PEAKS_IMPORT] ){
			m_selectedRadioButtonNo = i;
			break;
		}
	}

	return;
}


#endif
