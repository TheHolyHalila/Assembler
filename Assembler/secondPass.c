
#include "secondPass.h"

/*
    Description:
        loops over codeNode and switches each label reference with its IC using searchAddressByLabel()
        then adds IC to data nodes

    Arguments:
        code: table of codeWords
        label: labelTable

*/
void secondPass(AddressNode* code, LabelNode* label, AddressNode* data){
    AddressNode* ptr = code;

    incData(data, label);

    /* looping over code table */
    while(ptr != NULL){
        /* Handling label codeWords (add check if extern)*/

        handleLabel(&ptr, label);
        ptr = ptr->next;
    }

    ptr = data;
    while(ptr != NULL){
        addLine(ptr->address, ptr->value.data);
        ptr = ptr->next;
    }
    

}

void handleLabel(AddressNode **code, LabelNode *label)
{
    AddressNode* ptr = *code;
    int tmp; 

    if(ptr->type == OPCODE){
        if (ptr->value.code.AddressingMethodTarget == 2)
        {
            makeObjFromOpcode(ptr);
            handleRelativeAddress(ptr->next, label);
            *code = ptr->next;
        }
        else{
            makeObjFromOpcode(ptr);
        }
    }
    else if(ptr->type == MEMORY){

        if((tmp = searchAddressByLabel(label, ptr->value.labelName)) == ERROR){
            printCustomLineError(UNDEFINED_LABEL, ptr->lineNum, ptr->value.labelName);
        }
        /* incase of extern */
        else if(tmp == 0){
            ptr->are = 1;
            addExt(ptr->value.labelName, ptr->address);
        }
        else{
            ptr->are = 2;
        }
        ptr->value.immValue = tmp;

        makeObjFromNum(ptr->address, ptr->value.immValue, ptr->are);

    }
    else{
        makeObjFromNum(ptr->address, ptr->value.data,ptr->are);
    }
}

void handleRelativeAddress(AddressNode* code, LabelNode* label){
    unsigned tmp;
    AddressNode* ptr = code;

    if((tmp = searchAddressByLabel(label, ptr->value.labelName)) == ERROR){
        printCustomLineError(UNDEFINED_LABEL, code->lineNum, code->value.labelName);
    }
    else{
        ptr->are = ABSOLUTE;
        ptr->value.immValue = (unsigned)(tmp - (ptr->address -1));
        makeObjFromNum(ptr->address, ptr->value.immValue, ptr->are);
        
    }
}

/* 
    Description:
        Increments the data and addresses IC value
*/
void incData(AddressNode* dataTable, LabelNode* labelTable){
    LabelNode* labelPtr = labelTable;
    AddressNode* dataPtr = dataTable;
    int DCF;

    const int ICF = getIC();

    while(labelPtr != NULL){

        /* Incrementing IC if found a data or a string label */
        if(labelPtr->type == DATA || labelPtr->type == STRING){
            labelPtr->address+=ICF;
        }

        if(labelPtr->isEntry){
            addEnt(labelPtr->label,labelPtr->address);
        }

        labelPtr = labelPtr->next;
    }
    DCF = 0;
    while (dataPtr != NULL)
    {
        /* Incrementing address by IC for each data cell */
        dataPtr->address += ICF;
        dataPtr = dataPtr->next;
        DCF++;
    }
    addHeader(ICF - INITIAL_IC, DCF);

}



void makeObjFromNum(unsigned address, unsigned value, unsigned are){
   addLine(address, (value<<VAL_SHIFT)+are);
}

void makeObjFromOpcode(AddressNode* word){
    unsigned value = word->value.code.opcode << OP_SHIFT;
    value += word->value.code.AddressingMethodSource << AMS_SHIFT;
    value += word->value.code.sourceRegister << SR_SHIFT;
    value += word->value.code.AddressingMethodTarget << AMT_SHIFT;
    value += word->value.code.targetRegister << TR_SHIFT;
    value += word->value.code.funct << FUNCT_SHIFT;
    value += word->are;
    addLine(word->address, value);
}

void addLine(unsigned address, unsigned value){
    addObj(address, value);
}
