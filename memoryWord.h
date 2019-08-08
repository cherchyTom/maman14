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
            unsigned int unused: 4;            /* Unused Bit */
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
            unsigned int unused: 6;            /* Unused Bit */
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

/* return current code counter */
int getIC();

/*Increase by one data counter (IC) */
void increaseIC();

/*initiate DC */
void resetDC();

/*initiate IC */
void resetIC();


/* create memory word record set it in list and return a pointer to it */
memoryWord* createMemoryWord(segmentType segType);

/* get first code memory params and add it to code memory list
 * @params - are code, dest & src operands addressing code, operation code */
void addFirstCodeMemoryWord(int dest, int src,int opcode);

/* get immediate operand value and add it to code memory list
 * @params - operand value  */
void addOpValueMemoryWord(int value);

/* get address or offset  memory word params and add it to code memory list
 * @params - are code, value (operand address or array offset) */
void addAddressMemoryWord(areType are, int value);

/* get register values and add it to code memory list
 * @params - dest and src registers code
 * in case of only one register is represented the second one will be set to zero*/
void addRegisterMemoryWord(int dest, int src);

/* get int param and add it to data memory list*/
void addDataMemoryWord(int param);

/* get string param and code it to data memory words*/
void addStringMemoryWord(char* param);

/* update date memory word code address and concatenate it to the end of the code memory list */
void mergeCodeAndDataSegments();

/*get params from first memory word in list
/* Params - pointers to variables to save struct fields*/
void getNextWordParams(int *address, int *word);

/* get address memory word params and set them on the first memory word
 * @params - are code, value (operand address) */
void updateAddressMemoryWord(areType are, int value);

/*remove first memory word record from memory word list*/
void removeFirstMemoryWord();

/*return true if code list is empty otherwise false */
boolean isEmptyCodeListQ();

/* clear memory word list */
void ClearMemoryWordList ();

#endif //MAMAN14_MEMORYWORD_H
