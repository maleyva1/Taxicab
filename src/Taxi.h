/* Team 15 */
/* The taxi struct will contain relevant information on the taxi */
#ifndef TAXI__MAC
#define TAXI__MAC
#include "Coordinate.h"
typedef struct taxilist TAXILIST;
typedef struct taxientry_t TAXIENTRY;
typedef struct taxi_t TAXI;

struct taxientry_t {
    TAXILIST *taxilist;
    TAXIENTRY *taxinext;
    TAXIENTRY *taxiprev;
    TAXI *cab;
};

struct taxilist {
    TAXIENTRY *taxifirst;
    TAXIENTRY *taxilast;
};

struct taxi_t{
    COORDINATE *location;
    int taxi_ID;
    int available_seats;
    int taxi_type;
};


/* Returns the taxi's current location */
COORDINATE *getLocation(TAXI *taxi);

/* Returns the taxi's ID */
int getTaxicabID(TAXI *taxi);

/* Returns the amount of seating on a taxi */
int getSeating(TAXI *taxi);

/* Returns the taxi type */
int getTaxiType(TAXI *taxi);

TAXI *newTaxi();

TAXIENTRY *deleteTaxiEntry(TAXIENTRY *e);

void deleteTaxiList(TAXILIST *t);
#endif
