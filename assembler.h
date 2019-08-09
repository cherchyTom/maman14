/*This is the main header file of the program
 * Contains the program constant definitions and main data structures
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */

#ifndef MAMAN14_ASSEMBLER_H
#define MAMAN14_ASSEMBLER_H

#include <stdio.h>


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
#define ADDRESS_BITS MEMORY_WORD_BITS - 2
#define FIRST_ADDRESS 100
#define MEMORY_SIZE 4096
#define MAX_MEMORY_WORDS MEMORY_SIZE-FIRST_ADDRESS
#define EXTERN_DEFAULT_ADDRESS 0
#define DEFAULT_MEMORY_VALUE 0 /* for empty words/ empty bits */

/*===== Delimiter Chars ===== */
#define COMMA ','
#define SPACE ' '
#define COLON ':'
#define QUOTE '"'
#define OFFSET_START '['
#define OFFSET_END ']'

/*===== File Types Postfix ===== */
#define ASSEMBLY_POSTFIX ".as"
#define OBJ_POSTFIX ".ob"
#define EXT_POSTFIX ".ext"
#define ENT_POSTFIX ".ent"


/*===== data structures ===== */
typedef enum {FALSE = 0, TRUE} boolean;

/* sentance types the assembler support*/
typedef enum {INVALID_SENTENCE = -1, INSTRUCTION = 0, CODE, MACRO} sentanceType;


/* Instruction types the assembler support */
typedef enum {DATA = 0, STRING, ENTRY,EXTERN,INVALID_INSTRUCTION} instructionType;

/* Operands types*/
typedef enum {INVALID_OPERAND = -1,NUMBER = 0, LABEL = 1,  ARR = 2, REGISTER = 3} operandType;

/*
 * data structure of command including:
 * command name
 * command operation code
 * required parameters
 */
typedef struct
{
    char *name;
    unsigned int opcode : 4;
    int numOfParams;
} command;

/*
 * data structure of register including:
 * register name
 * register code
 * required parameters
 */
typedef struct
{
    char *name;
    unsigned int code : 3;
} rgstr;

/*
 * data structure of instruction including:
 * instruction type
 * instruction string
 * params parse function
 */
typedef struct
{
    instructionType type;
    char *string;
    void(*parseParamFunction)();
} instruction;

/* Operands struct - contains operand info*/
typedef struct{
    operandType type;		        /* Type */
    int value;				        /* Value */
    char label[MAX_LABEL_LEN];      /* Label Name */
    boolean isDefinedLabel;         /* True if label is defined in symbol table */
    int offset;                      /* Label address offset - 0 mean no offset*/
    int address;			        /* The address of the operand in the memory */
} operandInfo;

/* Line struct - contains line info */
typedef struct{
    int address;				            /* The address of the first word in the line */
    char originalString[MAX_LINE_LEN +2];	/* Original line content (+2 for \n\0 at the end of the line read from file)*/
    char *lineStr;				            /* pointer to the originalString - use to parse the line */
    boolean isContainLabel;			        /* A pointer to the lines label in labelArr */
    char labelValue[MAX_LABEL_LEN+1];       /*contains label value if exist (+1 for '\0' at the end string)*/
    sentanceType lineType;                  /* code, instruction or macro*/
    const instruction *instStruct;          /* pointer to instruction struct - data, string, extern or entry */
    /* Command line */
    const command *cmd;			            /* A pointer to the command in command struct */
    operandInfo targetOp;			        /* The 1st operand */
    operandInfo sourceOp;			        /* The 2nd operand */

} lineInfo;


#endif //MAMAN14_ASSEMBLER_H
