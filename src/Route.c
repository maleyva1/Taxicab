#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Trip.h"


ROUTELIST *FullRoute(COORDINATE *taxi, COORDINATE *pick, COORDINATE *dest,Map *m) {
    ROUTELIST *init = NULL;
    ROUTELIST *final = NULL;
    ROUTELIST *path = NULL;
    ROUTEENTRY *e = NULL;
    ROUTEENTRY *n = NULL;
    if(taxi && distancedif(taxi,pick) < 15) {
            init = SmallRoute(taxi,pick,m);
    }
    else if(taxi) {
            path = FindCoordinatePath(taxi,pick,m);
            e = path->first;
            init = SmallRoute(taxi,e->coord,m);
            e = e->next;
            while(e) {
                n = e->next;
                mergeRList(init,SmallRoute(init->last->coord,e->coord,m));
                taxi = init->last->coord;
                e = n;
            }
            mergeRList(init,SmallRoute(init->last->coord,pick,m));
    }
    if(distancedif(pick,dest) < 15) {
            final = SmallRoute(pick,dest,m);
    }
    else {
            path = FindCoordinatePath(pick,dest,m);

            e = path->first;
            final = SmallRoute(pick,e->coord,m);
            e = e->next;
            while(e) {
                n = e->next;
                mergeRList(final,SmallRoute(final->last->coord,e->coord,m));
                pick = final->last->coord;
                e = n;
            }
            mergeRList(final,SmallRoute(final->last->coord,dest,m));
            final = DeleteRepeat(final);
    }
    if(taxi) {
        return mergeRList(init,final);
    }
    else {
        return final;
    }
}

ROUTELIST *SmallRoute(COORDINATE *c,COORDINATE *d,Map *m) {
    ROUTELIST *route = FindnoBuildRoute(c,d,1,m);
    while(route->last->coord->x != d->x && route->last->coord->y != d->y) {
        mergeRList(route,FindnoBuildRoute(route->last->coord,d,1,m));
                }
    return route;
}

ROUTELIST *DeleteRepeat(ROUTELIST *route) {
    int Delete = 1;
    ROUTEENTRY *e = route->first;
    ROUTEENTRY *n = NULL;
    ROUTELIST *points = newRouteList();
    COORDINATE *temp = NULL;
    ROUTEENTRY *rip = NULL;
    ROUTEENTRY *nrip = NULL;
    while(e) {
        n = e->next;
        temp = newCoordinate(e->coord->x,e->coord->y);
        appendRouteEntry(points,temp);
        rip = points->first->next;
        while(rip) {
            nrip = rip->next;
            if(rip->coord->x == temp->x && rip->coord->y == temp->y) {
            Delete = -(Delete);
            break;   
            }
            rip = nrip;
        }
        if(Delete != 1) {
            e = DeleteMiddleEntry(e);
            e = e->next;
            Delete = -(Delete);
            continue;
        }
     e = n;   
    }
   DeleteRouteList(points); 
    return route;
}

ROUTEENTRY *DeleteMiddleEntry(ROUTEENTRY *e) {
    int x = e->coord->x;
    int y = e->coord->y;
    ROUTEENTRY *n = e->list->first;
    ROUTEENTRY *nf = NULL;
    while(n) {
        nf = n->next;
        if(n->coord->x == x && n->coord->y == y) break;
    n = nf;
    }
    ROUTEENTRY *b = NULL;
    do {
        b = n->prev;
        n = deleteEntryR(n);
        n->prev = b;
        if(b) {
        b->next = n;
        }
    }while (!((n->coord->x == x) && (n->coord->y == y)));
    return n;
}

    
        
int distancedif(COORDINATE *c,COORDINATE *d) {
    return abs(d->x - c->x) + abs(d->y - c->y);
    }
    
TAXI *FindTaxi(TAXILIST *taxlist,COORDINATE *pick,Map *m) {
    TAXIENTRY *e = taxlist->taxifirst;
    TAXIENTRY *n = NULL;
    TAXI *taxi = NULL;
    ROUTELIST *temp = NULL;
    int min = 1000;
    int bcount;
    while(e) {
        n = e->taxinext;
        temp = FullRoute(NULL,e->cab->location,pick,m);
        bcount = BlockCount(temp);
        if(bcount < min && e->cab->available_seats > 0) {
            min = bcount;
            taxi = e->cab;
        }
        DeleteRouteList(temp);
        e = n;
    }
    return taxi;
}

