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

#pragma once

#include <string>
#include <vector>

namespace kome {
	namespace massbank {
		namespace wizard {

			//-----------------------------------------------
			//  DB Access Class
			//-----------------------------------------------
			class DBAccClass {
				// DB Links�̏��
				struct DBLinks {
					std::string dbName;  //DB ��(CAS�Ƃ��j
					std::string URLHead; //URL�̐擪�� (ACCSESSION�̑O)
					std::string URLTail; //URL�̌㔼�� (ACCSESSION�̌�)
					std::string URL;     //�f��URL

					// constructor
					DBLinks(
						const std::string& d,
						const std::string& h,
						const std::string& t
						) : dbName(d), URLHead(h), URLTail(t){}
					
					void setURL(const std::string& acc) {
						URL = URLHead + acc + URLTail;
					}
				};
			public:
				// constructor
				DBAccClass();
				// destructor
				~DBAccClass(){}

			private:
				// DB Link�̍\���̌Q
				std::vector<DBLinks> DBLArr_;
				std::vector<kome::ident::ExternalDB*> extDbs_;
				void init();

				std::string searchedKeyword_;

			public:
				// �i�[���Ă���DB Link�̏�񐔂�Ԃ�
				unsigned int getMyNum()const{return DBLArr_.size();}

				// �Ώ�ID��DB����Ԃ��iString)
				std::string getNames(const unsigned int idx) const;

				// �Ώ�ID��DB����Ԃ��iconst char*)
				const char* getName(const unsigned int idx) const;

				// demo URL��Ԃ�(String)
				std::string getsURL(const unsigned int idx) const;

				// demo URL��Ԃ�
				const char* getURL(const unsigned int idx) const;

				// �{�̃p�l������ DB List�̒l���󂯎��B
				const std::string setListArrStrs(const std::string& listStr);

				const bool callSearchKeyWordtoExDB(
					std::string& searchedKeyword,
					std::vector<std::string>& results);

				const bool getKeyWordSearchResults(
					std::string& rstName,
					std::string& linksVal,
					std::string& accURL
					);

			private:
				const std::string getURL(
					const std::string& exdb,
					const std::string& accession);

			};

		}
	}
}
