#include "rush.h"

// PERF: Be so fr rn
void builtin_exit(void) { exit(0); }

// PERF: Light work no reaction
void builtin_cd(char *path) {
	if (chdir(path) != 0) {
		rush_report_error();
	}
}

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
