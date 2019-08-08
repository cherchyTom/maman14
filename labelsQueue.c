//
// Created by ubuntu16 on 8/5/19.
//

#include "labelsQueue.h"

/* =======Global Variables ======*/

/*pointers to symbol list head and tail*/
static qLabel *queueHead;
static qLabel *queueTail;

/* crate qLabel Struct in memory validate creation */
static qLabel* createQLabel(){
    qLabel* newLabel = (qLabel*)malloc(sizeof(qLabel));
    validateMemoryAllocation(newLabel,"when creating new symbol record\n");

    newLabel->next = NULL;
    return newLabel;
}

/* add qLabel struct to qLable queue */
void addLabelToQ(qLabelType labelType, char *label,boolean isDefined, int address, int lineNumber){
    /*create new record*/
    qLabel *newLabel = createQLabel();
    /* copy data */
    newLabel->labelType = labelType;
    strcpy(newLabel->label,label);
    newLabel->isDefined = isDefined;
    newLabel->address = address;
    newLabel->lineNumber = lineNumber;

    /*empty list*/
    if(!queueHead){
        queueHead = queueTail = newLabel;
        return;
    }

    /*add new symbol to list end*/
    queueTail->next = newLabel;
    queueTail = newLabel;
    return;
}

/*get params from of first label in queue
 * Params - pointers to variables to save struct fields*/
void getNextLabelParams(qLabelType *type, char* label,boolean *isDefined, int *address, int *lineNumber){
    /*empty queue */
    if(!queueHead){
        return;
    }
    *type = queueHead->labelType;
    strcpy(label,queueHead->label);
    *isDefined = queueHead->isDefined;
    *address = queueHead->address;
    *lineNumber = queueHead->lineNumber;
    return;
}

/*remove first label from labels queue */
void removeLabelFromQ(){
    qLabel* labelToDelete;
    /*empty queue */
    if(!queueHead){
        return;
    }
    labelToDelete = queueHead;
    queueHead = queueHead->next;
    free(labelToDelete);
    return;
}

/*return true if queue label is empty otherwise false */
boolean isEmptyQ(){
    return(!queueHead) ? TRUE : FALSE;
}

/*delete lablesQ records and initiate global parameters */
void ClearLablesQ (){
    qLabel *labelToDelete;
    /*free table memory*/
    while (queueHead){
        labelToDelete = queueHead;
        queueHead = queueHead->next;
        free(labelToDelete);
    }
    queueHead = queueTail = NULL;
}