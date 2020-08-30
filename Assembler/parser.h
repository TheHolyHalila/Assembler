#ifndef PARSER_H
#define PARSER_H

#include "globalHeader.h"
#include "linkedList.h"


/* Function prototypes */
bool isLineEmpty(char *line, int *column);
int dotSearch(char *line, int *lineNum);
int searchLabel(char *line, int *column, char **label, short definition);
bool goUntilComma(char *line, int *column,bool arg);
int handleCode(char *line, int *column, AddressNode** addressTable);
bool parseNumber(char *line , int *column, long *number);
int handleData(char *line, int *column, int declerationType, AddressNode** dataTable);
int handleEntryExtern(char *line, int *column, int declerationType, LabelNode* head);
bool incUntilNotWhite(char *line, int *column);
bool parseString(char *line, int *column, int *end, long *data);
int getOpcode(char *line, int *column, short *opcode, short *funct);
int getArg(char *line, int *column, Argument *arg);
int columnPP(int *column);
bool findEndOfString(char* line, int* column);
bool legaLabel(char* label, short column);

/* useful macros */
#define advanceBool(column) if(columnPP(column)==ERROR){return false;}
#define advanceError(column) if(columnPP(column)==ERROR){return ERROR;}


/* Defines and consts */
#define MAX_TYPE_LENGTH 6 /* Length of the word extern */
#define MAX_NUMBER 2097151
#define MIN_NUMBER -2097151
#define COMMA ','
#define PLUS '+'
#define MINUS '-'
#define DECIMAL_POINT '.'
#define START_OF_STRING '"'
#define ADDRESS_SYMBOL '&'
#define MAX_LENGTH_OF_NUMBER 8
#define MAX_OPCODE_LENGTH 5

#define START_LABEL 0
#define CODE_LABEL 1
#define ENTRY_EXTERN_LABEL 2

#endif
