//
// Created by ubuntu16 on 7/26/19.
//

#include "firstRead.h"

/*==== global variables =====*/
int currentLine = 0; /* represents the managed line number */
boolean isError = FALSE; /*flag represent if files read contains error*/
int IC = 0; /* Instruction counter*/

/* Params parse function declaration  */
void parseDataParams(lineInfo*line);
void parseStringParams(lineInfo*line);
void parseExtEntParams(lineInfo*line);

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
    if(!isLegalNum(nextWord,MEMORY_WORD_BITS,&macroValue))
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
    printf("----==== macro label is %s and value is %d ---========\n",macroLabel,getSymbolValue(macroLabel));
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

/*Parse data instruction - check for validity, coding parameter into memory words and update symbol if exist  */
void parseDataParams(lineInfo*line){

    char nextWord[MAX_LINE_LEN];
    int parameterValue;
    boolean isCommaFound = 1; /*initiate to one due to first param does need a comma separator */

    /* Find all the params and add them data list */
    while (!isEmptySTR(line->lineStr)){

        /* check for a comma between every two params except of the first one */
        if(!isCommaFound) {
            ERORR_MSG(("A comma must separate between two parameters\n"));
            return;
        }
        /* initiate comma flag to false */
        isCommaFound = FALSE;

        /*skip spaces and retrieve next parameter */
        leftTrim(&line->lineStr);
        getNextWordByDelimiter(nextWord,line->lineStr,COMMA,sizeof(nextWord));

        /* increase lineStr to point to the next position after parameter*/
        line->lineStr += strlen(nextWord);

        /*check if comma exist after the parameter - update the flag and increase lineStr*/
        if(*line->lineStr == COMMA) {
            line->lineStr++;
            isCommaFound = TRUE;
        }
        /*remove spaces from end of parameter*/
        rightTrim(nextWord);

        /*check for invalid comma at the beginning or middle of the parameter string*/
        if(isEmptySTR(nextWord)){
            ERORR_MSG(("Invalid \',\' char - Expect to get a number before comma\n"));
            return;
        }

        /* check if param is a valid number or macro*/
        if(isMacroExist(nextWord))
            parameterValue = getSymbolValue(nextWord);
        else if(!isLegalNum(nextWord,MEMORY_WORD_BITS,&parameterValue)) {
            return;
        }
        /* map number to memory word in case there is no error */
        if(!isError)
            /*add to memory function */
            printf("add function");

        printf("%d\n",parameterValue);

    }

    /*check if there is comma without a parameter at the end of line*/
    if(isCommaFound){
        ERORR_MSG(("There is a comma at the end of the line - Expecting for additional param\n"));
        return;
    }

    return;
}

/*Parse string instruction - check for validity, coding parameter into memory words and update symbol if exist  */
void parseStringParams(lineInfo*line){
    char nextWord[MAX_LINE_LEN];

    /*skip spaces and retrieve next parameter */
    leftTrim(&line->lineStr);
    getNextWordByDelimiter(nextWord,line->lineStr,SPACE,sizeof(nextWord));

    /*remove spaces from end of parameter*/
    rightTrim(nextWord);

    /*validate parameter format*/
    if(!isLegalStringParam(nextWord))
        return;

    /* increase lineStr to point to the next position after parameter*/
    line->lineStr += strlen(nextWord);
    /* validate there is no more parameter*/
    if(!isEmptySTR(line->lineStr)){
        ERORR_MSG(("Invalid parameter \"%s\"  on %s declaration - Expecting for exactly 1 parameter\n",line->lineStr,STRING_STR));
        return;
    }
    /* map string to memory word in case there is no error */
    if(!isError)
        /*add to addStringTo memory function */
        printf("add to addStringTo memory function");

    printf("\n----=====String Instruction====-----\n");
    return;
}

/* Parse extern/ entry instruction - check for validity and detect parameter
 * In case of extern add the parameter to the symbol list*/
void parseExtEntParams(lineInfo*line){
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

    printf("\n----=====%s Instruction====-----\n",(line->instStruct->type == EXTERN)? EXTERN_STR : ENTRY_STR);
    return;
}

/*Parse data or string instruction - check for validity, parse params and update symbol if exist  */
void parseDataStringInstruction(lineInfo*line){
    /* If exist update label in symbol list */
    if (line->isContainLabel){
        addSymbol(createSymbol(line->labelValue,INSTRUCTION,line->instStruct->type,getDC()));
    }

    /* check for empty parameters */
    if (isEmptySTR(line->lineStr)){
        ERORR_MSG(("No parameter on %s declaration - Expecting for exactly 1 parameter\n",(line->instStruct->string)));
        return;
    }
    /* call params parsing functions  */
    line->instStruct->parseParamFunction;

}

/* handle instruction label
 * For data / string add label to symbol list
 * For entry / extern remove label and warn about it*/
void parseInstructionLabel(lineInfo *line){
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
void parseInstruction (lineInfo *line) {
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

static void lineParse(lineInfo *line){

    line->lineStr = line->originalString;

    /* check if line is a macro */
    if(isMacroStatement(line)){
        printf("\n----=====MACRO====-----\n\n");
        macroParse(line->lineStr);
        return;
    }

    /* check if line contain valid label*/
    if(isContainValidLabel(line)){
        printf("\n----=====LABEL====-----\n\n");
    }

    if(isInstruction(line)) {
        parseInstruction(line);
    }

    //getLineInstructionType(line)
    return;


}


int firstRead(FILE*fd){
    lineInfo line;

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

        printf("%s\n",line.originalString);

        /*clear line */
        line.isContainLabel= FALSE;
        line.labelValue[0] = '\0';

    }


    return 0;

}