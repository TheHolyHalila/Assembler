#ifndef SECOND_H
#define SECOND_H

#include "globalHeader.h"
#include "filer.h"
#include "linkedList.h"

#define VAL_SHIFT 3
#define OP_SHIFT 18
#define AMS_SHIFT 16
#define SR_SHIFT 13
#define AMT_SHIFT 11
#define TR_SHIFT 8
#define FUNCT_SHIFT 3

void secondPass(AddressNode* code, LabelNode* label, AddressNode* data);
void handleLabel(AddressNode **code, LabelNode *label);
void incData(AddressNode *dataTable, LabelNode *labelTable) ;
void makeObjFromNum(unsigned address, unsigned value, unsigned are) ;
void makeObjFromOpcode(AddressNode *word) ;
void addLine(unsigned address, unsigned value);
void handleRelativeAddress(AddressNode* code, LabelNode* label);

#endif