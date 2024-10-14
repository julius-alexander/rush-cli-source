#include "core.h"
#include "processing.h"
#include "str_arr_ops.h"
#include <stdio.h>
#include <sys/wait.h>

// TODO: start trimming unnecessary comments, and unnecessary code
// like debug code or other stuff
int main(int argc, char **argv) {

	// it is an error to run rush with args
	if (argc > 1) {
		rush_report_error();
		exit(1);
	}

	// Initialize global variables for shell
	rush_init();

	int cmd = 0, i = 0;
	int num_children = 0;
	pid_t child_pids[MAX_CMDS];

	while (TRUE) {
		// Reset buffers after every line of user input
		num_children = 0;
		reset_str_arr(&argsv, MAX_ARGS);
		reset_str_arr(&commands_list, MAX_CMDS);
		for (cmd = 0; cmd < MAX_CMDS; cmd++) {
			child_pids[cmd] = 0;
		}

		rush_cli_prompt();

		fgets(raw_user_input, MAX_BUFFER, stdin);

		// TODO: might be redundant
		// user entered empty line
		if (strlen(raw_user_input) <= 1) {
			continue;
		}

		rush_build_cmds_list(raw_user_input, commands_list);

		for (cmd = 0; strcmp(commands_list[cmd], IMPOSSIBLE_STRING) != 0; cmd++) {
			// Sanitize input string
			// TODO: might be redundant
			normalize_input(commands_list[cmd]);
			tokenize_by_delim(argsv, commands_list[cmd], " ");

			// user entered empty line
			if (strcmp(argsv[0], IMPOSSIBLE_STRING) == 0) {
				continue;
			}

			// Check and execute cmd if builtin, otherwise run next command
			if (che_x_builtin(argsv, user_path)) {
				continue;
			}

			fflush(stdout);
			int rc = fork();
			if (rc < 0) {	 // no child created
				rush_report_error();
			}

			else if (rc == 0) {	   // child

				// TODO: might be able to wrap entire block in a func rush_execute_command()
				// void rush_execute_command(argsv, user_path);

				// find the index of the path of the command, if valid
				int located = located_path(argsv[0], user_path);
				if (located < 0) {
					rush_report_error();
					exit(1);
				}

				redirection_handler(argsv);
				insert_null(argsv);	   // Replaces IMPOSSIBLE_STRING with actual NULL

				strcpy(path_to_cmd, user_path[located]);
				strcat(path_to_cmd, argsv[0]);

				// fflush(stdout);
				execv(path_to_cmd, argsv);

				rush_report_error();
				exit(1);

			} else {
				child_pids[num_children++] = rc;	// keep track of children
			}
		}

		// Wait for ALL children to finish running
		for (i = 0; i < MAX_CMDS; i++) {
			waitpid(child_pids[i], NULL, 0);
		}
	}

	return 0;
}
