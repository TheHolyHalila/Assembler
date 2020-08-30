#include "parser.h"

/* Opcode and func settings defines */

/* to find specs of a function, search for its index in OP_NAMES */
const char *OP_NAMES[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
#define ARG_AMOUNT 16
/* in the same index as the name, the rest of the specs of that function are found in these arrays*/
const int opcodes[] = {0, 1, 2, 2, 4, 5, 5, 5, 5, 9, 9, 9, 12, 13, 14, 15}; /* function opcode */
const int AMOUNT_OF_ALLOWED_ARGS[] = {2,2,2,2,2,1,1,1,1,1,1,1,1,1,0,0};
const int functs[] = {
    0,
    0,
    1,
    2,
    0,
    1,
    2,
    3,
    4,
    1,
    2,
    3,
    0,
    0,
    0,
    0,
}; /* funct identifier for functions that share an opcode */

/* Information about legal adressing methods for both operands of each function */

/* Source operand legal adressing methods */
const bool sourceMethod0[] = {false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false};  /* Immidiate value */
const bool sourceMethod1[] = {true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false};      /* Direct adress */
const bool sourceMethod2[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}; /* Relative adress */
const bool sourceMethod3[] = {true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false};      /* Register Adress */

/* Target operand legal adressing methods */
const bool targetMethod0[] = {true, true, true, true, false, false, false, false, false, false, false, false, false, true, false, false}; /* Immidiate value */
const bool targetMethod1[] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false};              /* Direct adress */
const bool targetMethod2[] = {false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false};  /* Relative adress */
const bool targetMethod3[] = {true, true, true, true, false, true, true, true, true, false, false, false, true, true, false, false};          /* Register Adress */

/* search tables */
const char WHITE_CHARS[] = {' ', '\t', '\n','\r','\0'};
const char END_OF_ARG[] = {' ', '\t', '\n', ',', '\r', '\0'};
const char END_OF_LABEL[] = ":";
const char *LABEL_TYPES[] = {DATA_STR, STRING_STR, STRING_STR, ENTRY_STR};
const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
#define REGISTER_AMOUNT 8

const bool* SECOND_ARG_OPTIONS[] = {sourceMethod0, sourceMethod1, sourceMethod2, sourceMethod3};
const bool* FIRST_ARG_OPTIONS[] = {targetMethod0, targetMethod1, targetMethod2, targetMethod3};

/* 
    Description:
        Checking if the line is empty or a comment
    
    Returns:
        Boolean,
        true- if line is empty, comment or is invalid
        false- if line has content

    Arguments:
        char* line - The line of code that we want to check

    Error handeling:
        Misused comment, comment line must start with ;
*/
bool isLineEmpty(char* line, int *column){
    int i;

    /* Check if the first char is the comment char */
    if(*column == 0 && line[0] == COMMENT_CHAR){
        return true;
    } 

    /* Check if only white chars */
    for (i = *column; i <= strlen(line); i++)
    {
        if (line[i] == COMMENT_CHAR){
            /* Invalid comment placement */
            printCodeError(MISUSED_COMMENT, i);
            return true; 
        }

        if(!inCharArray(line[i], WHITE_CHARS)){
            /* not empty or shouldn't be ignored */
            return false; 
        }
    }

    /* Line is empty */
    return true; 
}


