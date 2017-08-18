
#include "Coordinate.h"
#ifndef ROUTE
#define ROUTE
typedef struct routeentry ROUTEENTRY;

typedef struct routelist ROUTELIST;

struct routeentry {
    ROUTELIST *list;
    ROUTEENTRY *next;
    ROUTEENTRY *prev;
    COORDINATE *coord;
};
struct routelist {
    ROUTEENTRY *first;
    ROUTEENTRY *last;
};


char *FindRoute(COORDINATE *pickup,COORDINATE *destin);

char *SNewCoordinate(int x, int y);

ROUTELIST *newRouteList();

ROUTEENTRY *newRouteEntry(ROUTELIST *l,COORDINATE *c);

ROUTELIST *FindRouteL(COORDINATE *pickup, COORDINATE *destin); 

ROUTEENTRY *deleteEntryR(ROUTEENTRY *e);

void AddOneSpace(ROUTELIST *l, int xin, int yin);

ROUTELIST *mergeRList(ROUTELIST *d, ROUTELIST *c);

void DeleteRouteList(ROUTELIST *l);

int CheckPath(int xin, int yin, int diffx, int diffy, int count);

ROUTEENTRY *appendRouteEntry(ROUTELIST *l,COORDINATE *c);

ROUTELIST *ClosestPoint(ROUTELIST *c, ROUTELIST *d);

int distancedif(COORDINATE *c, COORDINATE *d);

ROUTELIST *DeleteRepeat(ROUTELIST *route);

ROUTEENTRY *DeleteMiddleEntry(ROUTEENTRY *e);

int BlockCount(ROUTELIST *r);

ROUTELIST *copyRList(ROUTELIST *source);
#endif
