/**
 * @file OpenMSFunctions.h
 * @brief interfaces of OpenMS plug-in functions
 *
 * @author S.Tanaka
 * @date 2015.07.01
 * 
 * Copyright(C) 2015 Mass++ All rights reserved.
 */

#ifndef __KOME_THIRDPARTY_OPENMS_FUNCTIONS_H__
#define __KOME_THIRDPARTY_OPENMS_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant detectPeaksByWavelet( kome::objects::Parameters* params )
 * @brief detects peaks by local peak picking algorithm
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant detectPeaksByWavelet( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant detectPeaksByHiRes( kome::objects::Parameters* params )
 * @brief detects peaks by local peak picking algorithm
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant detectPeaksByHiRes( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix


#endif // __KOME_THIRDPARTY_OPENMS_FUNCTIONS_H__
