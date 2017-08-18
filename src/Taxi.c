
#include <stdlib.h>
#include "Trip.h"

TAXI *newTaxi() 
{
    TAXI *cab = malloc(sizeof(TAXI));
    cab->location = NULL;
    cab->taxi_ID = 0;
    cab->available_seats = 0; 
    cab->taxi_type = 0;
    return cab;
}

TAXILIST *newTaxiList() {
    TAXILIST *l = malloc(sizeof(TAXILIST));
    l->taxifirst = NULL;
    l->taxilast = NULL;
   return l;
}

TAXILIST *newEntryTaxi(TAXILIST *t, TAXI *tax) {
    TAXIENTRY *e = malloc(sizeof(TAXIENTRY));
    e->cab = tax;
    e->taxilist = t;
    if(t->taxifirst) {
        t->taxilast->taxinext = e;
        e->taxiprev = t->taxilast;
        t->taxilast = e;
        e->taxinext = NULL;

    }
    else {
        t->taxifirst = e;
        t->taxilast = e;
        e->taxinext = NULL;
        e->taxiprev = NULL;
    }
   return t;
}

TAXILIST *AllTaxi(Map *m) {
    int max = m->stands->size;
    int i,j,x,y;
    int ID = 0;
    TaxiStand *t = NULL;
    COORDINATE *temp = NULL;
    TAXI *cab = NULL;
    TAXILIST *l = newTaxiList();
    for(i = 0; i < max; i++) {
    t = (TaxiStand*)m->stands->items + i * m->stands->itemSize;
    x = t->mark.x;
    y = t->mark.y;
        for(j = 0; j < t->numTaxis;j++) {
            temp = newCoordinate(x,y);
            cab = newTaxi();
            cab->location = temp;
            cab->taxi_ID = ++ID;
            cab->available_seats = 1;
            newEntryTaxi(l,cab);
        }
    }
return l;
}

TAXIENTRY *deleteTaxiEntry(TAXIENTRY *e) {
    TAXIENTRY *n = e->taxinext;
    if(e->cab->location) {
    free(e->cab->location);
    e->cab->location= NULL;
    }
    free(e->cab);
    e->cab = NULL;
    free(e);
    e = NULL;
    return n;
}

void deleteTaxiList(TAXILIST *t) {
    TAXIENTRY *e = t->taxifirst;
    while(e) {
        e = deleteTaxiEntry(e);
    }
    free(t);
    return;
}
