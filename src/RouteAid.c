#include "Trip.h"

COORDINATE *SearchIdealLandmark(int xin,int yin, ROUTELIST *r,Map *m,COORDINATE *dest) {
    Landmark *land = NULL;
    COORDINATE *temp = NULL;
    COORDINATE *tempn = NULL;
    int i,x,y;
    if(xin == 10) {
        temp = newCoordinate(10,20);
        return temp;
    }
    for(i = 0; i < m->landmarks->size;i++) {
        land = m->landmarks->items + i * m->landmarks->itemSize;
        for(x = land->topLeft.x; x <= land->botRight.x; x++) {
            for(y = land->topLeft.y; y >= land->botRight.y; y--) {
                if(xin == x && yin == y) {
                    i = m->landmarks->size;
                    x = land->botRight.x + 1;
                    break;
                }
            }
        }       
    }
    if(land->topLeft.x == xin) {
        temp = newCoordinate(land->topLeft.x,land->topLeft.y);
        tempn = newCoordinate(land->topLeft.x,land->botRight.y);
        if(distancedif(temp,dest) > distancedif(tempn,dest)) {
            DeleteCoordinate(temp);
            return tempn;
        }
        else {
            DeleteCoordinate(tempn);
            return temp;
        }

    }

    else if(land->topLeft.y == yin) {
        temp = newCoordinate(land->topLeft.x,land->topLeft.y);
        tempn = newCoordinate(land->botRight.x,land->topLeft.y);
        if(distancedif(temp,dest) > distancedif(tempn,dest)) {
            DeleteCoordinate(temp);
            return tempn;
        }
        else {
            DeleteCoordinate(tempn);
            return temp;
        }

    }
    else if(land->botRight.x == xin) {
        temp = newCoordinate(land->botRight.x,land->topLeft.y);
        tempn = newCoordinate(land->botRight.x,land->botRight.y);
        if(distancedif(temp,dest) > distancedif(tempn,dest)) {
            DeleteCoordinate(temp);
            return tempn;
        }
        else {
            DeleteCoordinate(tempn);
            return temp;
        }

    }
    else {
        temp = newCoordinate(land->botRight.x,land->botRight.y);
        tempn = newCoordinate(land->topLeft.x,land->botRight.y);
        if(distancedif(temp,dest) > distancedif(tempn,dest)) {
            DeleteCoordinate(temp);
            return tempn;
        }
        else {
            DeleteCoordinate(tempn);
            return temp;
        }

    }
}
