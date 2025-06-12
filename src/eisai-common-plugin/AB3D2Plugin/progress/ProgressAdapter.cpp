/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressAdpt.cpp
 @brief progress adapter
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
    �v���O���X�𐧌䂷��O���̃x�[�X�ł��B
 */
/*********************************************************/
#include "../stdafx.h"
#include "ProgressAdapter.h"

// memo
// progress�ɂ���

// progress�́@setRange�Ŕ͈͂����߂āA
// setPosition�ňʒu�𓮂����A
// �v���O���X�ׂ̈̃J�E���g��p�ӂ��A
// setStatus�Ń��b�Z�[�W��ς���K�v���L��܂��B
// �葱�����ώG�ŁA�R�[�h�̉ǐ���������܂��B
// �Ȃ̂ŁA�N���X�Ƃ��Ă܂Ƃ߂Ă���܂��B

// constructor
ProgressAdapter::ProgressAdapter(
	kome::core::Progress* _progress,
	const int lastCount
	) : progress_(_progress)
	, firstCount_(0)
	, lastCount_(lastCount)
{
//	initMe();
}

// constrctor
ProgressAdapter::ProgressAdapter(
	kome::core::Progress* _progress,
	const int firstCount,
	const int lastCount
	) : progress_(_progress)
	, firstCount_(firstCount)
	, lastCount_(lastCount)
{
//	initMe();
}

// �������B�p���N���X���ŌĂяo���B
void ProgressAdapter::initMe()
{
	if (progress_ != nullptr) {
		onInitMe();
	}
}

// init�ŌĂяo�����B
void ProgressAdapter::onInitMe() 
{
	onShowStatus("");
}

// �Œ胁�b�Z�[�W�Ńv���O���X��i�߂鎞�͂�������Ă�
const bool ProgressAdapter::nextStep()
{
	if(progress_ == nullptr) {
		return false;
	}

	nowCount_++;
	if (lastCount_ < nowCount_) {
		return false;
	}
	progress_->setPosition(nowCount_);
	return onNextStep();
}

// �v���O���X������i�߂�B
// ���b�Z�[�W��ύX����Ƃ��͂�������g��
const bool ProgressAdapter::nextStep(
	const std::string& msg
){
	if(progress_ == nullptr) {
		return false;
	}

	nowCount_++;

	if (lastCount_ < nowCount_) {
		return false;
	}
	
	progress_->setPosition(nowCount_);
	onShowStatus(msg);
	return true;
}


const bool ProgressAdapter::onNextStep()
{
	// �i�s�󋵂�\�����b�Z�[�W��
	onShowStatus("");
	return true;
}

// progress ���N���[�Y
void ProgressAdapter::closeMe()
{
	if (progress_ != nullptr) {
		onCloseMe();
		progress_->fill();
	}
}

void ProgressAdapter::onCloseMe()
{
	// �I�����̃��b�Z�[�W
	onShowStatus("");
}

void ProgressAdapter::onShowStatus(const std::string& msg)
{
	if (progress_ != nullptr) {
		progress_->setStatus(msg.c_str());
	}
}
