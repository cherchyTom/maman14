/*This file manages the first input reading flow.
 *Including: parsing lines, validate code text, adding symbols to list,
 *          add labels for second read to queue, code memory words, merge code and data segments and update symbol tables values
 *
 *Writen by:
 *Tom Cherchy   302649397
 *Avrahamii XXXXXXXX
 *
 */

#include "firstRead.h"

/*==== global variables =====*/
int currentLine = 0; /* represents the managed line number */
boolean isError = FALSE; /*flag represent if a file read contains an error*/

/* Params parse function declaration  */
static void parseDataParams(lineInfo*line);
static void parseStringParams(lineInfo*line);
static void parseExtEntParams(lineInfo*line);

/* ====== Instructions List ====== */
const instruction instructionArr[] =
        {	/* Instruction Type |Instruction String | Parse Instruction Function | Parse Param Function */
                { DATA, DATA_STR, parseDataParams} ,
                { STRING, STRING_STR, parseStringParams} ,
                { EXTERN,EXTERN_STR, parseExtEntParams},
                { ENTRY,ENTRY_STR, parseExtEntParams},
                { INVALID_INSTRUCTION,NULL,NULL} /* represent the end of the array */
        };


/*====== MACROS ======== */

/* check if line contain a label, if so remove and warn about it*/
#define LABEL_WARNING(LINE_INFO)\
    if((LINE_INFO)->isContainLabel){\
        (LINE_INFO)->isContainLabel = FALSE;\
        printf("Waring in line %d:\nLabel is define before extern or entry instruction\n",currentLine);\
    }


/*====== Functions ====== */

/* copy content of source operand info struct to destination operand info struct */
static void operandCopy(operandInfo* dest, operandInfo* src){
    dest->type = src->type;
    dest->value = src->value;
    dest->address = src->address;
    dest->isDefinedLabel = src->isDefinedLabel;
    strcpy(dest->label,src->label);
    dest->offset = src->offset;
    return;
}

/* initiate content of operand info struct */
static void resetOperand(operandInfo* operand){
    operand->type = INVALID_OPERAND;
    operand->value = 0; /* default memory value */
    operand->address = 0;
    operand->isDefinedLabel = FALSE;
    *operand->label ='\0';
    operand->offset = 0;
    return;
}

/* initiate content of operand info struct */
static void resetLine(lineInfo* line) {
    line->address = 0;
    *line->originalString = '\0';
    line->lineStr = line->originalString; /* set line pointer to beginning of original string  */
    line->isContainLabel = FALSE;
    *line->labelValue = '\0';
    line->lineType = INVALID_SENTENCE;
    line->instStruct = NULL;
    line->cmd = NULL;
    resetOperand(&line->sourceOp);
    resetOperand(&line->targetOp);
    return;
}

/* Return true  ';' appears at the beginning of the line otherwise false*/
static isCommentLine(char *line){
    return (*line== ';') ? TRUE : FALSE;
}

/* Return true  if line length  <= MAX_LINE_LEN + 1 (for \0) otherwise false*/
static boolean isValidLineLength(char *line){
    return (strlen(line)<= MAX_LINE_LEN) ? TRUE : FALSE;
}

/* Return true if line represent a macro statement otherwise false
 * update line info(statement type & lineSTR) in case of macro
 * */

static boolean isMacroStatement(lineInfo *line){
    char nextWord[MAX_LINE_LEN+1];
    /*skip spaces and retrieve first word */
    leftTrim(&line->lineStr);
    getNextWordByDelimiter(nextWord,line->lineStr,SPACE,sizeof(nextWord));

    if(!strcmp(nextWord,MACRO_STR)){
        line->lineStr += sizeof(MACRO_STR)-1;
        line->lineType = MACRO;
        return TRUE;
    }
    return FALSE;
}

