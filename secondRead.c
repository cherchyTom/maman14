/*This file manages the second processing flow of user input.
 *This flow Including:
 *      Scanning the labels queue for second interpretation, validation and building  the empty memory words.
 *      Scanning memory Words table and symbols list table in order to build object, entry and extern files.
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */
#include "secondRead.h"

/*write the first line in output object file.
 * Assumption - an object file will be created to DC = 0 && IC = 0 (no code or instruction in input file.
 *Param - pointer to FILE */
static writeObjectFirstLine(FILE *fd){
    fprintf(fd,"\t%d\t%d\n",getIC(),getDC());
    return;
}


/*write memory address and  memory word, in special base, in output object file.
 *Param - pointer to FILE, address of word, memory word (integer) */
static writeObjectLine(FILE *fd, int address, int word){
    char specialBaseNum[MEMORY_WORD_BITS/2 +1]; /* set enough memory to store 4 based memory word + end of string sign */
    intToSpecial4Base(word,MEMORY_WORD_BITS,specialBaseNum); /*convert int to spacial base string*/
    fprintf(fd,"%04d\t%s\n",address,specialBaseNum);
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

/*validate label appears in symbol table and retrieve its params.
 *Param - pointer to label name, and pointer to address and type for getting the values into */
static boolean getLabelParams(char* label, int *address,sentanceType *symbolType,instructionType *instType){
    /* validate label exists in symbol table and get symbol params*/
    if(!getSymbolValues(label,address,symbolType,instType)){
        ERORR_MSG(("Label \"%s\" is not defined.\n",label));
        return FALSE;
    }
    /* validate symbol is not a macro*/
    if(*symbolType == MACRO){
        ERORR_MSG(("Macro name \"%s\" cannot declared or used as a label.\n",label));
        return FALSE;
    }
    return TRUE;
}

/*validate entry label appears in symbol table and write it to entry file.
 *Param - pointer to entry FILE, pointer to label name */
static void entryLabelHandler(FILE *entFd, char*label){
    /* variables to store label info */
    sentanceType symbolType;
    instructionType symbolInstType;
    int symbolAddress;

    /* validate label exists */
    if(!getLabelParams(label,&symbolAddress,&symbolType,&symbolInstType))
        return;
    /* write label in entry file */
    if(!getErrorStatus())
        writeEntExtLine(entFd,label,symbolAddress);
    return;
}

/*validate operand label appears in symbol table, update its memory word if required, write it to obj + ext files.
 *Param - pointer to entry FILE, pointer to label name */
static void operandLabelHandler(FILE *obFd,FILE *extFd, char*label,boolean *isDefined, int *wordAddress, int *wordValue){
    /* variables to store label info */
    sentanceType symbolType;
    instructionType symbolInstType;
    int symbolAddress;
    /* validate label exists */
    if(!getLabelParams(label,&symbolAddress,&symbolType,&symbolInstType))
        return;
    /*in case of extern label and no error write it in ext file */
    if(symbolInstType == EXTERN && !getErrorStatus())
        writeEntExtLine(extFd,label,*wordAddress);

    /*in case the label defined after it's usage update memory word and re-get its params*/
    if(!*isDefined){
        areType are = (symbolInstType == EXTERN) ? EXTENAL : RELOCATABLE;
        updateAddressMemoryWord(are,symbolAddress);
        getNextWordParams(wordAddress,wordValue);
    }
    /*write memory word object file */
    if(!getErrorStatus())
        writeObjectLine(obFd,*wordAddress,*wordValue);
    return;
}


/* Manage second "read" flow:
 * scanning labels in labelsQueue for second processing + scan memoryWord list (in parallel)
 * write the object, extern and entry files
 *@Params - pointers object, entry and extern files to write into*/
void secondRead(FILE *obFd, FILE *entFd, FILE *extFd){
    /* variables to save qlabel and memory word structs fields */
    qLabelType type;
    char label[MAX_LABEL_LEN];
    boolean isDefined;
    int labelAddress, lineNumber, wordAddress,wordValue;

    /*write object file first line*/
    writeObjectFirstLine(obFd);

    /*while label queue is not empty  */
    while(!isEmptyQ()){
        /*get next label params*/
        getNextLabelParams(&type,label,&isDefined, &labelAddress, &lineNumber);
        printf("Label params - type: %u, label: %s isDef: %d, address: %d lineNumber: %d\n", type,label , isDefined, labelAddress,lineNumber );

        /*set currentLine (the line in the original file that is actually proceed in the second run*/
        setCurrentLine(lineNumber);

        /*In case of entry label, process it, remove it from queue and get next label */
        if(type == ENTRY_LABEL){
            entryLabelHandler(entFd, label);
            removeLabelFromQ();
            continue;
        }
        /*==next label in queue is a code label ==*/

        /*write (in case of no error) and remove all memory words until the word related to the retrieved label*/
        for(getNextWordParams(&wordAddress,&wordValue); wordAddress < labelAddress;getNextWordParams(&wordAddress,&wordValue)){
            if(!getErrorStatus())
                writeObjectLine(obFd,wordAddress,wordValue);
            removeFirstMemoryWord();
        }
        /*handle  operand label */
        operandLabelHandler(obFd,extFd,label,&isDefined,&wordAddress,&wordValue);
        /* remove label and memory word from queues */
        removeFirstMemoryWord();
        removeLabelFromQ();
    }

    /* write memory words that left in queue */

    for(getNextWordParams(&wordAddress,&wordValue); !isEmptyCodeListQ();getNextWordParams(&wordAddress,&wordValue)){
        if(!getErrorStatus())
            writeObjectLine(obFd,wordAddress,wordValue);
        removeFirstMemoryWord();
    }

    return;
}
