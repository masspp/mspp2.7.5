/**
 * @file ExtendSettingPanel
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * @ note -=====================================
 *  SettingsPanel���p�����āAgetValueFromWindow������I��
 *  �g�p�ł���悤�ɂ����N���X�B�t�H�[������n���ƁA�l���Ԃ����
 */

#pragma once

#include <vector>

// Public �ɂ��Ĕėp������B
namespace kome {
	namespace massbank {
		namespace wizard {

			class ExtendSettingsPanel : public kome::window::SettingsPanel {
			public:
				// constructor
				ExtendSettingsPanel (
					wxWindow* parent,
					kome::plugin::SettingsPage* page,
					const bool dfltBtn = true,
					kome::objects::SettingParameterValues* settings = NULL,
					const bool saveVal = true
					);

				virtual ~ExtendSettingsPanel(){}

			private:

				// �t�H�[��������t�H�[���̂h�c����������
				int onSearchFormID_fromName(const std::string& keyStr);

			public:

				// �Ώۂ̃t�H�[�����̗v�f���擾����
				std::string getValue(const std::string& formTitle);

				// �Ώۂ̃t�H�[�����̗v�f�ɒl�����蓖�Ă�
				void setValue(
					const std::string& formTitle,
					const char* str
					);

				// �Ώۂ̃t�H�[�����̗v�f�ɒl�����蓖�Ă�
				void setValue(
					const std::string& formTitle, 
					const std::string& str
					);
			};
		}
	}
}