/*
    Description:
        Check if Label is declared, if so adds it to symbol table
        
    Returns:
        true if Label Declerared, otherwise false.

    Arguments:
        line: the line we search the label on
        column: the column in the line
        label: a variable to save the label in
        definition: when is the function called (start of line, as an opcode argument...)

    Error handeling:
        Invalid Label (Alphanumeric)
        Redecleration of Existing Label
        Misused comment (placement)
*/
int searchLabel(char *line, int *column, char **label, short definition){
    /* Declerations */
    int temp;
    int i;
    const char* endingChar;;

    /* Initializations */
    switch (definition)
    {
    case START_LABEL:
        endingChar = END_OF_LABEL;
        break;
    
    /* Encase its in a code (mov LABEL2, LABEL1) */
    case CODE_LABEL:
        endingChar = END_OF_ARG;
        break;

    /* Encase of extern or entry */
    default:
        endingChar = WHITE_CHARS;
        break;
    }

    
    /* Incrementing to ignore white chars */
    incUntilNotWhite(line, column);


    /* Save incase theres no label, and to print error incase first char of a label decleration is not alphaic */
    temp = *column;

    /* Validity of first letter is checked last */
    advanceError(column)

    /* Search for label */
    for (i = 1; (*column) <= strlen(line);){ /* i = 1 because skipped first letter */
        /* Check for errors and validity */
        if(i > MAX_LABEL_LENGTH){
            printLineError(MAX_LABEL_LENGTH_EXCEEDED);
        }

        /* Found the end of a label, then asserts start of label is alphaic */
        if (definition == START_LABEL && inString(line[*column], endingChar)){

            /* Check if the first letter is alpha */
            if (!isAlpha(line[temp]))
            {
                printCodeError(NON_ALPHA_LABEL,temp);
                return ERROR;
            }

            /* Calloc and copy */
            *label = (char *)calloc(1,(*column - temp) * sizeof(char));
            strncpy(*label, line + temp, *column - temp);

            advanceError(column)
            temp = *column;
            if(incUntilNotWhite(line,&temp) && definition == START_LABEL){
                printLineError(EMPTY_LABEL);
                return ERROR;
            }

            return SUCCESS;
        }

        else if (definition!=START_LABEL && inCharArray(line[*column], endingChar))
        {
            /* The first letter must be alpha */
            if (!isAlpha(line[temp])){
                printCodeError(NON_ALPHA_LABEL, temp);
                return ERROR;
            }

            *label = (char *)calloc(1,(*column - temp) * sizeof(char));
            strncpy(*label, line + temp, *column - temp);

            advanceError(column)
            temp = *column;

            if (incUntilNotWhite(line, &temp) && definition == START_LABEL){
                printLineError(EMPTY_LABEL);
                return ERROR;
            }

            return SUCCESS;
        }

        /* No label declared */
        if (inCharArray(line[*column], WHITE_CHARS)){
            *column = temp;
            return NOPE;
        }
        
        /* Found an invalid charecter */
        if(!isAlphanumeric(line[*column])){ 
            printCodeError(INVALID_LABEL_CHARACTER, *column);
            *column = temp;
            return ERROR;
        } 
        
        advanceError(column)
        i++;
    }

    /* Checking that column did not exceed the max length of a line */
    if(*column > MAX_LINE_LENGTH){
        printLineError(MAX_LINE_LENGTH_EXCEEDED);
        return false;
    }

    /* Couldn't find a label */
    *column = temp;
    return NOPE;
}

/* 
    Description:
        Check illegal use of commas and going until hitting a while char or a non comma after a comma

    Returns:
        true- if all went well
        false- there was an illegal use of commas

    Arguments:
        line: the line we are on
        column: the column to check from
        arg: are we checking for an arguemnt seperator

    Error handling:
        Illegal use of commas
*/
bool goUntilComma(char *line, int *column,bool arg){

    /* Ignore white chars */
    if(incUntilNotWhite(line, column)){
        return true;
    }

    /* Argument seperator must be comma */
    if(line[*column] != COMMA){
        printCodeError(MUST_HAVE_COMMA_BETWEEN_ARGS,*column);
        return false;
    }

    advanceBool(column)

    /* a comma cannot be the last non-white character in a line */
    if(incUntilNotWhite(line, column)){
        printCodeError(END_OF_LINE_COMMA, *column);
        return false;
    }
    
    /* Double commas */
    if(line[*column] == COMMA){
        printCodeError(EXECUTIVE_COMMAS, *column);
        return false;
    }

    return true;
}


