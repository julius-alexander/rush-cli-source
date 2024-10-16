/* ======================================================================================
 *
 * rush_processing.h - Header for input processing functions
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
void tokenize_by_delim(char **argsv, char *raw_user_input, char *delim);
int chexec_builtin(char **argsv, char **user_path);
int located_path(char *cmd, char **paths_to_search);
void redirection_handler(char **argsv);
void insert_null(char **argsv);

#endif
