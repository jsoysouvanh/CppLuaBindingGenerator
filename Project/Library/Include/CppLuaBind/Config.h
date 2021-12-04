/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the CppLuaBind library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

//Version
#define CPPLUABIND_VERSION_MAJOR 0
#define CPPLUABIND_VERSION_MINOR 1
#define CPPLUABIND_VERSION_PATCH 0
#define CPPLUABIND_VERSION (CPPLUABIND_VERSION_MAJOR * 10000 + CPPLUABIND_VERSION_MINOR * 100 + CPPLUABIND_VERSION_PATCH)

//Dynamic library import/export
#if defined(KODGEN_PARSING)

	#define CPPLUABIND_API
	#define CPPLUABIND_INTERNAL

#elif defined(_WIN32) || defined(__CYGWIN__)

	#if defined(CPPLUABIND_EXPORT)

		#if defined(__GNUC__)
			#define CPPLUABIND_API	__attribute__((dllexport))
		#else
			#define CPPLUABIND_API	__declspec(dllexport)
		#endif

	#else

		#if defined(__GNUC__)
			#define CPPLUABIND_API __attribute__((dllimport))
		#else
			#define CPPLUABIND_API __declspec(dllimport)
		#endif

	#endif

	#define CPPLUABIND_INTERNAL

#else

	#if __GNUC__ >= 4

		#if defined(CPPLUABIND_EXPORT)
			#define CPPLUABIND_API		__attribute__((visibility("default")))
			#define CPPLUABIND_INTERNAL	__attribute__((visibility("hidden")))
		#else
			#define CPPLUABIND_API
			#define CPPLUABIND_INTERNAL
		#endif

	#else
		#define CPPLUABIND_API
		#define CPPLUABIND_INTERNAL
	#endif

#endif