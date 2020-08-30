/*
    Description:
        Implementation of linked lists, according to each lists needs
*/
#include <string.h>
#include "linkedList.h"

/* address counters */
int IC = INITIAL_IC;
int DC = INITIAL_DC;

/*  gets IC for secondPass  */
int getIC(){return IC;}

/* resets counters for each file to compile */
void resetCounters(){
    IC = INITIAL_IC;
    DC = INITIAL_DC;
}

/*  Description:
        Creating a LabelNode and initializing it

    
*/
LabelNode* initLabelNode(char* label, int type){
    /* Creating the node */ 
    LabelNode* newLabelNode;
    newLabelNode = (LabelNode*)malloc(sizeof(LabelNode));

    /* Initializing the node with its address according to its type */
    switch(type){
        case EXTERN:
            newLabelNode->address = 0;
            break;
        case CODE_LINE:
            newLabelNode->address = IC;
            break;
        default: /* .data\.string */
            newLabelNode->address = DC;
    }

    /* inserting label name and handling entry flag */
    newLabelNode->label = (char*)calloc(1,sizeof(char)*MAX_LABEL_LENGTH);
    strcpy(newLabelNode->label, label);

    if(type == ENTRY){
        newLabelNode->isEntry = true;
    }
    else{
        newLabelNode->type = type;
    }
    newLabelNode->next = (LabelNode *)malloc(sizeof(LabelNode));
    newLabelNode->next = NULL;
    /* return node */
    return newLabelNode;
}

/*
    Adding a LabelNode in o(n) 
    (Addition can be done with O(1) but needs to check for label duplicates)
*/
void addLabelNode(LabelNode* head, char* label, int type){
    /* init node */
    LabelNode *newLabelNode;
    LabelNode *ptr;

    /* alias pointer to list, for safety */
    ptr = head;
    /* loop over list */
    while (ptr != NULL){
        /* Check for duplicated label decleration */
        if (!strcmp(ptr->label, label))
        {
            /* handling extern declerations/redecleration errors */
            if (type == EXTERN || ptr->type == EXTERN)
            {

                /* declaring a label twice as an extern is compilable, the assembler simply prints a warning and moves on */
                if (type == EXTERN && ptr->type == EXTERN)
                {
                    printLineWarning(DUPLICATE_EXTERN_DECLERATION);
                    return;
                }

                /* a label cannot be both an extern and entry/data/string/code */
                if (ptr->isEntry || ptr->type != UNDEFINED || type != UNDEFINED)
                {
                    printLineError(EXTERN_ERROR);
                    return;
                }
            }

            /* handling entry declerations */

            /* duplicate entry decleration */
            if (type == ENTRY && ptr->isEntry)
            {
                printLineWarning(DUPLICATE_ENTRY_DECLERATION);
                return;
            }

            /* turn on entry flag for node */
            else if (type == ENTRY)
            {
                ptr->isEntry = true;
                return;
            }

            /* add info to predeclared entry label according to label type */
            else if (ptr->isEntry)
            {
                ptr->type = type;
                if (type == CODE_LINE)
                {
                    ptr->address = IC;
                    return;
                }
                else
                {
                    ptr->address = DC;
                    return;
                }
            }
            /* any other type of duplicate decleration is also an error */
            printLineError(DUPLICATE_LABEL_DECLERATION);
            return;
        }
        /* if not last node and not returned yet (due to finding an error\label to update), continue looping over list */
        if(ptr->next != NULL){
            ptr = ptr->next;
        }
        else{break;}
    }

    /* if function didn't return yet, Add a new node */
    newLabelNode = initLabelNode(label, type);

    /* insert it to end of list */
    ptr->next = (LabelNode *) malloc(sizeof(LabelNode));
    ptr->next = newLabelNode;
}

/*
    Searching the address by its label
*/
int searchAddressByLabel(LabelNode* head, char* label){
    LabelNode* current = head;

    while (current != NULL)
    {
        /* Checking if the labels are the same */
        if (!strcmp(current->label,label)){
            return current->address;
        }
        current = current->next;
    }

    /* Incase not found return error and let second pass print the error (it has all the info) */
    return ERROR;
}

/*
    Description:
        Freeing the Label list memory
*/
void freeLabelList(LabelNode *head)
{
    LabelNode *ptr;
    ptr = head;

    /* looping and freeing the pointers */
    while (head != NULL)
    {
        ptr = head; 
        head = head->next;
        free(ptr);
    }
}

