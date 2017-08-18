#ifndef COORDINATE_H
#define COORDINATE_H



typedef struct coordinate_t {
    int x;
    int y;
} COORDINATE;

/* Creates a COORDINATE instance */
COORDINATE *newCoordinate(int x1, int y1);

void DeleteCoordinate(COORDINATE *c);

#endif
