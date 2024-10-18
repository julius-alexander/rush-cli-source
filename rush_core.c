/* ======================================================================================
 *
 * rush_core.c - Source file for core shell functionalities
 *
 * This file is part of rush, Rapid UNIX Shell
 *
 * Date: 13 October 2024
 *
 * License: This project is for educational purposes only and is not intended for redistribution.
 *
 * ====================================================================================== */

#include "rush_core.h"
#include "rush_processing.h"
#include "str_arr_ops.h"

// Global variables
char **user_path;					// list of valid paths to check
char **commands_list;				// will be cmds_list
char **argsv;						// a standard argv to be passed into exec
char path_to_cmd[MAX_BUFFER];		// to be passed in execv
char raw_user_input[MAX_BUFFER];	// pre-parsed input

// PERF: Set up rush shell state, one time only
void rush_init(void) {

	// Allocate memory based on program requirements
	init_str_arr(&argsv, MAX_ARGS, MAX_BUFFER);
	init_str_arr(&commands_list, MAX_CMDS, MAX_BUFFER);
	init_str_arr(&user_path, MAX_PATHS, MAX_BUFFER);

	// Set default values for convenience
	reset_str_arr(&argsv, MAX_ARGS);
	reset_str_arr(&commands_list, MAX_CMDS);
	reset_str_arr(&user_path, MAX_PATHS);

	// configure default path
	strcpy(user_path[0], "/bin/");
}

// PERF:
void rush_exec_child(char **user_path, char **argsv) {

	// find the index of the path of the command, if valid
	int located = located_path(argsv[0], user_path);
	if (located < 0) {
		rush_report_error();
		exit(1);
	}

	redirection_handler(argsv);	   // can kill child if error occurs
	insert_null(argsv);			   // Replaces IMPOSSIBLE_STRING with actual NULL

	strcpy(path_to_cmd, user_path[located]);
	strcat(path_to_cmd, argsv[0]);

	execv(path_to_cmd, argsv);

	rush_report_error();
	exit(1);
}

// PERF: Pretty obvious ngl
void rush_cli_prompt(void) {
	fputs("rush> ", stdout);
	fflush(stdout);
}

// PERF: Trivial
void rush_report_error(void) {
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
	fflush(stdout);
}

// PERF: Be so fr rn
void builtin_exit(void) { exit(0); }

// PERF: Light work no reaction
void builtin_cd(char *path) {
	if (chdir(path) != 0) {
		rush_report_error();
	}
}

// PERF: Looks good to me :3
void builtin_path(char **new_path, char **argsv) {
	int i;
	char temp[MAX_BUFFER];

	for (i = 0; i < MAX_PATHS; i++) {

		char *arg_ptr = argsv[i];
		while (isspace(*arg_ptr))
			arg_ptr++;

		strcpy(temp, argsv[i]);
		strcat(temp, "/");
		strcpy(new_path[i], temp);
	}
}
