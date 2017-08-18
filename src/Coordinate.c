#include "Coordinate.h"
#include <stdlib.h>
COORDINATE *newCoordinate(int x1, int x2)
{
    COORDINATE *toReturn = malloc(sizeof(COORDINATE));

    toReturn->x = x1;
    toReturn->y = x2;

    return toReturn;
}

void DeleteCoordinate(COORDINATE *c)
{
    free(c);
}
