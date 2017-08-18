/* Team 15 */
/* Original author: Mark Leyva */
#ifndef TRIP_H
#define TRIP_H
#include <time.h>
#include "Coordinate.h"
#include "Taxi.h"
#include "Route.h"
#include "map.h"
typedef struct triplist TRIPLIST;
typedef struct tripentry TRIPENTRY;
typedef struct trip_t TRIP;

struct trip_t {
    COORDINATE *pickup;
    COORDINATE *destination;
    TAXI *taxicab;
    int completed;
    double cost;
    ROUTELIST *Route;
    int trip_ID;
    time_t start;
    time_t ETA;
    char stringp[10];
    char stringd[10];
};

struct triplist {
    TRIPENTRY *tripfirst;
    TRIPENTRY *triplast;
};

struct tripentry {
    TRIPLIST *Tlist;
    TRIPENTRY *tripnext;
    TRIPENTRY *tripprev;
    TRIP *Trip;
};

TRIP *newTrip(COORDINATE *pick, COORDINATE *dest,ROUTELIST *l);

TRIPLIST *newTripList();

TRIPLIST *newTripEntry(TRIP *t, TRIPLIST *l);

COORDINATE *getPickup(TRIP *trip);

COORDINATE *getDest(TRIP *trip);

TRIP *searchConfirm(int c,TRIPLIST *l);

void MoveTaxi(TRIPLIST *l, time_t ctime);

int CheckFinish(TRIP *t);

TRIP *FindTaxiCoor(TRIPLIST *l,int find_taxi);

ROUTELIST *closestPointR(TRIPLIST *t);

void DeleteTripList(TRIPLIST *t, ROUTELIST *l);

int legalposition(COORDINATE *point,Map *m);

ROUTELIST *FindnoBuildRoute(COORDINATE *c, COORDINATE *d,int sig,Map *m);

ROUTELIST *FullRoute(COORDINATE *taxi, COORDINATE *pick, COORDINATE *dest,Map *m);

TAXI *FindTaxi(TAXILIST *taxlist, COORDINATE *pick,Map *m);

ROUTELIST *SmallRoute(COORDINATE *c,COORDINATE *d,Map *m);

ROUTELIST *FindCoordinatePath(COORDINATE *c, COORDINATE *d,Map *m);

ROUTELIST *FindIdealCoordinate(COORDINATE *c,COORDINATE *d,Map *m);

COORDINATE *SearchIdealLandmark(int xin,int yin, ROUTELIST *r, Map *m, COORDINATE *dest);

void deleteTripEntry(TRIPENTRY *e);

time_t ETAcalc(ROUTELIST *r, COORDINATE *c, COORDINATE *d, time_t ctime, time_t *calc);

time_t scaleTime(time_t curtime, time_t *dumtime, time_t *ptime);

TAXILIST *AllTaxi(Map *m);
#endif
