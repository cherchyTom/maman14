//
// Created by ubuntu16 on 7/20/19.
//

#ifndef MAMAN14_UTILS_H
#define MAMAN14_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

/*=======external functions prototypes=======*/
/* return currentLine value */
extern int getCurrentLine();
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

void validateMemoryAllocation(void* ptr, char* desc);
#endif //MAMAN14_UTILS_H

/*try to open a file with a given name ,postfix and mode (permissions)
 *In case of success return a pointer to FILE structure otherwise exit from the program
 */
FILE* openFile(char*fileName,char* filePostfix,char* mode);

/*Input File fd, buf get a string into, and maxLength of char to read excluding \n & \0
 * Puts line from file fd in buf
 * return false if read failed, otherwise false
 */
boolean readLine(FILE *fd, char *buf, int maxLength);

/*get int num which is stored in numOfBits bits and print it in spacial base 4 format
 * assumption - num of bits >= 2*/
void printSpecialBaseNum(int num, int numOfBits)