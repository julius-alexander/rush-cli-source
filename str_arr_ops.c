/* ======================================================================================
 *
 * str_arr_ops.c - Source file for convenient string array operations
 *
 * This file is part of rush, Rapid UNIX Shell
 *
 * Date: 13 October 2024
 *
 * License: This project is for educational purposes only and is not intended for redistribution.
 *
 * ====================================================================================== */

#include "str_arr_ops.h"

// PERF: Initializes 'row' number of strings of 'cols' size each
void init_str_arr(char ***arr, int rows, int cols) {

	*arr = (char **)calloc(rows, sizeof(char *));

	int i;
	for (i = 0; i < rows; i++) {
		(*arr)[i] = (char *)calloc(cols + 1, sizeof(char));
	}
}

// PERF: Resets to default vals, no need for realloc() or free()
void reset_str_arr(char ***arr, int arr_size) {
	int i;
	for (i = 0; i < arr_size; i++) {
		strcpy((*arr)[i], "✧(ꈍᴗꈍ)✧");
	}
}

// NOTE: debug purposes only!
// Might cause segfault if not properly NULL terminated
void print_str_arr(char **argsv) {
	int i = 0;
	while (argsv[i] != NULL) {
		puts(argsv[i]);
		i++;
	}
}
