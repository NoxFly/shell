#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int isEq(char *s1, char *s2)
{
    return strcmp(s1, s2) == 0;
}

// str_replace function (source: geekforgeeks)
char *strrep(char *source, char *substring, char *replacement)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(replacement);
    int oldWlen = strlen(substring);

    for (i = 0; source[i] != '\0'; i++)
    {
        if (strstr(&source[i], substring) == &source[i])
        {
            cnt++;
            i += oldWlen - 1;
        }
    }

    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*source)
    {
        if (strstr(source, substring) == source)
        {
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

void printArgs(char **command)
{
    int len = -1, i = 0;

    // recover number of argument passed in the array
    while (command[i++] != 0)
        len++;

    // create the array with a fixed size (allows to not do a malloc)
    char *argv[len];

    // recover each argument
    i = 0;
    while (i < len)
    {
        i++;
        argv[i - 1] = command[i];
    }

    // display
    printf("Commande : %s\nArgument%s (%d) : ", command[0], len > 1 ? "s" : "", len);

    for (int j = 0; j < len; j++)
        printf("%s%s", argv[j], (j < len - 1) ? ", " : "");
    printf("\n");
}