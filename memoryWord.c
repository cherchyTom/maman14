/*This file implements and manages the memoryWord module.
 *The memoryWord codes and holds all the memory words from the first and second read processing, that need to be writen in the output files.
 *The memoryWord is implemented as a linked list, and provide functions to operate it as a queue.
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avraham Yarblum 305567083
 *
 */

#include "memoryWord.h"

/* =======Global Variables ======*/

/* instruction and data counter*/
static int IC = 0;
static int DC = 0;

/*pointers to symbol list head and tail*/
static memoryWord *codeListHead;
static memoryWord *codeListTail;
static memoryWord *dataListHead;
static memoryWord *dataListTail;

/* =======Functions ======*/

int getDC(){
    return DC;
}

int getIC(){
    return IC;
}

void resetDC(){
    DC = 0;
    return;
}
void resetIC(){
    IC = 0;
    return;
}

/*add memory word to given list and set address
 * Params - given list head and tail, new word to add, word address*/
static void addToMemoryList(memoryWord **listHead,memoryWord **listTail, memoryWord *newWord, int address ){
    newWord->address = address; /* set address */
    newWord->nextWord = NULL;
    /* empty list */
    if( !listHead || !*listHead){
        *listHead = newWord;
        *listTail = newWord;
    }

    /*list is not empty */
    (*listTail)->nextWord = newWord;
    *listTail= newWord;
}

/* create memory word record set it in list and return a pointer to it */
static memoryWord* createMemoryWord(segmentType segType){
    memoryWord *newMW = (memoryWord*)malloc(sizeof(memoryWord));
    validateMemoryAllocation(newMW,"when creating new memory word record\n");
    /*validate memory size */
    if((IC+DC >= MAX_MEMORY_WORDS)){
        ERORR_MSG(("Too Many instructions - memory is limited to %d words\n",MAX_MEMORY_WORDS));
        return NULL;
    }

    /*add memory word to relevant list and increase its counter*/
    if(segType == CODE_SEGMENT)
        addToMemoryList(&codeListHead,&codeListTail,newMW,FIRST_ADDRESS + IC++);
    else if (segType == DATA_SEGMENT)
        addToMemoryList(&dataListHead,&dataListTail,newMW,FIRST_ADDRESS + DC++);
    else {
        ERORR_MSG(("Invalid Memory Type - Cannot handle\n"));
    }
    return newMW;
}

/* get first code memory params and add it to code memory list
 * @params - are code, dest & src operands addressing code, operation code */
void addFirstCodeMemoryWord(int dest, int src,int opcode){
    /* create new data word and validate success */
    memoryWord *firstCodeWord = createMemoryWord(CODE_SEGMENT);
    if(!firstCodeWord)
        return;
    /*set bits values*/
    firstCodeWord->word.firstCmd.are = ABSOLUTE; /* always zero at first code word */
    firstCodeWord->word.firstCmd.dest = dest;
    firstCodeWord->word.firstCmd.src = src;
    firstCodeWord->word.firstCmd.opcode = opcode;
    firstCodeWord->word.firstCmd.unused = DEFAULT_MEMORY_VALUE;
    return;
}

/* get immediate operand value and add it to code memory list
 * @params - operand value  */
void addOpValueMemoryWord(int value){
    /* create new data word and validate success */
    memoryWord *opValue = createMemoryWord(CODE_SEGMENT);
    if(!opValue)
        return;
    /*set bits values*/
    opValue->word.operandValue.are = ABSOLUTE;
    opValue->word.operandValue.value = value;
    return;
}

/* get address or offset  memory word params and add it to code memory list
 * @params - are code, value (operand address or array offset) */
void addAddressMemoryWord(areType are, int value){
    /* create new data word and validate success */
    memoryWord *AddressWord = createMemoryWord(CODE_SEGMENT);
    if(!AddressWord)
        return;
    /*set bits values*/
    AddressWord->word.operandAddress.are = are;
    AddressWord->word.operandAddress.value = value;
    return;
}

/* get register values and add it to code memory list
 * @params - dest and src registers code
 * in case of only one register is represented the second one will be set to zero*/
void addRegisterMemoryWord(int dest, int src){
    /* create new data word and validate success */
    memoryWord *reg = createMemoryWord(CODE_SEGMENT);
    if(!reg)
        return;
    /*set bits values*/
    reg->word.reg.are = ABSOLUTE;
    reg->word.reg.dest = dest;
    reg->word.reg.src = src;
    reg->word.reg.unused = 0;
    return;
}

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
    /* code all the chars to memory words */
    while(*param) {
        addDataMemoryWord((int) (*param));
        param++;
    }
    /* add zero for end of string */
    addDataMemoryWord(0);
    return;
}

/* update data memory word address by given offset */
static void updateDataSegmentByOffset(int offset){
    memoryWord *tmp;
    /*empty list*/
    if(!dataListHead)
        return;

    for(tmp = dataListHead; tmp != NULL; tmp = tmp->nextWord)
        tmp->address += offset;
    return;
}


/* concatenate data memory list to end of the code memory list */
void mergeCodeAndDataSegments(){
    /*Update data words memory address by adding IC*/
    updateDataSegmentByOffset(IC);
    /* empty code memory words list*/
    if(!codeListHead)
        codeListHead = codeListTail = dataListHead;
    /* concatenate data to code list*/
    else
        codeListTail->nextWord = dataListHead;

    return;
}

/*get params from first memory word in list
 * Params - pointers to variables to save struct fields*/
void getNextWordParams(int *address, int *word){
    /*empty list */
    if(!codeListHead){
        return;
    }
    *address = codeListHead->address;
    *word = codeListHead->word.value; /* word.value represents the int value of all the 14 bits of the word union struct */
    return;
}

/* get address memory word params and set them on the first memory word
 * @params - are code, value (operand address) */
void updateAddressMemoryWord(areType are, int value){
    /*empty queue */
    if(!codeListHead){
        return;
    }
    /*set bits values*/
    codeListHead->word.operandAddress.are = are;
    codeListHead->word.operandAddress.value = value;
    return;
}

/*remove first memory word record from memory word list*/
void removeFirstMemoryWord(){
    memoryWord* wordToDelete;
    /*empty queue */
    if(!codeListHead){
        return;
    }
    wordToDelete = codeListHead;
    codeListHead = codeListHead->nextWord;
    free(wordToDelete);
    return;
}

/*return true if code list is empty otherwise false */
boolean isEmptyCodeListQ(){
    return(!codeListHead) ? TRUE : FALSE;
}

/* clear memory word list */
void clearMemoryWordList(){
    memoryWord* wordToDelete;
    /*initiate IC & DC*/
    resetIC();
    resetDC();
    /*free table memory*/
    while (codeListHead){
        wordToDelete = codeListHead;
        codeListHead = codeListHead->nextWord;
        free(wordToDelete);
    }
    /*initiate pointers to code and data memory lists*/
    codeListHead = codeListTail = dataListHead = dataListTail = NULL;
    return;
}


