#pragma once

#ifdef MAINCORE_LIBRARY
	#define MAINCORE_API __declspec(dllexport)
#else
	#define MAINCORE_API __declspec(dllimport)
#endif