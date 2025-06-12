/**
 * @file MassBankSearchResultHandler.cpp
 * @brief implements of MassBankSearchResultHandler class
 *
 * @author S.Tanaka
 * @date 2009.09.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "../stdafx.h"
#include "MassBankSearchResultHandler.h"


using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define RESULT_TAG_NAME    "results"
#define ID_TAG_NAME        "id"
#define TITLE_TAG_NAME     "title"
#define FORMULA_TAG_NAME   "formula"
#define MASS_TAG_NAME      "exactMass"
#define SCORE_TAG_NAME     "score"
#define QUERY_TAG_NAME     "queryName"
#define FAULT_TAG_NAME     "faultstring"

// constructor
MassBankSearchResultHandler::MassBankSearchResultHandler(
	std::vector<MassBankManager::SearchResult>& results
	) : m_results(results) 
	, m_currResult()
	, m_resultMap()
	, m_query()
	,m_error("")

{
	onStartDocument();
}

// destructor
MassBankSearchResultHandler::~MassBankSearchResultHandler() {
}

// on start document
void MassBankSearchResultHandler::onStartDocument() {
	m_resultMap.clear();
	m_query.clear();
}

// on end document
void MassBankSearchResultHandler::onEndDocument() {
}

// on start element
void MassBankSearchResultHandler::onStartElement(
	const char* name, 
	kome::core::Properties& attrs
) {
	// each tags
	if( strcmp( name, RESULT_TAG_NAME ) == 0 ) {	// <results>
		m_currResult.id.clear();
		m_currResult.title.clear();
		m_currResult.formula.clear();
		m_currResult.mass = -1.0;
		m_currResult.score = -1.0;
		m_currResult.queryArr.clear();
	}
}

bool queryMore( const MassBankManager::queryPair& q0, double elem )
{
   return q0.second < elem;
}

// on end element
void MassBankSearchResultHandler::onEndElement(
	const char* name, 
	const char* text
) {
	// each tags
	if(strcmp(name, FAULT_TAG_NAME) == 0) {
		m_error = NVL(text,"");
	}

	if( strcmp( name, ID_TAG_NAME ) == 0 ) {	// </id>
		m_currResult.id = NVL( text, "" );
	}
	else if( strcmp( name, TITLE_TAG_NAME ) == 0 ) {	// </title>
		m_currResult.title = NVL( text, "" );
	}
	else if( strcmp( name, FORMULA_TAG_NAME ) == 0 ) {	// </formula>
		m_currResult.formula = NVL( text, "" );
	}
	else if( strcmp( name, MASS_TAG_NAME ) == 0 ) {    // </exactMass>
		m_currResult.mass = todouble( text, -1.0 );
	}
	else if( strcmp( name, SCORE_TAG_NAME ) == 0 ) {	// </score>
		m_currResult.score = todouble( text, -1.0 );
	}
	else if( strcmp( name, QUERY_TAG_NAME ) == 0 ) {    // </query>
		m_query = NVL( text, "" );
	}
	if( strcmp( name, RESULT_TAG_NAME ) == 0 ) {	// <results>
		MassBankManager::SearchResult* result = NULL;
		// 
		// resultMap : [id, ��b���] �̃}�b�v��p�ӂ���B
		// �n�b�V���L�[�C���Ɍ����������āA�Y��������̂��Ȃ����
		// �V���Ɋ�b���(ID,Title,Formula,extctMass)�Ƃ��Č��݂̃J�����g��o�^����B
		// �ȍ~�A���l��ID���o���瓯����b��񂵂������Ȃ��̂ŁA���̕����͗e�ʐߖ�̂��߂ɏȂ�
		//
		std::string& id = m_currResult.id;
		if( m_resultMap.find(id) == m_resultMap.end() ) {
			m_resultMap[id] = static_cast<int>(m_results.size());
			m_currResult.queryArr.clear();
			m_results.push_back(m_currResult);
			result = &( m_results.back() );
		}
		else {
			// MAP���ID������ꍇ�͂���������o���B
			// �L�[�ɂȂ����̂�[]�ŒT���Ə���ɍ����̂�Find�őO���������Ă���
			int idx = m_resultMap[id];
			result = &(m_results[idx]);
			// �J�����g�Ɣ�r���ăX�R�A���X�V���ꂽ��X�V����
			result->score = std::max( m_currResult.score, result->score );
		}

		// Query���p�[�X
		std::string query = m_query;
		int pos = query.find( ":" );
		if( pos != query.npos ) {
			query = query.substr( pos + 1 );
			query = trimstring( query.c_str() );
		}
		if( !query.empty() ) {
			bool doInsert = true;
			kome::massbank::MassBankManager::queryVector& tgt = result->queryArr;
			const double score = m_currResult.score;
			unsigned int cnt = tgt.size();
			// 10�Ƃ�������������̂ŁA����ɑ���
			if (cnt > 10 && tgt.at(cnt - 10).second > score) {
				doInsert = false;
			}
			// �X�R�A�������ŊǗ����Ė����P�O�������X�R�A�ɂȂ�l�Ɉ����B
			if ( doInsert ) {
				tgt.insert(
					std::lower_bound(tgt.begin(), tgt.end(), score, queryMore), 
					make_pair(query, score));
			}
		}
	}
}
