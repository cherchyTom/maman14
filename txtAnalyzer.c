/*This file contains string manipulation functions, which are in used in different parts of the program.
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */

#include "txtAnalyzer.h"

/* Removes white spaces from the beginning of a string
 * get pointer to string to adjust (char*) */
void leftTrim(char **str){
    /*empty string*/
    if (!*str || !**str)
        return;

    /* skip all white spaces until the first actual char from left side of the string */
    while(isspace(**str))
        (*str)++;
}

/* Removes white spaces from the end of a string
 * get pointer to string to adjust (char*) */
void rightTrim(char *str){
    char *eos;
    /*empty string*/
    if(!str || !*str)
        return;

    /* set pointer to end of string */
    eos = str + strlen(str) - 1;
    /* delete all white spaces until the fists actual char from right side of the string */
    while(isspace(*eos) && eos != str) {
        *eos = '\0';
        eos--;
    }
}

/*retrieve first word from a string str to buf by a given delimiter.
 * copy up to buff size - 1 chars
 * */
void getNextWordByDelimiter(char *buf, char *str, char delimiter,int bufSize){
    int i;
    /*copy char by char until delimiter or end of string */
    for(i=0;*str && *str != delimiter && i<bufSize -1; str++,buf++,i++)
        *buf = *str;
    /*add end of string to buf*/
    *buf = '\0';
    return;
}

/*
 *get a string and return true if contains only whitespaces chars otherwise return false
 */
boolean isEmptySTR(char* str){
    /*empty string*/
    if(!str)
        return TRUE;

    /* in case non white space found return true*/
    while (*str != '\0'){
        if(!isspace(*str))
            return FALSE;
        str++;
    }
    /* all chars are white spaces*/
    return TRUE;
}

/*
 *get a string and return true if contains only numeric chars and optional sign symbol otherwise return false
 */
static boolean isStrNumber(char *str){
    /*empty string*/
    if(!str)
        return FALSE;

    /*check for a sign symbol and skip it*/
    if(*str == '+' || *str == '-')
        str++;

    /* in case non digit is found return true*/
    while (*str != '\0'){
        if(!isdigit(*str))
            return FALSE;
        str++;
    }
    /* all chars are white spaces*/
    return TRUE;
}

/*
 *get a string numStr and convert it to signed integer
 * @Param *numStr - string to convert
 * @Param *numInt - pointer to save the converted number
 * @Param numOfBits - bits amount to save int number - for range validation
 * @Param isSigned - 1 for sign range and 0 of unsigned range
 *return true in success otherwise return false
 */
boolean isLegalNum(char *numStr, int numOfBits, int *numInt, int isSigned){
    /*max Unsigned number can be represented with #numOfBits = 2^n - 1 */
    /*min Unsigned number is 0*/
    /*max signed number can be represented with #numOfBits = 2^(n-1) - 1 */
    /*min signed number can be represented with #numOfBits = -maxNum - 1*/
    int n = numOfBits - isSigned;
    int maxNum = (1 << n) - 1;
    int minNum = (isSigned)? -maxNum -1 : 0;
    /*empty string*/
    if(!numStr || !*numStr){
        ERORR_MSG(("Empty parameter - expecting to get a number\n"));
        return FALSE;
    }
    /*check for number validity*/
    if(!isStrNumber(numStr)){
        ERORR_MSG(("Invalid parameter \"%s\" - expecting to get a number\n",numStr));
        return FALSE;
    }
    /* converting STR to int */
    *numInt = atoi(numStr);

    /* Check if the number within a range that is defined by given bits amount*/

    if (*numInt > maxNum || *numInt < minNum){
        ERORR_MSG(("\"%s\" is out of range, number must be between %d and %d.", numStr, minNum, maxNum));
        return FALSE;
    }

    return TRUE;
}


/*
 *get a string and return true if contains only alphanumeric chars otherwise return false
 */
boolean isalnumSTR(char* str){
    /*empty string*/
    if(!str)
        return FALSE;

    /* search for a non alpha numeric number*/
    while (*str != '\0'){
        if(!isalnum(*str))
            return FALSE;
        str++;
    }

    /* all chars are alphanumeric*/
    return TRUE;
}


