/*This is a header file of symbolList.c
 *Contains symbolList data structures and prototypes
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */

#ifndef MAMAN14_SYMBOLLIST_H
#define MAMAN14_SYMBOLLIST_H

#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "utils.h"

/* symbol record data structure */
typedef struct symbol_s {
    char label[MAX_LABEL_LEN+1]; /* symbol name (+1 for end of string char)*/
    sentanceType symbolType; /* code / instruction / macro*/
    instructionType instType; /* instruction type code */
    int value; /*address in memory or macro value*/
    struct symbol_s *next; /*pointer to next symbol*/
}symbol;

/* Function Prototypes */

/* add Symbol to end of the table
 * in case symbol is already exist return error
* @params - symbol fields - label,sentanceType,instType, value
*/
void addSymbol(char *label,sentanceType symbolType,instructionType instType, int value);

/* get symbol name (label) and pointer to int (value) to retrieve the symbol value into
* @return True if symbol was found otherwise false
*/
boolean getSymbolValue(char* label, int *value);

/* search for symbol and retrieve its fields.
 * get symbol label (name) to search and pointers to value and symbol type to retrieve  the parameters into
 */
boolean getSymbolParams(char* label, int *value, sentanceType *symbolType, instructionType *instType);

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

/* update data/string symbol addresses by given offset
 * @params - int offset - to increase address by
 */
void updateDataSymbolsByOffset(int offset);

/* clear symbol table at the end of the usage or before reuse
 * init DC and delete table records
*/
void clearSymbolTable();


#endif //MAMAN14_SYMBOLLIST_H