/* Parsing a macro statement, set error in case of invalid format, otherwise add the macro to the symbol table*/
static void macroParse(char* lineStr){
    char nextWord[MAX_LINE_LEN+1];
    char macroLabel[MAX_LABEL_LEN+1];
    int macroValue;

    /*skip spaces and retrieve macro label */
    leftTrim(&lineStr);
    getNextWordByDelimiter(nextWord,lineStr,SPACE, sizeof(nextWord));

    /* check for macro label name validity */
    if(!isValidLabel(nextWord)){
        return;
    }
    strcpy(macroLabel,nextWord);

    /* update lineStr to next position */
    lineStr+= strlen(nextWord);

    /*skip spaces and search for '=' symbol */
    leftTrim(&lineStr);
    if(*lineStr != '='){
        ERORR_MSG(("Expecting for '=' when defining macro: %s\n",macroLabel));
        return;
    }

    /*skip the '=' and spaces and retrieve macro value*/
    lineStr++;
    leftTrim(&lineStr);
    getNextWordByDelimiter(nextWord,lineStr,SPACE, sizeof(nextWord));

    /* check macro value validity*/
    if(!isLegalNum(nextWord,MEMORY_WORD_BITS,&macroValue,1))
        return;
    /* update lineStr to next position */
    lineStr+= strlen(nextWord);

    /*check if line is over*/
    if(!isEmptySTR(lineStr)){
        ERORR_MSG(("Too many arguments - Expecting for exactly 1 number on macro definition"));
        return;
    }

    /* macro is valid - add it to symbol table */
    addSymbol(createSymbol(macroLabel,MACRO,INVALID_INSTRUCTION,macroValue));
    return;
}


/* Return true if line contains a label statement otherwise false
 * update line info(statement type & lineSTR) in case of macro
 * */
static boolean isContainValidLabel(lineInfo *line){
    char nextWord[MAX_LINE_LEN+1];
    char *labelEnd = strchr(line->lineStr, ':');

    /*Label does not exist*/
    if(!labelEnd)
        return FALSE;
    /*skip spaces and retrieve label name */
    leftTrim(&line->lineStr);
    getNextWordByDelimiter(nextWord,line->lineStr,COLON,sizeof(nextWord));

    /* check for label name validity */
    if(!isValidLabel(nextWord)){
        return FALSE;
    }

    /* update line info with label value and new line str position*/
    line->isContainLabel = TRUE;
    strcpy(line->labelValue,nextWord);
    line->lineStr = labelEnd+1;
    return TRUE;
}

/* get next parameter (or operand) from a string with parameters which are separated by comma
*return true if retrieve a parameter or false in case if syntax error
*@Param - Line info
*@Param - pointer to flag if a comma where found before, the function validate its turned on and change it value depand on following comma
*@Param - nextWord store the param*
*/
static boolean getNextParameter(lineInfo *line, boolean *isCommaFound, char *nextWord){

    /* check for a comma between every two params except of the first one */
    if(!*isCommaFound) {
        ERORR_MSG(("A comma must separate between two parameters\n"));
        return FALSE;
    }
    /* Turn of comma flag */
    *isCommaFound = FALSE;


    /*skip spaces and retrieve next parameter */
    leftTrim(&line->lineStr);
    getNextWordByDelimiter(nextWord,line->lineStr,COMMA,MAX_LINE_LEN);

    /*increase lineStr to next position after parameter*/
    line->lineStr += strlen(nextWord);
    leftTrim(&line->lineStr);

    /*check if comma exist after the parameter - update the flag and increase lineStr*/
    if(*line->lineStr == COMMA) {
        line->lineStr++;
        *isCommaFound = TRUE;
    }
    /*remove spaces from end of parameter*/
    rightTrim(nextWord);

    /*check for invalid comma at the beginning or middle of the parameter string*/
    if(isEmptySTR(nextWord)){
        ERORR_MSG(("Invalid \',\' char - Expect to get a number before comma\n"));
        return FALSE;
    }

    return TRUE;
}

/*Parse data instruction - check for validity, coding parameter into memory words and update symbol if exist  */
static void parseDataParams(lineInfo*line){

    char nextWord[MAX_LINE_LEN];
    int parameterValue;
    boolean isCommaFound = 1; /*initiate to one due to first param does need a comma separator */

    /* Find all the params and add them data list */
    while (!isEmptySTR(line->lineStr)){
        /*get next parameter*/
        if(!getNextParameter(line,&isCommaFound,nextWord))
            return;
        /* check if param is a valid number or macro*/
        if(isMacroExist(nextWord))
            parameterValue = getSymbolValue(nextWord);
        else if(!isLegalNum(nextWord,MEMORY_WORD_BITS,&parameterValue,1)) {
            return;
        }
        /* map number to memory word in case there is no error */
        if(!isError)
            addDataMemoryWord(parameterValue);


    }

    /*check if there is comma without a parameter at the end of line*/
    if(isCommaFound){
        ERORR_MSG(("There is a comma at the end of the line - Expecting for additional param\n"));
        return;
    }

    return;
}

