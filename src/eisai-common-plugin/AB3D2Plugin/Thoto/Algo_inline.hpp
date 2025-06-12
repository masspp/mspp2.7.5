/**********************************************************
Copyright(C) 2006-2012 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
   @addtogroup C++ CommonFunctions
	@file       Algo_inline.h
	@brief      Lib�֐��Œx���������ߎ��A���S���Y���ō�����
	@date       2012.12.05
	@author     M.Fukuda
	@version    1.0.0
    @note       Boost���A�O�����C�u�����Ɋւ�鏈���͓���Ȃ��B
**********************************************************/


//	std::sqrt�̌v�Z���x���̂�
//	�j���[�g���@���g�����ߎ��A���S���Y���Ōv�Z
//
//	�匳�̕���
//	http://www.riken.jp/brict/Ijiri/study/fastsqrt.html
//
//	�X�ɂ��̌��̘_��
//		[1] David Eberly, Fast Inverse Square Root (Revisited), 
//			http://www.geometrictools.com/Documentation/ FastInverseSqrt.pdf, 1/2002-.
//
//  �C�e���[�V�����Q��Ł@
//    std::sqrt�Ə����_��6�ʂ܂� ���� ���{�ȏ㑬�x���o��B
//  �C�e���[�V����3���
//    std::sqrt�Ə����_��8-9�ʂ܂� ���� ����std::sqrt��葬��
//  �덷�����e�ł��đ��x���K�v�Ȃ獟���𐄏�

// ����������Ă���@sqrt
inline double t_sqrtD(const double &x) {
	double xHalf = 0.5 * x;
	long long int tmp = 0x5FE6EB50C7B537AAl - ( *(long long int*)&x >> 1);
	double xRes = * (double*)&tmp;

	//�C�e���[�V����
	xRes *= (1.5 - (xHalf * xRes * xRes));
	xRes *= (1.5 - (xHalf * xRes * xRes));
	return xRes * x;
}
