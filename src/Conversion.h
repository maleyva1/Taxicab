#ifndef CONVERSION_H
#define CONVERSION_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Trip.h"

/* Convert Pickup location to BNL coordinates */
char *toServer(char *street1, char *street2);

/* Convert BNL coordinates to steet names */
void fromClient(char *coordinate, char *street1, char *street2, Map *map);

/* Converts alphanumeric coordinates to numeric*/
/* E.g.: 'AO' -> (0, 0) */
COORDINATE *toNumeric(char *coordinate);

/* Converts numeric to alphanumeric */
char *toAlpha(COORDINATE *coord);
#endif
