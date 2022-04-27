/**
 * @file myShell.h
 * @author Usman Zaheer (Student# 1148583)
 * @date February 2021
 * @brief File containing the function definitions for all functions in myShell.c
 */

#include <stdio.h>       /* Input/Output */ 
#include <stdlib.h>      /* General Utilities */ 
#include <string.h>      /* String Processing */ 
#include <strings.h>     /* String Processing */ 
#include <ctype.h>       /* C Type Functions */ 
#include <unistd.h>      /* Symbolic Constants */ 
#include <errno.h>       /* Errors */ 
#include <fcntl.h>       /* Open and Write */   
#include <signal.h>      /* signal Functions */
#include <sys/types.h>   /* Primitive System Data Types */ 
#include <sys/wait.h>    /* Wait for Process Termination */   

/** Function to free any array of strings. 
 * @pre Pointer argument must not be NULL.
 * @post Frees any data malloced by the array of char pointers
 * @param stringArray - pointer to a char pointer (string array)
**/
void freeStringArray(char** stringArray);



/** Function to tokenize the string by using white space delimiters
 * @post The return double pointer must be freed by the caller of this function.
 * @return On a successful run, a double pointer containing pointers to each of the
 * tokenized values will be returned.
 * @param userString - char pointer to a string
**/
char** inputLineParse(char* userString); 



/** Function to initialize/parse a SVG struct while traversing the xmlDoc tree starting
 * at the xmlNode given.
 * @pre Pointer arguments must not be NULL.
 * @post It will process the command and call the necessary function to execute given command.
 * @return The return value will simply indicate the status of the command call.
 * if the return value is -1, A failure has occured with the given call
 * if the return value is 0, This simply means the user has called the exit command
 * if the return value is 1, The command that was called was sucessful and the shell will continue
 * @param userArguments - double char pointer (array of strings) containing the users command
**/
int findCommand(char** userArguments);



/** This function will return a double char pointer cointaining
 * all the command and flags within a given command call
 * @pre Pointer arguments must not be NULL.
 * @post The return double pointer must be freed by the caller of this function.
 * @return On a successful run, a double pointer containing pointers to each of the
 * tokenized commands/flags will be returned.
 * @param userArguments - double char pointer (array of strings) containing the users command
**/
char** parseCommands(char** userArguments);



/** Function to execute a linux command that is not built in by our
 * own shell (ls, <, >, etc).
 * @pre Pointer arguments must not be NULL.
 * @return On a successful execution, 1 will be returned. If for some reason there is failure, -1 is returned.
 * @param userArguments - double char pointer (array of strings) containing the users command
 * @param direction - A pointer containing the index of each redirection call (<, >), within the userArguments string array
 * @param isBackground - A function flag to let the function know whether we are executing the process in the background
**/
int executeCommand(char** userArguments, int* direction, int isBackground);



/** Built in cd command function. Based on the users command will process the according
 * cd call.
 * @pre Pointer arguments must not be NULL.
 * @return On a successful cd 1 is returned. Any failure will return -1
 * @param userArguments - double char pointer (array of strings) containing the users command
**/
int changeDirectory(char** userArguments);



/** This is the signal handler function to deal with background processes
 * whenever a signal is set on when its finished (or killed itself)
 * @post Global array will have been updated accordingly
**/
void signalHandler(int signal);



/** This function will initialize or edit the .CIS3110_profile file
 * based on whether it exists or not with the according values.
 * @post File must be closed by the calling function.
 * @return On a successful file creation/edit it will return a pointer to that file. Any failure will result in NULL return.
**/
FILE* getProfile();



/** This function will open the according file thats going to be used for redirection
 * (input redirection or output redirection)
 * @pre Pointer arguments must not be NULL.
 * @post The returned file must be closed by the calling function
 * @return On a successful run a file pointer to the redirection file will be returned. Any failure will result in NULL return.
 * @param userArguments - double char pointer (array of strings) containing the users command
 * @param index - The index in which the redirection symbol was found in userArguments (NOT the index of the filename itself)
**/
FILE* getFile(char** userArguments, int index);



/** Built in history command function. Based on the users command will process the according
 * history call with flags.
 * @pre Pointer arguments must not be NULL.
 * @return On a successful command call 1 will be returned. Any errors within the function will result in -1 return.
 * @param userArguments - double char pointer (array of strings) containing the users command
**/
int displayHistory(char** userArguments);



/** Helper function to get the number of lines in a file given file name
 * @pre Pointer arguments must not be NULL.
 * @return On a successful run it will return number of lines in the given file. 
 * Any errors within the function will result in -1 return.
 * @param filename - char pointer that contains the file name
**/
int getFileLines(char* filename);



/** Helper function to clear a file, given the file name
 * @pre Pointer arguments must not be NULL.
 * @param filename - char pointer that contains the file name
**/
void clearFile(char* filename);



/** Helper function to print the shell menu, along with any background processes that have finished
 * executing just like the normal shell
 * @post The Backgroud process global array will be changed
**/
void printShell();
