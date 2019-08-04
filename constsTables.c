//
// Created by ubuntu16 on 7/25/19.
//

#include "constsTables.h"

/* ====== Commands List ====== */
const command cmdArr[] =
        {	/* Name | Opcode | NumOfParams */
                { "mov", 0, 2 } ,
                { "cmp", 1, 2 } ,
                { "add", 2, 2 } ,
                { "sub", 3, 2 } ,
                { "not", 4, 1 } ,
                { "clr", 5, 1 } ,
                { "lea", 6, 2 } ,
                { "inc", 7, 1 } ,
                { "dec", 8, 1 } ,
                { "jmp", 9, 1 } ,
                { "bne", 10, 1 } ,
                { "red", 11, 1 } ,
                { "prn", 12, 1 } ,
                { "jsr", 13, 1 } ,
                { "rst", 14, 0 } ,
                { "stop",15, 0 } ,
                {NULL} /* represent the end of the array */
        };

/* ====== Registers List ====== */
const rgstr regArr[] =
        {	/* Name | reg code */
                { "r0", 0} ,
                { "r1", 1} ,
                { "r2", 2} ,
                { "r3", 3} ,
                { "r4", 4} ,
                { "r5", 5} ,
                { "r6", 6} ,
                { "r7", 7} ,
                { NULL } /* represent the end of the array */
        };

/* ====== Instruction List Declaration ====== */
extern const instruction instructionArr[];

/* ====== Functions ====== */

const command* getCmd(char* cmdName){
    const command* cmd = cmdArr;
    while(cmd->name){
        if(!strcmp(cmd->name,cmdName))
            return cmd;
        cmd++;
    }
    /* command name was not found*/
    return NULL;
}

static int getCmdCode(char* cmdName){
    const command* cmd = cmdArr;
    while(cmd->name){
        if(!strcmp(cmd->name,cmdName))
            return cmd->opcode;
        cmd++;
    }
    /* command name was not found*/
    return -1 ;
}

int getCmdNParams(char* cmdName){
    const command* cmd = cmdArr;
    while(cmd->name){
        if(!strcmp(cmd->name,cmdName))
            return cmd->numOfParams;
        cmd++;
    }
    /* command name was not found*/
    return -1;
}

int getRegCode(char* regName){
    const rgstr* reg = regArr;
    while(reg->name){
        if(!strcmp(reg->name,regName))
            return reg->code;
        reg++;
    }
    /* command name was not found*/
    return -1;
}

boolean isReservedWord (char* word){
    return (getCmdCode(word) != -1 || getRegCode(word) != -1) ? TRUE : FALSE;
}


const instruction* getInstructionByName(char* name){
    const instruction *inst = instructionArr;
    while(inst->type != INVALID_INSTRUCTION){
        if(!strcmp(inst->string,name))
            return inst;
        inst++;
    }
    /* Invalid instruction name */
    return inst;
}