#include <stdlib.h>

#include "Trip.h"

COORDINATE *getPickup(TRIP *trip)
{
    return trip->pickup;
}

COORDINATE *getDest(TRIP *trip)
{
    return trip->destination;
}


TRIP *newTrip(COORDINATE *pick, COORDINATE *dest,ROUTELIST *l)
{
    TRIP *trip = malloc(sizeof(TRIP));
    trip->pickup = pick;
    trip->destination = dest;
    trip->taxicab = NULL;
    trip->completed = -1;
    trip->cost = 0.0;
    trip->Route = l;
    trip->trip_ID = 0;
    trip->start = time(NULL);
    trip->ETA = 0;

    return trip;
}

TRIPLIST *newTripList() {
    TRIPLIST *l = malloc(sizeof(TRIPLIST));
    l->tripfirst = NULL;
    l->triplast = NULL;
    return l;
}

TRIPLIST *newTripEntry(TRIP *t, TRIPLIST *l) {
    TRIPENTRY *e = malloc(sizeof(TRIPENTRY));
        e->Trip = t;
        e->Tlist = l;
        if(l->tripfirst) {
            l->triplast->tripnext = e;
            e->tripprev = l->triplast;
            l->triplast = e;
            l->triplast->tripnext = NULL;
        }
        else {
            l->tripfirst = e;
            l->triplast = e;
            e->tripnext = NULL;
            e->tripprev = NULL;
         }
    return l;
}

TRIP *searchConfirm(int c, TRIPLIST *l) {
    TRIPENTRY *e = l->tripfirst;
    TRIPENTRY *n = NULL;
    while(e) {
        n = e->tripnext;
        if(e->Trip->trip_ID == c) {
            e->Trip->completed = 0;
            return e->Trip;
        }
        e = n;
    }
    return NULL;
}

int CheckFinish(TRIP *t) {
    COORDINATE *tax = t->taxicab->location;
    COORDINATE *destin =  t->destination;
    if((tax->x == destin->x) && (tax->y == destin->y)){
        t->completed = 1;
        t->taxicab->available_seats = 1;
        return 1;
    }
    else return 0;
}

void deleteTripEntry(TRIPENTRY *e) {
    if(e == e->Tlist->tripfirst) {
        e->Tlist->tripfirst = e->tripnext;
        if(e == e->Tlist->triplast) {
            e->Tlist->triplast = NULL;
            e->Tlist->tripfirst = NULL;
        }
        else {
        e->tripnext->tripprev = NULL;
        }
        free(e->Trip->pickup);
        free(e->Trip->destination);
        DeleteRouteList(e->Trip->Route);
        free(e);
        return;
    }
    else if(e == e->Tlist->triplast) {
        e->Tlist->triplast = e->tripprev;
        e->tripprev->tripnext = NULL;
        free(e->Trip->pickup);
        free(e->Trip->destination);
        DeleteRouteList(e->Trip->Route);
        free(e);
        return;
    }
    else {
        e->tripnext->tripprev = e->tripprev;
        e->tripprev->tripnext = e->tripnext;
        free(e->Trip->pickup);
        free(e->Trip->destination);
        DeleteRouteList(e->Trip->Route);
        free(e);
        return;
    }
}

void MoveTaxi(TRIPLIST *l, time_t ctime) {
    if(!l->tripfirst) return;
    TRIPENTRY *e = l->tripfirst;
    TRIPENTRY *n = NULL;
    while(e) {

        n = e->tripnext;
        while((ctime - e->Trip->start) >= 1) {
           e->Trip->start += 1;
           if(!e->Trip->Route->first->next) break;
           if(e->Trip->completed != -1) {
           if(e->Trip->taxicab->location->x - e->Trip->Route->first->next->coord->x < 0) {
               e->Trip->taxicab->location->x += 1;
               continue;
           }
           else if (e->Trip->taxicab->location->x - e->Trip->Route->first->next->coord->x > 0) {
               e->Trip->taxicab->location->x -= 1;
               continue;
            }
            else if (e->Trip->taxicab->location->y - e->Trip->Route->first->next->coord->y < 0) {
               e->Trip->taxicab->location->y += 1;
               continue;
            }
            else if (e->Trip->taxicab->location->y - e->Trip->Route->first->next->coord->y > 0) {
               e->Trip->taxicab->location->y -= 1;
               continue;
            }
            if(CheckFinish(e->Trip)) break;
            else if ((e->Trip->taxicab->location->y == e->Trip->Route->first->next->coord->y) && (e->Trip->taxicab->location->x == e->Trip->Route->first->next->coord->x)) {
                    if(!e->Trip->Route->first->next) break; 
                    e->Trip->Route->first = deleteEntryR(e->Trip->Route->first);
                    if(!e->Trip->Route->first->next) break;                    
                    if(e->Trip->taxicab->location->x - e->Trip->Route->first->next->coord->x < 0) {
                        e->Trip->taxicab->location->x += 1;
                    }    
                    else if (e->Trip->taxicab->location->x - e->Trip->Route->first->next->coord->x > 0) {
                        e->Trip->taxicab->location->x -= 1;
                    }
                    else if (e->Trip->taxicab->location->y - e->Trip->Route->first->next->coord->y < 0) {
                        e->Trip->taxicab->location->y += 1;
                    }
                    else if (e->Trip->taxicab->location->y - e->Trip->Route->first->next->coord->y > 0) {
                        e->Trip->taxicab->location->y -= 1;
                    }
            }
           }
        }
        e = n;
    }
}

TRIP *FindTaxiCoor(TRIPLIST *l,int find_taxi) {
    if(!l->tripfirst) return NULL;
    TRIPENTRY *e = l->tripfirst;
    TRIPENTRY *n = NULL;
    while(e) {
        n = e->tripnext;
        if(e->Trip->taxicab->taxi_ID == find_taxi) return e->Trip;
        e = n;
    }
    return NULL;
}

void DeleteTripList(TRIPLIST *t,ROUTELIST *l){
    TRIPENTRY *e = t->tripfirst;
    TRIPENTRY *n = NULL;
    while(e) {
        n = e->tripnext;
        if(e->Trip->Route != l) {
        DeleteRouteList(e->Trip->Route);
        }
        free(e->Trip);
        e->Trip = NULL;
        free(e);
        e = n;
    }
    free(t);
}

time_t ETAcalc(ROUTELIST *r,COORDINATE *c, COORDINATE *d, time_t ctime, time_t *calc) {
    ROUTEENTRY *e = r->first;
    ROUTEENTRY *n = NULL;
    int count = 0;
    while(e) {
        n = e->next;
        if((e->coord->x == d->x && e->coord->y == d->y)
                || n == NULL) {
                break;
        }
        count = count + distancedif(e->coord,n->coord);
        e = n;
    }
   *calc = count * 20 + ctime;
  return ctime;
}

time_t scaleTime(time_t curtime, time_t *dumtime, time_t *ptime) {
    *dumtime = *dumtime + (curtime - *ptime) * 20;
    *ptime = curtime;
    return *dumtime;
}