/*
    Description:
        Handles a line of code. (EXAMPLE: mov label, r4)

    Arguments:
        line: the line we are handling
        column: the column we are starting on
        addressTable: the address table we add to
*/
int handleCode(char *line, int *column, AddressNode** addressTable){
    short errorColumn;
    short opcode;
    short funct;
    short globalIndex;
    Argument *first = NULL;
    Argument *second = NULL;
    /* Incrementing to ignore white chars */
    incUntilNotWhite(line, column);

    /* Checking that column did not exceed the max length of a line */
    if(*column >= MAX_LINE_LENGTH){
        printLineError(MAX_LINE_LENGTH_EXCEEDED);
    }

    /* Saving the column for error broadcastring */
    errorColumn = *column;

    /* Checking that the opcode is valid (mov, jmp, ETC...) */
    if((globalIndex=getOpcode(line,column,&opcode, &funct)) == ERROR){
        return ERROR;
    }
    /* Read the args and save them */
    switch (AMOUNT_OF_ALLOWED_ARGS[globalIndex]) /* globalIndex is the index key */
    {
    case 0:
        /* Check if the rest of the line is empty */
        if(!isLineEmpty(line,column)){
            printCustomCodeError(WRONG_AMOUNT_OF_OPERANDS, *column, OP_NAMES[globalIndex], 0);
            return ERROR;
        }

        /* Add the code node */
        addCodeNode(opcode, first,second, funct, addressTable);
        
        break;
    
    case 1:
        if (isLineEmpty(line, column))
        {
            printCustomCodeError(WRONG_AMOUNT_OF_OPERANDS, *column, OP_NAMES[globalIndex], 1);
            return ERROR;
        }
        first = (Argument*)calloc(1,sizeof(Argument));

        errorColumn = *column;

        /* Get the argument */
        if(getArg(line, column, first) == ERROR){
            return ERROR;
        }


        /* Check if the type of argument is allowed */
        if(!FIRST_ARG_OPTIONS[first->type][globalIndex]){
            printCodeError(INVALID_ARG_TYPE, errorColumn);
            return ERROR;
        }

        /* Adding the code node */
        addCodeNode(opcode,first,second,funct,addressTable);

        /* Check if the rest of the line is empty */
        if(!isLineEmpty(line,column)){
            printCustomCodeError(WRONG_AMOUNT_OF_OPERANDS, *column, OP_NAMES[globalIndex], 1);
            return ERROR;
        }
        break;

    default:
        first = (Argument*)calloc(1,sizeof(Argument));
        second = (Argument *)calloc(1,sizeof(Argument));

        errorColumn = *column;

        /* Get the first argument */
        if(getArg(line, column, first) == ERROR){
            return ERROR;
        }

        /* Check if the type of argument is allowed */
        if(!FIRST_ARG_OPTIONS[first->type][globalIndex]){
            printCodeError(INVALID_ARG_TYPE, errorColumn);
            return ERROR;
        }

        /* Check for correct comma use */
        if(!goUntilComma(line,column,true)){
            return ERROR;
        }
        errorColumn = *column;
        
        if (isLineEmpty(line, column)){
            printCustomCodeError(WRONG_AMOUNT_OF_OPERANDS, *column, OP_NAMES[globalIndex], 2);
            return ERROR;
        }
        /* Get the second argument */
        if(getArg(line, column, second) == ERROR){
            return ERROR;
        }

        /* Check if the type of argument is allowed */
        if(!SECOND_ARG_OPTIONS[second->type][globalIndex]){
            printCodeError(INVALID_ARG_TYPE, errorColumn);
            return ERROR;
        }

        /* Adding the code node */
        addCodeNode(opcode,first,second,funct,addressTable);

        /* Check if the rest of the line is empty */
        if(!isLineEmpty(line,column)){
            printCustomCodeError(WRONG_AMOUNT_OF_OPERANDS, *column, OP_NAMES[globalIndex], 2);
            return ERROR;
        }
        break;
    }

    return SUCCESS;
}

/*
    Description:
        A function that retrives an opcode form a code line

    Arguments:
        line: the line we are taking the opcode from
        column: the column we start on
        opcode: a variable to save the opcode number in
        funct: a variable to save the funct number in
*/
int getOpcode(char *line, int *column, short *opcode, short *funct){
    char opcode_str[MAX_OPCODE_LENGTH];
    short i;
    short opcodeIndex;

    /* Ignoring white chars */
    incUntilNotWhite(line, column);

    /* Check the opcode and it's validity*/
    for (i = 0; !inCharArray(line[*column],WHITE_CHARS); i++){
        /* Copy the type to a string */
        opcode_str[i] = line[*column];

        advanceError(column)

        if(i > MAX_OPCODE_LENGTH){
            printCodeError(UNKNOWN_OPCODE,*column-i);
            return ERROR;
        }
    }
    /* Trimming the string */
    opcode_str[i] = '\0';

    if((opcodeIndex=inStringArray(opcode_str,OP_NAMES, ARG_AMOUNT)) == ERROR){
        printCodeError(UNKNOWN_OPCODE,*column-i);
        return ERROR;
    }

    *opcode = opcodes[opcodeIndex];
    *funct = functs[opcodeIndex];

    return opcodeIndex;
}


