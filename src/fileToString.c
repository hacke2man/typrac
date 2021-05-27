#include "fileToString.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int FileLength(FILE * fpointer){
    unsigned int length;
    fseek(fpointer, 0L, SEEK_END);
    length = ftell(fpointer);
    rewind(fpointer);

    return length;
}

char * AppendExecLocation(char * pathExtension){
    char * temp = malloc(PATH_MAX);
    if(!readlink("/proc/self/exe", temp, PATH_MAX)) exit(-1);
    int lastslash;
    for(int i = 0; i < strlen(temp); i++)
        if(temp[i] == '/') lastslash = i;

    temp[lastslash + 1] = 0;
    strcat(temp, pathExtension);

    return temp;
}

char * FileToString(char * path) {
    path = AppendExecLocation(path);

    FILE * fpointer = fopen(path, "r");
    int fileLength = FileLength(fpointer);
    char * outString = malloc(fileLength * sizeof(char) + 1);
    strcpy(outString, "\0");
    char tempString[150];

    printf("%s\n", outString);

    while(fgets(tempString, 150, fpointer)){
        strcat(outString, tempString);
    }

    return outString;
}
