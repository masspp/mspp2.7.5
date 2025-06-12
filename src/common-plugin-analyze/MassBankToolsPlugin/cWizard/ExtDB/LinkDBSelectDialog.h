/**
 * @file DBACCSelectDialog.h
 * @brief DB Access Select Dialog
 *
 * @author M.Fukuda
 * @date 2013.03.06
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include "../rel_stdafx.h"

namespace kome {
	namespace massbank {
		namespace wizard {
			/**
				DBLink�̒l�iDB��Accession�j�̒l���󂯎��������
				DB: Accession �ƂȂ�悤�Ɂ@�R���{�{�b�N�X��\���B
				�I�΂ꂽ���Ԃ��L�^���ĕԂ��B
			*/
			class DBACCSelectDialog : public kome::window::StandardDialog {
			public:
				// constructor
				DBACCSelectDialog(wxWindow* parent, std::vector<std::pair<std::string, std::string>>& dbarr);

				// destructor
				virtual ~DBACCSelectDialog();

				// get Selected ID
				int getSelectId() const {return selectId_;}

			protected:
				// create main sizer
				virtual wxSizer* createMainSizer();
				// on Combobox Select Evenet
				void onSelectComBoBox(wxCommandEvent& evt);

			private:
				//
				std::vector<std::pair<std::string, std::string>>& dbArr_;
				int selectId_;

				DECLARE_EVENT_TABLE();
			};
		} // end of namespace
	}
}
