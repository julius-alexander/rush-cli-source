/* ======================================================================================
 *
 * str_arr_ops.h - Header for convenient string array operations
 *
 * This file is part of rush, Rapid UNIX Shell
 *
 * Project 1 for COP 4600 at the University of South Florida, Tampa
 *
 * Author: John Garzon-Ferrer
 * NetID: garzonferrer@usf.edu
 *
 * Date: 13 October 2024
 *
 * License: This project is for educational purposes only and is not intended for redistribution.
 *
 * ====================================================================================== */

#ifndef STR_ARR_OPS
#define STR_ARR_OPS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String array operations
void init_str_arr(char ***arr, int rows, int cols);
void reset_str_arr(char ***arr, int arr_size);
void print_str_arr(char **argv);	// debug purposes only

#endif