int BlockCount(ROUTELIST *r) {
    int countx = 0, county = 0;
    ROUTEENTRY *e = r->first;
    ROUTEENTRY *n = NULL;
    while(e->next) {
        n = e->next;
        countx += abs(e->next->coord->x - e->coord->x);
        county += abs(e->next->coord->y - e->coord->y);
        e = n;
    }
    return countx + county;
}

ROUTELIST *newRouteList()
{
    ROUTELIST *l = malloc(sizeof(ROUTELIST));
    l->first = NULL;
    l->last = NULL;
    return l;
}
ROUTEENTRY *newRouteEntry(ROUTELIST *l,COORDINATE *c)
{
    ROUTEENTRY *e = malloc(sizeof(ROUTEENTRY));
    e->coord = c;
    e->list = l;
    if(l->first) {
        l->last->next = e;
        e->prev = l->last;
        l->last = e;
        l->last->next = NULL;
    }
    else {
        l->first = e;
        l->last = e;
        e->next = NULL;
        e->prev = NULL;
    }
    return e;
}

   
ROUTEENTRY *deleteEntryR(ROUTEENTRY *e) {
    ROUTEENTRY *n = e->next;
    if(!e) return NULL;
   if(e == e->list->first) {
      e->list->first = n;
   } 
   free(e->coord);
   free(e);
    
    return n;
}


ROUTELIST *FindCoordinatePath(COORDINATE *c, COORDINATE *d,Map *m) {
    int x,y,xd,yd,search = 0;
    ROUTELIST *current = FindIdealCoordinate(c,d,m);
    ROUTELIST *destin = FindIdealCoordinate(d,c,m);
    ROUTELIST *partone = newRouteList();
    ROUTELIST *parttwo = newRouteList();
    ROUTELIST *l = ClosestPoint(current,destin);
    COORDINATE *temp = NULL;
    ROUTELIST *staple = NULL;
    x = l->first->coord->x;
    y = l->first->coord->y;
    xd = l->last->coord->x;
    yd = l->last->coord->y;
    while(l->first->coord->x != l->last->coord->x 
            || l->first->coord->y != l->last->coord->y) {
        if(!search){
        temp = newCoordinate(l->first->coord->x,l->first->coord->y);
        newRouteEntry(partone,temp);
        temp = newCoordinate(l->last->coord->x,l->last->coord->y);
        appendRouteEntry(parttwo,temp);
        }
        else {
            search = 0;
        }
        if(distancedif(partone->last->coord,d) < 15) {
            return mergeRList(partone,SmallRoute(partone->last->coord,d,m));
        }
        DeleteRouteList(current);
        DeleteRouteList(destin);
        current = FindIdealCoordinate(l->first->coord,l->last->coord,m);
        destin = FindIdealCoordinate(l->last->coord,l->first->coord,m);
        DeleteRouteList(l);
        l = ClosestPoint(current,destin);
        if(x == l->first->coord->x
                && y == l->first->coord->y
                && xd == l->last->coord->x
                && yd == l->last->coord->y)
        {
            search = 1;
            temp = SearchIdealLandmark(x,y,partone,m,d);
            DeleteRouteList(partone);
            DeleteRouteList(parttwo);
            parttwo = newRouteList();
            if(!staple) {
                partone = FindCoordinatePath(c,temp,m);
                newRouteEntry(partone,temp);
                staple = copyRList(partone);

            }
            else {
                partone = FindCoordinatePath(staple->last->coord,temp,m);
                newRouteEntry(partone,temp);
                partone = mergeRList(staple,partone);
                staple = copyRList(partone);
                }
            DeleteRouteList(l);
            l = newRouteList();
            temp = newCoordinate(partone->last->coord->x,partone->last->coord->y);
            newRouteEntry(l,temp);
            temp = newCoordinate(d->x,d->y);
            newRouteEntry(l,temp);
        }
        x = l->first->coord->x;
        y = l->first->coord->y;
        xd = l->last->coord->x;
        yd = l->last->coord->y;
    }
    if(!partone->first){
        temp = newCoordinate(l->first->coord->x,l->first->coord->y);
        newRouteEntry(partone,temp);
    }
    if(!parttwo->first){
        temp = newCoordinate(l->last->coord->x,l->last->coord->y);
        appendRouteEntry(parttwo,temp);
    }
    if(staple) {
        DeleteRouteList(staple);
    }
    return mergeRList(partone,parttwo); 
}
ROUTELIST *copyRList(ROUTELIST *source) {
    ROUTELIST *copy = newRouteList();
    ROUTEENTRY *e = source->first;
    ROUTEENTRY *n = NULL;
    COORDINATE *temp = NULL;
    while(e) {
        n = e->next;
        temp = newCoordinate(e->coord->x,e->coord->y);
        newRouteEntry(copy,temp);
        e = n;
    }
    return copy;
}


