#include "rush.h"

// initializes any 2d array with specified size of rows and cols
void init_str_arr(char ***arr, int rows, int cols) {

	*arr = (char **)calloc(rows, sizeof(char *));

	int i;
	for (i = 0; i < rows; i++) {
		(*arr)[i] = (char *)calloc(cols + 1, sizeof(char));
	}
}

// PERF: resets to default vals, no need for realloc() or free()
void reset_str_arr(char ***arr, int arr_size) {
	int i;
	for (i = 0; i < arr_size; i++) {
		strcpy((*arr)[i], IMPOSSIBLE_STRING);
	}
}

// debug purposes only!
void print_str_arr(char **argsv) {
	int i = 0;
	while (argsv[i] != NULL) {
		puts(argsv[i]);
		i++;
	}
}