/*Parse string instruction - check for validity, coding parameter into memory words and update symbol if exist  */
static void parseStringParams(lineInfo*line){
    char nextWord[MAX_LINE_LEN];

    /* validate there is a parameter to parse*/
    if(isEmptySTR(line->lineStr)){
        ERORR_MSG(("There is no parameter on %s declaration - Expecting for exactly 1 parameter\n",STRING_STR));
        return;
    }
    /*skip spaces */
    leftTrim(&line->lineStr);

    /*validate string param start with quote*/
    if(*line->lineStr != QUOTE){
        ERORR_MSG(("Invalid parameter %s on %s declaration - Parameter must be enclosed by quotes\n",line->lineStr,STRING_STR));
        return;
    }
    /* lineStr position after the first quote*/
    line->lineStr++;

    /*retrieve next string parameter */
    getNextWordByDelimiter(nextWord,line->lineStr,QUOTE,sizeof(nextWord));

    /*validate no empty param */
    if(isEmptySTR(nextWord)){
        ERORR_MSG(("Invalid parameter \"\"  on %s declaration - Empty param was declared\n",STRING_STR));
        return;
    }

    /* set lineStr position after the parameter (on the postfix quote)*/
    line->lineStr += strlen(nextWord);

    /*validate string param end with quote*/
    if(*line->lineStr != QUOTE){
        ERORR_MSG(("Invalid parameter %s  on %s declaration - Parameter must be enclosed by quotes\n",nextWord,STRING_STR));
        return;
    }
    /* lineStr position after the first quote*/
    line->lineStr++;

    /* validate there is no more parameter*/
    if(!isEmptySTR(line->lineStr)){
        ERORR_MSG(("Invalid parameter %s  on %s declaration - Expecting for exactly 1 parameter\n",line->lineStr,STRING_STR));
        return;
    }
    /* map string to memory word in case there is no error */
    if(!isError)
        addStringMemoryWord(nextWord);
    return;
}

/* Parse extern/ entry params - check for validity and detect parameter
 * In case of extern add the parameter to the symbol list*/
static void parseExtEntParams(lineInfo*line){
    char nextWord[MAX_LINE_LEN];

    /*skip white spaces and get label*/
    leftTrim(&line->lineStr);
    getNextWordByDelimiter(nextWord,line->lineStr,SPACE,sizeof(nextWord));

    /* check for param(label) validity*/
    if(!isValidLabel(nextWord))
        return;

    /* update lineStr to next position */
    line->lineStr+= strlen(nextWord);

    /*validate line is over*/
    if(!isEmptySTR(line->lineStr)){
        ERORR_MSG(("Too many arguments - Expecting for exactly 1 parameter on %s declaration\n",(line->instStruct->string)));
        return;
    }

    /*In case of extern instruction add declared parameter to symbol list*/
    if(line->instStruct->type == EXTERN)
        addSymbol(createSymbol(nextWord,INSTRUCTION,EXTERN,EXTERN_DEFAULT_ADDRESS));
    /* in case of entry add the declared param to label queue in order to handle on second read */
    else
        addLabelToQ(ENTRY_LABEL,nextWord,FALSE,DEFAULT_MEMORY_VALUE,currentLine);
    return;
}

/* handle instruction label
 * For data / string add label to symbol list
 * For entry / extern remove label and warn about it*/
static void parseInstructionLabel(lineInfo *line){
    /* If label does not exist */
    if (!line->isContainLabel)
        return;
    /*in case of data or string instruction add label to symbol list*/
    if(line->instStruct->type == DATA || line->instStruct->type == STRING)
        addSymbol(createSymbol(line->labelValue, INSTRUCTION, line->instStruct->type, getDC()));
    else
    /* In case of entry or extern instruction remove label and warn about it*/
    LABEL_WARNING(line)

    return;
}

/* Parse Instruction type, check for parameter exist and  call related parse function
 * In case instruction does not exist set an error message */
