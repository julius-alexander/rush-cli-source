#include "core.h"
#include "str_arr_ops.h"

// Global variables
char **user_path;			 // list of valid paths to check
char **commands_list;		 // will be cmds_list
char **argsv;				 // a standard argv to be passed into exec
char path_to_cmd[255];		 // to be passed in execv
char raw_user_input[255];	 // pre-parsed input

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

	strcpy(user_path[0], "/bin/");	  // configure default path
	// strcpy(user_path[1], "/usr/bin/"); // Another convenient path
}

// PERF: Pretty obvious ngl
void rush_cli_prompt(void) {
	// fflush(stdout);
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
		while (isspace(*arg_ptr)) {
			arg_ptr++;
		}
		strcpy(temp, argsv[i]);
		strcat(temp, "/");
		strcpy(new_path[i], temp);
		// puts(new_path[i]);
	}
}
// echo 1 & echo 2 & echo 3 & echo 4 & echo 5 & echo 6
