#ifndef PROCESSING
#define PROCESSING

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Command processing functions
void normalize_input(char *raw_user_input);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);
void tokenize_by_delim(char **argsv, char *raw_user_input, char *delim);
int che_x_builtin(char **argsv, char **user_path);
int located_path(char *cmd, char **paths_to_search);
void redirection_handler(char **argsv);
void insert_null(char **argsv);

#endif
