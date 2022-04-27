#include "myShell.h"

/*
These globals structs are to store IDs of background processes
so that we can keep track of all of them and print out 
any finished background processes. Background count 
keeps tracks of the number of finished processes to print.
*/
pid_t allBackground[100];
pid_t finishedBackground[50];
int backgroundCount = 0;
int numCommands = 1;

void freeStringArray(char** stringArray) {
    if(stringArray == NULL) {
        printf("Pointer argument should not be NULL\n");
        return;
    }

    int count = 0;
    while(stringArray[count] != NULL) {
        free(stringArray[count]);
        count++;
    }
    free(stringArray);
}

char** inputLineParse(char* userString) {   
    char** userArguments = malloc(sizeof(char*));
    if(userArguments == NULL) {
        fprintf(stderr, "Malloc failed in inputLineParse\n");
        return NULL;
    }

    char* tempArg = strtok(userString, " \t\r\n\v\f");
    int i = 0;

    while(tempArg != NULL) {
        int tempLen = strlen(tempArg) + 1;
        userArguments[i] = (char*)malloc(sizeof(char) * tempLen);
        if(userArguments[i] == NULL) {
            fprintf(stderr, "Malloc failed in inputLineParse 2\n");
            return NULL;
        }

        strcpy(userArguments[i], tempArg);
        i++;

        userArguments = realloc(userArguments, sizeof(char*) * (i + 1));
        if(userArguments == NULL) {
            fprintf(stderr, "Malloc failed in inputLineParse 3\n");
            return NULL;
        }

        tempArg = strtok(NULL, " \t\r\n\v\f");
    }

    userArguments[i] = NULL;

    if(i == 0) {
        freeStringArray(userArguments);
        return NULL;
    }

    return userArguments;
}

int findCommand(char** userArguments) {
    if(userArguments == NULL) {
        fprintf(stderr, "userArguments argument cannot be NULL\n");
        return -1;
    }
    int status = 1;
    int isBackground = 0;
    int* hasDirection = malloc(sizeof(int));
    if(hasDirection == NULL) {
        fprintf(stderr, "Malloc failed in findCommand\n");
        return -1;
    }
    hasDirection[0] = -1;
    int count = 0;
    int i = 0;

    if(strcasecmp(userArguments[0], "exit") == 0) {
        status = 0;
    }

    else if(strcasecmp(userArguments[0], "cd") == 0) {
        status = changeDirectory(userArguments);
    }

    else if(strcasecmp(userArguments[0], "history") == 0) {
        status = displayHistory(userArguments);
    }

    else {
        while(userArguments[i] != NULL) {
            if(strcmp(userArguments[i], ">") == 0 || strcmp(userArguments[i], "<") == 0) {
                if(userArguments[i + 1] == NULL) {
                    printf("myShell: syntax error near unexpected token 'newline\n");
                    free(hasDirection);
                    return status;
                }
                else {
                    //Record the indices of each redirection and store it into an int pointer
                    hasDirection[count] = i;
                    count++;
                    hasDirection = realloc(hasDirection, sizeof(int) * (count + 1));
                    if(hasDirection == NULL) {
                        fprintf(stderr, "Realloc failed in findCommand\n");
                        return -1;
                    }
                    hasDirection[count] = -1;
                }
            }
            i++;
        }
        if(strcmp(userArguments[i - 1], "&") == 0) {
            //If process is supposesd to be run in background, then do so
            isBackground = 1;
        }
        status = executeCommand(userArguments, hasDirection, isBackground);
    }
    free(hasDirection);
    return status;
}

char** parseCommands(char** userArguments) {
    if(userArguments == NULL) {
        fprintf(stderr, "userArguments argument cannot be NULL\n");
        return NULL;
    }
    int i = 0;
    int indexCount = 0;
    char** arrayToReturn = malloc(sizeof(char*));
    if(arrayToReturn == NULL) {
        fprintf(stderr, "Malloc failed in parseCommands\n");
        return NULL;
    }
    
    while(userArguments[i] != NULL) {
        if(strcmp(userArguments[i], ">") == 0 || strcmp(userArguments[i], "<") == 0 || strcmp(userArguments[i], "&") == 0) {
            break;
        }
        else {
            int memLen = strlen(userArguments[i]) + 1;
            arrayToReturn[indexCount] = (char*)malloc(sizeof(char) * memLen);
            if(arrayToReturn[indexCount] == NULL) {
                fprintf(stderr, "Malloc failed in parseCommands 2\n");
                return NULL;
            }
            strcpy(arrayToReturn[indexCount], userArguments[i]);
            indexCount++;

            arrayToReturn = realloc(arrayToReturn, sizeof(char*) * (indexCount + 1));
            if(arrayToReturn == NULL) {
                fprintf(stderr, "Malloc failed in parseCommands 3\n");
                return NULL;
            }
        }
        i++;
    }
    arrayToReturn[indexCount] = NULL;
    return arrayToReturn;
}

