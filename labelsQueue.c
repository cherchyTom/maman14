//
// Created by ubuntu16 on 8/5/19.
//

#include "labelsQueue.h"

/* =======Global Variables ======*/

/*pointers to symbol list head and tail*/
static qLabel *qLabelListHead;
static qLabel *qlabelListTail;

/* crate qLabel Struct in memory validate creation */
static qLabel* createQLabel(){
    qLabel* newLabel = (qLabel*)malloc(sizeof(qLabel));
    validateMemoryAllocation(newLabel,"when creating new symbol record\n");

    newLabel->next = NULL;
    return newLabel;
}

/* add qLabel struct to qLable queue */
void addLabelToQ(qLabelType labelType, char *label, int address, int lineNumber){
    /*create new record*/
    qLabel *newLabel = createQLabel();
    /* copy data */
    newLabel->labelType = labelType;
    strcpy(newLabel->label,label);
    newLabel->address = address;
    newLabel->lineNumber = lineNumber;

    /*empty list*/
    if(!qLabelListHead){
        qLabelListHead = qlabelListTail = newLabel;
        return;
    }

    /*add new symbol to list end*/
    qlabelListTail->next = newLabel;
    qlabelListTail = newLabel;
    return;
}

/*get params from of first label in queue
 * Params - pointers to variables to save struct fields*/
void getNextLabelParams(qLabelType *type, char* label, int *address, int *lineNumber){
    /*empty queue */
    if(!qLabelListHead){
        return;
    }
    *type = qLabelListHead->labelType;
    strcpy(label,qLabelListHead->label);
    *address = qLabelListHead->address;
    *lineNumber = qLabelListHead->lineNumber;
    return;
}

/*remove fisrt label from labels queue */
void removeLabelFromQ(){
    qLabel* labelToDelete;
    /*empty queue */
    if(!qLabelListHead){
        return;
    }
    labelToDelete = qLabelListHead;
    qLabelListHead = qLabelListHead->next;
    free(labelToDelete);
    return;
}

/*remove fisrt label from labels queue */
boolean isEmptyQ(){
    return(!qLabelListHead) ? TRUE : FALSE;
}

/*delete lablesQ records and initiate global parameters */
void ClearLablesQ (){
    qLabel *labelToDelete;
    /*free table memory*/
    while (qLabelListHead){
        labelToDelete = qLabelListHead;
        qLabelListHead = qLabelListHead->next;
        free(labelToDelete);
    }
    qLabelListHead = qlabelListTail = NULL;
}