/*This is a header file of firstRead.c
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avraham Yarblum 305567083
 *
 */

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


/*========Prototypes =====*/

/* return currentLine value */
int getCurrentLine();

/* get int line number and set it to be currentLine value */
void setCurrentLine(int lineNumber);

/* return isError value */
boolean getErrorStatus();

/* get boolean status and set it to be isError value */
void setErrorStatus(boolean status);

/* Manage first read flow:
 * line parsing -> code validation -> create symbol list, memory word list and label list for second read
 *@Param pointer to lineInfo struct  */
void firstRead(FILE*fd);
#endif
