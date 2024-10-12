#include "helper.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

	// it is an error to run rush with args
	if (argc > 1) {
		rush_report_error();
		builtin_exit();
	}

	char path_to_cmd[255];    // to be passed in execv
	char raw_user_input[255]; // pre-parsed input

	char **user_path;           // list of valid paths to check
	char **argsv;               // a standard argv to be passed into exec
	char **raw_single_commands; // will be cmds_list

	// Allocate memory based on program requirements
	init_str_arr(&user_path, MAX_PATHS, MAX_BUFFER);
	init_str_arr(&argsv, MAX_ARGS, MAX_BUFFER);
	init_str_arr(&raw_single_commands, MAX_CMDS, MAX_BUFFER);

	// Set default values for convenience
	reset_str_arr(&user_path, MAX_PATHS);
	strcpy(user_path[0], "/bin/");     // configure default path
	strcpy(user_path[1], "/usr/bin/"); // configure default path

	// =======================================================================================

	// =======================================================================================

	while (TRUE) {

		// Reset buffers after every line of user input
		reset_str_arr(&argsv, MAX_ARGS);
		reset_str_arr(&raw_single_commands, MAX_CMDS);

		// print shell message
		rush_cli_prompt();

		// take raw input
		fgets(raw_user_input, MAX_BUFFER, stdin);

		// user entered empty line
		if (strlen(raw_user_input) <= 1) {
			continue;
		}

		// wrap the entire rest of loop in for loop?
		//
		// parse(cmds_list, raw_input)
		// for cmd in cmds_list:
		//      if not che_x_builtin(cmd, path):
		//
		//          fork_ops()
		// wait(NULL)

		// parse and build command

		rush_parse(argsv, raw_user_input, raw_single_commands);

		// check and execute cmd if builtin, otherwise search in paths
		if (!che_x_builtin(argsv, user_path)) {

			// find the index of the path of the command
			int located = located_path(argsv[0], user_path);
			if (located < 0) {
				printf("couldn't locate command in path... ");
				rush_report_error();
				continue;
			}

			int rc = fork();
			if (rc < 0) {
				rush_report_error();
			}

			else if (rc == 0) {
				redirection_handler(argsv);
				// print_str_arr(argsv);
				insert_null(argsv); // Replaces IMPOSSIBLE_STRING with actual NULL

				if (det_parse_errors(argsv)) {
					puts("that was a parse error");
					rush_report_error();
					builtin_exit();
				}

				strcpy(path_to_cmd, user_path[located]);
				strcat(path_to_cmd, argsv[0]);

				execv(path_to_cmd, argsv);
				rush_report_error();
				builtin_exit();
			}

			else {
				// parent
				wait(NULL);
			}
		}
	}

	return 0;
}
