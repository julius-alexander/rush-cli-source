#ifndef _CORE_
#define _CORE_

// String impossible to type accidentally, used as a placeholder for safe iteration
#define IMPOSSIBLE_STRING "˜Ø—„ÅÁ—ÁØ¨—ˇÁ∏´Î—ˇÓˆÍ—Ø˜—ÅÇÇˆÎ´˜ˇ\0"
#define ERROR_MSG         "An error has occurred\n"
#define MAX_BUFFER        255 // maximum raw user input buffer size
#define MAX_CMDS          128 // max number of possible commands to run in parallel
#define MAX_ARGS          128 // max number of args we can have in **argsv
#define MAX_PATHS         20  // max number of paths we can have defined
#define TRUE              1
#define FALSE             0

// Global variables
extern char **user_path;         // list of valid paths to check
extern char **commands_list;     // will be cmds_list
extern char **argsv;             // a standard argv to be passed into exec
extern char path_to_cmd[255];    // to be passed in execv
extern char raw_user_input[255]; // pre-parsed input

// Core functions
/** @brief initializes global variables to safe values **/
void rush_init(void);
void rush_cli_prompt(void);
void rush_parse(char **argv, char *raw_user_input, char **raw_single_commands);
void rush_report_error(void);

#endif
