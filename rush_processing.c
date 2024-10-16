/* ======================================================================================
 *
 * rush_processing.c - Source file for input processing functions
 *
 * This file is part of rush, Rapid UNIX Shell
 *
 * Date: 13 October 2024
 *
 * License: This project is for educational purposes only and is not intended for redistribution.
 *
 * ====================================================================================== */

#include "rush_processing.h"
#include "rush_core.h"

// PERF: Removes leading, trailing whitespace; ensures only 1 space between args
void normalize_input(char *raw_user_input) {
	// if raw_user_input already matches MAX_BUFFER, we will overflow when
	// attempting to insert whitespace between operators
	char temp_buffer[MAX_BUFFER * 4];
	int i = 0, j = 0;
	int word_flag = 0;

	// write valid contents of buffer to temp_buffer
	for (i = 0, j = 0; i < strlen(raw_user_input); i++) {

		// encounter any char
		if (!isspace(raw_user_input[i])) {
			word_flag = 1;

			// surround & and > with exactly 1 whitespace
			if (raw_user_input[i] == '&' || raw_user_input[i] == '>') {
				if (j > 0 && temp_buffer[j - 1] != ' ') {
					temp_buffer[j++] = ' ';
				}
				temp_buffer[j++] = raw_user_input[i];
				temp_buffer[j++] = ' ';
			}

			else {
				temp_buffer[j++] = raw_user_input[i];
			}
		}

		// If the character is a space, insert exactly 1 whitespace char between words
		else if (word_flag) {
			if (j > 0 && temp_buffer[j - 1] != ' ') {
				temp_buffer[j++] = ' ';
			}
			word_flag = 0;
		}
	}

	temp_buffer[j] = '\0';	  // NOTE: Forgetting this line led to HOURS of bug-chasing '\001'
	strcpy(raw_user_input, temp_buffer);
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

	strcpy(tokens_list[i], IMPOSSIBLE_STRING);	  // sentinel value
}

// PERF: Checks and executes if command is a builtin
int chexec_builtin(char **argsv, char **user_path) {
	int isBuiltin = FALSE;

	// CD should have exactly 1 argument!
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

		else
			rush_report_error();

	}

	// EXIT should never be run with args
	else if (strcmp(argsv[0], "exit") == 0) {
		isBuiltin = TRUE;

		if (strcmp(argsv[1], IMPOSSIBLE_STRING) == 0) {
			builtin_exit();
		}

		else
			rush_report_error();

	}

	else if (strcmp(argsv[0], "path") == 0) {
		isBuiltin = TRUE;
		builtin_path(user_path, argsv);
	}

	return isBuiltin;
}

// PERF: Returns the index of the path if the given command is found
int located_path(char *cmd, char **paths_to_search) {
	char temp[MAX_BUFFER];

	int i;
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
			// Make sure there is exactly ONE arg after '>'
			if (strcmp(argsv[i + 1], IMPOSSIBLE_STRING) != 0 &&
				strcmp(argsv[i + 2], IMPOSSIBLE_STRING) == 0) {

				// Redirect output to a file with the same name as that ONE arg
				int file_desc = open(argsv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(file_desc, STDOUT_FILENO);
				close(file_desc);
				strcpy(argsv[i], IMPOSSIBLE_STRING);	// clear the '>' symbol
			}

			else {
				rush_report_error();
				exit(0);
			}
		}
	}
}

// PERF: Inserts NULL at first instance of IMPOSSIBLE_STRING in string array
void insert_null(char **str_arr) {
	int i;
	for (i = 0; i < MAX_ARGS; i++) {
		if (strcmp(str_arr[i], IMPOSSIBLE_STRING) == 0) {
			str_arr[i] = NULL;
			return;
		}
	}
}
