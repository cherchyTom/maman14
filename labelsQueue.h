//
// Created by ubuntu16 on 8/5/19.
//

#ifndef MAMAN14_LABELSQUEUE_H
#define MAMAN14_LABELSQUEUE_H
#include <stdlib.h>
#include "assembler.h"
#include "utils.h"

/*=========data structures=====*/

typedef enum {OPERAND_LABEL = 0, ENTRY_LABEL} qLabelType;

/* label to keep in queue for second scan
 * this queue holds 2 type of labels to reprocess:
 *      OPERAND_LABEL that were used as command operands
 *      ENTRY_LABEL - Used on entry declaration*/

typedef struct qLabel_s
{
    qLabelType labelType;       /*Type of label in queue*/
    char label[MAX_LABEL_LEN];  /* label name */
    boolean isDefined;          /* if the label defined before it used or not, for Entry label default value is 0*/
    int address;                /*memory word address (for OPERAND_LABEL only, Entry get 0 as default)*/
    int lineNumber;             /*line number in input file */
    struct qLabel_s *next;      /*pointer to next qLabel */
} qLabel;


/*======== Function Prototypes=========== */

/* add qLabel struct to qLable queue
 * Params - variable to insert to struct fields - labelType, isDefined, address, lineNumber */
void addLabelToQ(qLabelType labelType, char *label,boolean isDefined, int address, int lineNumber);

/*get params from of first label in queue
 * Params - pointers to variables to save struct fields*/
void getNextLabelParams(qLabelType *type, char* label,boolean *isDefined, int *address, int *lineNumber);

/*remove first label from labels queue */
void removeLabelFromQ();

/*remove first label from labels queue */
boolean isEmptyQ();

/*delete labelsQ records and initiate global parameters */
void clearLabelsQ();

#endif //MAMAN14_LABELSQUEUE_H
