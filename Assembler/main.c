/*
    Assembler - lab in system development

    Authors:
        Hali Lev Ari ID 325350627
        Noam Ben David

    Description:
        Assembly compiler to machine code
*/
#include "main.h"

char currentFileName[MAX_FILE_NAME + EXTENTION_LENGTH];

/* Main function */
int main(int argc,char *argv[]){
    int i;

    if (argc < 2) { /*checking if there is no files*/
        printError(NO_INPUT_FILE);
        exit(0);
    }   

    /* Looping on the files and compiling them */
    for (i = 1; i < argc; i++)
    {
        /* Setting the file name */
        setFileName(argv[i]);
        
        /* Checking for correct file name length */
        if (strlen(argv[i]) > MAX_FILE_NAME)
        {
            printFileError(FILE_NAME_TOO_LONG);
            continue;
        }

        /* Compiling */
        compile();
    }

    return 0;
}