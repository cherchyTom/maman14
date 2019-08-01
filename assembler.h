//
// Created by ubuntu16 on 7/4/19.
//

#ifndef MAMAN14_ASSEMBLER_H
#define MAMAN14_ASSEMBLER_H

/* Instruction strings */

#define MACRO_STR ".define"
#define DATA_STR ".data"
#define STRING_STR ".string"
#define ENTRY_STR ".entry"
#define EXTERN_STR ".extern"

/*===== constant definition ===== */
#define MAX_LINE_LEN 80
#define MAX_LABEL_LEN 31
#define MEMORY_WORD_BITS 14
#define FIRST_ADDRESS 100
#define MEMORY_SIZE 4096
#define MAX_MEMORY_WORDS MEMORY_SIZE-FIRST_ADDRESS
#define EXTERN_DEFAULT_ADDRESS 0

/*===== Delimiter Chars ===== */
#define COMMA ','
#define SPACE ' '
#define COLON ':'



/*===== data structures ===== */
typedef enum {FALSE = 0, TRUE} boolean;

/* sentance types the assembler support*/
typedef enum {INSTRUCTION = 0, CODE, MACRO} sentanceType;


/* Instruction types the assembler support */
typedef enum {DATA = 0, STRING, ENTRY,EXTERN,INVALID_INSTRUCTION} instructionType;

#endif //MAMAN14_ASSEMBLER_H
