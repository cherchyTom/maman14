//
// Created by ubuntu16 on 7/25/19.
//

#ifndef MAMAN14_CONSTSTABLES_H
#define MAMAN14_CONSTSTABLES_H

#include <string.h>
#include "assembler.h"
//#include "firstRead.h"

/* ====== Functions Prototypes====== */

/*
 * get command name and return pointer to its struct or NULL if not exist
 */
const command* getCmd(char* cmdName);

/*
 * get command name and return its number of params or -1 if not exist
 */
int getCmdNParams(char* cmdName);

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

#endif //MAMAN14_CONSTSTABLES_H
