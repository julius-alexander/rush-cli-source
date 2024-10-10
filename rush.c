#include "helper.h"
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	FILE *stream = stdout;

	// ========================================================================
	// testing
	// TEST: parse error
	//	char trimmed_raw_input[255];
	//	strcpy(trimmed_raw_input, "& ls");
	//	if (detect_parse_errors(trimmed_raw_input)) {
	//		puts("error detected");
	//	} else {
	//		puts("no error detected");
	//	}
	//	exit(0);

	// ========================================================================

	// it is an error to run rush with args
	if (argc > 1) {
		rush_report_error(stream);
		builtin_exit();
	}

	char path_to_cmd[255];    // to be passed in execv
	char raw_user_input[255]; // pre-parsed input

	// no more than 20 paths, each path no more than 255 chars
	char **user_path;
	init_2d_arr(&user_path, MAX_PATH, MAX_BUFFER);
	reset_2d_arr(&user_path, MAX_PATH);
	strcpy(user_path[0], "/bin/"); // configure default path

	char **argsv;
	init_2d_arr(&argsv, MAX_ARGS, MAX_BUFFER);
	reset_2d_arr(&argsv, MAX_ARGS); // init all to NULL

	char **raw_single_commands;
	init_2d_arr(&raw_single_commands, MAX_CMDS, MAX_BUFFER);
	reset_2d_arr(&raw_single_commands, MAX_CMDS);

	while (TRUE) {
		reset_2d_arr(&argsv, MAX_ARGS); // init all to NULL
		reset_2d_arr(&raw_single_commands, MAX_CMDS);

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

		// check and execute cmd if builtin, otherwise fork
		if (!che_x_builtin(argsv, user_path)) {

			int located = located_path(argsv[0], user_path);
			if (located < 0) {
				rush_report_error(stdout);
				continue;
			}

			int rc = fork();
			if (rc < 0) {
				rush_report_error(stdout);
			}

			else if (rc == 0) {
				insert_null(argsv); // inserts NULL to terminate, instead of placeholder
				strcpy(path_to_cmd, user_path[located]); // should be "found" instead of 0
				strcat(path_to_cmd, argsv[0]);

				execv(path_to_cmd, argsv);
				rush_report_error(stdout);
				builtin_exit();
			}
		}

		wait(NULL);
	}

	return 0;
}
