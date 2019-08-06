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
#include "labelsQueue.h"

/*=========Data Structures======*/


/*========Prototypes =====*/

/* return currentLine value */
int getCurrentLine();

/* get int line number and set it to be currentLine value */
void setCurrentLine(int lineNumeber);

/* return isError value */
boolean getErrorStatus();

/* get boolean status and set it to be isError value */
void setErrorStatus(boolean status);

/* Manage first read flow:
 * line parsing -> code validation -> create symbol list, memory word list and label list for second read
 *@Param pointer to lineInfo struct  */
void firstRead(FILE*fd);
#endif //MAMAN14_FIRSTREAD_H
