//
// Created by ubuntu16 on 7/25/19.
//

#ifndef MAMAN14_TXTANALYZER_H
#define MAMAN14_TXTANALYZER_H

#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "utils.h"
#include "constsTables.h"

/*====MACROS====== */



/*====Functions Prototypes====== */

/* Removes white spaces from the beginning of a string
 * get pointer to char* in order to change char* pointer value
 * */
void leftTrim(char **str);

/* Removes white spaces from the end of a string */
/* get pointer to char */

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
 *return true in success otherwise return false
 */
boolean isLegalNum(char *numStr, int numOfBits, int *numInt);

/* Returns if the strParam is a legal string param (enclosed in quotes), and remove the quotes
 * @param - pointer to char strParam*/
boolean isLegalStringParam(char *strParam);

/*
 * get Label/ Macro name and return true if is valid otherwise return true
 */
boolean isValidLabel(char* label);

#endif //MAMAN14_TXTANALYZER_H
