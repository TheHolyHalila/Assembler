/*
    Error messages
*/

#ifndef ERRORS_H
#define ERRORS_H

#include "globalHeader.h"

/* defining colors for pretier error messages */
#define RED "\x1B[31m"
#define YEL "\x1B[33m"
#define WHT "\x1B[37m"

/* Function prototypes */
bool getFaultyFile();
void setFaultyFile();
void printLineError(char* error);
void printCodeError(char* error, int column);
void printCodeWarning(char* warning, int column);
void printLineWarning(char* warning);
void printFileError(char* error);
void printError(char* error);
void incLineNum();
void setFileName(char *name);
void printCustomCodeError(const char *error, int column, const char *arg, int attribute);
void printCustomLineError(const char *error, int lineNum, const char *arg);
int getLineNum();
char *getFileName();
void resetError();

/* Error constants */
#define FILE_DOESNT_EXIST "File doesn't exist."
#define NO_INPUT_FILE "No input file provided."
#define MISUSED_COMMENT "Misused comment, comment line must start with ;."
#define INVALID_LABEL_CHARACTER "Invalid label character. A label must start with a letter and may only contain alphanumeric characters."
#define INVALID_DOT_EXPRESSION "Invalid .type expression."
#define DUPLICATE_LABEL_DECLERATION "A label cannot be declared twice."
#define UNDEFINED_LABEL "%s label undefined."
#define MISSING_ENTRY_LABEL ".entry takes a label as an argument."
#define MISSING_EXTERN_LABEL ".extern takes a label as an argument"
#define EXECUTIVE_COMMAS "There cannot be two commas in a row."
#define THIS_ARG_MUST_BE_NUMERIC "This argument must be an integer."
#define NUMBER_MUST_BE_INTEGER "Numbers must be integers (no decimal point)."
#define NUMBER_OUTSIDE_BOUNDS "The number is outside of the allowed range."
#define MUST_HAVE_COMMA_BETWEEN_ARGS "Argument seperator must me a comma."
#define VARS_MUST_HAVE_ARGS "Variables must have arguments."
#define EXTERN_ERROR "An extern label cannot also be any other type of label (e.g. a code label)."
#define MAX_LABEL_LENGTH_EXCEEDED "Label length exceeded its maximum length (31)."
#define STRING_ARG_MUST_HAVE_QUOTATION_MARK "A .string must have \" surrounding its text argument."
#define MAX_LINE_LENGTH_EXCEEDED "Max line length exceeded."
#define UNKNOWN_OPCODE "Unknown opcode."
#define INVALID_ARG_TYPE "This function does not accept this type of operand for this argument."
#define INVALID_USE_OF_RESERVED_WORD "Invalid use of reserved word."
#define NON_ALPHA_LABEL "The start of a label must be alpha."
#define END_OF_LINE_COMMA "A line cannot end with a comma."
#define JUNK_TEXT ".string command must end with quotation mark."
#define EMPTY_LABEL "A label cannot be declared as nothing."
#define ADDRESS_NO_LABEL "The Addressing symbol (&) cannot be used without a label."
#define FILE_NAME_TOO_LONG "Exceeded maximum file name length for input."

/* opcode error constants */
#define WRONG_AMOUNT_OF_OPERANDS "%s command recieves exactly %d opernads."

/* Warning constants */
#define USELESS_LABEL "A label declared before a .entry or .extern decleration is useless."
#define DUPLICATE_EXTERN_DECLERATION "Declaring a label as an extern twice is useless."
#define DUPLICATE_ENTRY_DECLERATION "Declaring a label as an entry twice is useless."

    /* Variables */
    extern char currentFileName[MAX_FILE_NAME + EXTENTION_LENGTH]; /* used for error messages and output file names*/
int currentLineNumber; /* used for error messages */
bool FaultyFile;

#endif 
