/**
 * @file DetailsListCtrl.cpp
 * @brief implements of DetailsListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DetailsListCtrl.h"
#include "EditSampleUserPropertiesDialog.h"

#define ID_ADD_BUTTON                     66

using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( DetailsListCtrl, kome::window::EditGridListCtrl )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, DetailsListCtrl::onSelectItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, DetailsListCtrl::onSelectItem )
	EVT_LIST_COL_CLICK( wxID_ANY, DetailsListCtrl::onClickCol )
END_EVENT_TABLE()


// constructor
DetailsListCtrl::DetailsListCtrl( EditSampleUserPropertiesDialog* pParentDlg ) 
	: kome::window::EditGridListCtrl( pParentDlg, false ),
		m_pParentDlg(pParentDlg){
	// create list ctrl
	create();
	
	// Context Menu
	setAddMenuName( "New" );
	setPopupMenuFlag( POPUP_COPY | POPUP_DELETE | POPUP_SAVE );
}

// destructor
DetailsListCtrl::~DetailsListCtrl() {
}

// clear properties
void DetailsListCtrl::clear() {
	deselectAll();
	m_properties.clear();
	clearData( true );
}

// set properties
void DetailsListCtrl::setProperties( kome::core::Properties& props ) {
	// clear
	m_properties.clear();
	clearData( false );

	// get properties
	m_properties.reserve( props.getNumberOfProperties() );
	for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
		m_properties.push_back( std::make_pair( props.getKey( i ), props.getValue( i ) ) );
		addData( i, false );
	}

	// update list
	updateList();
}

// get properties
void DetailsListCtrl::getProperties( kome::core::Properties& props ) {
	
	std::vector< std::pair< std::string, std::string > >::iterator propertiesIterator;
	for (propertiesIterator = m_properties.begin();
			propertiesIterator != m_properties.end(); ++propertiesIterator) {
		
			props.setValue(propertiesIterator->first.c_str(), propertiesIterator->second.c_str());
	}
}

// validate properties
bool DetailsListCtrl::validateProperties(  ) {
	
	bool bValid = true;

	std::set< std::string > keySet;
	std::vector< std::pair< std::string, std::string > >::iterator propertiesIterator;
	for (propertiesIterator = m_properties.begin();
			propertiesIterator != m_properties.end() && bValid; ++propertiesIterator) {
		
			keySet.insert(propertiesIterator->first);

			if (propertiesIterator->first == "<KEY>")
			{
				kome::window::WindowTool::showInfo("Key cannot be \'<KEY>\'.");
				SetItemState( propertiesIterator - m_properties.begin(), wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

				bValid = false;
			}
			else if (propertiesIterator->second == "<VALUE>")
			{
				kome::window::WindowTool::showInfo("Value cannot be \'<VALUE>\'.");
				SetItemState( propertiesIterator - m_properties.begin(), wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
				bValid = false;
			}
	}

	// If validation has succeeded so far, all elements have been
	// inserted into keySet, and we can now check whether there
	// are duplicate keys in m_properties.
	if (bValid)
	{
		bValid = (keySet.size() == m_properties.size());
		if (!bValid)
		{
			kome::window::WindowTool::showInfo("All Key values must be unique.");
		}
	}

	return bValid;
}

// create
void DetailsListCtrl::create() {
	// insert column
	addColumn( "Key", TYPE_STRING );
	addColumn( "Value", TYPE_STRING );
	SetSingleStyle(wxLC_SINGLE_SEL);
}

// get string
std::string DetailsListCtrl::getString( const long long data, const int column ) const {
	std::string s;
	if( data >= 0 && data < (int)m_properties.size() ) {
		if( column == 0 ) {
			s = m_properties[ data ].first;
		}
		else if( column == 1 ) {
			s = m_properties[ data ].second;
		}
	}

	return s;
}

// set string
void DetailsListCtrl::setString( const long long data, const int column,
											const std::string s ) {
	if( data >= 0 && data < (int)m_properties.size() ) {
		if( column == 0 ) {
			m_properties[ data ].first = s;
		}
		else if( column == 1 ) {
			m_properties[ data ].second = s;
		}
	}
}

void DetailsListCtrl::onSelectItem( wxListEvent& evt )
{
	const long lSelectedItem = evt.GetIndex();

	m_pParentDlg->ItemSelected(lSelectedItem, getItemText(lSelectedItem, 0), getItemText(lSelectedItem, 1));
}

void DetailsListCtrl::onActivateData( const long long data ) {
}

// on delete data edit
bool DetailsListCtrl::onDeleteData( const long long data ){
	// SPEC No.91661: Sample User Properties�_�C�A���O�ŁADelete�{�^�����N���b�N����Ɨ�����
	// @date 2013.03.15 <Mod> M.Izumi
	EditSampleUserPropertiesDialog* dlg = (EditSampleUserPropertiesDialog*)m_pParentDlg;
	if( dlg != NULL ){
		dlg->NoListItemSelected();
		dlg->setEdit( true );
	}
	m_properties.erase( m_properties.begin() + data );
	
	clearData( false );
	for( unsigned int i=0; i < m_properties.size(); i++ ){
		addData( i, true );
	}
	return false;
}

// Stop the list control from sorting its contents when a column is clicked.
void DetailsListCtrl::onClickCol( wxListEvent& evt )
{
	evt.Veto();
}

// delete items
void DetailsListCtrl::deleteItems()
{
	int iDeletedItemIndex = -1;

	for( int i = GetItemCount() - 1; i >= 0; i-- ) {
		if( GetItemState( i, wxLIST_STATE_SELECTED ) > 0 ) {
			
			deleteData( getData(i), true );
			iDeletedItemIndex = i;
		}
	}
	
	// Select the item at the same position as the deleted item, if possible
	if (iDeletedItemIndex > static_cast<int>(m_properties.size()) - 1)
	{
		iDeletedItemIndex = m_properties.size() - 1;
	}
	if (iDeletedItemIndex >= 0)
	{
		// Deselect and reselect the item so that, in the case of the same item
		// being selected, an EVT_LIST_ITEM_SELECTED event is received, thus enabling
		// the delete button, which is the expected behaviour.
		SetItemState( iDeletedItemIndex, !wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		SetItemState( iDeletedItemIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	else
	{	// Otherwise, deselect all
// >>>>>>	@Date:2013/09/10	<Modify>	A.Ozaki
// �S���ڂ��Ȃ��Ȃ����Ƃ��ɁA���X�g�R���g���[���̒��g�ɁA�󔒍s���c���Ă��܂�
// ���̍s��I�����āA[Delete]����Ɨ����Ă��܂��s��̏C��
//   deselectAll( )���Ăяo���Ă��܂��ƁAonSelectItem( )���Ăяo�����̂ŁA
//   [Delete]�{�^������������̂ŁA�f�[�^�ނ̃N���A�����ɂ��܂���
//
//		deselectAll();
		m_properties.clear( );
		clearData( true );
//
// <<<<<<	@Date:2013/09/10	<Modify>	A.Ozaki
	}
}

void DetailsListCtrl::newItem()
{
	m_properties.push_back( std::make_pair( "<KEY>", "<VALUE>" ) );
	addData( m_properties.size() - 1, true );
	SetItemState( m_properties.size() - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void DetailsListCtrl::updateItem(const long lRow, const int iColumn, const char *s)
{
	setItemValue(lRow, iColumn, s);
	updateList();
}

void DetailsListCtrl::setItemValue( const long long data, const int column, const char* s )
{
	// string object
	std::string str = s;

	// column type
	if( column < (int)m_columnTypes.size() ) {
		m_columnTypes[ column ] = TYPE_STRING;
	}

	setString( data, column, str );
}

// create menu
wxMenu* DetailsListCtrl::createMenu(){
	wxMenu* menu = NULL;
	size_t uiInsertLocation = 0;

	int nSelectedItemCount = GetSelectedItemCount();	// ���X�g�ō��ڂ����I������Ă��邩��Ԃ�
	// ���X�g�ō��ڂ�1���I������Ă��Ȃ��ꍇ�� Add �ɑ������郁�j���[�݂̂�\������

	if(nSelectedItemCount == 0){
		// Add �ɑ������郁�j���[�݂̂�\��
		menu = new wxMenu();
	}else{
		menu = GridListCtrl::createMenu();	// �e�N���X��createMenu()���Ăяo��
	}

	bool flgMenuDisp=false;	// menu��1���ڂł��Z�b�g������true�BSEPARATOR�\�����ʗp�B

	if(nSelectedItemCount == 0){
		setAddMenuName( "New" );
	}else{
		setAddMenuName( "" );
		if( getActivateName().empty() ){
			;	// �����񂪎w�肳��Ă��Ȃ�
			uiInsertLocation = 0;
		}else{
			// �����񂪎w�肳��Ă��܂�->�擪�ɓ���menu������
			uiInsertLocation = 2;
		}
	}

	// Add or New
	if( getAddMenuName().empty() ){
		// ADD menu �\�����Ȃ�
	}else{
		menu->Insert( uiInsertLocation, ID_ADD_BUTTON, wxT( getPopupMenuName(POPUP_ADD).c_str() ) );
		uiInsertLocation++;
		flgMenuDisp = true;
	}

	return menu;
}
