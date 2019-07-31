//
// Created by ubuntu16 on 7/4/19.
//
#include "symbolList.h"

/*data counter*/
static DC = 0;

/*pointers to symbol list head and tail*/
static symbol *listHead;
static symbol *listTail;

int getDC(){
    return DC;
}
void increaseDC(){
    DC++;
    return;
}

symbol* getSymbolListHead(){
    return listHead;
}

symbol* getSymbolListTail(){
    return listTail;
}

symbol* createSymbol(char *label,sentanceType symbolType,instructionType instType, int value){
    symbol* newSymbol = (symbol*)malloc(sizeof(symbol));
    validateMemoryAllocation(newSymbol,"when creating new symbol record");

    strcpy(newSymbol->label,label);
    newSymbol->symbolType = symbolType;
    newSymbol->instType = instType;
    newSymbol->value = value;
    newSymbol->next = NULL;
    return;
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
    /*init DC */
    DC = 0;
    /*free table memory*/
    while (listHead){
        symbol1ToDelete = listHead;
        listHead = listHead->next;
        free(symbol1ToDelete);
    }

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

boolean isMacroExist(char* label) {
    symbol *symbol1ToCheck = searchSymbolByLabel(label);
    if (symbol1ToCheck && symbol1ToCheck->symbolType == MACRO)
        return TRUE;
    return FALSE;
}

