#pragma once

#ifdef CORESYSTEMS_LIBRARY
	#define CORESYSTEMS_API __declspec(dllexport)
#else 
	#define CORESYSTEMS_API __declspec(dllimport)
#endif