#pragma once
#ifdef _MSC_VER
#include <assert.h>
#if defined(EXPORT_LIBNLE)
#	define LIBNLE_FUNC  __declspec ( dllexport )
#	define LIBNLE_DATA  __declspec ( dllexport )
#	define LIBNLE_CLASS __declspec ( dllexport )
#else 
#	define LIBNLE_FUNC  __declspec ( dllimport )
#	define LIBNLE_DATA  __declspec ( dllimport )
#	define LIBNLE_CLASS __declspec ( dllimport )
#endif
#elif __GNUC__
#	define LIBNLE_FUNC 
#	define LIBNLE_DATA  
#	define LIBNLE_CLASS 
#endif

