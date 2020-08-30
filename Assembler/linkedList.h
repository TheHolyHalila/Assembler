/*Linked list nodes types*/
/* 
    TODO: 
        change int to bit field for memory efficiency
*/

#ifndef NODES_H
#define NODES_H

#include "globalHeader.h"

#define UNDEFINED -2 /* for labels that dont have a type at the time of their initialization, helps with defining entries */

/* A,R,E possibe values (decimal) */
#define ABSOLUTE 4
#define RELATIVE 2
#define EXTERNAL 1

/* initial counter values */
#define INITIAL_IC 100
#define INITIAL_DC 0

typedef struct LabelNode
{
    int address;
    char *label;
    int type; /* see globalheader, DATA, STRING... */
    bool isEntry; /* any non-extern label can also be an entry label*/
    struct LabelNode *next;

} LabelNode;    

/*
    A struct to store the code word
*/
typedef struct{
    unsigned int opcode :6;
    unsigned int AddressingMethodSource :2;
    unsigned int sourceRegister :3;
    unsigned int AddressingMethodTarget :2;
    unsigned int targetRegister :3;
    unsigned int funct :5;
} WordCode;

/* 
    The last data type for storing the address and its value before putting it in the obj file
*/
typedef struct AddressNode
{
    int address;
    int type :3; /* immediate values, memory refrences (lables) and opcode lines require different proccesing post-second pass */
    union value
    {
        WordCode code;
        unsigned int immValue : 21; /* used for immediate values (and label adresses after second-pass) */
        char *labelName;
        unsigned int data : 24;
    } value;
    unsigned int are :3;
    int lineNum;
    struct AddressNode *next;
} AddressNode;

/* Function prototypes */
int getIC();
void resetCounters();
LabelNode* initLabelNode(char* label,  int type);
void addLabelNode(LabelNode* head, char* label, int type);
int searchAddressByLabel(LabelNode* head, char* label);
void freeLabelList(LabelNode *head);
void freeAddressList(AddressNode* head);
AddressNode *initCodeNode(unsigned int opcode, Argument *firstArg, Argument *secondArg, unsigned int funct, AddressNode **tail);
void addCodeNode(unsigned int opcode, Argument *firstArg, Argument *secondArg, unsigned int funct, AddressNode **tail);
AddressNode *initMemoryNode(char *labelName);
void addMemoryNode(char *labelName, AddressNode** tail);
AddressNode *initValueNode(int value);
void addValueNode(int value, AddressNode** tail);
AddressNode* initDataNode(long data);
void addDataNode(long data, AddressNode** tail);
int handleArg(Argument *arg, AddressNode **tail);

#endif