ROUTEENTRY *appendRouteEntry(ROUTELIST *l,COORDINATE *c) {
ROUTEENTRY *e = malloc(sizeof(ROUTEENTRY));
    e->coord = c;
    e->list = l;
    if(l->first) {
        e->next = l->first;
        l->first->prev = e;
        l->first = e;
        e->prev = NULL;
    }
    else {
        l->first = e;
        l->last = e;
        e->next = NULL;
        e->prev = NULL;
    }
    return e;
}

ROUTELIST *ClosestPoint(ROUTELIST *c, ROUTELIST *d) {
    ROUTELIST *l = newRouteList();
    ROUTEENTRY *ec = c->first;
    ROUTEENTRY *ed = d->first;
    ROUTEENTRY *nc = NULL;
    ROUTEENTRY *nd = NULL;
    COORDINATE *tempc = NULL;
    COORDINATE *tempd = NULL;
    int minx = 1000, miny = 1000;
    while(ec) {
        nc = ec->next;
        ed = d->first;
        while(ed) {
            nd = ed->next;
            if (((abs(ed->coord->x - ec->coord->x) <= minx)
                    && (abs(ed->coord->y - ec->coord->y) <= miny)) 
                    || l->first == NULL) {
                minx = abs(ed->coord->x - ec->coord->x);
                miny = abs(ed->coord->y - ec->coord->y);
                if(l->first) {
                    DeleteRouteList(l);
                    l = newRouteList();
                }
                tempd = newCoordinate(ed->coord->x,ed->coord->y);
                tempc = newCoordinate(ec->coord->x,ec->coord->y);
                newRouteEntry(l,tempc);
                newRouteEntry(l,tempd);
            }
            ed = nd;
        }
        ec = nc;
    }
return l;
}

    

ROUTELIST *FindIdealCoordinate(COORDINATE *c,COORDINATE *d,Map *m) {
    int xin,yin;
    if(d->x - c->x > 0) xin = 1;
    else xin = -1;

    if(d->y - c->y > 0) yin = 1;
    else yin = -1;

    ROUTELIST *test = NULL;
    ROUTELIST *ideal = newRouteList();
    COORDINATE *testpoint = NULL;
    int i, j, testx, testy;
    for(i = -5; i < 5; i++) {
        for(j = -5; j < 5; j++) {
            testx = c->x + xin * i;
            testy = c->y + yin * j;
            testpoint = newCoordinate(testx,testy);
            test = FindnoBuildRoute(c,testpoint,0,m);
            if(test && test->last->coord->x == testpoint->x && test->last->coord->y == testpoint->y) {
                newRouteEntry(ideal,testpoint);
                DeleteRouteList(test);
                test = NULL;
            }
            else {
                DeleteCoordinate(testpoint);
                testpoint = NULL;
            }
        }
    }
return ideal;
}
    