/*
    Creating an AddressNode
*/
AddressNode* initCodeNode(unsigned int opcode, Argument *firstArg, Argument *secondArg, unsigned int funct, AddressNode **tail){
    /* Node decleration */
    AddressNode* newAddressNode;
    newAddressNode = (AddressNode*)calloc(1,sizeof(AddressNode));

    /* Initialize the node */
    newAddressNode->type = OPCODE;
    newAddressNode->value.code.funct = funct;
    newAddressNode->address = IC;

    /* Incrementing IC */
    IC++;

    newAddressNode->value.code.opcode = opcode;

    /* checking if command needs more words (depending on adressing method) and adding them */
    if (secondArg != NULL && firstArg != NULL){
        newAddressNode->value.code.AddressingMethodTarget = secondArg->type;
        newAddressNode->value.code.AddressingMethodSource = firstArg->type;
    }
    else if(firstArg != NULL){
        newAddressNode->value.code.AddressingMethodTarget = firstArg->type;
        newAddressNode->value.code.AddressingMethodSource = 0;
    }
    else{
        newAddressNode->value.code.AddressingMethodSource = 0;
        newAddressNode->value.code.AddressingMethodTarget = 0;
    }

    newAddressNode->lineNum = getLineNum();
    /* first word of each line of code has are value of 4 */
    newAddressNode->are = 4;

    return newAddressNode;
}

void addCodeNode(unsigned int opcode, Argument *firstArg, Argument *secondArg, unsigned int funct, AddressNode **tail){
    /* Declaring variables */
    AddressNode *newCodeNode;
    

    /* Init the new AddressNode */
    newCodeNode = initCodeNode(opcode, firstArg, secondArg, funct, tail);

    /* Insertion */
    (*tail)->next = newCodeNode;
    *tail = (*tail)->next;
    newCodeNode->value.code.sourceRegister = handleArg(firstArg, tail);
    newCodeNode->value.code.targetRegister = handleArg(secondArg, tail);
}

/* 
    Description:
            Adds aditional words for label/value as operands 
*/
int handleArg(Argument *arg, AddressNode **tail){
    if(!arg){return 0;}
    switch (arg->type){


        case IMMEDIATE_AM:
           addValueNode(arg->arg.value, tail);
           return 0;
        
        /* Registers dont require an additional word */
        case REGISTER_AM:
            return arg->arg.registerNum;
        
        /* Direct and relative adressing methods are handled the same */
        default:
            addMemoryNode(arg->arg.label, tail);
            return 0;

    }

}
AddressNode* initMemoryNode(char *labelName){
    /* Node decleration */
    AddressNode* newMemoryNode;
    newMemoryNode = (AddressNode*)calloc(1,sizeof(AddressNode));

    /*  Insertion */
    newMemoryNode->type = MEMORY;
    newMemoryNode->value.labelName = labelName;
    newMemoryNode->address = IC;
    newMemoryNode->lineNum = getLineNum();

    return newMemoryNode;
}

void addMemoryNode(char *labelName, AddressNode** tail){
    /* node decleration */
    AddressNode* newMemoryNode;

    /* Init the node */
    newMemoryNode = initMemoryNode(labelName);

    /* Insertion */
    (*tail)->next = newMemoryNode;
    *tail = (*tail)->next;
    
    /* incrementing IC */
    IC++;
}

AddressNode* initValueNode(int value){
    /* Node decleration */
    AddressNode* newValueNode;
    newValueNode = (AddressNode*)calloc(1,sizeof(AddressNode));

    /*  Insertion */
    newValueNode->type = IMMEDIATE_VAL;
    newValueNode->value.immValue= value;
    newValueNode->address = IC;
    newValueNode->lineNum = getLineNum();

    /* first word of each line of code has are value of 4 */
    newValueNode->are = 4;


    return newValueNode;
}

void addValueNode(int value, AddressNode** tail){
    /* node decleration */
    AddressNode* newValueNode;

    /* Init the node */
    newValueNode = initValueNode(value);

    /* Insertion */
    (*tail)->next = newValueNode;
    *tail = (*tail)->next;

    /* incrementing IC */
    IC++;
}



AddressNode* initDataNode(long data){
    /* Node decleration */
    AddressNode* newAddressNode;
    newAddressNode = (AddressNode*)calloc(1,sizeof(AddressNode));

    /*  Insertion */
    newAddressNode->value.data = data;
    newAddressNode->address = DC;

    return newAddressNode;
}

void addDataNode(long data, AddressNode** tail){
    /* node decleration */
    AddressNode* newDataNode;

    /* Init the node */
    newDataNode = initDataNode(data);

    /* insertion */
    /* Insertion */
    (*tail)->next = newDataNode;
    *tail = (*tail)->next;   
        
    /* incrementing DC */
    DC++;     
}


/*
    Description:
        Freeing the Adress list memory
*/
void freeAddressList(AddressNode* head){
    AddressNode *ptr;

    /* Looping on the list and freeing the pointers */
    while(head != NULL){
        ptr = head->next;
        free(head);
        head = ptr;
    }
}