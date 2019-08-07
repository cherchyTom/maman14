#include "utils.h"
void validateMemoryAllocation(void* ptr, char* desc){
    if(!ptr){
        ERORR_MSG(("Memory allocation failure %s\n",desc));
        exit(1);
    }
}

FILE* openFile(char*fileName, char* filePostfix, char* mode){
    FILE *fd;
    char *fullFileName = (char*)malloc(strlen(fileName) + strlen(filePostfix) + 1); /*store full file name*/
    validateMemoryAllocation(fullFileName,"when trying to open file to read\n");

    /* concatenate file name and postfix */
    sprintf(fullFileName, "%s%s", fileName, filePostfix);
    if(!(fd = fopen(fullFileName,mode))){
        fprintf(stdout,"Failed to open file %s\n",fullFileName);
        free(fullFileName);
        exit(1);
    }
    free(fullFileName);
    return fd;
}


boolean readLine(FILE *fd, char *buf, int maxLength){
    char *endOfLine;

    /* read line and return false at EOF or Error */
    /* use maxLength+2 for additional chars of \n and \0 at the end of the line */
    if (!fgets(buf, maxLength+2, fd))
        return FALSE;

    /* search for \n and replace with \0 */
    endOfLine = strchr(buf, '\n');
    if (endOfLine)
        *endOfLine = '\0';
        /*in case \n was not found keep read chars until the end of line */
    else{
        char c;
        do{
            c = fgetc(fd);
        } while (c != '\n' && c != EOF);
    }

    return TRUE;
}

/*get int num which is stored in numOfBits bits and print it in spacial base 4 format in FILE fd
 * assumption - num of bits >= 2*/
void fprintSpecialBaseNum(int num, int numOfBits, FILE *fd){
    unsigned long int mask = 3 ;/*init mask with format of 00..11 */
    char special4Base[numOfBits/2]; /* array to store new base chars */
    int i;

    /* convert each 2 bits from num to special char in the "quick conversion method"
     * do it for numOfBits/2  pairs and store in the array in descending order*/
    for(i= numOfBits/2 - 1; i>=0; i--, mask <<= 2){
        switch (num&mask){
            case 0: {
                special4Base[i] = '*';
                break;
            }
            case 1: {
                special4Base[i] = '#';
                break;
            }
            case 2: {
                special4Base[i] = '%';
                break;
            }
            case 3: {
                special4Base[i] = '!';
                break;
            }
            default:
                break;
        }
    }

    /*print array*/
    for(i=0; i<numOfBits/2; i++)
        fputc(special4Base[i],fd);

    return;
}