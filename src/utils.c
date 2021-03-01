#include "utils.h"

#include <string.h>
#include <stdlib.h>

int isEq(char* s1, char* s2) {
    return strcmp(s1, s2) == 0;
}

// str_replace function (source: geekforgeeks)
char* strrep(char* source, char* substring, char* replacement) {
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(replacement);
    int oldWlen = strlen(substring);
 
    for (i = 0; source[i] != '\0'; i++) {
        if (strstr(&source[i], substring) == &source[i]) {
            cnt++;
            i += oldWlen - 1;
        }
    }
 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
 
    i = 0;
    while (*source) {
        if (strstr(source, substring) == source) {
            strcpy(&result[i], replacement);
            i += newWlen;
            source += oldWlen;
        }
        else
            result[i++] = *source++;
    }
 
    result[i] = '\0';
    return result;
}