/*
    The core code of the assembler, resets linked lists and important variables for each file.
    calls first pass and second pass and passes them initialized linked lists.
*/
#include "assembler.h"

/*
    Description:
        Handels the calls for first pass, second pass and file creation
*/
void compile(){

    /* Declaring variables */

    /* pointer to the current file to read from */
    FILE** fp = (FILE**)malloc(sizeof(FILE*));

    /* Storage Tables */
    LabelNode *labelTable; /* one for labels, initialized (malloced) later */
    AddressNode *addressTableHead = (AddressNode*) malloc(sizeof(AddressNode));     /* one for code */
    AddressNode *dataTableHead = (AddressNode*) malloc(sizeof(AddressNode));    /* and one for data */

    /* initializing pointer to end of list, to add to list in o(1) */
    AddressNode* dataTail = dataTableHead; 
    AddressNode* addressTail = addressTableHead;

    AddressNode* dummy;
    LabelNode* dummyLabel;

    /* initializing label table with an empty label, helps with looping over it later */
    labelTable = initLabelNode("", 0);

    /* Inits for files and a few file-dependent variables */
    setFiles();
    setFaultyFile();
    resetCounters();
    resetEntExt();
    resetError();

    /* Check for existance */
    if (!readFile(currentFileName,fp)){ /* Exit incase file doesn't exist */
        printFileError(FILE_DOESNT_EXIST);
        return;
    }
    /* Incase file exist- continue compiling */

    /* Next equal to NULL, helps managing linked lists  */
    dataTableHead->next = NULL;
    labelTable->next = NULL;
    addressTableHead->next = NULL;

    /* Do first pass */
    firstPass(fp, labelTable, &addressTail, &dataTail);

    /* Ignore the 'trash' tables heads */
    dummy = dataTableHead;
    dataTableHead = dataTableHead->next;
    free(dummy);

    dummy = addressTableHead;
    addressTableHead = addressTableHead->next;
    free(dummy);

    dummyLabel = labelTable;
    labelTable = labelTable->next;
    free(dummyLabel);
    /* Do second pass */
    secondPass(addressTableHead, labelTable, dataTableHead);

    /* Freeing the lists */
    freeAddressList(addressTableHead);
    freeAddressList(dataTableHead);
    freeLabelList(labelTable);

    /* closing files */
    closeFiles();

    /* Remove files incase they are faulty */    
    removeFiles();
    
}
