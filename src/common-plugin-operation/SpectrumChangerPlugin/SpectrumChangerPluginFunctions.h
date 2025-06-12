/**
 * @file SpectrumChangerPluginFunctions.h
 * @brief implementation of plug-in functions
 *
 * @author okada_h
 * @date 2010.09.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifdef __unix
	extern "C" {
#endif  // __unix

#ifndef __KOME_SPECTRUM_CHANGERPLUGIN_H__
#define __KOME_SPECTRUM_CHANGERPLUGIN_H__

// �v���g�^�C�v�錾
/**
 * @fn kome::objects::Variant openNextSpectrum( kome::objects::Parameters* params )
 * @brief open next spectrum
 * @param params params
 * @return ������ true, ���s�� false
 */
kome::objects::Variant openNextSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openPrevSpectrum( kome::objects::Parameters* params )
 * @brief open prev spectrum
 * @param params params
 * @return ������ true, ���s�� false
 */
kome::objects::Variant openPrevSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openNextPrevSpectrum( kome::objects::Parameters* params, int flgPrevNext )
 * @brief open next prevSpectrum
 * @param params params
 * @param flgPrevNext 0:prev 1:next
 * @return �߂�l (������ true, ���s�� false)
 */
kome::objects::Variant openNextPrevSpectrum( kome::objects::Parameters* params, int flgPrevNext );

/**
 * @fn kome::objects::Variant onSpecKeyDownX( kome::objects::Parameters* params );
 * @brief on spec key down
 * @param params params
 */
kome::objects::Variant onSpecKeyDownX( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant enableNextSpectrum( kome::objects::Parameters* params );
 * @brief enable next spectrum
 * @param params params
 * @return true:�I�𒆂̃X�y�N�g��������
 */
kome::objects::Variant enableNextSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant enablePrevSpectrum( kome::objects::Parameters* params );
 * @brief enable prev spectrum
 * @param params params
 * @return true:�I�𒆂̃X�y�N�g�����擪
 */
kome::objects::Variant enablePrevSpectrum( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_SPECTRUM_CHANGERPLUGIN_H__
