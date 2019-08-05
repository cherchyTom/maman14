#include <stdio.h>
#include "assembler.h"
#include "utils.h"
#include "firstRead.h"



int main(int argc, char *argv[]){
    FILE *fd;
    char line[MAX_LINE_LEN+1];



    if(argc < 2) {
        perror("Please run with at list one file name argument\n");
        return 1;
    }

    while (*(++argv)){
        fd = openFileRead(*argv);
        firstRead(fd);
        printf("\n................................print the original file:............................\n");
        rewind(fd);
        while (fgets(line, MAX_LINE_LEN+2, fd))
            printf("%s", line);
        fclose(fd);
    }

    return 0;
}

