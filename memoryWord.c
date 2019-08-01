//
// Created by ubuntu16 on 8/1/19.
//

#include "memoryWord.h"

/* =======Global Variables ======*/

/* instruction and data counter*/
static IC = FIRST_ADDRESS;
static DC = 0;

/*pointers to symbol list head and tail*/
static memoryWord *codeListHead;
static memoryWord *codeListTail;
static memoryWord *dataListHead;
static memoryWord *dataListTail;


int getDC(){
    return DC;
}

void increaseDC(){
    DC++;
    return;
}

int getIC(){
    return IC;
}

void increaseIC(){
    IC++;
    return;
}

void resetDC(){
    DC = 0;
    return;
}
void resetIC(){
    IC = FIRST_ADDRESS;
    return;
}

/*add memory word to given list and set address
 * Params - given list head and tail, new word to add, word address*/
static void addToMemoryList(memoryWord *listHead,memoryWord *listTail, memoryWord *newWord, int address ){
    newWord->address = address; /* set address */
    /* empty list */
    if(!listtHead){
        listHead = listTail = newWord;
    }

    /*list is not empty */
    listTail->nextWord = newWord;
    listTail= newWord;
}

/* create memory word record set it in list and return a pointer to it */
memoryWord* createMemoryWord(segmentType segType){
    memoryWord *newMW = (memoryWord*)malloc(sizeof(memoryWord));
    validateMemoryAllocation(newMW,"when creating new memory word record\n");
    /*validate memory size */
    if(!(IC+DC < MAX_MEMORY_WORDS)){
        ERORR_MSG(("Too Many instructions - memory is limited to %d words\n",MAX_MEMORY_WORDS));
        return NULL;
    }

    /*add memory word to relevant list and increase its counter*/
    if(segType == CODE_SEGMENT)
        addToMemoryList(codeListHead,codeListTail,newMW,IC++);
    else if (segType == DATA_SEGMENT)
        addToMemoryList(codeListHead,codeListTail,newMW,DC++);
    else {
        ERORR_MSG(("Invalid Memory Type - Cannot handle\n"));
    }
    return newMW;
};

/* get int param and add it to data memory list*/
void addDataMemoryWord(int param){
    /* create new data word and validate success */
    memoryWord *dataWord = createMemoryWord(DATA_SEGMENT);
    if(!dataWord)
        return;
    /*set data value*/
    dataWord->word.value = param;
    return;
}

/* get string param and code it to data memory words*/
void addStringMemoryWord(char* param){
    /* code all the chars between " " to memory words */
    param++; /*skip first '"'*/
    while(*param != '"')
        addDataMemoryWord((int)(*param));
    return;
}

/* update data memory word address by given offset */
void updateDataSegmentByOffset(int offset){
    memoryWord *tmp;
    /*empty list*/
    if(!dataListHead)
        return;

    for(tmp = dataListHead; tmp != NULL; tmp = tmp->next)
        tmp->address += offset;
}

/* update data memory word address by given offset */
void mergeCodeAndDataSegments(){
    /* empty code memory words list*/
    if(!codeListHead)
        codeListHead = codeListTail = dataListHead;
    /* concatenate data to code list*/
    else
        codeListTail->nextWord = dataListHead;

    return;
}

