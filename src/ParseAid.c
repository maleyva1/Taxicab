#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ParseAid.h"

/* Function breaks the buffer into different tokens */
char ** parseString(char **parsed, char *buffer)
{
    parsed[0] = strtok(buffer," ");

    int i = 1;
    while( (parsed[i] = strtok(NULL," ")) != NULL)
    {
        i++;
    }

    parsed[i] = NULL;
    return parsed;
}
