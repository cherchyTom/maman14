/*This is a header file of constTables.c
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avraham Yarblum 305567083

 *
 */

#ifndef MAMAN14_CONSTSTABLES_H
#define MAMAN14_CONSTSTABLES_H

#include <string.h>
#include "assembler.h"

/* ====== Functions Prototypes====== */

/*
 * get command name and return pointer to its struct or NULL if not exist
 */
const command* getCmd(char* cmdName);


/*
 * get register name and return its code or -1 if not exist
 */
int getRegCode(char* regName);

/*
 * get string and return true if it is a reserved word (command or register name) otherwise return false
 */
boolean isReservedWord (char* word);

/*
 * get instruction name and return pointer to the relevant instruction struct or to invalid struct in case name is not found
 */
const instruction* getInstructionByName(char* name);

#endif
