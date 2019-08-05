//
// Created by ubuntu16 on 8/5/19.
//

#ifndef MAMAN14_LABELSQUEUE_H
#define MAMAN14_LABELSQUEUE_H
#include <stdlib.h>
#include "assembler.h"
#include "utils.h"

/*=========data structures=====*/

typedef enum {PRE_DEFINED_LABEL = 0, ENTRY_LABEL} qLabelType;

/* label to keep in queue for second scan
 * this queue holds 2 type of labels to reprocess:
 *      PRE_DEFINED_LABEL that were used as operand before it was defined
 *      ENTRY_LABEL - Used on entry declaration*/

typedef struct qLabel_s
{
    qLabelType labelType;       /*Type of label in queue*/
    char label[MAX_LABEL_LEN];   /* label name */
    int address;                /*memory word address (for PRE_DEFINED_LABEL only, Entry get 0 as default)*/
    int lineNumber;             /*line number in input file */
    struct qLabel_s *next;      /*pointer to next qLabel */
} qLabel;


/*======== Function Prototypes=========== */

/* add qLabel struct to qLable queue */
void addLabelToQ(qLabelType labelType, char *label, int address, int lineNumber);

/*get params from of first label in queue
 * Params - pointers to variables to save struct fields*/
void getNextLabelParams(qLabelType *type, char* label, int *address, int *lineNumber);

/*remove fisrt label from labels queue */
void removeLabelFromQ();

/*remove fisrt label from labels queue */
boolean isEmptyQ();

/*delete lablesQ records and initiate global parameters */
void ClearLablesQ ();

#endif //MAMAN14_LABELSQUEUE_H
