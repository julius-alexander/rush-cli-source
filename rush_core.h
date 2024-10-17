/* ======================================================================================
 *
 * rush_processing.h - Source file for core shell functionalities
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

#ifndef CORE
#define CORE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// String is impossible to type accidentally, used as a placeholder for safe iteration
#define IMPOSSIBLE_STRING "✧(ꈍᴗꈍ)✧"	 // also it's kinda cute
#define MAX_BUFFER		  255			 // maximum raw user input buffer size
#define MAX_CMDS		  128			 // max number of possible commands to run in parallel
#define MAX_ARGS		  128			 // max number of args we can have in **argsv
#define MAX_PATHS		  20			 // max number of paths we can have defined
#define TRUE			  1
#define FALSE			  0

// Global variables
extern char **user_path;				   // list of valid paths to check
extern char **commands_list;			   // list of commands to run
extern char **argsv;					   // a standard argv to be passed into execv
extern char path_to_cmd[MAX_BUFFER];	   // to be passed in execv
extern char raw_user_input[MAX_BUFFER];	   // pre-parsed input buffer

// Core functions
void rush_init(void);
void rush_cli_prompt(void);
void rush_run_commands(char **commands_list);	 // TODO: implement, optional
void rush_exec_child(char **user_path, char **argsv);
void rush_report_error(void);

// builtin functions
void builtin_exit(void);
void builtin_cd(char *path);
void builtin_path(char **new_path, char **argsv);

#endif
