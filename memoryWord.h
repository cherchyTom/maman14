//
// Created by ubuntu16 on 8/1/19.
//

#ifndef MAMAN14_MEMORYWORD_H
#define MAMAN14_MEMORYWORD_H

#include <stdlib.h>
#include "assembler.h"
#include "utils.h"

/*=========data structures=====*/

typedef enum { CODE_SEGMENT = 0, DATA_SEGMENT} segmentType;
typedef enum { ABSOLUTE = 0, EXTENAL = 1, RELOCATABLE = 2 } areType;

/* Memory Word */

typedef struct memoryWord_s/* 14 bits */
{
    int address; /*memory word address*/

    union {
        /* firstCommands word  */
        struct {
            unsigned int are : 2;       /* are type */
            unsigned int dest : 2;        /* Destination op addressing method code */
            unsigned int src : 2;        /* Source op addressing method code */
            unsigned int opcode : 4;    /* Command code */
            unsigned int  : 4;            /* Unused Bit */
        } firstCmd;

        /* immediate addressing operand value */
        struct {
            unsigned int are : 2;       /* are type */
            int value : 12;             /* address or offset value */
        } operandValue;

        /* addressWord - memory address or offset (array offset) */
        struct {
            unsigned int are : 2;       /* are type */
            unsigned int value : 12;    /* address or offset value */
        } operandAddress;

        /* Registers  */
        struct {
            unsigned int are : 2;       /* are type */
            unsigned int dest : 3;      /* Source register code */
            unsigned int src : 3;       /* Destination register code */
            unsigned int : 6;            /* Unused Bit */
        } reg;

        /* data  */
        int value : 14; /* string / data words */

    } word;

    struct memoryWord_s *nextWord;
} memoryWord;


/* Function Prototypes */

/* return current data counter */
int getDC();

/*Increase by one data counter (DC) */
void increaseDC();

#endif //MAMAN14_MEMORYWORD_H
