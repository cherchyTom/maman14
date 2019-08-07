/*This file manages the second processing flow of user input.
 *This flow Including:
 *      Scanning the labels queue for second interpretation, validation and building  the empty memory words.
 *      Scanning memory Words table and symbols list table in order to build object, entry and extern files.
 *      the labels queue processing and files writing is done in parallel.
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */
#include "secondRead.h"

/*write the first line in output object file.
 *Param - pointer to FILE */
static writeObjectFirstLine(FILE *fd){
    int c = getCodeTail()->address;
    int d = getDatahead()->address;
    fprintf(fd,"\t%d\t%d\n",getIC(),getDC());

    fprintf(fd,"last code number = %d\n",c);
    fprintf(fd,"first data number = %d\n",d);

    return;
}

/*write memory address and  memory word, in special base, in output object file.
 *Param - pointer to FILE, address of word, memory word (integer) */
static writeObjectLine(FILE *fd, int address, int word){
    fprintf(fd,"%04d\t%s\n",address,intToSpecialBase(word));
    return;
}

/*write a line in output entry/extern file.
 *Param - pointer to FILE, pointer to label name and its value (address) */
static void writeEntExtLine(FILE *fd, char*label, int value){
    /* variables to store label info */
    if(!fd)
        return;
    fprintf(fd,"%s\t%04d\n",label,value);
    return;
}


/*validate entry label appears in symbol table and write it to entry file.
 *Param - pointer to entry FILE, pointer to label name */
static void entryLabelHandler(FILE *entFd, char*label){
    /* variables to store label info */
    sentanceType symbolType;
    int symbolAddress;

    /* validate symbol exists and get symbol params*/
    if(!getSymbolValues(label,&symbolAddress,&symbolType)){
        ERORR_MSG(("Entry Label \"%s\" is not defined.\n",label));
        return;
    }
    /* validate symbol is not a macro*/
    if(symbolType == MACRO){
        ERORR_MSG(("Macro name \"%s\" cannot declared as an entry label.\n",label));
        return;
    }
    /* write label in entry file */
    if(!getErrorStatus())
        writeEntExtLine(entFd,label,symbolAddress);
    return;
}

void secondRead(FILE *obFd, FILE *entFd, FILE *extFd){
    /* variables to save qlabel struct fields */
    qLabelType type;
    char label[MAX_LABEL_LEN];
    boolean isDefined;
    int address, lineNumber;

    /*write object file first line*/
    writObjectFirstLine(obFd);

    /*while label queue is not empty  */
    while(!isEmptyQ()){
        /*get next label params*/
        getNextLabelParams(&type,label,&isDefined, &address, &lineNumber);
        printf("Label params - type: %u, label: %s isDef: %d, address: %d lineNumber: %d\n", type,label , isDefined, address,lineNumber );

        /*set currentLine (the line in the original file that is actually proceed in the second run*/
        setCurrentLine(lineNumber);

        /*In case of entry label handle it, remove it from queue and get next label */
        if(type == ENTRY_LABEL){
            entryLabelHandler(entFd, label);
            removeLabelFromQ();
            continue;
        }


        removeLabelFromQ();
    }


    return;
}
