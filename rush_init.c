#include "rush.h"

// Global variables
char **user_path;         // list of valid paths to check
char **commands_list;     // will be cmds_list
char **argsv;             // a standard argv to be passed into exec
char path_to_cmd[255];    // to be passed in execv
char raw_user_input[255]; // pre-parsed input

void rush_init(void) {

	// Allocate memory based on program requirements
	init_str_arr(&user_path, MAX_PATHS, MAX_BUFFER);
	init_str_arr(&argsv, MAX_ARGS, MAX_BUFFER);
	init_str_arr(&commands_list, MAX_CMDS, MAX_BUFFER);

	// Set default values for convenience
	reset_str_arr(&argsv, MAX_ARGS);
	reset_str_arr(&commands_list, MAX_CMDS);
	reset_str_arr(&user_path, MAX_PATHS);

	strcpy(user_path[0], "/bin/");     // configure default path
	strcpy(user_path[1], "/usr/bin/"); // configure default path
}