int executeCommand(char** userArguments, int* direction, int isBackground) {
    if(userArguments == NULL || direction == NULL) {
        fprintf(stderr, "userArguments argument cannot be NULL\n");
        return -1;
    }
    int status = 0;
    int execResult = 1;
    pid_t newProcessID;
    int processCounter = 0;

    newProcessID = fork();

    if(newProcessID < (pid_t)(0)) {
        fprintf(stderr, "Child process creation failed\n");
        return -1;
    }

    else { //Fork Success
        if(newProcessID == (pid_t)(0)) { //CHILD PROCESS
            FILE* finalFile = NULL;
            char** userCommand;
            int count = 0;
            int fileFail = 0;

            //Extract core command and flags
            userCommand = parseCommands(userArguments);
            if(userCommand == NULL) {
                return -1;
            }
            while(direction[count] != -1) {
                //get every file listed for redirection
                finalFile = getFile(userArguments, direction[count]);
                if(finalFile == NULL) {
                    fileFail = -1;
                }
                count++;
            }

            if(fileFail != -1) {
                //execute command if no file failure occured (only check if redirection occurs)
                execResult = execvp(userCommand[0], userCommand);
            }

            //Cleanup and process if exec failed
            if(finalFile != NULL) {
                fclose(finalFile);
            }
            freeStringArray(userCommand);
            if(execResult < 0) {
                printf("myShell: %s: command not found:\n", userArguments[0]);
                exit(execResult);
            }
        }

        else { //PARENT
            //Store any new background processes in background process array.
            if(isBackground == 1) {
                pid_t result = waitpid(newProcessID, &status, WNOHANG);
                if(result != -1) {
                    for(int i = 0; i < 100; i++) {
                        if(allBackground[i] == 0) {
                            printf("[%d] %d\n", i + 1, newProcessID);
                            allBackground[i] = newProcessID;
                            break;
                        }
                    }
                }
            }
            //If regular process simply wait.
            else {
                (void)waitpid(newProcessID, &status, 0);
            } 
        }
    }
    return 1;
}

int changeDirectory(char** userArguments) {
    if(userArguments == NULL) {
        fprintf(stderr, "userArguments argument cannot be NULL\n");
        return -1;
    }
    if(strncmp(userArguments[1], "..", 2) == 0) {
        if(chdir(userArguments[1]) != 0) {
            fprintf(stderr, "Moving up directory failed\n");
            return -1;
        }
    }

    else if(strncmp(userArguments[1], "~", 1) == 0) {
        if(chdir(getenv("HOME")) != 0) {
            fprintf(stderr, "Moving to HOME directory failed\n");
            return -1;
        }
    }

    else if(strcasecmp(userArguments[1], "PATH") == 0 || strcasecmp(userArguments[1], "myPATH") == 0) {
        if(chdir(getenv("PATH")) != 0) {
            fprintf(stderr, "Moving to PATH directory failed\n");
            return -1;
        }
    }

    //Else change to whatever directory the user specififed
    else {  
        int dirExist;
        int memLen = strlen(userArguments[1]) + 1;
        char* givenPath = (char*)malloc(sizeof(char) * memLen);

        strcpy(givenPath, userArguments[1]);

        if(chdir(givenPath) != 0) {
            fprintf(stderr, "myShell: %s No such file or directory\n", givenPath);
        }

        free(givenPath);
    }

    return 1;
}

FILE* getFile(char** userArguments, int index) {
    if(userArguments == NULL) {
        fprintf(stderr, "userArguments argument cannot be NULL\n");
        return NULL;
    }
    FILE* finalFile;
    char* fileName;
    int memLen = 0;
    int i = 0;

    memLen = strlen(userArguments[index + 1]) + 1;

    fileName = (char*)malloc(sizeof(char) * memLen);
    if(fileName == NULL) {
        fprintf(stderr, "Malloc failed in getFile\n");
        return NULL;
    }

    //check if file to be openeed is for input
    if(strcmp(userArguments[index],"<") == 0) {
        strcpy(fileName, userArguments[index + 1]);
        if(access(fileName, F_OK) == 0) {
            finalFile = freopen(fileName, "r", stdin);
            if(finalFile == NULL) {
                fprintf(stderr, "Opening file failed in getFile 1\n");
                free(fileName);
                return NULL;
            }
        }
        else {
            printf("myShell: %s: No such file or directory\n", fileName);
            free(fileName);
            return NULL;
        }
    }

    //check if file to be opened is for output
    else if(strcmp(userArguments[index],">") == 0) {
        strcpy(fileName, userArguments[index + 1]);
        finalFile = freopen(fileName, "w", stdout);
        if(finalFile == NULL) {
            fprintf(stderr, "Opening file failed in getFile 2\n");
            free(fileName);
            return NULL;
        }
    }

    free(fileName);
    return finalFile;
}

