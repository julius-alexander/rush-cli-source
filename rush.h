#ifndef _RUSH_
#define _RUSH_

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// String impossible to type accidentally, used as a placeholder for safe iteration
#define IMPOSSIBLE_STRING "˜Ø—„ÅÁ—ÁØ¨—ˇÁ∏´Î—ˇÓˆÍ—Ø˜—ÅÇÇˆÎ´˜ˇ\0"
#define ERROR_MSG         "An error has occurred\n"
#define TRUE              1
#define FALSE             0
#define MAX_BUFFER        255 // maximum raw user input buffer size
#define MAX_CMDS          128 // max number of possible commands to run in parallel
#define MAX_ARGS          128 // number of args we can have in **argsv
#define MAX_PATHS         20  // maximum number of paths we can have defined

// Core functions
void rush_init(void);
void rush_cli_prompt(void);
void rush_parse(char **argv, char *raw_user_input, char **raw_single_commands);
void rush_report_error(void);

// Command processing functions
void normalize_input(char *buffer);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);
void rush_tokenize_command(char **argsv, char *raw_user_input);
int che_x_builtin(char **argsv, char **user_path);
int located_path(char *cmd, char **paths_to_search);
void redirection_handler(char **argsv);
void set_stream(char *file_name);
void insert_null(char **argsv);

// Built-in commands
void builtin_exit(void);
void builtin_cd(char *path);
void builtin_path(char **new_path, char **argsv);

// String array operations
void init_str_arr(char ***arr, int rows, int cols);
void reset_str_arr(char ***arr, int arr_size);
void print_str_arr(char **argv); // debug purposes only

// Global variables
extern char **user_path;         // list of valid paths to check
extern char **commands_list;     // will be cmds_list
extern char **argsv;             // a standard argv to be passed into exec
extern char path_to_cmd[255];    // to be passed in execv
extern char raw_user_input[255]; // pre-parsed input

#endif
