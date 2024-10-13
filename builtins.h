#ifndef _CONSTANTS_H
#define _CONSTANTS_H

// Built-in commands
void builtin_exit(void);
void builtin_cd(char *path);
void builtin_path(char **new_path, char **argsv);

#endif
