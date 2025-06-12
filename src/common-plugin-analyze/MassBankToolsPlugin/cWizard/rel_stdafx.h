/**
 * @file rel_stdafx.h
 * @brief for stdafx.h's relative path
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

// �t�H���_�\�����قȂ�̂ŁA
// stdafx �̈ʒu�֌W���ς��ƑS�����������ɂȂ�B
// ���Έʒu��������ŊǗ�����B
#pragma once
#include "../stdafx.h"

//define �ŋ��ʔF�������ׂ����ڂ͂�����
#define BORDER_SIZE 3
#define GRID_GAP    3

/**
Help, ��^���͋ɗ͈ꂩ���ɓZ�߂�B

Wizard��1�y�[�W�ɂ��AWizardPage,��{Panel�������� .h, .cpp�ō\������B

kome::window::StandardWizardPage�@���p���������̂��@Wizard�̍��q�Ƃ���B
����ɂ́A�摜�ƁAParametersAndHelpPanel�̎q�𒣂�t���邾���ɂ���

ParametersAndHelpPanel���p������Panel�Ƃ��č��B
�����ɋɗ͂��ׂĂ��������ށB
���G�ȏ������K�v�ɂȂ�p�l���́@���̕��������@�Ɨ��������p�l���Ƃ��Ē�`����B
(Panels�̃t�H���_���ɓ����j

���炩�̎����setDesc���g���Ȃ����AHelp���K�v�ȍ��ڂ́A
kome::window::HelpGetterPanel���p�������p�l�����쐬���āA
�K�v�ɉ����ĕ��������œ����B
*/

wxBitmap getWizImg(const wxString& imgPath);
wxBitmap getWizImg(const std::string& imgPath);
