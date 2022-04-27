Course: CIS 3110 W22
Professor: Deborah Stacey
Due Date: February 2, 2022
Program Name: myShell
Program Description: my version of a simple Bash shell with various built-in commands (cd and history)

Compilation (All of the below work):

1. make
2. gcc -std=gnu99 -Wpedantic -o myShell myShell.c
3. make myShell

Execution Testing/Sample:

./myShell
ls
ls -lt
ls -al > foo
wc < foo > output (SEE DISCLAIMER AT THE BOTTOM BEFORE RUNNING THIS)
cd ..
cd ..
cd (back to old directory)
ls
cd ~
ls
history 3
history
sleep 4 &
sleep 5
(Press enter key)
./bgSleep 3 &
exit
Repeat all of above again without recompiling the program. (Test for environment variable initialization)

Functionality:

myShell includes all internal shell commands as well as 2 built-in shell commands(cd and history)
Everything specified by Deborah Stacey in the assignment description and the examples provided is fully functional except set 2 part 4 (pipes) and set 3 part 10 (export function).

Functions:

All internal commands with their respective flags:
- ls, sort, ./executable, make, mkdir directoryName, etc.
Two Built-in commands with the given flag requirements:
- cd and history

Built-in Functions

history:

I have implemented the shell command history which displays all history of the shell in the format given in the assignment description. The history command also supports both the -c flag and a number flag to indicate how much of recent history to print.
"history -c" will clear the .CIS3110_history file
"history n" will show the last n commands.

cd

The cd command can be used to change directories and there are 3 parameters: "..", "~", "PATH"
"cd .." will result in the user going back one directory
"cd ~" will result in the user going to the home directory as set by the HOME environment variable
"cd PATH" will cd to the PATH environment variable directory.
"cd directoryPath" will change to the given directory to a directory given by the user path name.

Files Created:

.CIS3110_history is created if it doesn't already exist
.CIS3110_profile is created with defaults if it doesn't already exist
Files will also be created by redirects if they honest already exist in the working directory

Error Checking:

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

Assumptions:

This program assumes that the user will only deal with 3 environment variables - myPATH, myHOME, and myHISTFILE.

Limitations:

The only limitation is that my program does not support the usage of pipes NOR have I implemented the export built in command. However all other commands and functions were implemented accordingly.

Future Improvements:

The main problem with starting this is that I didn't know what kind of structures to use, what information/processing I should put in what function and just overall how to organize and implement it to make it as easy for me and concise as possible. If I were to Redo this assignment again my first step would be to really internalize everything before hand making sure I understand the whole process and what runs first and in which order. Then I would create a plan using pen and paper to try and map out what problems I need to break up and how I can implement them into my design while trying to incorporate necessary data structures.

Miscellaneous:

On the assignment description, it tells us to use /bin as the default path for $PATH if the user does not include a .CIS3110_profile file. I'm on the schools linux server and if you use 
/bin as the environment variable for $PATH it doesn't allow the user to use wc or sort, but allows for other basic Linux commands. I do not know if the TA's /bins are different, I had the path set to /usr/bin:/bin before and everything worked so I just wanted to leave a disclaimer incase sort or wc isn't working..