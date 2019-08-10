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

/*====external functions declarations======= */
/* reset memory word module */
extern void clearMemoryWordList();
/*reset labelsQueue  module */
extern void clearLabelsQ();
/*reset symbol table module */
extern void clearSymbolTable();


/*=======Functions=======*/

/*create output files
 * get char* fileName and pointers to FILE* for each file - object, entry and extern*/
static void createOutputFiles(char* fileName,FILE **obFd, FILE **extFd, FILE **entFd){
    /*create object, entry and extern files */
    *obFd = openFile(fileName,OBJ_POSTFIX,"w+");
    *extFd = openFile(fileName,EXT_POSTFIX,"w+");
    *entFd = openFile(fileName,ENT_POSTFIX,"w+");
    return;
}

/*close output files / delete in cases of error or empty files
 *get char* fileName and FILE pointers for each file - object, entry and extern*/
static void closeFiles(char* fileName, FILE *obFd, FILE *extFd, FILE *entFd){
    /*create object, entry and extern files */
    closeOutputFile(fileName,OBJ_POSTFIX,obFd);
    closeOutputFile(fileName,EXT_POSTFIX,extFd);
    closeOutputFile(fileName,ENT_POSTFIX,entFd);
    return;
}

/*reset assembler modules, free memory and init globals*/
static void resetAssembler(){
    clearSymbolTable(); /* reset symbol table */
    clearLabelsQ(); /* reset labels queue */
    clearMemoryWordList(); /* reset memoryWord list */
    setErrorStatus(FALSE); /* initiate error flag to false */
    setCurrentLine(0); /* initiate currentLine counter to 0 */
}

/*Manage line parsing flow
 * get char* fileName to parse*/
void parseFile(char*fileName){
    FILE *asFd, *obFd, *entFd , *extFd;

    /*open assembly file */
    asFd = openFile(fileName,ASSEMBLY_POSTFIX,"r");
    /*send assembly file to first read processing*/
    printf("Start processing \"%s%s\" file.\n",fileName,ASSEMBLY_POSTFIX);
    firstRead(asFd);
    fclose(asFd);

    /*In case of error finish here*/
    if(getErrorStatus()){
        printf("Failed to process \"%s%s\" file.\n",fileName,ASSEMBLY_POSTFIX);
        return;
    }

    /*create output files and run second read */
    createOutputFiles(fileName, &obFd,&extFd, &entFd);
    secondRead(obFd,extFd,entFd);

    /* close (& delete empty ) output files */
    closeFiles(fileName,obFd,extFd,entFd);
    printf("Finish processing \"%s%s\" file %s.\n\n",fileName,ASSEMBLY_POSTFIX,(getErrorStatus()? "with errors" : "successfully"));

    /* reset assembler module*/
    resetAssembler();
    return;
}

/*Main Program function - read file names from command line and send then to parsing.
 *Params - command line arguments*/
int main(int argc, char *argv[]){

    /* check for valid argument amount */
    if(argc < 2) {
        printf("Please run with at list one file name argument\n");
        return 1;
    }
    /*send each file from the argument line to parsing*/
    while (*(++argv)){
        parseFile(*argv);
    }

    return 0;
}

