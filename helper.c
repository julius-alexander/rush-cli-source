#include "helper.h"

void rush_cli_prompt(void) {
	fflush(stdout);
	fputs("rush> ", stdout);
	// fflush(stdout);
}

void rush_report_error(void) {
	fputs(ERROR_MSG, stdout);
	fflush(stdout);
}

// Parse raw user input
void rush_parse(char **argsv, char *raw_user_input, char **commands_list) {

	normalize_input(raw_user_input);

	// TODO: split by '&'
	// split_into_single_commands(commands_list, raw_user_input);

	// build argsv by tokenizing the normalized input
	rush_tokenize_command(argsv, raw_user_input);

	// for i in raw_single_commands
}

// PERF: resets to default vals, no need for realloc() or free()
void reset_str_arr(char ***arr, int arr_size) {
	int i;
	for (i = 0; i < arr_size; i++) {
		strcpy((*arr)[i], IMPOSSIBLE_STRING);
	}
}
void normalize_input(char *buffer) {
	// removes leading and trailing whitespace,
	// ensures exactly 1 whitespace char lies between words, '&', and '>'

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

	temp_buffer[j] = '\0'; // PERF: forgetting this line led to HOURS of bug-chasing
	strcpy(buffer, temp_buffer);
}

void rush_tokenize_command(char **argsv, char *raw_single_command) {

	char *token, *save_ptr;
	int i = 0;
	token = strtok_r(raw_single_command, " ", &save_ptr);
	for (i = 0; i < MAX_ARGS && token != NULL; i++) {
		strcpy(argsv[i], token);
		token = strtok_r(NULL, " ", &save_ptr);
	}

	strcpy(argsv[i], IMPOSSIBLE_STRING);
}

// TODO: implement
void split_into_single_commands(char **raw_single_commands, char *raw_user_input) {}

// PERF: Light work no reaction
void builtin_cd(char *path) {
	if (chdir(path) != 0) {
		rush_report_error();
	}
}

// PERF: Be so fr rn
void builtin_exit(void) { exit(0); }

// PERF: Looks good to me :3
void builtin_path(char **new_path, char **argsv) {
	int i;
	char temp[MAX_BUFFER];
	for (i = 0; i < MAX_PATHS; i++) {
		strcpy(temp, argsv[i]);
		strcat(temp, "/");
		strcpy(new_path[i], temp);
	}
}

// debug purposes only!
void print_str_arr(char **argsv) {
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

// initializes any 2d array with specified size of rows and cols
void init_str_arr(char ***arr, int rows, int cols) {

	*arr = (char **)calloc(rows, sizeof(char *));

	int i;
	for (i = 0; i < rows; i++) {
		(*arr)[i] = (char *)calloc(cols + 1, sizeof(char));
	}
}

// return the index of the path where the given command was found
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

// NOTE: Might not need this anymore; keeping, but possibly deprecated
int detect_parse_errors(char *trimmed_raw_input) {

	if (*trimmed_raw_input == '>' || *trimmed_raw_input == '&') {
		return TRUE;
	}

	// if '>>' or '&&' exists or
	// starts with '&' or '>'
	char *tmp;
	for (tmp = trimmed_raw_input; *tmp != '\0' && *(tmp + 1) != '\0'; tmp++) {
		if (((*tmp == '&') || (*tmp == '>')) &&
		    (*(tmp + 1) == '&' || *(tmp + 1) == '>')) {
			return TRUE; // might do (tmp + 2) because of "cmd1 & & cmd2"
		}
	}

	// or ends with '&' or '>'
	// then: report error and exit
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
			return;
		}
	}
}

int det_parse_errors(char **argsv_single_cmd) {
	int error_found = 0;

	// parse error to start with '&' or '>'
	// do not execute any command
	if (strcmp(argsv_single_cmd[0], "&") == 0 || strcmp(argsv_single_cmd[0], ">") == 0) {
		error_found = 1;
	}

	int i;
	// reminder: IMPOSSIBLE_STRING should already be replaced with NULL
	for (i = 0; argsv_single_cmd[i] != NULL; i++) {

		// ALL '&' should have been removed to make a list of single commands
		// so ANY remaining '&' are errors
		if (strcmp(argsv_single_cmd[0], "&") == 0) {
			error_found = 1;
		}
	}

	// parse error to end with '>'
	if (strcmp(argsv_single_cmd[i - 1], ">") == 0) {
		error_found = 1;
	}

	return error_found;
}

// write to file, given a file_name, one will be created if it doesn't exist
void set_stream(char *file_name) {
	int file_desc = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(file_desc, STDOUT_FILENO);
	close(file_desc);
}

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
				set_stream(argsv[i + 1]);
				strcpy(argsv[i], IMPOSSIBLE_STRING);
			}

			else {
				rush_report_error();
				exit(0);
			}
		}
	}
}
