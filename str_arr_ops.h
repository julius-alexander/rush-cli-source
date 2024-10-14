#ifndef STR_ARR_OPS
#define STR_ARR_OPS

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String array operations
void init_str_arr(char ***arr, int rows, int cols);
void reset_str_arr(char ***arr, int arr_size);
void print_str_arr(char **argv);	// debug purposes only

#endif
