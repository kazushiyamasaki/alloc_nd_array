/*
 * alloc_nd_array.c -- implementation part of a library that provides functions for
 *                     allocating multi-dimensional arrays that can be freed with a
 *                     single free() call
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

#include "alloc_nd_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#undef malloc
#undef calloc
#undef free


#if !defined (__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
	#error "This program requires C99 or higher."
#endif


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-macros"

	#define LIKELY(x)   __builtin_expect(!!(x), 1)
	#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#pragma GCC diagnostic pop
#else
	#define LIKELY(x)   (x)
	#define UNLIKELY(x) (x)
#endif


static void* calloc_wrapper (size_t size) {
	return calloc(1, size);
}


static size_t align_up (size_t value, size_t alignment) {
	if (alignment == 0 || alignment == 1) return value;

	if (value > (SIZE_MAX - (alignment - 1))) return 0;  /* オーバーフロー対策 */

	if ((alignment & (alignment - 1)) == 0) {  /* alignment が2のべき乗ならビット演算 */
		return (value + (alignment - 1)) & ~(alignment - 1);
	}
	return ((value + (alignment - 1)) / alignment) * alignment;  /* それ以外なら割り算 */
}


/* 注意: *result_ptrs_size は *result_padding_size を「含まない」ので注意が必要 */
bool calculate_nd_array_size (const size_t sizes[], size_t dims, size_t elem_size, size_t* result_ptrs_size, size_t* result_padding_size, size_t* result_total_elements) {
	*result_ptrs_size = 0;
	*result_padding_size = 0;
	*result_total_elements = 1;

	if (elem_size == 0 || dims == 0) return false;

	if (dims == 1) {  /* 1次元 (ただの配列) の場合はそのまま掛け算でサイズを求めて返却 */
		if (sizes[0] == 0 || sizes[0] > (SIZE_MAX / elem_size))
			return false;

		*result_total_elements = sizes[0];
		return true;
	}

	/* 総要素数 (データ部分) と各階層のポインタ数の合計を計算 */
	size_t total_elements = 1;
	size_t total_ptrs = 0;
	for (size_t i = 0; i < dims; i++) {
		if (sizes[i] == 0 || total_elements > (SIZE_MAX / sizes[i]))
			return false;
		total_elements *= sizes[i];

		if (i < dims - 1)  /* 最下層以外はポインタ数を加算 */
			total_ptrs += total_elements;
	}

	if (total_ptrs > (SIZE_MAX / sizeof(void*))) return false;
	size_t size_ptrs = total_ptrs * sizeof(void*);

	/* アラインメント違反を防ぐため必要に応じて切り上げ */
	size_t size_padding = 0;  /* パディングのサイズを格納する */
	if (elem_size > sizeof(void*)) {
		size_t size_ptrs_max = align_up(size_ptrs, elem_size);
		if (size_ptrs_max == 0) {
			return false;
		}
		size_padding = size_ptrs_max - size_ptrs;
	}

	if ((total_elements > (SIZE_MAX / elem_size)) ||
	   ((total_elements * elem_size) > (SIZE_MAX - size_ptrs - size_padding)))
		return false;

	*result_ptrs_size = size_ptrs;
	*result_padding_size = size_padding;
	*result_total_elements = total_elements;
	return true;
}


void* allocate_and_initialize_nd_array (const size_t sizes[], size_t dims, size_t elem_size, size_t size_ptrs, size_t size_padding, size_t total_elements, allocFuncPtr alloc_func) {
	if (dims == 1)  /* 1次元 (ただの配列) の場合はそのまま malloc に渡す */
		return alloc_func(total_elements * elem_size);

	/* メモリブロックを確保 */
	void** base = alloc_func(size_ptrs + size_padding + (total_elements * elem_size));
	if (UNLIKELY(base == NULL)) return NULL;

	void** ptr = base;  /* ポインタの開始位置を格納 */

	if (dims != 2) {  /* 3次元以上の時だけ実行 */
		/* 最下層を除くポインタ位置を設定 (下層もポインタであるため、そのまま加算) */
		size_t curr_level = 1;
		for (size_t d = 0; d < dims - 2; d++) {
			curr_level *= sizes[d];
			size_t next_level = sizes[d + 1];

			for (size_t i = 0; i < curr_level; i++) {
				ptr[i] = (void*)(ptr + curr_level + (i * next_level));
			}
			ptr += curr_level;
		}
	}

	/* 最下層のポインタ位置を設定 (下層は実データであるため、elem_sizeを掛けて計算) */
	char* data = (char*)(base) + size_ptrs + size_padding;
	for (size_t i = 0; i < total_elements / sizes[dims - 1]; i++) {
		ptr[i] = data + ((i * sizes[dims - 1]) * elem_size);
	}

	return (void*)base;
}


void* alloc_nd_array (const size_t sizes[], size_t dims, size_t elem_size) {
	size_t size_ptrs, size_padding, total_elements;
	if (calculate_nd_array_size(sizes, dims, elem_size, &size_ptrs, &size_padding, &total_elements))
		return NULL;

	return allocate_and_initialize_nd_array(sizes, dims, elem_size, size_ptrs, size_padding, total_elements, malloc);
}


void* calloc_nd_array (const size_t sizes[], size_t dims, size_t elem_size) {
	size_t size_ptrs, size_padding, total_elements;
	if (calculate_nd_array_size(sizes, dims, elem_size, &size_ptrs, &size_padding, &total_elements))
		return NULL;

	return allocate_and_initialize_nd_array(sizes, dims, elem_size, size_ptrs, size_padding, total_elements, calloc_wrapper);
}


void free_nd_array (void* array) {
	free(array);
}
