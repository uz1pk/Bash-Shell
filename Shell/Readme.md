## Compilation (All of the below work):

1. make
2. gcc -std=gnu99 -Wpedantic -o myShell myShell.c
3. make myShell

## Execution Testing/Sample:

- ./myShell
- ls
- ls -lt
- ls -al > foo
- wc < foo > output
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
"history n" will show the last n commands.

### cd

The cd command can be used to change directories and there are 3 parameters: "..", "~", "PATH"
"cd .." will result in the user going back one directory
"cd ~" will result in the user going to the home directory as set by the HOME environment variable
"cd PATH" will cd to the PATH environment variable directory.
"cd directoryPath" will change to the given directory to a directory given by the user path name.

## Assumptions:

- This program assumes that the user will only deal with 3 environment variables - myPATH, myHOME, and myHISTFILE.

## Miscellaneous (Important):

I have /bin as the environment variable for $PATH which doesn't allow the user to use wc or sort, but allows for other basic Linux commands. To fix it set your path manually in the .Profile file to this: /usr/bin:/bin and everything will work.
