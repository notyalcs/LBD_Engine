#pragma once

#ifdef LBD_BUILD_DLL
	#define LBD_API __declspec(dllexport)
#else
	#define LBD_API __declspec(dllimport)
#endif
