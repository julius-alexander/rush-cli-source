#ifndef _RUSH_
#define _RUSH_

#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <unistd.h>

extern FILE *stream; // for flexibility of output destination

// there's no way any user would ever type this
// this string is the result of typing "no-way-you-typed-this-on-accident"
// while holding option and shift on Mac
#define IMPOSSIBLE_STRING "˜Ø—„ÅÁ—ÁØ¨—ˇÁ∏´Î—ˇÓˆÍ—Ø˜—ÅÇÇˆÎ´˜ˇ"
#define TRUE              1
#define FALSE             0
#define MAX_ARGS          20  // number of args we can have in **argsv
#define MAX_CMDS          86  // max number of possible commands assuming a 255 char limit
#define MAX_BUFFER        255 // maximum input buffer size to be tested on
#define MAX_PATH          20  // maximum number of paths we can have defined
#define ERROR_MSG         "An error has occurred\n"

// "Big picture" functions
void rush_cli_prompt(void);
void rush_report_error(FILE *stream);
void rush_parse(char **argv, char *raw_user_input, char **raw_single_commands);
void reset_argv(char **argv);
void reset_2d_arr(char ***arr, int arr_size);
void ignore_escape_chars(char *buffer);
void rush_tokenize_command(char **argsv, char *raw_user_input);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);

// built-in commands
void builtin_exit(void);
void builtin_cd(char *path);
void builtin_path(char **new_path, char **argsv);

// helper functions
int che_x_builtin(char **argsv, char **user_path);
int detect_parse_errors(char *trimmed_raw_input);
int located_path(char *cmd, char **paths_to_search);
void init_2d_arr(char ***arr, int rows, int cols);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);
void insert_null(char **argsv);

// utility functions, delete later
void print_argv(char **argv);

#endif