/*
    Description:
        This function is for getting the argument of an opcode
    
    Arguments:
        line, column - standarts
        arg: the argument we save the data in
*/
int getArg(char *line, int *column, Argument *arg){
    char *arg_str = (char*)calloc(1,sizeof(char));
    short i;
    short registerNum;
    long data;

    short errorColumn;
    /* Ignoring white chars */
    incUntilNotWhite(line, column);
    errorColumn = *column;
    
    /* Encase found an immidiate value */
    if (line[*column] == IMMIDIATE_VAL_SYMBOL)
    {
        advanceError(column)

        /* Parsing the number */
        if(!parseNumber(line,column,&data)){
            return ERROR;
        }

        /* Store the data */
        arg->type = IMMEDIATE_AM;
        arg->arg.value = data;
        return SUCCESS;
    }

    /* Incase found an address */
    if (line[*column] == ADDRESS_SYMBOL)
    {
        advanceError(column)

        /* Parsing the number */
        switch (searchLabel(line, column, &arg_str, CODE_LABEL))
        {
            case ERROR:
                return ERROR;
            case NOPE:
                printCodeError(ADDRESS_NO_LABEL, *column);
                return ERROR;
            case SUCCESS: 
                /* Store the data */
                arg->type = RELATIVE_AM;
                arg->arg.label = arg_str;
                return SUCCESS;
        }        
    }

    /* Copy contents of argument */
    for (i = 0; !inCharArray(line[*column],WHITE_CHARS) && line[*column] != COMMA; i++){
        /* Check for valid label length */
        if(i > MAX_LABEL_LENGTH){
            printCodeError(MAX_LABEL_LENGTH_EXCEEDED,*column-i);
            return ERROR;
        }

        advanceError(column)
    }
    /* Copy the arg to a string */
    strncpy(arg_str,line+*column-i,i);

    /* Trimming the string */
    arg_str[i] = '\0';

    /* Check if not reserved word */
    if(inStringArray(arg_str,OP_NAMES, ARG_AMOUNT) != NOT_FOUND){
        printCodeError(INVALID_USE_OF_RESERVED_WORD, *column-1-i);
        return ERROR;
    }

    /* Checking if its a register */
    if((registerNum=inStringArray(arg_str,registers, REGISTER_AMOUNT)) != NOT_FOUND){
        arg->type = REGISTER_AM;
        arg->arg.registerNum = registerNum;
        return SUCCESS;
    }

    /* Direct addressing method. store the data */
    else if(legaLabel(arg_str,errorColumn)){
        arg->type = DIRECT_AM;
        arg->arg.label = arg_str;

        return SUCCESS;
    }
    return ERROR;
}


/*  
    Description:
        This function checks that the label is valid

    Arguments:
        label: the label that we want to check
        column: for error printing
*/
bool legaLabel(char* label,short column){
    int i;

    /* checking that the first letter is alpha */
    if (!isAlpha(label[0])){
        printCodeError(NON_ALPHA_LABEL, column);
        return false;
    }

    /* checking that the rest is valid */
    for(i = 1; label[i]; i++){
        if(!isAlphanumeric(label[i])){
            printCodeError(INVALID_LABEL_CHARACTER,column);
            return false;
        }
    }
    return true;   
}

