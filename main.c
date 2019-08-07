/*The main file.
 *This file manages the assembling process.
 *It calls the first and second read methods, and then creates the output files.
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */

#include <stdio.h>
#include "assembler.h"
#include "utils.h"
#include "firstRead.h"
#include "secondRead.h"

/*create output files
 * get char* fileName and pointers to FILE* for each file - object, entry and extern*/
void createOutPutFiles(char* fileName,FILE **obFd, FILE **extFd, FILE **entFd){
    /*create object, entry and extern files */
    *obFd = openFile(fileName,OBJ_POSTFIX,"w");
    *extFd = openFile(fileName,EXT_POSTFIX,"w");
    *entFd = openFile(fileName,ENT_POSTFIX,"w");
    return;
}

/*Manage line parsing flow
 * get char* fileName to parse*/
void parseFile(char*fileName){
    FILE *asFd, *obFd, *entFd , *extFd;
    char line[MAX_LINE_LEN+1]; /*TO DELETE */

    /*open assembly file */
    asFd = openFile(fileName,ASSEMBLY_POSTFIX,"r");
    /*send assembly file to first read processing*/
    printf("Start processing \"%s%s\" file.\n",fileName,ASSEMBLY_POSTFIX);
    firstRead(asFd);

    printf("\n................................print the original file:............................\n"); /*TO DELETE */
    rewind(asFd); /*TO DELETE */
    while (fgets(line, MAX_LINE_LEN+2, asFd))/*TO DELETE */
        printf("%s", line);/*TO DELETE */
    fclose(asFd);

    /*In case of error finish here*/
    if(getErrorStatus()){
        printf("Failed to process \"%s%s\" file.\n",fileName,ASSEMBLY_POSTFIX);
        return;
    }

    createOutPutFiles(fileName, &obFd, &entFd, &extFd);
    secondRead(obFd,extFd,entFd);

    /*delete files in case of error / delete empty files*/
    fclose(obFd);
    fclose(entFd);
    fclose(extFd);

    /* reset */

    return;
}


int main(int argc, char *argv[]){


    /* check for valid argument amount */
    if(argc < 2) {
        perror("Please run with at list one file name argument\n");
        return 1;
    }
    /*send each file from the argument line to parsing*/
    while (*(++argv)){
        parseFile(*argv);
    }

    return 0;
}

