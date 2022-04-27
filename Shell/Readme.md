# Usman Zaheer CIS 3110 (Shell)

## Compilation (All of the below work):

1. make
2. gcc -std=gnu99 -Wpedantic -o myShell myShell.c
3. make myShell

## Execution Testing/Sample:

- ./myShell
- ls
- ls -lt
- ls -al > foo
- wc < foo > output (SEE DISCLAIMER AT THE BOTTOM BEFORE RUNNING THIS)
- cd ..
- cd ..
- cd (back to old directory)
- ls
- cd ~
- ls
- history 3
- history
- sleep 4 &
- sleep 5
- (Press enter key)
- ./bgSleep 3 &
- exit <br>
Repeat all of above again without recompiling the program. (Test for environment variable initialization)

## Functionality:

myShell includes all internal shell commands as well as 2 built-in shell commands(cd and history). The shell does not handle pipes/piping

## Functions:

All internal commands with their respective flags:
- ls, sort, ./executable, make, mkdir directoryName, etc.
Two Built-in commands with the given flag requirements:
- cd and history

## Built-in Functions

### history:

I have implemented the shell command history which displays all history of the shell in the format given in the assignment description. The history command also supports both the -c flag and a number flag to indicate how much of recent history to print.
"history -c" will clear the .CIS3110_history file
"history n" will show the last n commands.

### cd

The cd command can be used to change directories and there are 3 parameters: "..", "~", "PATH"
"cd .." will result in the user going back one directory
"cd ~" will result in the user going to the home directory as set by the HOME environment variable
"cd PATH" will cd to the PATH environment variable directory.
"cd directoryPath" will change to the given directory to a directory given by the user path name.

## Files Created:

- .CIS3110_history is created if it doesn't already exist
- .CIS3110_profile is created with defaults if it doesn't already exist <br>

## Error Checking:

myShell has various methods of error checking
- If a command does not exist, myShell will report the error to the user. The shell will continue to run
- If a command with necessary parameters is ran but no parameters are entered the shell will detect this and let the user know.
- If a a file to be executed does not exist, myShell will let the user know that the file does not exist. The shell will continue to run.
- If a command is ran with unknown parameters, myShell will report the error to the user
- myShell will report any errors involved with fork() failures and file opening failures
- myShell will also report and log any memory allocation or file opening errors
- When using input redirection (<). My shell will detect and lets the user know if the file
Being accessed for input redirection exists or not. 
- myShell also has approbate user error handling, meaning even if the user inputs values which aren't supposed to be used, the shell won't just crash it will continue to run while also printing the same error handling statements that normal bash would do.

## Assumptions:

- This program assumes that the user will only deal with 3 environment variables - myPATH, myHOME, and myHISTFILE.

## Limitations:

- The only limitation is that my program does not support the usage of pipes NOR have I implemented the export built in command. However all other commands and functions were implemented accordingly. The only other limitation is that the user may have to manually change the environment variable profile to access/edit the correct PATH location based on which shell commands they want to run.

## Future Improvements:

The main problem with starting this is that I didn't know what kind of structures to use, what information/processing I should put in what function and just overall how to organize and implement it to make it as easy for me and concise as possible. If I were to Redo this assignment again my first step would be to really internalize/understand everything before I even write a line of code to understand the entire system/program design. Then I would draw out the system to get an idea of how it would work and create data structures according to what I would need.

## Miscellaneous (Important):

I have /bin as the environment variable for $PATH which doesn't allow the user to use wc or sort, but allows for other basic Linux commands. To fix it set your path manually in the .CIS3110_Profile file to this: /usr/bin:/bin and everything will work.