#ifndef PROCESSING
#define PROCESSING

// Command processing functions
void normalize_input(char *buffer);
void split_into_single_commands(char **raw_single_commands, char *raw_user_input);
void rush_tokenize_command(char **argsv, char *raw_user_input);
int che_x_builtin(char **argsv, char **user_path);
int located_path(char *cmd, char **paths_to_search);
void redirection_handler(char **argsv);
void set_stream(char *file_name);
void insert_null(char **argsv);

#endif
