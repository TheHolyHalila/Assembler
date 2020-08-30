#include "firstpass.h"

#define MAX_LINE_OFFSET 2

/*
    Description:
        parsing each line in .as file, adding lines of code/data to apropriate linked lists while counting IC/DC.
        adding lables to label linked list and info about them (address, .type).

    Arguments:
        FILE **fp - pointer to file pointer (to match format of other functions).
        LabelNode *labelTable - stores lables here.
        AddressNode **addressTable - stores code lines here, pointer to pointer as we need parseLineFP() to change it each time.
        AddressNode **dataTable - same as addressTable, just for data (.data\.string).
    
*/
void firstPass(FILE **fp,LabelNode *labelTable, AddressNode **addressTable, AddressNode **dataTable){  
    /* Declaring variables */
    char line[MAX_LINE_LENGTH];

    /* looping over file line by line and sending to parser, stops at end of file */
    while (readNextLine(line,fp))
    {
        /* making sure line is not too long to be legally compiled */
        if(strlen(line) > MAX_LINE_LENGTH-MAX_LINE_OFFSET){
            printLineError(MAX_LINE_LENGTH_EXCEEDED);
            continue;
        }

        /* parse line, check for errors */
        parseLineFP(line, labelTable,addressTable, dataTable);

        incLineNum(); /* needed for specific error message printing */
    }

}


/*  
    Description:    
        parses line (first pass), updates linked lists

    Arguments:
        line: the line to parse
        labelTable: pointer to start of labeltable
        addressTable: list of instructions
        dataTable: a list of the data 

    Returns:
        In case of an error, returns -1 (ERROR).
        Otherwise, returns number of words to advance IC

    TODO:
        keep track of lines w unknown lables so second pass doesnt need to read whole file again?
*/
int parseLineFP(char* line, LabelNode *labelTable, AddressNode **addressTable, AddressNode **dataTable)
{
    /* Declare variables */
    char *label; /* label name will be stored here (if exists) */
    int declerationType; /* .data .entry and so on, check global header for .type definitions */
    int labelSwitch; /* stores what is returned from serachLabel so dotSearch can be called before entering the label handling switch */
    int column; /* current column number to reduce parsing time and for error messages */

    /* Init variables */
    column = 0; /* initialise column to start of line */

    /* if line (or comment) is empty move to next line */
    if(isLineEmpty(line, &column)){
        return NOPE;
    }

    /* checking if a label is declared and if there is a dot (.) statement */
    labelSwitch = searchLabel(line, &column, &label, START_LABEL); /* searching for a label definition */
    declerationType = dotSearch(line, &column);

    /* checking if there is a label to add (or if an error occured) */
    switch (labelSwitch){   
         
        /* incase of a label error no need to continue parsing the line */
        case ERROR:
            return ERROR;

        /* Incase starting with a label */
        case SUCCESS:

            /* declerationType .entry or .extern causes a warning if declared after a label decleration */
            if(declerationType == ENTRY || declerationType == EXTERN){
                printCodeWarning(USELESS_LABEL, column);
                return handleEntryExtern(line, &column, declerationType, labelTable);
            }
            
            /* Handle a code line following a label */
            else if(declerationType == CODE_LINE){
                addLabelNode(labelTable, label, CODE_LINE); 
                return handleCode(line, &column, addressTable);
            }

            /* add .data or .string label and handle the data */
            else if(declerationType == STRING || declerationType == DATA){
                addLabelNode(labelTable, label, declerationType); /* add as DC */
                return handleData(line, &column, declerationType, dataTable);
            }
            else{
                return ERROR;
            }
            
        /* continue parsing a line with no label */    
        default:


            /* No label, resetting the column */
            column = 0;
            declerationType = dotSearch(line, &column);

            /* parsing according to .type (or a code line) */
            switch (declerationType)
            {
                /* .data and .string are parsed by the same function */
                case DATA:
                case STRING:
                    return handleData(line, &column, declerationType, dataTable);
                    break;

                case CODE_LINE:
                    return handleCode(line, &column, addressTable); 
                    break;

                /* .entry and .extern are parsed by the same function */
                case ENTRY:
                case EXTERN:
                    return handleEntryExtern(line, &column, declerationType, labelTable);
                    break;
                
                /* any other option is not compilable */
                default:

                    return ERROR;
            }
            
    }
}
