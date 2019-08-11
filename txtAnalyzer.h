/*This is a header file of txtAnalyzer.c
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avraham Yarblum 305567083
 *
 */
#ifndef MAMAN14_TXTANALYZER_H
#define MAMAN14_TXTANALYZER_H

#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "utils.h"


/*====Functions Prototypes====== */

/* Removes white spaces from the beginning of a string
 * get pointer to char* in order to change char* pointer value
 * */
void leftTrim(char **str);

/* Removes white spaces from the end of a string */
/* get pointer to char* to adjust*/

void rightTrim(char *str);

/*retrieve first part from a string str to buf by a given delimiter or by the end of string */
void getNextWordByDelimiter(char *buf, char *str, char delimiter,int bufSize);

/*
 *get a string and return true if contains only whitespaces chars otherwise return false
 */
boolean isEmptySTR(char* str);

/*
 *get a string numStr and convert it to signed integer
 * @Param *numStr - string to convert
 * @Param *numInt - pointer to save the converted number
 * @Param numOfBits - bits amount to save int number - for range validation
 * @Param isSigned - 1 for sign range and 0 of unsigned range
 *return true in success otherwise return false
 */
boolean isLegalNum(char *numStr, int numOfBits, int *numInt, int isSigned);

/*
 *get a string and return true if contains only alphanumeric chars otherwise return false
 */
boolean isalnumSTR(char* str);


#endif
