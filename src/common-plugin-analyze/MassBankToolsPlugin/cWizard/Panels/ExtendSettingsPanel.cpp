/**
 * @file ExtendSettingPanel
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "ExtendSettingsPanel.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


ExtendSettingsPanel::ExtendSettingsPanel (
	wxWindow* parent,
	kome::plugin::SettingsPage* page,
	const bool dfltBtn,
	kome::objects::SettingParameterValues* settings,
	const bool saveVal
	) : kome::window::SettingsPanel(parent, page, dfltBtn, settings, saveVal)
{
}

// �����̃t�H�[�����ƈ�v����t�H�[����T���A�h�c��Ԃ�
int ExtendSettingsPanel::onSearchFormID_fromName(const std::string& keyStr) {
	// input error
	if (keyStr.empty()) return -1;
	for each(const auto& tmp in m_forms) {
		if(keyStr.compare(tmp.form->getTitle()) == 0) {
			return tmp.id;
		}
	}
	return -1;
}

// �Ώۂ̃t�H�[�����̗v�f�̒l���擾����
std::string ExtendSettingsPanel::getValue(const std::string& formTitle) {
	if (formTitle.empty()) return "";
	int idx = onSearchFormID_fromName(formTitle);
	if(idx >= 0) {
		return this->getValueFromWindow(idx);
	}
	return "";
}


// �Ώۂ̃t�H�[�����̗v�f�ɒl�����蓖�Ă�
void ExtendSettingsPanel::setValue(
	const std::string& formTitle, 
	const char* str
) {
	int idx = onSearchFormID_fromName(formTitle);
	if(idx >= 0) {
		this->setValueFromString(idx, str);
	}
}

// �Ώۂ̃t�H�[�����̗v�f�ɒl�����蓖�Ă�
void ExtendSettingsPanel::setValue(
	const std::string& formTitle, 
	const std::string& str
) {
	int idx = onSearchFormID_fromName(formTitle);
	if(idx >= 0) {
		this->setValueFromString(idx, str.c_str());
	}
}
