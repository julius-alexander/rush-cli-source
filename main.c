/* ======================================================================================
 *
 * main.c - Entry point for rush program
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

// TODO: start trimming unnecessary comments, and unnecessary code
// like debug code or other stuff
int main(int argc, char **argv) {

	if (argc > 1) {
		(void)argv;	   // to silence -Wextra warnings
		rush_report_error();
		exit(1);
	}

	// Initialize global variables for shell
	rush_init();
	int cmd, num_children;
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
		char raw_user_input[MAX_BUFFER];
		fgets(raw_user_input, MAX_BUFFER, stdin);

		// Sanitize input string and split into single commands
		normalize_input(raw_user_input);
		tokenize_by_delim(commands_list, raw_user_input, "&");

		// iterate through list of commands and execute in parallel
		for (cmd = 0; strcmp(commands_list[cmd], IMPOSSIBLE_STRING) != 0; cmd++) {
			tokenize_by_delim(argsv, commands_list[cmd], " ");

			// user entered empty line
			if (strcmp(argsv[0], IMPOSSIBLE_STRING) == 0) {
				continue;
			}

			// Check and execute if command is builtin, otherwise fork and run next command
			if (chexec_builtin(argsv, user_path)) {
				continue;
			}

			fflush(stdout);
			int rc = fork();
			if (rc < 0) {
				rush_report_error();
			} else if (rc == 0) {
				rush_exec_child(user_path, argsv);
			} else {
				child_pids[num_children++] = rc;	// keep track of children
			}
		}

		// Wait for ALL children to finish running
		int i;
		for (i = 0; i < MAX_CMDS; i++) {
			waitpid(child_pids[i], NULL, 0);
		}
	}

	return 0;
}
