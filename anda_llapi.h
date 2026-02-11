/*
 * anda_llapi.h -- interface for alloc_nd_array's low-level API group
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

#ifndef ANDA_LLAPI_H
#define ANDA_LLAPI_H



#include "anda_macros.h"



ANDA_CPP_C_BEGIN



#include "alloc_nd_array.h"

#include <stddef.h>
#include <stdbool.h>



/*
 * These functions allow manual specification of padding, potentially improving memory
 * efficiency compared to automatic padding.
 *
 * However, incorrect usage may lead to alignment violations and undefined behavior.
 *
 * Use these APIs with extreme caution and ensure proper understanding of memory
 * alignment requirements.
 */



/*
 * alloc_nd_array_manual_padding
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param padding_bytes: size of the padding in bytes (combined size of pointer array and padding must be alignment multiple)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @return: pointer to the multi-dimensional array or NULL on failure
 * @note: After calling, cast the returned pointer to the appropriate type (e.g., int***, double**, etc.) to access it as the multi-dimensional array. The allocated memory must be freed using free() when no longer needed. The returned memory is uninitialized (use calloc_nd_array_manual_padding if zero-initialization is desired).
 */
extern void* alloc_nd_array_manual_padding (const size_t sizes[], size_t dims, size_t padding_bytes, size_t elem_size);

/* A macro is available that automatically calculates the type size using sizeof(type). */
#define alloc_nd_array_manual_padding_t (sizes, dims, padding_bytes, elem_type) \
	alloc_nd_array_manual_padding ((sizes), (dims), (padding_bytes), sizeof(elem_type))


/*
 * calloc_nd_array_manual_padding
 * @param sizes: array containing sizes for each dimension (must have length equal to dims)
 * @param dims: number of array dimensions (designed for 2+ dimensions but supports 1D arrays)
 * @param padding_bytes: size of the padding in bytes (combined size of pointer array and padding must be alignment multiple)
 * @param elem_size: size of each element in bytes (e.g., sizeof(int), sizeof(double), etc.)
 * @return: pointer to the multi-dimensional array or NULL on failure
 * @note: After calling, cast the returned pointer to the appropriate type (e.g., int***, double**, etc.) to access it as the multi-dimensional array. The allocated memory must be freed using free() when no longer needed. Use alloc_nd_array_manual_padding if zero-initialization is not required and higher performance is desired.
 */
void* calloc_nd_array_manual_padding (const size_t sizes[], size_t dims, size_t padding_bytes, size_t elem_size);

/* A macro is available that automatically calculates the type size using sizeof(type). */
#define calloc_nd_array_manual_padding_t (sizes, dims, padding_bytes, elem_type) \
	calloc_nd_array_manual_padding ((sizes), (dims), (padding_bytes), sizeof(elem_type))



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

/* A macro is available that automatically calculates the type size using sizeof(type). */
#define allocate_and_initialize_nd_array_t (sizes, dims, elem_type, size_ptrs, size_padding, total_elements, alloc_func) \
	allocate_and_initialize_nd_array ((sizes), (dims), sizeof(elem_type), (size_ptrs), (size_padding), (total_elements), (alloc_func))



ANDA_CPP_C_END



#endif
