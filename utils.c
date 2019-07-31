#include "utils.h"
void validateMemoryAllocation(void* ptr, char* desc){
    if(!ptr){
        ERORR_MSG(("Memory allocation failure %s\n",desc));
        exit(1);
    }
}

FILE* openFileRead(char*fileName){
    FILE *fd = fopen(fileName,"r");
    if(!fd){
        fprintf(stdout,"Failed to open file %s\n",fileName);
        exit(1);
    }
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