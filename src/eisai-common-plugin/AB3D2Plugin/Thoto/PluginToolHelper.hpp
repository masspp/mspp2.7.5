///****************************************************************
//  Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
//-----------------------------------------------------------------
//    @addtogroup Mspp PluginCall helper headers
//    @file       PluginCallHelper.hpp
//	@brief      PluginCall���ꂽ��̎葱����Z�߂��֐��Q
//	@date       2012.12.13
//	@author     M.Fukuda
//	@version    1.0.0
//****************************************************************/
//#pragma once
//
//// stdafx_mininumSet���ɉ��L�C���N���[�h�K�[�h������
//#ifndef __NO_NEED_EMS_MINIMUN_STFAFX_H__
//#include "./Mspp/stdafx_mininumSet.h"
//#endif
//
///* -- How to Use --
// * �����t�@�N�g���[�֐��i�l�́j�^
//Ex)
//#define __NO_NEED_EMS_MINIMUN_STFAFX_H__
//#define __CALLTYPE_PEAKPICKING__
//#include "../Z_Horus_Plugin/hpp/PluginToolHelper.hpp"
//
// * 1. EMS/Mass++�̃��C�u�������Փ˂���ꍇ�͑O��
//      #define __NO_NEED_EMS_MINIMUN_STFAFX_H__
//	  �Ə���
// * 2. �Ăяo�������R�[���^�C�v�� define�Ő錾����
//      #define __CALLTYPE_*CALL*TYPE*NAME*__
//      �Ή�����w�b�_�݂̂��Ă΂��B
// * 3. ������C���N���[�h����B
// * 4. RFN::WRAP::xxxxxx�̗l�ɌĂяo���B
// */
//
//#ifdef __CALLTYPE_PEAKPICKING__
//#include "./hpp/Peak_Picking.hpp"
//#endif
//
#ifdef __CALLTYPE_2DPEAKPICKING__
#include "./hpp/Check2DPeakPicking.hpp"
#endif
//
//#ifdef __CALLTYPE_2DPEAKPOSITION__
////#include "./hpp/CheckPeakPositions.hpp"
//#endif