/*This file implements and manages the symbolList.
 *The symbolList holds all symbol table records from the first read processing and use for second processing and writing the output files.
 *The symbolList is implemented as a linked list.
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */
#include "symbolList.h"

/*pointers to symbol list head and tail*/
static symbol *listHead;
static symbol *listTail;

/* create new symbol
 * @return pointer to new symbol
 * In case of error print a massage and exit from the program
 */
static symbol* createSymbol(){
    symbol* newSymbol = (symbol*)malloc(sizeof(symbol));
    validateMemoryAllocation(newSymbol,"when creating new symbol record\n");
    newSymbol->next = NULL;
    return newSymbol;
}

/* search symbol in table by label
* @params - symbol label
* @return pointer to founded symbol or NULL in case it was not found
*/
static symbol* searchSymbolByLabel(char* label){
    symbol* symbol1ToCheck = listHead;

    while (symbol1ToCheck){
        if(!strcmp(symbol1ToCheck->label,label))
            return symbol1ToCheck;
        symbol1ToCheck = symbol1ToCheck->next;
    }
    /*label does not exist in table*/
    return NULL;
}

/* add Symbol to end of the table
 * in case symbol is already exist return error
* @params - symbol fields - label,sentanceType,instType, value
*/
void addSymbol(char *label,sentanceType symbolType,instructionType instType, int value){
    symbol* newSymbol = createSymbol(); /* create a new symbol */

    /* set symbol data */
    strcpy(newSymbol->label,label);
    newSymbol->symbolType = symbolType;
    newSymbol->instType = instType;
    newSymbol->value = value;

    /*check is symbol label already exist*/
    if(searchSymbolByLabel(newSymbol->label)) {
        ERORR_MSG(("symbol %s is declared more then once\n", newSymbol->label));
        return;
    }

    /*empty list*/
    if(!listHead){
        listHead = listTail = newSymbol;
        return;
    }

    /*add new symbol to list end*/
    listTail->next = newSymbol;
    listTail = newSymbol;
    return;
}

/* get symbol name (label) and pointer to int (value) to retrieve the symbol value into
* @return True if symbol was found otherwise false
*/
boolean getSymbolValue(char* label, int *value){
    symbol* symbol= searchSymbolByLabel(label);
    /*symbol does not exist*/
    if(!symbol){
        return FALSE;
    }
    /*if label is found set its params */
    *value = symbol->value;
    return TRUE;
}

/* search for symbol and retrieve its fields.
 * get symbol label (name) to search and pointers to value and symbol type to retrieve  the parameters into
 */
boolean getSymbolParams(char* label, int *value, sentanceType *symbolType,instructionType *instType){
    symbol* symbol= searchSymbolByLabel(label);
    /*symbol does not exist*/
    if(!symbol){
        return FALSE;
    }
    /*if label is found set its params */
    *value = symbol->value;
    *symbolType = symbol->symbolType;
    *instType = symbol->instType;
    return TRUE;
}

/* search a symbol in table by label
* @params - symbol label
* @return TRUE if instruction type is EXTERN otherwise false
*/
boolean isExternalSymbol(char* label){
    symbol* symbol= searchSymbolByLabel(label);
    if(!symbol){
        return FALSE;
    }
    return (symbol->instType == EXTERN) ? TRUE : FALSE;
}

/* search a macro symbol in table by label
* @params - symbol label
* @return TRUE if exist otherwise false
*/
boolean isMacroExist(char* label) {
    symbol *symbol1ToCheck = searchSymbolByLabel(label);
    if (symbol1ToCheck && symbol1ToCheck->symbolType == MACRO)
        return TRUE;
    return FALSE;
}

/* update data/string symbol addresses by given offset */
void updateDataSymbolsByOffset(int offset){
    symbol *tmp;
    /*empty list*/
    if(!listHead)
        return;
    /*update data/string symbols*/
    for(tmp = listHead; tmp != NULL; tmp = tmp->next)
        if(tmp->instType == DATA || tmp->instType == STRING)
            tmp->value += (offset);
    return;
}


/* clear symbol table at the end of the usage or before reuse
 * init DC and delete table records
*/
void clearSymbolTable(){
    symbol* symbol1ToDelete;
    /*free table memory*/
    while (listHead){
        symbol1ToDelete = listHead;
        listHead = listHead->next;
        free(symbol1ToDelete);
    }
    listHead = listTail = NULL;
}