static void parseInstruction (lineInfo *line) {
    char nextWord[MAX_LINE_LEN+1];
    /* set line type */
    line->lineType = INSTRUCTION;

    /* get instruction value */
    getNextWordByDelimiter(nextWord,line->lineStr,SPACE,sizeof(nextWord));
    line->instStruct = getInstructionByName(nextWord);

    /*validate instruction name*/
    if(line->instStruct->type == INVALID_INSTRUCTION){
        ERORR_MSG(("Invalid instruction name: \"%s\"\n",nextWord));
        return;
    }

    /*set lineStr position  */
    line->lineStr += strlen(line->instStruct->string);

    /*parse instruction line label*/
    parseInstructionLabel(line);

    /*call to related parse function*/
    line->instStruct->parseParamFunction(line);

    return;
}

/* Returns true if the statement represents an instruction */
static boolean isInstruction(lineInfo*line){
    /*skip white spaces*/
    leftTrim(&line->lineStr);
    return (*line->lineStr == '.') ? TRUE : FALSE;
}

/* return true if there is a valid command name otherwise false */
static boolean isValidCommandName(lineInfo *line){
    char nextWord[MAX_LINE_LEN+1];
    /*skip spaces and retrieve first word */
    leftTrim(&line->lineStr);
    getNextWordByDelimiter(nextWord,line->lineStr,SPACE,sizeof(nextWord));

    /* check for empty command name -> meaning label with no text after */
    if(isEmptySTR(nextWord)){
        ERORR_MSG(("No command/ instruction after label definition\n"));
        return FALSE;
    }

    /*get command and check for its validity */
    line->cmd = getCmd(nextWord);
    if(!line->cmd){
        ERORR_MSG(("Invalid command name \'%s\"\n",nextWord));
        return FALSE;
    }
    /* update line info & increase lineStr to next position */
    line->lineType = CODE;
    line->address = getIC();
    line->lineStr += strlen(line->cmd->name);
    return TRUE;
}


/* gets operand info and offset value from its second position (after '[' ) and check for its validity and remove last ']'
 * assumption - offset can be positive only. the programmer responsibility to validate the offset does not make a segmentation fault */
static boolean isValidOffset(operandInfo *opInfo, char* offset){
    /*check for empty string */
    if(isEmptySTR(offset)){
        ERORR_MSG(("Invalid '[' - Expect to get an array offset in the format of \"[offset]\"\n"));
        return FALSE;
    }
    /*validate last char and remove it */
    if(offset[strlen(offset)-1] != OFFSET_END){
        ERORR_MSG(("Invalid offset param - Expect to get an array offset in the format of \"[offset]\"\n"));
        return FALSE;
    }
    offset[strlen(offset)-1] = '\0';
    /*validate offset is macro or defined number */
    if(isMacroExist(offset))
        opInfo->offset = getSymbolValue(offset);
    else if(!isLegalNum(offset,ADDRESS_BITS,&opInfo->offset,0))
        return FALSE;
    /*validate offset is positive */
    if(opInfo->offset < 0){
        ERORR_MSG(("Invalid offset param \"%s\" - Offset must be positive\n",offset));
        return FALSE;
    }

    return TRUE;
}

/* get operandInfo struct and operand value - validate operand and update info
 * return true in case of valid operand else */
static boolean getOperandInfo(operandInfo *opInfo, char*operand){
    char *offsetStartPosition = NULL;
    boolean isOffset = 0;

    /*if number operand (immediate addressing)*/
    if(*operand == '#'){
        operand++; /*skip '#' */
        /*validate operand is a defined macro or a number */
        if(isMacroExist(operand))
            opInfo->value = getSymbolValue(operand);
        else if(!isLegalNum(operand,ADDRESS_BITS,&opInfo->value,1))
            return FALSE;

        opInfo->type = NUMBER;
        return TRUE;
    }
    /*if register operand*/
    opInfo->value = getRegCode(operand);
    if(opInfo->value != -1){
        opInfo->type = REGISTER;
        return TRUE;
    }

    /*Check for offset in order to separate between label and offset*/
    if(offsetStartPosition = strchr(operand,OFFSET_START)){
        isOffset = 1;
        *offsetStartPosition = '\0';
        *offsetStartPosition++;
    }

    /*validate label name, update opInfo and retrieve its value if already defined */
    if(!isValidLabel(operand)){
        ERORR_MSG(("Invalid label name: \"%s\"\n",operand));
        return FALSE;
    }
    opInfo->type = LABEL;
    strcpy(opInfo->label,operand);
    if(searchSymbolByLabel(operand)) {
        opInfo->value = getSymbolValue(operand);
        opInfo->isDefinedLabel = TRUE;
    } else
        opInfo->isDefinedLabel = FALSE;

    /* handle offset if exist and update opInfo*/
    if(isOffset){
        if(!isValidOffset(opInfo,offsetStartPosition))
            return FALSE;
        opInfo->type = ARR;
    }

    return TRUE;
}

