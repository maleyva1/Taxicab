/* Team 15 */
#ifndef PARSER_H
#define PARSER_H

#include "Trip.h"
#include "Route.h"
#include "ParseAid.h"
#include "Taxi.h"

/* This module parses a request from the client */
char *ServResponse(char *buffer,int size, TRIPLIST *l,TAXILIST *taxlist,Map *m, int *trip_IDs, double *totalmoney,time_t *reftime, time_t *dumtime);
#endif
