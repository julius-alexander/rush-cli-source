# RUSH - Rapid UNIX Shell

- Rush is a simple UNIX-like shell that supports redirection and parallel commands
- User input is supported only up to 255 chars, longer input sequences may cause undefined behavior

## Features

- Three built-in commands: cd, path, and exit

- cd takes in exactly 1 argument, the path of the directory specified; it is an error to pass 0 or >1 args
- path supports up to 20 unique paths that the shell can search through for a specified command.
  Passing 0 commands will allow only built-in commands to run, and the default path is set to /bin on start-up
- exit simply quits rush, and it is an error to run exit with any args

- In addition to built-in commands, rush also supports redirection and parallel commands

- redirection is only supported for 1 output target, for example:
  `echo 123 > target.txt target2.txt` is not allowed, since there are two args to the right of the redirection symbol
- parallel commands run independently of each other, so it is possible for one command to succeed and another to fail
  for example: `pwd & not_a_command` will return the current working directory, AND the message "An error has occurred"