/* validate given command and operands types combinations
 * get lineInfo line, return true if valid or false otherwise*/
static boolean isValidOperandsType(lineInfo *line){
    switch(line->cmd->opcode) {
        /* mov,add,sub,not,clr,inc,dec,red */
        case 0: case 2: case 3: case 4: case 5: case 7: case 8: case 11: {
            if (line->targetOp.type == NUMBER) {
                ERORR_MSG(("Invalid Operand Type: Target operand of \"%s\" cannot be a number\n", line->cmd->name));
                return FALSE;
            }
            break;
        }
        /*lea*/
        case 6: {
            if (line->sourceOp.type == NUMBER || line->sourceOp.type == REGISTER) {
                ERORR_MSG(("Invalid Operand Type: Source operand of \"%s\" must be a label or array\n", line->cmd->name));
                return FALSE;
            }
            if (line->targetOp.type == NUMBER) {
                ERORR_MSG(("Invalid Operand Type: Target operand of \"%s\" cannot be a number\n", line->cmd->name));
                return FALSE;
            }
            break;
        }
        /*jmp,bne,jsr */
        case 9: case 10: case 13: {
            if (line->targetOp.type == NUMBER || line->targetOp.type == ARR) {
                ERORR_MSG(("Invalid Operand Type: Target operand of \"%s\" must be a label or register\n", line->cmd->name));
                return FALSE;
            }
            break;
        }
        default:
            break;
    }
    return TRUE;
}

/* fine operands in cmd line, check their validity */
static void parseOperands (lineInfo *line){
    char nextWord[MAX_LINE_LEN];
    int opCnt = 0; /* operands counter */
    boolean isCommaFound = 1; /*if comma found before operand - initiate to 1 as first operand do not require a comma */

    /* Find all the operands and add them to line info */
    while (!isEmptySTR(line->lineStr)) {
        /*get next parameter*/
        if (!getNextParameter(line, &isCommaFound, nextWord))
            return;
        /*increase operand counter validate operands amount*/
        if (++opCnt > line->cmd->numOfParams)
            break;
        /*update operand info*/
        switch (opCnt) {
            /*get first operand info into target operand */
            case 1: {
                if (!getOperandInfo(&line->targetOp, nextWord))
                    return;
                break;
            }
                /*copy target operand into source operand and get second operand info into target operand */
            case 2: {
                operandCopy(&line->sourceOp, &line->targetOp);
                if (!getOperandInfo(&line->targetOp, nextWord))
                    return;
                break;
            }
            default:
                break;
        }

    }

    /*check if there is comma without a parameter at the end of line
     * In case of not operands the initial flag will be on even there is no invalid comma*/
    if(isCommaFound && opCnt >0){
        ERORR_MSG(("There is a comma at the end of the line\n"));
        return;
    }

    /*validate number of operands*/
    if(line->cmd->numOfParams != opCnt){
        ERORR_MSG(("Invalid amount of operands. \"%s\" expect to get exactly %d operands\n",line->cmd->name,line->cmd->numOfParams));
        return;
    }
    /*validate operands Types*/
    if(!isValidOperandsType(line)){
        return;
    }

    return;
}

/* code single operand to memory word/s depend on its type
 *Param - operand info struct to code
 *Param - Flag isSourceOp - True for source operand False for destination operand*/

