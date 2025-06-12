/**
 * @file DBAccessClass
 * @brief Data Processing List Box Informations
 *
 * @author M.Fukuda
 * @date 2013.02.26
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * @ note -=====================================
 *     Link DB�̒l�𓝊�����B
 *     (has-a) DBACCSelectDialog : �I���̃R���{�{�b�N�X
 */

#include "../rel_stdafx.h"
#include "LinkDBAccessClass.h"
#include "LinkDBSelectDialog.h"
#include "../calcFunc/StrFunc.h"

#include "ExternalDbSearchDialog.h"
#include "NameSelectToAccession.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


DBAccClass::DBAccClass() 
	: DBLArr_()
{
	init();
	// �Q�lURL�Ƃ��ā@�f��URL���������ā@URL�l�Ɏ����Ă����B
	std::for_each(DBLArr_.begin(),DBLArr_.end(),[&](DBLinks& tgt){tgt.setURL("ACCESSION");});
}

void DBAccClass::init()
{
	DBLArr_.push_back(DBLinks("CAS",       "http://webbook.nist.gov/cgi/cbook.cgi?Str2File=C",""));
	DBLArr_.push_back(DBLinks("CHEBI",     "http://www.ebi.ac.uk/chebi/saveStructure.do?defaultImage=true&chebiId=",""));
	DBLArr_.push_back(DBLinks("CHEMSPIDER","http://www.chemspider.com/FilesHandler.ashx?type=str&striph=yes&id=",""));
	DBLArr_.push_back(DBLinks("KEGG",      "http://www.genome.jp/dbget-bin/www_bget?-f+m+",""));
	DBLArr_.push_back(DBLinks("KNAPSACK",  "http://knapsack3d.sakura.ne.jp/mol3d/",".3d.mol"));
	DBLArr_.push_back(DBLinks("LIPIDMAPS", "http://www.lipidmaps.org/data/LMSDRecord.php?LMID=","&Mode=Download"));
	DBLArr_.push_back(DBLinks("PUBCHEM",   "http://pubchem.ncbi.nlm.nih.gov/summary/summary.cgi?cid=","&disopt=DisplaySDF"));

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	unsigned int num = plgMgr.getNumberOfFunctionItems("EXTERNAL_DB");

	for(unsigned int i = 0; i < num; i++) {
		// external DB
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem("EXTERNAL_DB", i);
		if( item != NULL ) {
			void* pt = item->getCall()->invoke(NULL).prim.pt;
			kome::ident::ExternalDB* db = (kome::ident::ExternalDB*)pt;
			if (db != nullptr) {
				extDbs_.push_back(db);
			}
		}
	}
}

// �Ώ�ID��DB����Ԃ��iString)
std::string DBAccClass::getNames(const unsigned int idx) const 
{
	return DBLArr_[idx].dbName;
}

// �Ώ�ID��DB����Ԃ��iconst char*)
const char* DBAccClass::getName(const unsigned int idx) const 
{
	return DBLArr_[idx].dbName.c_str();
}

std::string DBAccClass::getsURL(const unsigned int idx) const
{
	return DBLArr_[idx].dbName;
}

// �Ώ�ID��URL���������B�i�f��URL���Ԃ�j
const char* DBAccClass::getURL(const unsigned int idx) const 
{
	return DBLArr_[idx].URL.c_str();
}

