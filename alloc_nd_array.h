/*
 * alloc_nd_array.h -- interface of a library that provides functions for allocating
 *                     multi-dimensional arrays that can be freed with a single free()
 *                     call
 * version 0.9.5, June 22, 2025
 *
 * License: zlib License
 *
 * Copyright (c) 2025 Kazushi Yamasaki
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

#ifndef ALLOC_ND_ARRAY_H
#define ALLOC_ND_ARRAY_H



#include "anda_macros.h"



ANDA_CPP_C_BEGIN



#include <stddef.h>
#include <stdbool.h>



/*
 * anda_errfunc is a global variable that stores the name of the function
 * where the most recent error occurred within this library.
 *
 * It is set to NULL when no error has occurred.
 * This variable is used to provide more informative error diagnostics,
 * especially in combination with errno.
 *
 * It is recommended to check this variable and errno after calling
 * any library function that may fail.
 */
#ifdef THREAD_LOCAL
	extern THREAD_LOCAL const char* anda_errfunc;
#else
	extern const char* anda_errfunc;
#endif



/*
 * alloc_nd_array
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @return: pointer to the multi-dimensional array or NULL on failure
 * @note: After calling, cast the returned pointer to the appropriate type (e.g., int***, double**, etc.) to access it as the multi-dimensional array. The allocated memory must be freed using free() when no longer needed. The returned memory is uninitialized (use calloc_nd_array if zero-initialization is desired).
 */
extern void* alloc_nd_array (const size_t sizes[], size_t dims, size_t elem_size);

/* A macro is available that automatically calculates the type size using sizeof(type).
 *
 * alloc_nd_array_t
 */
#define alloc_nd_array_t(sizes, dims, elem_type) \
	alloc_nd_array((sizes), (dims), sizeof(elem_type))


/*
 * calloc_nd_array
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @return: pointer to the multi-dimensional array or NULL on failure
 * @note: After calling, cast the returned pointer to the appropriate type (e.g., int***, double**, etc.) to access it as the multi-dimensional array. The allocated memory must be freed using free() when no longer needed. Use alloc_nd_array if zero-initialization is not required and higher performance is desired.
 */
extern void* calloc_nd_array (const size_t sizes[], size_t dims, size_t elem_size);

/* A macro is available that automatically calculates the type size using sizeof(type).
 *
 * calloc_nd_array_t
 */
#define calloc_nd_array_t(sizes, dims, elem_type) \
	calloc_nd_array((sizes), (dims), sizeof(elem_type))


/*
 * free_nd_array
 * @param array: pointer to the multi-dimensional array allocated by alloc_nd_array or calloc_nd_array
 * @note: this function frees the multi-dimensional array (the actual memory block allocated for it)
 */
extern void free_nd_array (void* array);


/*
 * calculate_nd_array_size
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @param result_ptrs_size: pointer to store the size of the pointer array (not including padding)
 * @param result_padding_size: pointer to store the size of the padding
 * @param result_total_elements: pointer to store the total number of elements in the array
 * @return: true if the size was successfully calculated, false if an error occurred (e.g., zero dimensions or element size)
 * @note: This function calculates the size of the multi-dimensional array without allocating memory. It is useful for checking if the requested sizes and element size are valid before actual allocation.
 */
extern bool calculate_nd_array_size (const size_t sizes[], size_t dims, size_t elem_size, size_t* result_ptrs_size, size_t* result_padding_size, size_t* result_total_elements);

/* A macro is available that automatically calculates the type size using sizeof(type).
 *
 * calculate_nd_array_size_t
 */
#define calculate_nd_array_size_t(sizes, dims, elem_type, result_ptrs_size, result_padding_size, result_total_elements) \
	calculate_nd_array_size((sizes), (dims), sizeof(elem_type), (result_ptrs_size), (result_padding_size), (result_total_elements))



/*
 * The following function is not part of this library's original purpose, but we ended up
 * creating one that is generally useful during development, so we've decided to make it
 * publicly available.
 */


/*
 * anda_align_up
 * @param value: the value to align
 * @param alignment: the alignment value
 * @return: the aligned value, or 0 if an error occurred (e.g., alignment is zero or causes overflow)
 * @note: this function aligns the given value up to multiple of the specified alignment
 */
extern size_t anda_align_up (size_t value, size_t alignment);



ANDA_CPP_C_END



#endif
