#include "rush.h"

void rush_cli_prompt(void) {
	fflush(stdout);
	fputs("rush> ", stdout);
	// fflush(stdout);
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

void rush_report_error(void) {
	fputs(ERROR_MSG, stdout);
	fflush(stdout);
}
