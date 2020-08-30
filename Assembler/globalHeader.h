/*Global header*/

#ifndef GLOBAL_H
#define GLOBAL_H

/*Includes*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_FILE_NAME 15
#define EXTENTION_LENGTH 3
#define FILE_EXTENTION ".as"

/* general return codes for functions */
#define ERROR -1 /* Faulty function */
#define NOPE 0 /* Function is passive */
#define SUCCESS 1 /* The function succeeded */
#define NOT_FOUND -1 /* for search functions incase search target was not found */


/* types of lines */
#define OPCODE 0
#define MEMORY 1
#define IMMEDIATE_VAL 2

/* AM = Addressing Methods */
#define IMMEDIATE_AM 0
#define DIRECT_AM 1
#define RELATIVE_AM 2
#define REGISTER_AM 3

/* language limitations */
#define MAX_LINE_LENGTH 81 /* for 80 chars */
#define MAX_LABEL_LENGTH 32

/* reserved assembly symbols */
#define COMMENT_CHAR ';'
#define IMMIDIATE_VAL_SYMBOL '#'
#define TYPE_DECLARATION_SYMBOL '.'

/* alphanumeric constants */
#define LOWERCASE_A 'a'
#define LOWERCASE_Z 'z'

#define UPPERCASE_A 'A'
#define UPPERCASE_Z 'Z'

#define ZERO '0'
#define NINE '9'

/* .type defines */
#define CODE_LINE 0
#define DATA 1
#define STRING 2
#define EXTERN 3
#define ENTRY 4

#define DATA_STR "data"
#define STRING_STR "string"
#define EXTERN_STR "extern"
#define ENTRY_STR "entry"


/*Types*/
typedef enum {
    false,
    true
} bool;

/* Argument struct to temporarily save the argument to an opcode */
typedef struct Argument{
    unsigned int type:2; /* 0 - Immidiate value, 1 - label, 2 - Relative address, 3 - Register */
    union arg{
        unsigned int registerNum:3; /* Register number */
        char *label; /* Label as text */
        int value:21; /* The immidiate value */
    } arg;
} Argument;


/* Function prototypes */
bool inCharArray(char ch,const char *charArray);
int inStringArray(char *str, const char **stringArray, short length);
bool inString(char ch, const char *charArray);

bool isLower(char ch);
bool isNumeric(char ch);
bool isAlpha(char ch);
bool isAlphanumeric(char ch);

#include "errors.h"

#endif
