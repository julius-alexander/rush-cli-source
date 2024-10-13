#include "rush.h"
#include <stdio.h>
#include <stdlib.h>

// PERF: Pretty obvious ngl
void rush_cli_prompt(void) {
	fflush(stdout);
	fputs("rush> ", stdout);
	// fflush(stdout);
}

// PERF: Trivial
void rush_report_error(void) {
	fputs(ERROR_MSG, stdout);
	fflush(stdout);
}

// PERF: Does what it needs to do
void rush_parse(char *raw_user_input, char **commands_list) {

	// Normalizes whitespace only
	normalize_input(raw_user_input);

	// Splits input by '&' only
	tokenize_by_delim(commands_list, raw_user_input, "&");
}

// echo 1 & echo 2 & echo 3 & echo 4 & echo 5 & echo 6
