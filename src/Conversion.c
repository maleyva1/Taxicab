#include "Conversion.h"

/* Street1 and Street2 are the strings that hold
 * the horizontal and vertical streets
 */
char *toServer(char *street1, char *street2)
{
    char *destin = malloc(sizeof(char) * 3);

    /* Set the null vale */
    memset(destin, '\0', sizeof(destin));

    /* Copy the first charcter from both arrays
     * and store it in destin char
     */
    strncpy(destin, street1, 1);
    strncpy(destin, street1, 1);

    return destin;
}

void fromClient(char *coordinate, char *street1, char *street2, Map *map)
{
    if(!map)
    {
        perror("Error with map! Exiting...\n");
        exit(10);
    }

    int i = 0;
    for(i = 0; i < map->col; i++)
    {
        if(coordinate[0] == map->colNames[i])
        {
            *street1 = map->colNames[i];
        }
    }
    for(i = 0; i < map->row; i++)
    {
        if(coordinate[1] == map->rowNames[i])
        {
            *street2 = map->rowNames[i];
        }
    }
}

/*cover alphanumeric to numeric: "A0" would become "0 0" */
COORDINATE *toNumeric(char *coordinate)
{
    COORDINATE *toReturn = NULL;

    /* x1 is the first coordinate
     * x2 is the second coordinate
     */

    int x1 = (int)((coordinate[0] & 63) - 1);

    char *temp2;
    char temp[2];
    sprintf(temp,"%c",coordinate[0]);
    temp2 = strtok(coordinate, temp);

    int x2 = atoi(temp2);

    toReturn = newCoordinate(x2, x1);

    return toReturn;
}

/* Converts numeric to alphanumeric */
char *toAlpha(COORDINATE *coord)
{
    /* Assuming that the second cordinate, that is Y, will
     * never be greater than double digits
     */
    char *toReturn = malloc(sizeof(char) * 3);

    /* OR 0100 000 with the corodinate value + 1 */
    toReturn[0] = 64 | (coord->y + 1);

    char temp[2];
    sprintf(temp, "%d", coord->x);
    strcat(toReturn, temp);

    return toReturn;
}

