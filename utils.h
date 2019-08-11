/*This is a header file of utils.c
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avraham Yarblum 305567083
 *
 */

#ifndef MAMAN14_UTILS_H
#define MAMAN14_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

/*=======external functions prototypes=======*/
/* return currentLine value */
extern int getCurrentLine();
/* return isError value */
extern boolean getErrorStatus();
/* get boolean status and set it to be isError value */
extern void setErrorStatus(boolean status);

/*=======Macros=======*/
#define ERORR_MSG(msgParams)\
    printf("Error in line %d:\n",getCurrentLine());\
    printf msgParams;\
    setErrorStatus(TRUE);

/* check if ptr is NULL or not
 * In case of null print error massage and exit program
 * @params - pointer to check and string to add to the massage
 */

/*=======Function prototypes=======*/
/*check if a pointer has a null value and retun error - for using after dynamic memory allocation
 * Params - void pointer *ptr and constant string for error massage*/
void validateMemoryAllocation(void* ptr, char* desc);

/*try to open a file with a given name ,postfix and mode (permissions)
 *In case of success return a pointer to FILE structure otherwise exit from the program
 */
FILE* openFile(char*fileName,char* filePostfix,char* mode);

/*Input File fd, buf get a string into, and maxLength of char to read excluding \n & \0
 * Puts line from file fd in buf
 * return false if read failed, otherwise false
 */
boolean readLine(FILE *fd, char *buf, int maxLength);

/* get pointer to FILE and return true if it is empty, otherwise false */
boolean isEmptyFile(FILE *fd);

/*close a given file. in case the file is empty or assembler detected and error, it is deleted.
 * Params- fileName, file postfix and FILE pointer to the open file.
 */
void closeOutputFile(char*fileName, char* filePostfix, FILE* fd);

/*get int num which is stored in numOfBits bits and return it in spacial base 4 format as a string, stored in input char*param
 *assumption - num of bits >= 2*/
void intToSpecial4Base(int num, int numOfBits, char* specialBaseNum);

#endif