/*
    Description:
        Parsing an integer

    Returns:
        true- if all parsed well
        false- if parsing failed

    Arguments:
        line: the line we want to check
        column: the column we start at
        number: a varible to store the number at
*/
bool parseNumber(char *line , int *column, long *number){
    int i = 0;
    int ptrToStartOfNum;;

    /* Ignore white chars */
    incUntilNotWhite(line,column);

    /* Check if the number is not a decimal number */
    if(line[*column] == DECIMAL_POINT){
        printCodeError(NUMBER_MUST_BE_INTEGER, *column);
        return false;
    }

    /* If not a valid start */
    if(line[*column] == MINUS || line[*column] == PLUS){
        ptrToStartOfNum = *column;

        if (columnPP(column) == ERROR)
        {
            return false;
        }
    }

    /* Not numeric - error */
    else if (!isNumeric(line[*column])){    
        printCodeError(THIS_ARG_MUST_BE_NUMERIC, *column);
        return false;
    }
    else{
        ptrToStartOfNum = *column;
    }
    /*  
        keep on construction the number
        algorithm:
        text = "text 1234 text"
        Get length of number (4)
        number = strtol(number_text)
    */

    do{
        /* Error checking */

        /* Checking the numbers validity */
        if (i > MAX_LENGTH_OF_NUMBER)
        {
            printCodeError(NUMBER_OUTSIDE_BOUNDS, ptrToStartOfNum);
            return false;
        }

        /* Check if the number is not a decimal number */
        if(line[*column] == DECIMAL_POINT){
            printCodeError(NUMBER_MUST_BE_INTEGER, *column);
            return false;
        }

        /* If the number is not numeric */
        if(!isNumeric(line[*column])){
            printCodeError(THIS_ARG_MUST_BE_NUMERIC, *column);
            return false;
        }

        advanceBool(column)
        i++;
    } while (line[*column] != COMMA && !inCharArray(line[*column], WHITE_CHARS));

    /* Converting to number */
    *number = strtol(line + ptrToStartOfNum, NULL, 10);

    /* Checking the numbers validity */
    if (*number < MIN_NUMBER || *number > MAX_NUMBER){
        printCodeError(NUMBER_OUTSIDE_BOUNDS, ptrToStartOfNum);
        return false;
    }

    /* Checking that column did not exceed the max length of a line */
    if(*column > MAX_LINE_LENGTH){
        printLineError(MAX_LINE_LENGTH_EXCEEDED);
    }

    return true;
}

/*
    Descriptions:
        For handling the data (.data 45, 6  .string "str")

    Arguments:
        line: the line we want to check
        column: the column we start at
        declerationType: the type of the data (numeric\string)
        dataTable: the list we put the data in
*/
int handleData(char *line, int *column, int declerationType, AddressNode** dataTable){
    long data;    
    short dataAmount;
    int tempColumn;
    dataAmount=0;

    /* Incrementing to check for argument existance */
    if(incUntilNotWhite(line, column)){
        printLineError(VARS_MUST_HAVE_ARGS);
    }
    switch (declerationType){
        /* Encase numeric */
        case DATA:
            do{
                incUntilNotWhite(line,column);

                /* Parsing the number */
                if (!parseNumber(line, column, &data)){ /* Check for parsing error */
                    return ERROR;
                }
                
                /* Check for comma */
                if(!goUntilComma(line, column,false)){ /* Check for comma errors */
                    return ERROR;
                }

                dataAmount++;
                
                /* Adding to the list */
                addDataNode(data, dataTable);

            } while (!incUntilNotWhite(line, column));

            return isLineEmpty(line,column);

        /* Encase a string */
        default:
            /* .string argument must start with " */
            if(line[*column] != START_OF_STRING){
                printCodeError(STRING_ARG_MUST_HAVE_QUOTATION_MARK,*column);
                return ERROR;
            }
            
            tempColumn = *column;
            advanceBool(column)

            /* finding the end of the string */
            if(!findEndOfString(line, &tempColumn)){
                return ERROR;
            }

            /* Parsing the string */
            while(parseString(line,column,&tempColumn, &data)){

                /* Adding to the list */
                addDataNode(data, dataTable);
                dataAmount++;
                advanceBool(column)
            } 
            /* Adding \0 */
            addDataNode(0, dataTable);
            dataAmount++;

            break;
        }

    /* Ensuring that the rest of the line is empty */
    if(!isLineEmpty(line,column)){
        return ERROR;
    }
    return dataAmount;
}

/*
    Description:
        finds end of string, for parsing strings
    
    Argments:
        Line: current line to parse
        column: location in line of start of string

    Error Handling:
        if last non-white char in line is not ' " ', JUNK_TEXT
    
    Return:
        True - all went well, column is at end of string
        False - Error, stop parsing line

*/
bool findEndOfString(char* line, int* column){
    
    /* finding end of line */
    while(line[*column] != '\0')
    {
        if (columnPP(column) == ERROR)
        {
            return false;
        }
    }

    /* finding last non-white char (while making sure line isn't too long) */
    while(inCharArray(line[*column], WHITE_CHARS)){(*column)--;}

    /* if last non-white char isnt ' " ' raise error and return false*/
    if (line[*column]!=START_OF_STRING){
        printCodeError(JUNK_TEXT,*column);
        return false;
    }

    /* all went well, return true */
    return true;

}

