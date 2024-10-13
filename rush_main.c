#include "rush.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	// it is an error to run rush with args
	if (argc > 1) {
		rush_report_error();
		builtin_exit();
	}

	// Initialize global variables for shell
	rush_init();

	int cmd = 0;

	while (TRUE) {

		// Reset buffers after every line of user input
		reset_str_arr(&argsv, MAX_ARGS);
		reset_str_arr(&commands_list, MAX_CMDS);

		// print shell message
		rush_cli_prompt();

		// take raw input
		fgets(raw_user_input, MAX_BUFFER, stdin);

		// user entered empty line
		if (strlen(raw_user_input) <= 1) {
			continue;
		}

		// normalize_input and split by '&'
		rush_parse(raw_user_input, commands_list);

		// FIX: Commands are executed concurrently, but rush prompt is getting messed up
		// Parent should wait for all children to exec, but that may not be happening
		for (cmd = 0; strcmp(commands_list[cmd], IMPOSSIBLE_STRING) != 0; cmd++) {
			int rc = fork();
			if (rc < 0) { // no child created
				rush_report_error();
			}

			else if (rc == 0) { // child

				normalize_input(commands_list[cmd]);
				tokenize_by_delim(argsv, commands_list[cmd], " ");

				// check and execute cmd if builtin, otherwise search in paths
				if (!che_x_builtin(argsv, user_path)) {

					// find the index of the path of the command, if valid
					int located = located_path(argsv[0], user_path);
					if (located < 0) {
						// printf("couldn't locate command in path... ");
						rush_report_error();
						continue;
					}

					redirection_handler(argsv);
					insert_null(argsv); // Replaces IMPOSSIBLE_STRING with actual NULL

					strcpy(path_to_cmd, user_path[located]);
					strcat(path_to_cmd, argsv[0]);

					execv(path_to_cmd, argsv);

					rush_report_error();
					builtin_exit();
				}

				else { // parent
					while (wait(&rc) > 0) {
						;
					}
				}
			}
		}
	}

	return 0;
}
