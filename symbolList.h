//
// Created by ubuntu16 on 7/4/19.
//

#ifndef MAMAN14_SYMBOLLIST_H
#define MAMAN14_SYMBOLLIST_H

#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "utils.h"

/* symbol record data structure */
typedef struct symbol_s {
    char label[MAX_LABEL_LEN+1]; /* symbol name */
    sentanceType symbolType; /* code / instruction / macro*/
    instructionType instType; /* instruction type code */
    int value; /*address in memory or macro value*/
    struct symbol_s *next; /*pointer to next symbol*/
}symbol;

/* Function Prototypes */

/* return current data counter */
int getDC();

/*Increase by one data counter (DC) */
void increaseDC();

/*@return symbol table list head */
symbol* getSymbolListHead();

/*@return symbol table list tail */
symbol* getSymbolListTail();

/* create new symbol
 * @params - symbol fields - label,sentanceType,instType, value
 * @return pointer to new symbol
 * In case of error print a massage and exit from the program
 */
symbol* createSymbol(char *label,sentanceType symbolType,instructionType instType, int value);

/* add Symbol to end of the table
 * in case symbol is already exist return error
* @params - symbol record
*/
void addSymbol(symbol* newSymbol);

/* clear symbol table at the end of the usage or before reuse
 * init DC and delete table records
*/
void ClearSymbolTable();

/* search symbol in table by label
* @params - symbol label
* @return pointer to founded symbol or NULL in case it was not found
*/
symbol* searchSymbolByLabel(char* label);

/* search a symbol in table by label
* @params - symbol label
* @return TRUE if instruction type is EXTERN otherwise false
*/
boolean isExternalSymbol(char* label);

/* search a macro symbol in table by label
* @params - symbol label
* @return TRUE if exist otherwise false
*/
boolean isMacroExist(char* label);

/* get address of symbol
* @params - symbol record
* @return symbol value
*/
int getSymbolValue(char* label);


#endif //MAMAN14_SYMBOLLIST_H
