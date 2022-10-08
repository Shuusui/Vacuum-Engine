#pragma once

#ifdef RENDERER_LIBRARY
	#define RENDERER_API __declspec(dllexport)
#else
	#define RENDERER_API __declspec(dllimport)
#endif