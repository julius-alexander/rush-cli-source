#ifndef CORE
#define CORE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// String impossible to type accidentally, used as a placeholder for safe iteration
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
void rush_build_cmds_list(char *raw_user_input, char **commands_list);
void rush_run_commands(char **commands_list);
void rush_exec_child(char **user_path, char **argsv);
void rush_report_error(void);

// builtin functions
void builtin_exit(void);
void builtin_cd(char *path);
void builtin_path(char **new_path, char **argsv);

#endif
