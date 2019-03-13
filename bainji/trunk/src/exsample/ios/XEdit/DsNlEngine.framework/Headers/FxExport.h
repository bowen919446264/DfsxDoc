#pragma once
#ifdef _MSC_VER
#include <assert.h>
#include <InitGuid.h>

#if defined(EXPORT_FXCORE)
#	define LIBFX_FUNC  __declspec ( dllexport )
#	define LIBFX_DATA  __declspec ( dllexport )
#	define LIBFX_CLASS __declspec ( dllexport )
#else 
#	define LIBFX_FUNC  __declspec ( dllimport )
#	define LIBFX_DATA  __declspec ( dllimport )
#	define LIBFX_CLASS __declspec ( dllimport )
#endif
#elif __GNUC__
#	define LIBFX_FUNC  
#	define LIBFX_DATA  
#	define LIBFX_CLASS 
#endif
//compilation configuration
//#define		MATROX_SUPPORT