static void codeOperandToMemory(operandInfo *opInfo, boolean isSourceOp) {

    /*No operand */
    if(opInfo->type == INVALID_OPERAND)
        return;

    /*add number operand */
    if(opInfo->type == NUMBER){
            addOpValueMemoryWord(opInfo->value);
            return;
    }

    /*add register operand */
    if(opInfo->type == REGISTER){
        if(isSourceOp) /*source operand*/
            addRegisterMemoryWord(DEFAULT_MEMORY_VALUE,opInfo->value);
        else /*dest operand*/
            addRegisterMemoryWord(opInfo->value,DEFAULT_MEMORY_VALUE);
    return;
    }

    /*add LABEL / ARR operand */

    /* check if label defined, update its address and add it to label queue for second read */
    if(opInfo->isDefinedLabel) {
        boolean isExternal = isExternalSymbol(opInfo->label);
        areType are = (isExternal) ? EXTENAL : RELOCATABLE; /*set are type*/
        /*add label to queue for second read */
        addLabelToQ(OPERAND_LABEL,opInfo->label,TRUE,getIC(),currentLine);
        /*code operand label to memory word */
        addAddressMemoryWord(are,opInfo->value);
    }
    /* in case if undefined label insert an empty word  to memory list*/
    else {
        /*add label to queue for second read */
        addLabelToQ(OPERAND_LABEL,opInfo->label,FALSE,getIC(),currentLine);
        /*add empty word to memory*/
        addAddressMemoryWord(DEFAULT_MEMORY_VALUE, DEFAULT_MEMORY_VALUE);
    }
    /* update offset word for array operand */
    if(opInfo->type == ARR)
        addAddressMemoryWord(ABSOLUTE,opInfo->offset);

    return;
}


/* code command to memory - first word + operands
 *Param - lineInfo
*/
static void codeCommandToMemory(lineInfo *line){
    /* set operands addressing method */
    int srcOpAddressingMethod = (line->sourceOp.type == INVALID_OPERAND) ? 0 : line->sourceOp.type;
    int destOpAddressingMethod = (line->targetOp.type == INVALID_OPERAND) ? 0 : line->targetOp.type;

    /*validate line represent a command*/
    if(line->lineType != CODE)
        return;
    /*build first memory word */
    addFirstCodeMemoryWord(destOpAddressingMethod,srcOpAddressingMethod,line->cmd->opcode);

    /*Add Operands memory words */
    /*in case 2 operands are registers insert them as 1 memory word*/
    if(line->sourceOp.type == REGISTER && line->targetOp.type == REGISTER){
        addRegisterMemoryWord(line->targetOp.value,line->sourceOp.value);
        return;
    }

    codeOperandToMemory(&line->sourceOp,1); /* source operand */
    codeOperandToMemory(&line->targetOp,0); /* target (dest) operand */
    return;
}

/* Parse command type, check for operand exist and  call related parse function
 * In case command does not exist set an error message */
static void parseCommand (lineInfo *line) {

    /*validate command name*/
    if(!isValidCommandName(line))
        return;
    /*parse operands*/
    parseOperands(line);

    /*In case of no error code cmd to memory*/
    if(!isError){
        codeCommandToMemory(line);
    }
}

/* Manage line parsing flow:
 * check for macro -> check for label -> check for instruction -> check for command
 *@Param pointer to lineInfo struct  */
static void lineParse(lineInfo *line){

    /* check if line is a macro  and parse it*/
    if(isMacroStatement(line)){
        macroParse(line->lineStr);
        return;
    }

    /* check if line contain valid label and parse it*/
    if(isContainValidLabel(line)){
    }
    /* check if line is an instruction and parse it*/
    if(isInstruction(line)) {
        parseInstruction(line);
        return;
    }

    /*parse line as a command*/
    parseCommand(line);
    return;
}

/* Manage first read flow:
 * line parsing -> code validation -> create symbol list, memory word list and label list for second read
 *@Param pointer to lineInfo struct  */
void firstRead(FILE*fd){
    lineInfo line;
    resetLine(&line); /*initiate lineInfo struct */

    /* read line, parse them and code instruction and command to memory words */
    while(readLine(fd,line.originalString,MAX_LINE_LEN)){
        currentLine++;


        /* check for line length validity */
        if(!isValidLineLength(line.originalString)){
            ERORR_MSG(("Line contains too many characters, number of characters per line limit to %d\n", MAX_LINE_LEN));
            continue;
        }

        /*skip on comment or empty line */
        if(isCommentLine(line.originalString) || isEmptySTR(line.originalString))
            continue;

        /* Parse the current line */
        lineParse(&line);
        resetLine(&line); /*initiate lineInfo struct */

        printf("%s\n",line.originalString);

    }
    /*In case of error finish here */
    if(isError)
        return;

    /*concatenate data memory list to code memory list*/
    mergeCodeAndDataSegments();
    /*update address of data/string labels in symbol list*/
    updateDataSymbolsByOffset(getIC());
    return;
}