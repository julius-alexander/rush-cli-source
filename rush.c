#include "helper.h"
#include <stdio.h>

int main(int argc, char **argv) {

	// it is an error to run rush with args
	if (argc > 1) {
		rush_report_error(stdout);
		builtin_exit();
	}

	char path_to_cmd[255];    // to be passed in execv
	char raw_user_input[255]; // pre-parsed input

	// no more than 20 paths, each path no more than 100 chars
	char user_path[MAX_PATH][MAX_BUFFER];
	strcpy(user_path[0], "/bin/"); // configure default path

	char *argsv[MAX_ARGS];

	// TEST:
	// strcpy(user_path[1], "/usr/bin/");
	argsv[0] = malloc((strlen("path") * sizeof(char)) + 1);
	strcpy(argsv[0], "path");
	argsv[1] = malloc((strlen("/bin/") * sizeof(char)) + 1);
	strcpy(argsv[1], "/bin/");
	argsv[2] = malloc((strlen("/usr/bin/") * sizeof(char)) + 1);
	strcpy(argsv[2], "/usr/bin/");

	builtin_path(user_path, argsv);
	puts("debug print");
	int i;
	for (i = 0; i < 2; i++) {
		puts(user_path[i]);
	}
	builtin_exit();

	// TODO: will need a configurable (FILE *stream) for redirection?

	while (TRUE) {

		reset_argv(argsv); // init all to NULL

		// print shell message
		rush_cli_prompt();

		// take raw input
		fgets(raw_user_input, MAX_BUFFER, stdin);

		// parse and build command

		rush_parse(argsv, raw_user_input);

		// handler for empty input
		if (che_x_builtin(argsv, user_path)) {
			continue;
		}

		int rc = fork();
		if (rc < 0) {
			// lmaooo im ded XD
			rush_report_error(stdout);
		} else if (rc == 0) {
			// I am baby :3
			// path_to_cmd = "\bin\{cmd}"
			strcpy(path_to_cmd, user_path[0]); // TODO: replace literal with variable
			strcat(path_to_cmd, argsv[0]);

			// execv("/bin/cat", {"cat", "helper.h", NULL});
			execv(path_to_cmd, argsv);
			rush_report_error(stdout);
			builtin_exit();
		}
		wait(NULL);
	}

	return 0;
}