ROUTELIST *FindnoBuildRoute(COORDINATE *c, COORDINATE *d,int sig,Map *m) {
    int diffx = d->x - c->x;
    int diffy = d->y - c->y;
    int xin, yin, count,i,op,skip = 0;;
    TRIPLIST *t = NULL;
    ROUTELIST *l = NULL;
    COORDINATE *temp = NULL;
    if(diffx < 0) xin = -1;
    else xin = 1;
    if(diffy < 0) yin = -1;
    else yin = 1;
    t = newTripList();
    for(count = 0; count < pow(2,(double) (abs(diffx) + abs(diffy))); count++) {
         if(!sig && !(CheckPath(xin,yin,diffx,diffy,count))) {
            continue;
        }
        l = newRouteList();
        temp = newCoordinate(c->x,c->y);
        newRouteEntry(l,temp);
        temp = newCoordinate(c->x,c->y);
        newRouteEntry(l,temp);
        if(skip) {
            t->triplast->Trip->Route = l;
            skip = 0;
        }
        else{            
        newTripEntry(newTrip(c,d,l),t);
        }  
        for(i = 0; i < (abs(diffx) + abs(diffy)); i++) {
            op = (int) pow(2,i);
            if((count & op) > 0) {
                AddOneSpace(l,xin,0);
                if(!legalposition(l->last->coord,m)) {
                    skip = 1;
                    DeleteRouteList(l);
                    l = NULL;
                    t->triplast->Trip->Route = NULL;
                    break;
                }
            }
            else{
                AddOneSpace(l,0,yin);
                if(!legalposition(l->last->coord,m)) {
                    skip = 1;
                    DeleteRouteList(l);
                    l = NULL;
                    t->triplast->Trip->Route = NULL;
                    break;
                }
            }
        }
        if(!skip && l->last->coord->x == d->x && l->last->coord->y == d->y) {
            DeleteTripList(t,l);
            return l;
        }
    }
    /* out here means building in the way*/
    if(sig) {
        return closestPointR(t);
    }
    else {
        return NULL;
    }
}
int CheckPath(int xin, int yin, int diffx, int diffy, int count) {
    int i,op,countx = 0,county = 0;
    for(i = 0; i < (abs(diffx) + abs(diffy)); i++) {
            op = (int) pow(2,i);
            if((count & op) > 0) {
                countx += xin;
            }
            else{
                county += yin;
                }
            }
    if(countx == diffx && county == diffy) return 1;
    else return 0;
}
void AddOneSpace(ROUTELIST *l, int xin, int yin) {
    COORDINATE *c = l->last->coord;
    COORDINATE *p = NULL;
    COORDINATE *n = NULL;
    if(l->last->prev != NULL) {
        p = l->last->prev->coord;
    
    if( ((c->x + xin - p->x) != 0) && ((c->y + yin - p->y) != 0)) {
        if(xin != 0) {
           n = newCoordinate(c->x + xin,c->y);
           newRouteEntry(l,n);
           return;
        }
        else {
           n = newCoordinate(c->x,c->y + yin);
           newRouteEntry(l,n);
           return;
        }
    }
    }
    c->x = c->x + xin;
    c->y = c->y + yin;
    return;
}
     

ROUTELIST *closestPointR(TRIPLIST *t) {
    TRIPENTRY *e = t->tripfirst;
    TRIPENTRY *n = NULL;
    ROUTELIST *min = NULL;
    int ydist,xdist;
    int xdestin = t->tripfirst->Trip->destination->x;
    int ydestin = t->tripfirst->Trip->destination->y;
    int distance = -1;
    int temp;
    while(e) {
        n = e->tripnext;
        if(!e->Trip->Route) {
            e = n;
            continue;
        }
        ydist = ydestin - e->Trip->Route->last->coord->y;
        xdist = xdestin - e->Trip->Route->last->coord->x;
        temp = ydist * ydist + xdist * xdist;
        if(temp < distance || distance == -1) {
            distance = temp;
            min = e->Trip->Route;
        }
        e = n;
    }
    DeleteTripList(t,min);
    return min;
}

ROUTELIST *mergeRList(ROUTELIST *d,ROUTELIST *c) {
    ROUTEENTRY *e = NULL;
    ROUTEENTRY *p = c->first;
    ROUTEENTRY *n = NULL;
    COORDINATE *temp = NULL;
    while(p) {
        e = d->last;
        n = p->next;
        if(!((e->coord->x == p->coord->x) && (e->coord->y == p->coord->y))) {
        temp = newCoordinate(p->coord->x,p->coord->y);
        newRouteEntry(d,temp);
        }
        p = n;
    }
    DeleteRouteList(c);
    return d;
}

void DeleteRouteList(ROUTELIST *l) {
    if(!l) return;
    
    ROUTEENTRY *e = l->first;
    ROUTEENTRY *n = NULL;
    while (e) {
    n = deleteEntryR(e);
    e = n;
    }
    free(l);
}

/*int legalposition(COORDINATE *point,Map *m) {
    if(point->x < 0 || point->y < 0) return 0;
    int i, j;
    for (i = 11; i < 15; i++) {
        for (j = 0; j < 20; j++) {
            if(i == point->x && j == point->y) return 0;
        }
    }
    return 1;
} */

int legalposition(COORDINATE *point, Map *m) {
    if(point->x < 0 || point->y < 0) return 0;
    Landmark *land = NULL;
    int i,x,y;
    if(m->landmarks != NULL) {
    for(i = 0; i < m->landmarks->size; i++) {
        land = m->landmarks->items + i * m->landmarks->itemSize;
        for(x = land->topLeft.x + 1; x <= land->botRight.x - 1; x++) {
           for(y = land->topLeft.y - 1; y >= land->botRight.y + 1; y--) {
              if(point->x == x && point->y == y) {
                 return 0;
              }
           }
        }
    }
    }
   return 1;
} 