void signalHandler(int signal) {
    pid_t currProcess;

    //Anytime a background process finishes, add it to the finshed processes list to be printed
    for (int i = 0; i < 100; i++) {
        if(allBackground[i] != 0) {
            currProcess = waitpid(allBackground[i], NULL, WNOHANG);
            if(currProcess > 0) {
                finishedBackground[backgroundCount] = currProcess;
                backgroundCount++;
                allBackground[i] = 0;
            }
        }
    }
}

FILE* getProfile() {
    char homeDir[500];
    strcpy(homeDir, getenv("HOME"));
    strcat(homeDir, "/.CIS3110_profile");
    FILE* profile = fopen(homeDir, "r");
    char userDir[500];
    char fileLine[500];
    int j = 0;
    int k = 14;
    
    //If profile already exists, update environment variables
    //with export values specified in the file
    if(profile != NULL) {
        for(int i = 0; i < 3; i++) {
            char* pathExportName;
            char* homeExportName;
            char* histExportName;
    
            if(fgets(fileLine, 500, profile) == NULL) {
                fprintf(stderr, "Error occurred in getting profile File line in getProfile\n");
                return NULL;
            }
            if(strncmp(fileLine, "export myPATH", 13) == 0) {
                pathExportName = strchr(fileLine, '/');
                pathExportName[strlen(pathExportName) - 1] = '\0';
                setenv("PATH", pathExportName, 1);
                k = 14;
                j = 0;
            }
            if(strncmp(fileLine, "export myHOME", 13) == 0) {
                homeExportName = strchr(fileLine, '/');
                homeExportName[strlen(homeExportName) - 1] = '\0';
                setenv("HOME", homeExportName, 1);
                k = 18;
                j = 0;
            }
            if(strncmp(fileLine, "export myHISTFILE", 17) == 0) {
                histExportName = strchr(fileLine, '/');
                histExportName[strlen(histExportName) - 1] = '\0';
                setenv("HISTFILE", histExportName, 1);
                k = 14;
                j = 0;
            }
        }
    }

    /*
    If file does not exist assign new environment variables
    along with the profile file for future myShell initializations all
    given environment variables are the ones specified in the assignment description
    */
    else {
        if(getcwd(userDir, 500) == NULL) {
            fprintf(stderr, "Error occurred  getting directory in getProfile\n");
            return NULL;
        }
        profile = fopen(homeDir, "w+");
        if(profile == NULL) {
            fprintf(stderr, "Error occurred in opening profile File in getProfile\n");
            return NULL;
        }
        char histDir[500];
        strcpy(histDir, getenv("HOME"));
        strcat(histDir, "/.CIS3110_history");

        fprintf(profile, "export myPATH=/bin\n");
        fprintf(profile, "export myHOME=%s\n", getenv("HOME"));
        fprintf(profile, "export myHISTFILE=%s\n", histDir);
        setenv("PATH", "/bin", 1);
        setenv("HOME", getenv("HOME"), 1);
        setenv("HISTFILE", histDir, 1);
    }
    return profile;
}

int displayHistory(char** userArguments) {
    if(userArguments == NULL) {
        fprintf(stderr, "userArguments argument cannot be NULL\n");
        return -1;
    }
    char currCommand[500];
    char historyDir[500];
    strcpy(historyDir, getenv("HISTFILE"));
    int index = -1;
    int isClear = -1;
    int count = 0;

    //check for flags
    while(userArguments[count] != NULL) {
        if(strcmp(userArguments[count], "-c") == 0) {
            isClear = 1;
        }
        else if(isdigit(userArguments[count][0]) != 0) {
            index = userArguments[count][0] - '0';
        }
        count++;
    }

    //execute based on flags
    if(isClear == 1) {
        clearFile(historyDir);
    }

    else if(index != -1) {
        int numLinesInFile = getFileLines(historyDir);
        int currCount = 0;
        FILE* readerFile = fopen(historyDir, "r");
        if(readerFile != NULL) {
            while(fgets(currCommand, 500, readerFile) != NULL) {
                currCount++;
                if(currCount > (numLinesInFile - index)) {
                    printf("%s", currCommand);
                }
            }
            fclose(readerFile);
        }
    }

    else {
        FILE* readerFile = fopen(historyDir, "r");
        if(readerFile != NULL) {
            while(fgets(currCommand, 500, readerFile) != NULL) {
                printf("%s", currCommand);
            }
        }
        fclose(readerFile);
    }

    return 1;
}

