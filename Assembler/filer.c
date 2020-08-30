/*
    Description:
        File handeler functions
*/
#include "filer.h"

/* File extentions */
#define OBJ ".ob"
#define ENT ".ent"
#define EXT ".ext"
#define AS_LENGTH 3 /* The length of ".as" */

/* Formats for file printing */
#define OBJ_FORMAT "%07u %06x\n"
#define ENTEXT_FORMAT "%s %07u\n"
#define HEADER_FORMAT "%d %d\n"

/* Declaring variables */
char currentFileName[];

FILE *extFile;
FILE *entFile;
FILE *objFile;

bool extExists;
bool entExists;


/* Resets a few variables */
void resetEntExt(){
    extExists = false;
    entExists = false;
}

/* Creates the files */
void setFiles(){
    char fileName[MAX_FILE_NAME + EXTENTION_LENGTH];
    char tempName[MAX_FILE_NAME + EXTENTION_LENGTH];

    /* Copying the file name */
    strncpy(fileName, currentFileName, strlen(currentFileName) - AS_LENGTH);
    strcpy(tempName, fileName);

    /* Creating the .ext file */
    openToWrite(tempName, EXT, &extFile);
    strcpy(tempName, fileName);

    /* Creating the .ent file */
    openToWrite(tempName, ENT, &entFile);
    strcpy(tempName, fileName);

    /* Creating the .ob file */
    openToWrite(tempName, OBJ, &objFile);
}

/* Closing the files and resetting variables */
void closeFiles(){
    /* Close */
    fclose(extFile);
    fclose(entFile);
    fclose(objFile);

    /* Reset */
    resetEntExt();
}


/*
    Returns true or false based on the existance of the file
    And attaches a FILE pointer if exist
*/
bool readFile(char* fname, FILE** fp) {
    /* Pointer Assignment */
    *fp = fopen(fname, "r");

    /* Checking for file existance */
    if (!fileExists(*fp)){
        return false;
    }

    /* Encase exists */
    return true;
}

/* 
    Description:
        Getting the line and filling it with the current line from the file 
    
    Arguments:
        line: the line that will be filled
        fp: a file pointer from whom we take the text

    Returns:
        The line as char*
*/
char* readNextLine(char* line, FILE** fp){
    return fgets(line, MAX_LINE_LENGTH+LINE_BUFFER, *fp);
} 

/*
    Returns true or false based on the existance of the file
*/
bool fileExists(FILE* fp){
    return fp!=NULL;
}

/* 
    Description:
        Opening a file for writing in

    Arguments:
        fname- the file name
        extention- the extention of the file (.ext, .ob, ETC...)
        fp- the file pointer that gets assigned to the file 
*/
void openToWrite(char *fname, char *extention, FILE **fp){
    *fp = fopen(strcat(fname,extention), "w");
}

/*
    Description:
        adding to the .ob file with the format:
        ADDRESS VALUE

    Arguments:
        address: the address of the line
        value: the value that sits in the address
*/
void addObj(unsigned address, unsigned value){
    fprintf(objFile,OBJ_FORMAT, address, value);
}

/*
    Description:
        adding to the .ent file with the format:
        LABEL ADDRESS

    Arguments:
        label: the entry label 
        address: the labels address
*/
void addEnt(char* label, unsigned address){
    entExists = true;
    fprintf(entFile, ENTEXT_FORMAT, label, address);
}

/*
    Description:
        adding to the .ext file with the format:
        LABEL ADDRESS

    Arguments:
        label: the extern label 
        address: the labels address
*/
void addExt(char *label, unsigned address)
{
    extExists = true;
    fprintf(extFile, ENTEXT_FORMAT, label, address);
}

/* 
    Description:
        Adding the header that says the amount of code addresses 
        and the amount of data in the format:
        CODE DATA

    Arguments:
        commandAmount: the amount of the code addresses
        dataAmount: the amount of data addresses

*/
void addHeader(int commandAmount,int dataAmount)
{
    fprintf(objFile,HEADER_FORMAT,commandAmount, dataAmount);
}

/*
    Description:
        Removing the files encase faulty files (errors)
*/
void removeFiles(){
    char fileName[MAX_FILE_NAME + EXTENTION_LENGTH];
    char tempName[MAX_FILE_NAME + EXTENTION_LENGTH];

    strncpy(fileName, currentFileName, strlen(currentFileName) - AS_LENGTH);
    fileName[strlen(fileName)] = '\0';

    /* Removing the .ext file encase theres no externs or faulty */
    if(extExists || getFaultyFile()){
        strcpy(tempName,fileName);
        strcat(tempName, EXT);
        remove(tempName);
    }

    /* Removing the .ent file encase theres no entries or faulty */
    if (entExists || getFaultyFile()){
        strcpy(tempName, fileName);
        strcat(tempName, ENT);
        remove(tempName);
    }

    /* removing the .ob files encase faulty files */
    if(getFaultyFile()){
        strcpy(tempName, fileName);
        strcat(tempName, OBJ);
        remove(tempName);
    }
}