/*
    Description:
        returning the string until it got to the end

    Arguments:
        line: the line we want to check
        column: the column we start at
        end: the ending character
        data: a variable to save the ascii character
*/
bool parseString(char *line, int *column, int *end, long *data){
    /* Checking that the string hasn't ended (") */
    if(*column != *end){
        *data = line[*column];
        return true;
    }
    return false;
}

/*
    Description:
        Asserts correct arguments for .data/.extern expression and adds label to labelTable.
        incase of predefined label it only adds type definition to that label.
    
    Arguments:
        line: current parsing line.
        column: start point to parse from.
        declerationType: wether .entry or .extern.
        head: head of labelTable needed to add to table.

    Error Handling:
        Missing label as argument.

    Returns:
        ERROR - in case of error
        otherwise, returns from addLabelNode (not used).
*/
int handleEntryExtern(char *line, int *column, int declerationType, LabelNode* head){
    char* label;
    int error;

    label = "";

    /* Incrementing to ignore white chars */
    incUntilNotWhite(line, column);

    /* searching for the operand (a label) and storing its return value in error to assert that a label has been found */
    error = searchLabel(line, column, &label, ENTRY_EXTERN_LABEL);
    if(error != SUCCESS){
        /* Incase if entry */
        if(declerationType == ENTRY){
            printCodeError(MISSING_ENTRY_LABEL, *column);
        }            
        /* Incase of extern */
        else if(declerationType == EXTERN){
            printCodeError(MISSING_EXTERN_LABEL, *column);
        }
        FaultyFile = true;
        return ERROR;
    }
    addLabelNode(head, label, declerationType);
    return SUCCESS;
}

/*
    Description:
        Incrementing the column until it hits a non white charecter or the end of the line

    Arguments:
        line: current line to parse
        column: starts from column and moves it so that next functions can parse easily

    Returns:
        Boolean,
        true- got to the end of the line
        false- else
*/
bool incUntilNotWhite(char *line, int *column){
    /* Incrementing until not white chars */
    while (inCharArray(line[*column], WHITE_CHARS)){ 
        if (line[*column] == '\0'){ /* Got to the end of the line */
            return true;
        }

        /* Incrementing the coloumn */
        if(columnPP(column)==ERROR){return false;}
    }
    return false;
}


/*
    Description:
        searching for .type

    Argument:
        line: the line to search in
        column: where in the line to search

    Return:
        CODE_LINE - no type declared
 
        DATA - .data
        STRING - .string
        EXTERN - .exrern
        ENTRY - .entry

        ERROR - Invalid dot type expression

    Error handling:
        Invalid dot type expression

*/
int dotSearch(char *line, int *column){
    /* Declerations */
    int i;
    char checkedType[MAX_TYPE_LENGTH+1];

    /* Ignoring white chars */
    incUntilNotWhite(line, column);

    /* Checking if started with a . */
    if(line[*column] != TYPE_DECLARATION_SYMBOL){
        return CODE_LINE;       
    }
    advanceError(column)

    /* Check the type and it's validity */

    for (i = 0; !inCharArray(line[*column],WHITE_CHARS); i++){
        /* Copy the type to a string */
        checkedType[i] = line[*(column)];

        advanceError(column)

        if(i > MAX_TYPE_LENGTH){
            printCodeError(INVALID_DOT_EXPRESSION,*column-i);
            return ERROR;
        }
    }

    /* Checking that column did not exceed the max length of a line */
    if(*column > MAX_LINE_LENGTH){
        printLineError(MAX_LINE_LENGTH_EXCEEDED);
        return false;
    }
    /* Trimming the string */
    checkedType[i] = '\0';

    /* Check the type of the type */
    if (!strcmp(checkedType, DATA_STR)){
        return DATA;
    }
    if (!strcmp(checkedType, STRING_STR)){
        return STRING;
    }
    if (!strcmp(checkedType, ENTRY_STR)){
        return ENTRY;
    }
    if (!strcmp(checkedType, EXTERN_STR)){
        return EXTERN;
    }

    /* Incase of invalid type */
    printCodeError(INVALID_DOT_EXPRESSION,*column);

    return ERROR;
}

/*
    Description:
        Incrementing the column while checking that it doesn't over flow

    Arguments:
        column: the column we want to increment
*/
int columnPP(int *column){
    /* Incrementing the column while checking its validity */
    if(++(*column) > MAX_LINE_LENGTH) {
        printLineError(MAX_LINE_LENGTH_EXCEEDED);
        return ERROR;
    }
    return SUCCESS;
}