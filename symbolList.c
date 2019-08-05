//
// Created by ubuntu16 on 7/4/19.
//
#include "symbolList.h"

/*pointers to symbol list head and tail*/
static symbol *listHead;
static symbol *listTail;


symbol* getSymbolListHead(){
    return listHead;
}

symbol* getSymbolListTail(){
    return listTail;
}

symbol* createSymbol(char *label,sentanceType symbolType,instructionType instType, int value){
    symbol* newSymbol = (symbol*)malloc(sizeof(symbol));
    validateMemoryAllocation(newSymbol,"when creating new symbol record\n");

    strcpy(newSymbol->label,label);
    newSymbol->symbolType = symbolType;
    newSymbol->instType = instType;
    newSymbol->value = value;
    newSymbol->next = NULL;
    return newSymbol;
}

void addSymbol(symbol* newSymbol){
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

void ClearSymbolTable (){
    symbol* symbol1ToDelete;
    /*free table memory*/
    while (listHead){
        symbol1ToDelete = listHead;
        listHead = listHead->next;
        free(symbol1ToDelete);
    }
    listHead = listTail = NULL;
}

symbol* searchSymbolByLabel(char* label){
    symbol* symbol1ToCheck = listHead;

    while (symbol1ToCheck){
        if(!strcmp(symbol1ToCheck->label,label))
            return symbol1ToCheck;
        symbol1ToCheck = symbol1ToCheck->next;
    }
    /*label does not exist in table*/
    return NULL;
}


int getSymbolValue(char* label){
    symbol* symbol= searchSymbolByLabel(label);
    if(!symbol){
        ERORR_MSG(("symbol was not found\n"));
        return 0;
    }

    return symbol->value;
}

boolean isExternalSymbol(char* label){
    symbol* symbol= searchSymbolByLabel(label);
    if(!symbol){
        return FALSE;
    }
    return (symbol->instType == EXTERN) ? TRUE : FALSE;
}

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
            tmp->value += offset;
    return;
}

