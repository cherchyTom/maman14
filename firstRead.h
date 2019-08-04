//
// Created by ubuntu16 on 7/26/19.
//

#ifndef MAMAN14_FIRSTREAD_H
#define MAMAN14_FIRSTREAD_H

#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "utils.h"
#include "constsTables.h"
#include "txtAnalyzer.h"
#include "symbolList.h"
#include "memoryWord.h"

/*=========Data Structures======*/


/* Operands */
typedef enum {INVALID_OPERAND = -1,NUMBER = 0, LABEL = 1,  ARR = 2, REGISTER = 3} operandType;

typedef struct{
    operandType type;		        /* Type */
    int value;				        /* Value */
    char label[MAX_LABEL_LEN];      /* Label Name */
    boolean isDefinedLabel;         /* True if label is defined in symbol table */
    int offset;                      /* Label address offset - 0 mean no offset*/
    int address;			        /* The address of the operand in the memory */
} operandInfo;

/* Line */
typedef struct{
    int lineNumber;				/* line number in the file */
    int address;				/* The address of the first word in the line */
    char originalString[MAX_LINE_LEN +2];		/* Original line content */
    char *lineStr;				/* pointer to the originalString - use to parse the line */
    boolean isContainLabel;			/* A pointer to the lines label in labelArr */
    char labelValue[MAX_LABEL_LEN+1]; /*contains label value if exist */
    sentanceType lineType;       /* code, instruction or macro*/
    const instruction *instStruct;   /* pointer to instruction struct - data, string, extern or entry */


    char *commandStr;			/* The string of the command or instruction */

    /* Command line */
    const command *cmd;			/* A pointer to the command in g_cmdArr */
    operandInfo targetOp;			/* The 1st operand */
    operandInfo sourceOp;			/* The 2nd operand */

} lineInfo;


/*========Prototypes =====*/
int firstRead(FILE*fd);
#endif //MAMAN14_FIRSTREAD_H
