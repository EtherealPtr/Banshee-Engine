#pragma once

#ifdef BANSHEE_EXPORTS
	#define BANSHEE_ENGINE __declspec(dllexport)
#else
	#define BANSHEE_ENGINE __declspec(dllimport)
#endif