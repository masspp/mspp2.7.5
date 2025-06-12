/**
 * @file InstrumentTypesMgr.cpp
 * @brief MassBank Instrument Type Manager
 *
 * @author M.Fukuda
 * @date 2011.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "InstrumentTypesMgr.h"
#include "../stdafx.h"

//#include "MassBankManager.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>
#define foreach BOOST_FOREACH

#define ORG_INSTLIST "CE-ESI-TOF:CI-B:EI-B:EI-EBEB:ESI-IT-MS/MS:ESI-QqQ-MS/MS:ESI-QqTOF-MS/MS:FAB-B:FAB-EB:FAB-EBEB:FD-B:GC-EI-TOF:LC-APPI-QQ:LC-ESI-IT:LC-ESI-ITFT:LC-ESI-ITTOF:LC-ESI-Q:LC-ESI-QIT:LC-ESI-QQ:LC-ESI-QTOF:MALDI-TOF"

#define SECTION                    "MassBank"
#define INSTRUMENT_TYPES_KEY       "INSTRUMENT_LIST"
#define INSTRUMENT_OK_CHECK        "INSTRUMENT_OK_LIST"

// --- Instrument Types Set ----------------------------------------------------------

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


InstrumentMgr::InstrumentMgr(){}
InstrumentMgr::~InstrumentMgr(){}

// clears instrument types
void InstrumentMgr::clearInstrumentTypes() {
	m_instruments.clear();
}

// get the number of instrument types
unsigned int InstrumentMgr::getNumberOfInstrumentTypes() {
	return m_instruments.size();
}

// get instrument type
const char* InstrumentMgr::getInstrumentType(
	const unsigned int idx
){
	if(idx >= m_instruments.size()) {
		return NULL;
	}
	return m_instruments[idx].m_name.c_str();
}

// get target idx instrument type is shown ? 
const bool InstrumentMgr::isShown(
	const unsigned int idx
){
	if(idx >= m_instruments.size()) {
		return false;
	}
	return m_instruments[idx].isShown;
}

// set shown Datas
void InstrumentMgr::setInstrumentTypeShownFlagsWithIDArray(
	const std::vector<long long>& idArray
){
	// reset All Menber's Shown Flag
	foreach ( auto& tgt, m_instruments ) {
		tgt.isShown = false;
	}
	
	int mySize = m_instruments.size();
	foreach( int idNum, idArray ) {
		if ( idNum < mySize ) {
			m_instruments[idNum].isShown = true;
		}
	}
}

// get shown instrumens type
void InstrumentMgr::getCheckedInstrumentTypes(
	std::vector<std::string>& instArr
){
	instArr.clear();
	foreach (auto& tgt, m_instruments) {
		if (tgt.isShown) {
			instArr.push_back(tgt.m_name);
		}
	}
}

// �SInstrumentType�̒l��Ԃ�
void InstrumentMgr::getAllInstrumentTypes(
	std::vector<std::string>& insttypes
){
	insttypes.clear();
	foreach(auto& tgt, m_instruments) {
		insttypes.push_back(tgt.m_name);
	}
}

// check having instrument types.
// if this method call first time, call updateInstrumentType fo initialize
// Already gotton this, meybe m_instruments.size > 0, so return true.
bool InstrumentMgr::checkHavingInstrumentTypes(bool& netInfo) {
	if (netInfo || m_instruments.empty()) {
		netInfo = false;
		return updateInstrumentTypes();
	} else {
		return true;
	}
}

int InstrumentMgr::numberOfCheckedRow() const {
	int c = 0;
	foreach(auto& t, m_instruments) {
		if(t.isShown) c++;
	}
	return c;
}

// update instrument types
bool InstrumentMgr::updateInstrumentTypes() {
	// get instrument types
	std::vector<std::string> instruments, checkedArr;
	bool bReadIni(false);

	// Instrument�̃��X�g�������o��
	onLoadInstrumentTypesFromAny(instruments, bReadIni);
	// �`�F�b�N�ς݂̗v�f���擾����
	onLoadCheckedInstLists(checkedArr);

	//Clear
	m_instruments.clear();
	m_instruments.reserve(instruments.size());

	if(checkedArr.empty()) {
		//������ԂȂ̂ŁA�S�`�F�b�N��Ԃɂ���
		for each(auto& s in instruments) {
			m_instruments.push_back(InstrumentSet(s, true));
		}
	} else {
		//�`�F�b�N�ς݂̃t���O�𔽉f����
		for each(auto& s in instruments) {
			bool fx = std::find(checkedArr.begin(), checkedArr.end(), s) != checkedArr.end();
			m_instruments.push_back(InstrumentSet(s, fx));
		}
	}

	// set to ini
	if (!bReadIni) {
		onSetIniParam(INSTRUMENT_TYPES_KEY, joinWithPipe(instruments));
//		onSaveALLInstrumentTypes();
	}

	return true;
}

// Instrument Type���K���ɏ]���ČĂяo���B
// 1st : MassBank �� API
// 2nd : kome.ini �� �ۑ��l
// 3rd : ��������Ă���f�t�H���g�l
bool InstrumentMgr::onLoadInstrumentTypesFromAny(
	std::vector<std::string>& instArr, 
	bool& readini
){
	// Begining get - set ini files param
	// Set_f as MassBankManager::getInstrumentTypesFromMassBank
	const bool rst_getInst = setF_(instArr);

	// Mass Bank Error. so. Read ini Files Param
	if (!rst_getInst) {
		instArr.clear();
		std::string orgInstStr = onGetIniParam(INSTRUMENT_TYPES_KEY);

		//ini�ɒl���Ȃ��ꍇ,�����Œ�l���g�p����B
		if (orgInstStr.empty()) {
			orgInstStr = ORG_INSTLIST;
		} else {
			readini = true;
		}
		stringseparate(orgInstStr.c_str(), ELEMENT_VALUES_DELIMITER, instArr);
	}
	return true;
}

// �SInstrumentType��MassBank Format�ɕϊ����ĕԂ�
const std::string InstrumentMgr::getAll_ConvertMassBankSorceStr()
{
	std::vector<std::string> elms;
	getCheckedInstrumentTypes(elms);
	std::string rst("");
	foreach (auto& tgt, elms) {
		rst.append(FMT("<instrumentTypes>%s</instrumentTypes>\r\n", tgt.c_str()));
	}
	return rst;
}

// �`�F�b�N���ꂽ���ڂ�ۑ�����
void InstrumentMgr::saveCheckedStatus() 
{
	std::vector<std::string> elms;
	getCheckedInstrumentTypes(elms);
	onSetParam(INSTRUMENT_OK_CHECK, joinWithPipe(elms));
}

// ���X�g�ɂ��邷�ׂĂ� Instrument Type��ۑ�����
void InstrumentMgr::onSaveALLInstrumentTypes()
{
	std::vector<std::string> elms;
	getAllInstrumentTypes(elms);
	onSetIniParam(INSTRUMENT_TYPES_KEY, joinWithPipe(elms));
}

std::string InstrumentMgr::joinWithPipe(
	const std::vector<std::string>& src) const
{
	return boost::algorithm::join(src, ELEMENT_VALUES_DELIMITER);
}

//�`�F�b�N����Ă��郊�X�g��ǂݏo���ĕ�������
void InstrumentMgr::onLoadCheckedInstLists(std::vector<std::string>& ckArr)
{
	std::string orgInstStr = onGetParam(INSTRUMENT_OK_CHECK);
	// �`�F�b�N�ς݃��X�g�𕪊�
	if (!orgInstStr.empty()) {
		stringseparate(orgInstStr.c_str(), ELEMENT_VALUES_DELIMITER, ckArr);
	}
}

// Kome.ini�ɒl��ۑ�����
void InstrumentMgr::onSetIniParam(
	const char* key, const std::string& xstr
) {
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if(ini != NULL) {
		ini->setString(SECTION, key, xstr.c_str());
	}
}

// Kome.ini ���� �l���擾����
const std::string InstrumentMgr::onGetIniParam(const char* key)
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if(ini != NULL) {
		return ini->getString(SECTION, key, "");
	}
	return "";
}

// Parameters.ini �ɒl��ۑ�����
void InstrumentMgr::onSetParam(const char* key, const std::string& xstr) 
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();
	if( ini != NULL ) {
		ini->setString(SECTION, key, xstr.c_str());
	}
}

// Parameters.ini ���� �l���擾����
const std::string InstrumentMgr::onGetParam(const char* key)
{
	// parameters
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();
	if(ini != NULL) {
		return ini->getString(SECTION, key, "");
	}
	return "";
}
