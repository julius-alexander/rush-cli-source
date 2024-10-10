#include "helper.h"
#include <stdio.h>
#include <string.h>

void rush_cli_prompt(void) {
	fputs("rush> ", stdout);
	fflush(stdout);
}

void rush_report_error(FILE *stream) {
	fputs(ERROR_MSG, stream);
	fflush(stream);
}

// Parse raw user input
void rush_parse(char **argsv, char *raw_user_input, char **raw_single_commands) {
	ignore_escape_chars(raw_user_input);

	// reset_2d_arr(&raw_single_commands, MAX_CMDS);

	// TODO: split by '&'
	// split_into_single_commands(raw_single_commands, raw_user_input);

	// TODO: tokenize single_command;
	rush_tokenize_command(argsv, raw_user_input);
}

// PERF: resets vals, no need for realloc() or free()
void reset_2d_arr(char ***arr, int arr_size) {

	int i;
	for (i = 0; i < arr_size; i++) {
		strcpy((*arr)[i], "placeholder");
	}
}

void ignore_escape_chars(char *buffer) {
	int i;
	// replace illegal inputs with whitespace
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r' ||
		    buffer[i] == '\a' || buffer[i] == '\b' || buffer[i] == '\v' ||
		    buffer[i] == '\f') {
			buffer[i] = ' ';
		}
	}
}

void rush_tokenize_command(char **argsv, char *raw_single_command) {
	char *token;
	int i = 0;
	token = strtok(raw_single_command, " ");
	while (token != NULL) {
		// argsv[i] = malloc(strlen(token) + 1);
		strcpy(argsv[i], token);
		token = strtok(NULL, " ");
		i++;
	}

	strcpy(argsv[i], IMPOSSIBLE_STRING);
}

// NOTE: Idk if this even works; it looks soooo sus bro
void split_into_single_commands(char **raw_single_commands, char *raw_user_input) {
	char *token;
	int i = 0;

	while ((token = strsep(&raw_user_input, "&")) != NULL) {
		raw_single_commands[i] = token; // Store each token in the array
		i++;
	}
	raw_single_commands[i] = NULL; // Mark the end of the array with NULL
}

// PERF: Light work no reaction
void builtin_cd(char *path) {
	if (chdir(path) != 0) {
		rush_report_error(stdout);
	}
}

// PERF: Be so fr rn
void builtin_exit(void) { exit(0); }

// PERF: Looks good to me :3
void builtin_path(char **new_path, char **argsv) {
	int i;
	char temp[MAX_BUFFER];
	for (i = 0; i < MAX_PATH; i++) {
		strcpy(temp, argsv[i]);
		strcat(temp, "/");
		strcpy(new_path[i], temp);
	}
}

// debug purposes only!
void print_argv(char **argsv) {
	int i = 0;
	while (argsv[i] != NULL) {
		puts(argsv[i]);
		i++;
	}
}

// Check and execute if command is a builtin
int che_x_builtin(char **argsv, char **user_path) {
	int isBuiltin = FALSE;

	// cd should have exactly 1 argument!
	if (strcmp(argsv[0], "cd") == 0) {
		isBuiltin = TRUE;

		if ((strcmp(argsv[1], IMPOSSIBLE_STRING) != 0) &&
		    strcmp(argsv[2], IMPOSSIBLE_STRING) == 0) {
			builtin_cd(argsv[1]);
		}

		else {
			rush_report_error(stdout);
		}
	}

	// it is an error for exit to be run with args
	else if (strcmp(argsv[0], "exit") == 0) {
		isBuiltin = TRUE;
		if (strcmp(argsv[1], IMPOSSIBLE_STRING) == 0) {
			builtin_exit();
		} else {
			rush_report_error(stdout);
		}
	}

	else if (strcmp(argsv[0], "path") == 0) {
		isBuiltin = TRUE;
		builtin_path(user_path, argsv);
	}

	return isBuiltin;
}

// initializes any 2d array with specified size of rows and cols
void init_2d_arr(char ***arr, int rows, int cols) {
	*arr = (char **)malloc(rows * sizeof(char *));

	int i;
	for (i = 0; i < rows; i++) {
		(*arr)[i] = (char *)malloc(cols * sizeof(char));
	}
}

// return the index of the path where the given command was found
int located_path(char *cmd, char **paths_to_search) {
	int i;
	char temp[MAX_BUFFER];
	for (i = 0; i < MAX_PATH; i++) {
		strcpy(temp, paths_to_search[i]);
		strcat(temp, cmd);
		if (access(temp, X_OK) == 0) {
			return i;
		}
	}
	return -1;
}

int detect_parse_errors(char *trimmed_raw_input) {

	// if '>>' or '&&' exists or
	// starts with '&' or '>'
	// or ends with '&' or '>'
	// then: report error and exit

	if (*trimmed_raw_input == '>' || *trimmed_raw_input == '&') {
		return TRUE;
	}

	char *tmp;
	for (tmp = trimmed_raw_input; *tmp != '\0' && *(tmp + 1) != '\0'; tmp++) {
		if (((*tmp == '&') || (*tmp == '>')) &&
		    (*(tmp + 1) == '&' || *(tmp + 1) == '>')) {
			return TRUE;
		}
	}

	if (*tmp == '&' || *tmp == '>') {
		return TRUE;
	}

	return FALSE;
}

void insert_null(char **argsv) {
	int i;
	for (i = 0; i < MAX_ARGS; i++) {
		if (strcmp(argsv[i], IMPOSSIBLE_STRING) == 0) {
			argsv[i] = NULL;
		}
	}
}

int detect_redir_error(char *single_command) {
	return FALSE;
	;
}
