#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "globalHeader.h"
#include "parser.h"
#include "filer.h"

/* Function prototypes */

void firstPass(FILE **fp, LabelNode *LabelNode, AddressNode **addressTable, AddressNode **dataTable);
int parseLineFP(char* line, LabelNode *labelTable, AddressNode **addressTable, AddressNode **dataTable);


#endif