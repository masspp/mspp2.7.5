/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2015.07.01
 * 
 * Copyright(C) 2015 Mass++ All rights reserved.
 */


#ifndef __KOME_THIRDPARTY_OPENMS_STD_AFX_H__
#define __KOME_THIRDPARTY_OPENMS_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// peak detect namespace 
	namespace thirdparty {
		/// Local Maximum Labeling plug-in namespace
		namespace openms {
		}
	}
}


//
// macros
//

#define WXUSINGDLL


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4190 )
	#pragma warning( disable:4244 )
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
	#pragma warning( disable:4819 )
	#pragma warning( disable:4996 )
#endif	// _MSC_VER


//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "CoreFunctions" )
	#pragma comment( lib, "CoreToolkits" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "PluginTools" )
#endif	// _MSC_VER



//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/PluginTools.h"


#endif	// __KOME_THIRDPARTY_OPENMS_STD_AFX_H__
