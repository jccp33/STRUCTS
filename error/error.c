#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FILENAME "errors/errors.txt"

void printErrorOnFile(const char* originfile, int line, const char* message){
    time_t rawtime;
    time(&rawtime);
    char* temp_time = ctime(&rawtime);
    if (temp_time) {
        temp_time[24] = '\0'; 
    }
    FILE* file = fopen(FILENAME, "a");
    if (!file) {
        printf("ERROR [%s][%s][%d]: %s\n", temp_time, originfile, line, message);
    }else{
        fprintf(file, "ERROR [%s][%s][%d]: %s\n", temp_time, originfile, line, message);
        printf("ERROR: view error in file '%s'\n", FILENAME);
        fclose(file);
    }
}
