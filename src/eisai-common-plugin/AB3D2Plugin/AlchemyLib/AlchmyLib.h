/**********************************************************
Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
    @addtogroup Mass++ Plugins Support Lib
	@file       AlchemyLib.h
	@brief      AlchemyLib ���ʂ̃w�b�_�B
	@date       2012.12.06
	@author     M.Fukuda
	@version    1.0.0
    @note       �]�v�Ȓ�`�͓���Ȃ��B
**********************************************************/
#pragma once


namespace ALC {
	enum CHROM_FLAG {
		NO_USE = 0,         // Chrom �L���̔���Ȃ� 
		HAS_CHROM = 1,      // Chrom ������
		NOT_HAS_CHROM = -1, // Chrom �������Ȃ��i�ʏ�͎g��Ȃ��j
		// -------------------
		CHROM_FLAG_NUM,
	};
} // end of namespace ALC
