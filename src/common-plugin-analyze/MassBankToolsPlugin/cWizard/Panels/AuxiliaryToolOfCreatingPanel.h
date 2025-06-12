/**
 * @file CreatePanelToolOfCreatePanel.h
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <vector>

namespace pfc {
//��^��
	const int knNoUse(-INT_MAX);
	const double kdNoUse(-DBL_MAX);

// ���ʂ���@�w�b�_���A���C�����̍쐬���s��
// param[in] parent    �eWindow
// param[in] addrownum ���C���̌�ɒǉ������v�f���B
// param[in] headLabel �w�b�_���x���̓��e
// ����Sizer�͍Œ�ł��Q�i�w�b�_���x���ƃ��C���j���m�ۂ��Ă��āA
// �X�ɒǉ�����`�ɂȂ�
wxFlexGridSizer* createHeaders(
	wxWindow* parent,
	const int addrownum,
	const wxString& headLabel
	);

//SettingsParam�ւ̒�`�����쐬����
//Param[in] sName  : �p�����[�^�̖��O�B
//Param[in] holdfg : MassBank Section & sName�̃A�T�C��������
kome::plugin::SettingsParam* onSetParamAdpt (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	const std::string& sName,
	bool holdfg = false);

void addEnumItem(	
	kome::plugin::SettingsValue*& val,
	const std::string& str1
	);

//SettingsForm�ւ̒�`�����쐬����
//Param[in] sTitle  : �t�H�[����
//Param[in] sDesc   : �t�H�[���̐�����
kome::plugin::SettingsForm* onSetFormAdpt(
	kome::plugin::SettingsPage*& page,
	kome::plugin::SettingsParam*& param,
	const std::string& sTitle,
	const std::string& sDesc);

//ParameterSettings�ցAval,page�������Ɋ���t����B
//  Param[in] inParams : param�̌��ɂȂ�param
//  Param[in] val      : param�Ɋ��蓖�Ă�val
//  Param[in] page     : form�̌��ɂȂ�page
//  Param[in] sName    : �p�����[�^�̖��O
//  Param[in] sTitle   : �t�H�[����
//  Param[in] sDesc    : �t�H�[���̐�����
//  Param[in] holdfg   : �ۑ��p��MassBank Section����邩�ۂ�
void setParamAndFormAdpt (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	kome::plugin::SettingsPage*& page,
	const std::string& sName,
	const std::string& sTitle,
	const std::string& sDesc,
	bool holdfg = false);

//ParameterSettings�ցAval,page�������Ɋ���t����B
//  Param[in] inParams : param�̌��ɂȂ�param
//  Param[in] val      : param�Ɋ��蓖�Ă�val
//  Param[in] page     : form�̌��ɂȂ�page
//  Param[in] sgroup   : form�̑�����O���[�v
//  Param[in] sName    : �p�����[�^�̖��O
//  Param[in] sTitle   : �t�H�[����
//  Param[in] sDesc    : �t�H�[���̐�����
//  Param[in] holdfg   : �ۑ��p��MassBank Section����邩�ۂ�
void setParamAndFormAdptGroup (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	kome::plugin::SettingsPage*& page,
	kome::plugin::SettingsGroup* sgroup,
	const std::string& sName,
	const std::string& sTitle,
	const std::string& sDesc,
	bool holdfg = false);

// SubTag & Value �́@ListCtrl�� Value�����쐬����B
kome::plugin::SettingsValue* setSubTagListValue (
	kome::plugin::ParameterSettings& inParams,
	const std::vector<std::string>& subtagArr,
	const std::vector<std::string>* subtagNGArr = nullptr);

kome::plugin::SettingsValue* setListChildStringValue (
	kome::plugin::ParameterSettings& inParams,
	const std::string& childname,
	const std::vector<std::string>* childval,
	const bool required,
	const bool hasChecked = false
	);

// String �� Value �������쐬����
//  Param[in] inParams : param�̌��ɂȂ�param
//  Param[in] def      : �f�t�H���g�l�B�g��Ȃ��Ƃ��� ""
//  Param[in] reqFlg   : ���͕K�{���ڂ��H Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createStringSTValue(
	kome::plugin::ParameterSettings& params,
	const std::string& def,
	const bool reqFlg);

// Integer �� Value �������쐬����
//  Param[in] inParams : param�̌��ɂȂ�param
//  Param[in] nMin     : �ŏ��l�B�g��Ȃ��ꍇ�� knNoUse.
//  Param[in] nMax     : �ő�l�B�g��Ȃ��ꍇ�� knNoUse.
//  Param[in] def      : �f�t�H���g�l�B�g��Ȃ��Ƃ��� ""
//  Param[in] reqFlg   : ���͕K�{���ڂ��H Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createIntegerSTValue(
	kome::plugin::ParameterSettings& params,
	const int nMin,
	const int nMax, 
	const std::string& def,
	const bool reqFlg);

// Double �� Value �������쐬����
//  Param[in] inParams : param�̌��ɂȂ�param
//  Param[in] dMin     : �ŏ��l�B�g��Ȃ��ꍇ�� kdNoUse.
//  Param[in] dMax     : �ő�l�B�g��Ȃ��ꍇ�� kdNoUse.
//  Param[in] def      : �f�t�H���g�l�B�g��Ȃ��Ƃ��� ""
//  Param[in] reqFlg   : ���͕K�{���ڂ��H Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createDoubleSTValue(
	kome::plugin::ParameterSettings& params,
	const double dmin,
	const double dMax,
	const std::string& def,
	const bool reqFlg);

}
