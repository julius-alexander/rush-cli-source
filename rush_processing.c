#include "rush.h"
#include <stdio.h>
#include <string.h>

// Command processing functions

// PERF: Removes leading, trailing whitespace; ensures only 1 space between args
void normalize_input(char *buffer) {

	char temp_buffer[MAX_BUFFER];
	int i, j;
	int word_flag = 0;

	// write valid contents of buffer to temp_buffer
	for (i = 0, j = 0; i < strlen(buffer); i++) {

		// encounter any char
		if (!isspace(buffer[i])) {
			word_flag = 1;

			// surround & and > with exactly 1 whitespace
			if (buffer[i] == '&' || buffer[i] == '>') {
				if (j > 0 && temp_buffer[j - 1] != ' ') {
					temp_buffer[j++] = ' ';
				}
				temp_buffer[j++] = buffer[i];
				temp_buffer[j++] = ' ';
			}

			// copy buffer char into temp
			else {
				temp_buffer[j++] = buffer[i];
			}

		}

		// encounter whitespace, insert exactly 1 whitespace char between words
		else if (word_flag) {
			if (j > 0 && temp_buffer[j - 1] != ' ') {
				temp_buffer[j++] = ' ';
			}
			word_flag = 0;
		}
	}

	temp_buffer[j] = '\0'; // NOTE: Forgetting this line led to HOURS of bug-chasing '\001'
	strcpy(buffer, temp_buffer);
}

// PERF: Just splits string by ONE (1) delimiter
void tokenize_by_delim(char **tokens_list, char *input, char *delim) {

	char *token, *save_ptr;
	int i = 0;
	token = strtok_r(input, delim, &save_ptr);
	for (i = 0; i < MAX_CMDS && token != NULL; i++) {
		strcpy(tokens_list[i], token);
		token = strtok_r(NULL, delim, &save_ptr);
	}
	strcpy(tokens_list[i], IMPOSSIBLE_STRING);
}

// PERF: Checks and executes if command is a builtin
int che_x_builtin(char **argsv, char **user_path) {
	int isBuiltin = FALSE;

	// cd should have exactly 1 argument!
	if (strcmp(argsv[0], "cd") == 0) {
		isBuiltin = TRUE;
		if (strcmp(argsv[1], IMPOSSIBLE_STRING) == 0) {
			rush_report_error();
			return isBuiltin;
		}
		if ((strcmp(argsv[1], IMPOSSIBLE_STRING) != 0) &&
		    strcmp(argsv[2], IMPOSSIBLE_STRING) == 0) {
			builtin_cd(argsv[1]);
		}

		else {
			rush_report_error();
		}
	}

	// it is an error for exit to be run with args
	else if (strcmp(argsv[0], "exit") == 0) {
		isBuiltin = TRUE;
		if (strcmp(argsv[1], IMPOSSIBLE_STRING) == 0) {
			builtin_exit();
		} else {
			rush_report_error();
		}
	}

	else if (strcmp(argsv[0], "path") == 0) {
		isBuiltin = TRUE;
		builtin_path(user_path, argsv);
	}

	return isBuiltin;
}

// PERF: Returns the index of the path if the given command is found
int located_path(char *cmd, char **paths_to_search) {
	int i;
	char temp[MAX_BUFFER];
	for (i = 0; i < MAX_PATHS; i++) {
		strcpy(temp, paths_to_search[i]);
		strcat(temp, cmd);
		if (access(temp, X_OK) == 0) {
			return i;
		}
	}
	return -1;
}

// PERF: Determines if redirection is necessary, or if redirection error is encountered
void redirection_handler(char **argsv) {

	int i;
	for (i = 0; i < MAX_ARGS; i++) {
		// reached NULL before '>', so no redirection required
		if (strcmp(argsv[i], IMPOSSIBLE_STRING) == 0) {
			return;
		}

		// we either need valid redirection, or throw an error
		else if (strcmp(argsv[i], ">") == 0) {
			// exactly ONE arg after '>'
			if (strcmp(argsv[i + 1], IMPOSSIBLE_STRING) != 0 &&
			    strcmp(argsv[i + 2], IMPOSSIBLE_STRING) == 0) {

				int file_desc = open(argsv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(file_desc, STDOUT_FILENO);
				close(file_desc);
				strcpy(argsv[i], IMPOSSIBLE_STRING);
			}

			else {
				rush_report_error();
				exit(0);
			}
		}
	}
}

// PERF: Writes to file, given a file_name; one will be created if it doesn't exist
void set_stream(char *file_name) {
	int file_desc = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(file_desc, STDOUT_FILENO);
	close(file_desc);
}

// PERF: Inserts NULL at first instance of IMPOSSIBLE_STRING in string array
void insert_null(char **argsv) {
	int i;
	for (i = 0; i < MAX_ARGS; i++) {
		if (strcmp(argsv[i], IMPOSSIBLE_STRING) == 0) {
			argsv[i] = NULL;
			return;
		}
	}
}
