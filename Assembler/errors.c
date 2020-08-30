#include "errors.h"

/* If an error occurs, faultyFile is turned on and no output files will be made */
bool faultyFile;

/* current file name, used for error messages */
char currentFileName[MAX_FILE_NAME+EXTENTION_LENGTH];

/* getting (before finishing a file) and resetting (when starting a new file) faultyFile flag */
bool getFaultyFile(){return faultyFile;}
void setFaultyFile(){faultyFile = false;}

char *getFileName(){return currentFileName;}

/* tracks current line number to help with error messages */
int currentLineNumber = 1;

/*
    Prints the error in a line of code in the format:
    FILENAME:LINE: ERROR

    updates faulty file
    used for lines that only do one thing

    Example:
    assembly.as:5:13: Illegal charecter
*/
void printLineError(char* error){
    faultyFile = true;
    fprintf(stderr, RED);
    fprintf(stderr, "ERROR: %s:%d: %s\n", currentFileName, currentLineNumber, error);
    fprintf(stderr, WHT);
}

/*
    Prints the error in the code in the c format:
    FILENAME:LINE:COLUMN: ERROR

    updates faulty file

    Example:
    assembly.as:53 Illegal charecter
*/
void printCodeError(char* error, int column){
    faultyFile = true;
    fprintf(stderr, RED);
    fprintf(stderr,"ERROR: %s:%d:%d: %s\n", currentFileName, currentLineNumber, column+1, error);
    fprintf(stderr, WHT);
}

/*
    Prints the error in the opcode in the c format:
    FILENAME:LINE:COLUMN: ERROR

    updates faulty file
*/
void printCustomCodeError(const char *error, int column, const char* arg, int attribute)
{
    faultyFile = true;
    fprintf(stderr, RED);
    fprintf(stderr, "ERROR: %s:%d:%d: ", currentFileName, currentLineNumber, column + 1);
    fprintf(stderr, WHT);
    fprintf(stderr, error,arg,attribute);
    fprintf(stderr,"\n");
}

/*
    Prints the error in the opcode in the c format:
    FILENAME:LINE: ERROR

    updates faulty file
*/
void printCustomLineError(const char *error, int lineNum, const char *arg)
{
    faultyFile = true;
    fprintf(stderr, RED);
    fprintf(stderr, "ERROR: %s:%d: ", currentFileName, lineNum);
    fprintf(stderr, WHT);
    fprintf(stderr, error, arg);
    fprintf(stderr, "\n");
}

/*
    Prints the warning in the code in the c format:
    FILENAME:LINE:COLUMN: ERROR

    does not update faulty file, as a file wit a warning is still compilabe.

    Example:
    Warning assembly.as:53:6 Declaring a label as an extern twice is useless.
*/
void printCodeWarning(char* warning, int column){
    fprintf(stderr, YEL);
    fprintf(stderr, "WARNING: %s:%d:%d: %s\n", currentFileName, currentLineNumber, column+1, warning);
    fprintf(stderr, WHT);
}

/*
    Prints the warning in the code in the following:
    FILENAME:LINE: ERROR

    used for lines that only do one thing
    does not update faulty file, as a file wit a warning is still compilabe.

    Example:
    assembly.as:53:6: Duplicate .extern decleration
*/
void printLineWarning(char* warning){
    fprintf(stderr, YEL);
    fprintf(stderr, "WARNING: %s:%d: %s\n", currentFileName, currentLineNumber, warning);
    fprintf(stderr, WHT);
}

/*
    Prints the error in the file:
    FILENAME: ERROR

    Example:
    assembly.as: File doesn't exist 
*/
void printFileError(char* error){
    fprintf(stderr, RED);
    fprintf(stderr, "ERROR: %s: %s\n", currentFileName, error);
    fprintf(stderr, WHT);
}

/*
    Prints an error:
    ERROR

    Example:
    No input file
*/
void printError(char* error){
    fprintf(stderr, RED);
    fprintf(stderr, "ERROR: %s\n", error);
    fprintf(stderr, WHT);
}


/*
    Increments line line number (while keeping encapsulation)
*/
void incLineNum(){
    currentLineNumber++;
}

/*
    Reseting variables (while keeping encapsulation)
*/
void resetError(){
    currentLineNumber = 1;
    faultyFile = false;
}

/*
    Returns the line number
*/
int getLineNum(){
    return currentLineNumber;   
}

/*
    Setting the file name (while keeping encapsulation)
*/
void setFileName(char *name){
    strcpy(currentFileName, name);
    strcat(currentFileName, FILE_EXTENTION);
}