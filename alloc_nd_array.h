/*
 * alloc_nd_array.h -- interface of a library that provides functions for allocating
 *                     multi-dimensional arrays that can be freed with a single free()
 *                     call
 * version 0.9.1, June 12, 2025
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


#ifndef ANDA_CPP_C_BEGIN
	#ifdef __cplusplus  /* C++ */
		#define ANDA_CPP_C_BEGIN extern "C" {
		#define ANDA_CPP_C_END }
	#else               /* not C++ */
		#define ANDA_CPP_C_BEGIN
		#define ANDA_CPP_C_END
	#endif
#endif


ANDA_CPP_C_BEGIN


#include <stddef.h>
#include <stdbool.h>


/*
 * alloc_nd_array
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @return: pointer to the multi-dimensional array or NULL on failure
 * @note: After calling, cast the returned pointer to the appropriate type (e.g., int***, double**, etc.) to access it as the multi-dimensional array. The allocated memory must be freed using free() when no longer needed. The returned memory is uninitialized (use calloc_nd_array if zero-initialization is desired).
 */
extern void* alloc_nd_array (const size_t sizes[], size_t dims, size_t elem_size);

/*
 * calloc_nd_array
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @return: pointer to the multi-dimensional array or NULL on failure
 * @note: After calling, cast the returned pointer to the appropriate type (e.g., int***, double**, etc.) to access it as the multi-dimensional array. The allocated memory must be freed using free() when no longer needed. Use alloc_nd_array if zero-initialization is not required and higher performance is desired.
 */
extern void* calloc_nd_array (const size_t sizes[], size_t dims, size_t elem_size);

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


/*
 * The following function is provided for special use cases where it is necessary to
 * separate the size calculation and the allocation of a multidimensional array.
 * Its usage is complex and prone to errors, so it is generally not recommended for
 * regular use.
 */


typedef void* (*allocFuncPtr)(size_t);

/*
 * allocate_and_initialize_nd_array
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @param size_ptrs: size of the pointer array (not including padding)
 * @param size_padding: size of the padding
 * @param total_elements: total number of elements in the array
 * @param alloc_func: function pointer to the memory allocation function (e.g., malloc, calloc_wrapper)
 * @return: pointer to the allocated and initialized multi-dimensional array or NULL on failure
 */
extern void* allocate_and_initialize_nd_array (const size_t sizes[], size_t dims, size_t elem_size, size_t size_ptrs, size_t size_padding, size_t total_elements, allocFuncPtr alloc_func);


ANDA_CPP_C_END


#endif