int getFileLines(char* filename) {
    if(filename == NULL) {
        fprintf(stderr, "filename argument cannot be NULL\n");
        return -1;
    }

    FILE* givenFile = fopen(filename, "r");
    int count = 0;

    if(givenFile == NULL) {
        fprintf(stderr, "Could not open given file");
        return -1;
    }

    //loop through each character in file, while counting all \n chars (num lines)
    for(char c = getc(givenFile); c != EOF; c = getc(givenFile)) {
        if(c == '\n') {
            count++;
        }
    }

    fclose(givenFile);
    return count;
}

void clearFile(char* filename) {
    if(filename == NULL) {
        fprintf(stderr, "filename argument cannot be NULL\n");
        return;
    }

    //If file exists, clear it
    if(access(filename, F_OK) == 0) {
        FILE* givenFile = fopen(filename, "w");
        fclose(givenFile);
        numCommands = 1;
    } 
}

void printShell() {
    char userDir[500];
    int totalCount = backgroundCount;

    signal(SIGCHLD, signalHandler);

    if(getcwd(userDir, 500) == NULL) {
        fprintf(stderr, "Error occurred getting directory in printShell\n");
    }

    //Print all finished background processes
    for(int i = 0; i < totalCount; i++) {
        printf("[%d]+ done:\t%d\n", i + 1, (int)finishedBackground[i]);
        finishedBackground[i] = 0;
        backgroundCount--;
        fflush(stdin);
    }
    //Print users current working directory
    printf("%s> ", userDir);

}

/*
Driver Function also the main loop of the shell
STATUS = 1 MEANS CONTINUE SHELL, STATUS = -1 SHELL ERROR, STATUS = 0 END SHELL
*/
int main(int argc, char **argv) {
    FILE* userProfile;
    FILE* userHistory;
    char userHome[500];
    int status = 1;
    char** userArguments;

    //Initializing for use in shell
    for(int i = 0; i < 100; i++) {
        allBackground[i] = 0;
        if(i < 50) {
            finishedBackground[i] = 0;
        }
    }

    userProfile = getProfile();
    strcpy(userHome, getenv("HOME"));
    strcat(userHome, "/.CIS3110_history");
    userHistory = fopen(userHome, "w+");

    if(userHistory == NULL || userProfile == NULL) {
        fprintf(stderr, "Bash file creation/mutation failed\n");
    }

    printShell();
    char* userInp = (char*)malloc(sizeof(char) * 500);
    if(userInp == NULL) {
        fprintf(stderr, "Malloc failed in main\n");
        exit(-1);
    }

    if(fgets(userInp, 500, stdin) == NULL) {
        fprintf(stderr, "fgets failed in main outside loop\n");
        exit(-1);
    }
    fprintf(userHistory, " %d  %s", numCommands++, userInp);
    fclose(userHistory);
    
    //Main shell loop
    while(status != 0) {
        userArguments = inputLineParse(userInp);
        free(userInp);
        if(userArguments != NULL) {
            status = findCommand(userArguments);
            freeStringArray(userArguments);
        }

        if(status == 0) {
            break;
        }
        else if(status == -1) {
            printf("System error occurred: Terminating Shell\n");
            exit(status);
        }

        printShell();
        userInp = (char*)malloc(sizeof(char) * 500);
        if(userInp == NULL) {
            fprintf(stderr, "Malloc failed in main\n");
            exit(-1);
        }

        userHistory = fopen(userHome, "a");
        if(fgets(userInp, 500, stdin) == NULL) {
            fprintf(stderr, "fgets failed in main INSIDE loop\n");
            exit(-1);
        }
        fprintf(userHistory, " %d  %s", numCommands++, userInp);
        fclose(userHistory);
    }

    fclose(userProfile);
    printf("myShell is terminating\n");
    //Kill all processes (associated with myShell)
    kill(0, SIGINT);
 
   return 0;
}
