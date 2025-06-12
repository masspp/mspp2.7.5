/*********************************************************/
/*!
 @addtoGroup AB3D
 @file progressAB3D.cpp
 @brief progress adapter (for AB3D)
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/

#include "../stdafx.h"
#include "progressAB3D.h"

// AB3D�̐i�s�󋵂̃v���O���X�ł��B
// Detector�̐i�s�󋵃��b�Z�[�W�A�p�x����
// onNextStep����K�X���������Ă��������B
// �p�x�����߂���Ƃ��ꎩ�̂��{�g���l�b�N�ɂȂ�܂��B
// �����I�ɁA30�J�E���g�ŗp�ӂ��Ă���܂��B
// nextstep���ĂԂ��тɁA�P�J�E���g�������܂��B
// PeakDetector����25�J�E���g
// ����ȊO�̌ʂ̃t�F�[�Y��5��Ă΂�Ă��܂��B

// Close(Fill)���ĂԂƕs�����͕⊮���ăN���[�Y���Ă���܂��B
// �͈͂��I�[�o�[����Ɓ@������̂ň����ɒ��ӂ��ĉ������B

AB3DProg::AB3DProg(
	kome::core::Progress* _progress,
	const int lastcount
	) : ProgressAdapter(_progress, lastcount)
{
	this->initMe();
}

void AB3DProg::onInitMe()
{
	nowCount_ = firstCount_;
	// Progress�͈̔͂ł��BsetPosition�ŃQ�[�W�ʒu���ς��܂��B
	progress_->setRange(firstCount_, lastCount_);
	progress_->setPosition(firstCount_);
}

const bool AB3DProg::onNextStep()
{
	std::string msg = FMT("AB3D Peak Detect.. %02d", nowCount_ * 4);
	msg += "%";
	onShowStatus(msg);
	return true;
}

void AB3DProg::onCloseMe()
{
}
