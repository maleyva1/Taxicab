#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char ** parseString();

int main() {
    char *buffer = malloc(sizeof(char*));
    strcpy(buffer,"NOOB KID LMAO");
    char **parsed = malloc(sizeof(char*) * 10);
    parsed = parseString(parsed,buffer);   
    
    printf("temp[0]: %s temp[1]: %s temp[2]: %s ",parsed[0],parsed[1],parsed[2]);
    return 0;
}

char ** parseString(char **parsed, char *buffer){

    
    parsed[0] = strtok(buffer," ");
    int i = 1;
    while( (parsed[i] = strtok(NULL," ")) != NULL) {
        i++;
    }

    return parsed;
}