// Data List ����̒l(listStr)�𕪉����āA
// ComboBox�̑I����� (DBACCSelectDialog)���Ăяo���B
// �����I�΂ꂽ��A���[���ɏ]���ā@URL���������ĕ�����Ƃ��ĕԂ��B
const std::string DBAccClass::setListArrStrs(const std::string& listStr) 
{
	if (listStr.empty() || listStr == "none") return "";

	typedef std::pair<std::string, std::string> strPAIR;
	std::vector<strPAIR> dbcomArr; // DBName, Accession��Pair
	std::vector<std::string> lineTokens, innerTokens;

	// Data List ����̒l(listStr)�𕪉��B�R���{�{�b�N�X�̒l�̏���
	kome::plugin::SettingsValue::separateListValue(listStr.c_str(), lineTokens);
	std::for_each(lineTokens.begin(), lineTokens.end(), [&](const std::string& tmp) {
		innerTokens.clear();
		kome::plugin::SettingsValue::separateListValue(tmp.c_str(), innerTokens);
		std::string dbn = kome::plugin::SettingsValue::convertToDisplayingString(innerTokens[0].c_str());
		std::string acc = kome::plugin::SettingsValue::convertToDisplayingString(innerTokens[1].c_str());
		dbcomArr.push_back(strPAIR(dbn,acc));
	});

	// �R���{�{�b�N�X��\���B
	// �o�^����ID��Ⴄ���[���Ȃ̂œo�^��A�i�o�^�����j�Ń\�[�g���Ȃ����ƁB
	DBACCSelectDialog dlg(kome::window::WindowTool::getMainWindow(), dbcomArr);
	std::string URL("");

	if(dlg.ShowModal() == wxID_OK) {
		// OK���N���b�N���ꂽ�̂Ń_�C�A���O����ID��Ⴄ�B
		int sid = dlg.getSelectId();
		// �ςȒl���G���[���Ԃ��Ă����B
		if (sid < 0) return URL;

		std::string dbKey = ext::tranceLowerToUpper(dbcomArr.at(sid).first); 
		std::string accKey = dbcomArr.at(sid).second; 

		// DB������v������̂����X�g����T���A
		// URL�ɃA�N�Z�b�V���������ݍ���Ł@URL�Ƃ��ĕԂ��B
		// LINK �ɓo�^���ꂽ�Ƃ��Ɂ@�啶���������ňقȂ�\���L�B
		URL = getURL(dbKey,accKey);
	}
	return URL;
}

const std::string DBAccClass::getURL(
	const std::string& exdb,
	const std::string& accession
) {
	std::string url("");
	std::for_each(DBLArr_.begin(), DBLArr_.end(),[&](const DBLinks& dblks){
	if(exdb.compare(dblks.dbName) == 0) {
		url = dblks.URLHead + accession + dblks.URLTail;
	}});
	return url;
}

const bool DBAccClass::callSearchKeyWordtoExDB(
	std::string& searchedKeyword,
	std::vector<std::string>& refRsts
) {
	// get LinkDBArr from Panels
	SearchExternalDbDialog dlg(kome::window::WindowTool::getMainWindow(), extDbs_, searchedKeyword);

	if(dlg.ShowModal() == wxID_OK) {
		std::vector<kome::ident::ExternalDB*> extDbs;
		dlg.getExtDbs(extDbs);
		searchedKeyword = dlg.getSearchKeyword();

		kome::window::DialogProgress progress(
			kome::window::WindowTool::getMainWindow(), "Get Extra DB..");
		
		progress.setInformation("Information", "Get Extra DB..");
		unsigned int extDbcount = extDbs.size();
		progress.setRange(0, extDbcount);
		progress.setPosition(0);
		unsigned int progcnt(0);
	
		std::vector<kome::ident::ExternalDB::SearchResult> results;

		for each(auto& edb in extDbs) {
			progcnt++;
			progress.setPosition(progcnt);
			progress.setStatus(
				FMT("Access... %s [%d/%d]", edb->getName(), progcnt, extDbcount).c_str());
			results.clear();
			edb->search(results, searchedKeyword.c_str());
			for each(auto& rst in results) {
				refRsts.push_back(edb->getName());
				refRsts.push_back(rst.name);
				refRsts.push_back(rst.accession);
			}
		}
		progress.fill();
		return true;
	}
	return false;
}

const bool DBAccClass::getKeyWordSearchResults(
	std::string& nameVal,
	std::string& linksVal,
	std::string& accURL
) {
	std::vector<std::string> refRsts;
	if(this->callSearchKeyWordtoExDB(searchedKeyword_, refRsts)) {
		NameSelectToAccessionDialog ddlg(
			kome::window::WindowTool::getMainWindow(),
			searchedKeyword_,
			refRsts);

		if(ddlg.ShowModal() == wxID_OK) {
			std::string extdb = ddlg.getDBName();
			std::string accession = ddlg.getAccession();
			accURL = getURL(extdb, accession);

			linksVal = "[[" + extdb + "]:[" + accession + "]]";
			std::string tmp = "[" + ddlg.getInfoName() + "]";
			tmp = replacestring(tmp.c_str(), ",", "\\,");
			nameVal = tmp;
			return true;
		}
	}
	return false;
}
