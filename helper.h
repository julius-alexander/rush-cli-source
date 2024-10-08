#ifndef _RUSH_
#define _RUSH_

#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TRUE       1
#define ERROR_MSG  "An error has occurred\n"
#define MAX_ARGS   20
#define MAX_CMDS   20
#define MAX_BUFFER 255
#define MAX_PATH   20

void rush_cli_prompt(void);
void rush_report_error(FILE *stream);
void rush_parse(char **argv, char *raw_user_input);
void reset_argv(char **argv);
void print_argv(char **argv);
void populate_argv(char **argv);
void ignore_escape_chars(char *buffer);
void rush_tokenize_command(char **argsv, char *raw_user_input);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);
void reset_list_single_cmds(char **list_single_cmds);
int search_paths(char **paths, char *cmd);

void builtin_exit(void);
void builtin_cd(char *path);
void builtin_path(char new_path[MAX_PATH][MAX_BUFFER], char **argsv);
int che_x_builtin(char **argsv, char user_path[MAX_PATH][MAX_BUFFER]);

void init_raw_single_cmds(char **raw_single_commands);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);
void free_argsv(char **argsv);

#endif
