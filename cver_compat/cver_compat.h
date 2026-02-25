/*
 * cver_compat.h -- a header file defining wrapper macros for C keywords
 * version 0.9.0, Feb. 25, 2026
 *
 * License: zlib License
 *
 * Copyright (c) 2026 Kazushi Yamasaki
 *
 * This software is provided ‘as-is’, without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#pragma once

#ifndef CVER_COMPAT_H
#define CVER_COMPAT_H


#if defined(__GNUC__) && !defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-macros"
#endif


#if !defined(PTR_NULL)
	/* C/C++ versions where nullptr is guaranteed to be available */
	#if (defined(__cplusplus) && ((__cplusplus >= 201103L) || \
		(defined(_MSVC_LANG) && (_MSVC_LANG >= 201103L)) \
	)) || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L))
		#define PTR_NULL nullptr

	/* C++ */
	#elif defined(__cplusplus)
		#define PTR_NULL 0

	/* C */
	#else
		#define PTR_NULL ((void*)0)
	#endif
#endif


#if defined(__GNUC__) && !defined(__clang__)
	#pragma GCC diagnostic pop  /* -Wunused-macros */
#endif


#endif  /* CVER_COMPAT_H */
