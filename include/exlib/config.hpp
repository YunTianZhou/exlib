#pragma once

#pragma warning(disable : 4251 4275)

#ifndef EXLIB_STATIC
	#if defined(_WIN32)
		#ifdef EXLIB_EXPORTS
			#define EXLIB_API __declspec(dllexport)
		#else
			#define EXLIB_API __declspec(dllimport)
		#endif
	#elif defined(__GNUC__)
		#define EXLIB_API __attribute__((visibility("default")))
	#else
		#define EXLIB_API
	#endif
#else
	#define EXLIB_API
#endif
