#include "helper.h"
#include <_string.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <unistd.h>

void rush_cli_prompt(void) {
	fputs("rush> ", stdout);
	fflush(stdout);
}

void rush_report_error(FILE *stream) {
	fputs(ERROR_MSG, stream);
	fflush(stream);
}

// TODO: parse raw user input
void rush_parse(char **argsv, char *raw_user_input) {
	ignore_escape_chars(raw_user_input);

	// char *raw_single_commands[MAX_CMDS];
	// reset_argv(raw_single_commands);

	// init_raw_single_cmds(raw_single_commands); // initialize empty list of command
	// lines

	// TODO: split by '&'
	// result is an array with raw single commands

	// split_into_single_commands(raw_single_commands, raw_user_input);

	// TODO: tokenize single_command;
	// replace [0] with i and make a for loop

	rush_tokenize_command(argsv, raw_user_input);
}

// FIX: malloc?? more like mad sus...
void reset_argv(char **argsv) {
	// int i;
	// for (i = 0; i < MAX_ARGS; i++) {
	//	argsv[i] = (char *)malloc(MAX_BUFFER * sizeof(char));
	// }
}

void ignore_escape_chars(char *buffer) {
	int i;
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r') {
			buffer[i] = ' ';
		}
	}
}

void rush_tokenize_command(char **argsv, char *raw_single_command) {

	/* Tokenize input */
	char *token;
	int i = 0;
	token = strtok(raw_single_command, " ");
	while (token != NULL) {
		argsv[i] = malloc(strlen(token) + 1);
		strcpy(argsv[i], token);
		token = strtok(NULL, " ");
		i++;
	}
	argsv[i] = NULL;

	// If user enters empty string, do nothing
	if (i == 0) {
		argsv[0] = ":";

		// if exit is called with args, report error; else exit
	} else if (strcmp(argsv[0], "exit") == 0) {
		if (argsv[1] != NULL) {
			rush_report_error(stdout);
		} else {
			exit(0);
		}
	}
}

void split_into_single_commands(char **raw_single_commands, char *raw_user_input) {
	// char *token;
	// int i = 0;

	// while ((token = strsep(&raw_user_input, "&")) != NULL) {
	// 	raw_single_commands[i] = token; // Store each token in the array
	// 	i++;
	// }
	// raw_single_commands[i] = NULL; // Mark the end of the array with NULL
}

int search_paths(char **paths, char *cmd) {
	return 1;
	// char full_path[MAX_BUFFER];
	// int i;
	// for (i = 0; i < MAX_PATH && paths[i] != NULL; i++) {
	// 	strcpy(full_path, paths[i]);
	// 	strcat(full_path, "/");
	// 	strcat(full_path, cmd);
	// 	if (access(full_path, X_OK) == 0) {
	// 		return 1;
	// 	}
	// }
	// return 0;
}

// PERF: Function works, but caller input is hardcoded
void builtin_cd(char *path) {
	if (chdir(path) != 0) {
		rush_report_error(stdout);
	}
}

// PERF: Be so fr
void builtin_exit(void) { exit(0); }

void builtin_path(char new_path[MAX_PATH][MAX_BUFFER], char **argsv) {
	int i, j;
	for (i = 0, j = 1; i < 2 && argsv[j] != NULL; i++, j++) {
		strcpy(new_path[i], argsv[j]);
	}

	puts("debug print");
	for (; i < MAX_PATH; i++) {
		new_path[i][0] = '\0';
	}
}

void init_raw_single_cmds(char **raw_single_commands) {
	;
	;
}

void print_argv(char **argsv) {
	// debug purposes only!
	int i = 0;
	while (argsv[i] != NULL) {
		puts(argsv[i]);
		i++;
	}
}

// Check and execute if cmd is a builtin
int che_x_builtin(char **argsv, char user_path[MAX_PATH][MAX_BUFFER]) {
	int ret = 0;

	if (strcmp(argsv[0], ":")) {
		ret = 1;
	}

	else if (strcmp(argsv[0], "cd")) {
		builtin_cd(argsv[1]);
		ret = 1;
	}

	else if (strcmp(argsv[0], "exit")) {
		builtin_exit();
		ret = 1;
	}

	else if (strcmp(argsv[0], "path")) {
		builtin_path(user_path, argsv);
		ret = 1;
	}

	return ret;
}
