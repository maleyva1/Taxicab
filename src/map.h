#ifndef MAP_H
#define MAP_H

#define LEN 80
#include "utils.h"
typedef struct Point_t {
    int x; int y;
} Point;

typedef struct TaxiStand_t {
    Point mark;    /*the mark position of taxi stand*/
    int numTaxis;   /* the number of taxis located in this stand*/
    char name[LEN];  /* name of the landmark*/
} TaxiStand; 

typedef struct Landmark_t {
    Point mark;
    Point topLeft;  /* top left point*/
    Point botRight; /* bottom right point*/
    char name[LEN];  /* name of the landmark*/
} Landmark; 

typedef struct Map_t {
    char title[LEN];    /* title of the map*/
    /* street names*/
    int row;    /* number of rows*/
    int col;    /* number of columns*/
    int *rowNames; /* names of the rows*/
    int *colNames; /* names of the columns*/
    Array *landmarks;
    Array *stands;  /* pointer to array of taxi stands*/
} Map;


/* Point API */
void setPoint(Point *point, int x, int y);

/* compare point with input (x, y), return 0 if matched, otherwise 1 */
int cmpPoint(const Point *point1, int x, int y);

/* Landmark API */
Landmark* newLandmark();

void deleteLandmark(Landmark *landmark);

/* parse the given string, set landmark value */
void setLandmark(const char *line, Landmark *landmark);

void printLandmark(const Landmark *landmark);

/* TaxiStand API */
TaxiStand* newTaxiStand();

void deleteTaxiStand(TaxiStand *taxiStand);

/* parse the input string, set taxi stand value */
void setTaxiStand(const char *line, TaxiStand *taxiStand);

void printTaxiStand(const TaxiStand *taxiStand);

/* Utility API */
/* utility function: given string, get integer part of it as index of map */
int getIntegerIndex(const char *str, int *pIndex);

/* utility function: given string, get alphabetic index from given position */
int getAlphabeticIndex(const char *str, int *pIndex);

/* copy the last remaining string to name */
void setObjName(const char *str, int *pIndex, char *name);

/* utility function used by my_itoa, reverse a string */
void reverse(char buffer[], int length);

/* convert integer to string */
void my_itoa(int num, char *buffer, int base);

void setDefaultStreetNames(int *names, int size, int base);

/* Map API */
Map* newMap();

void deleteMap(Map *map);

int loadDefaultMap(Map *map);

/* load map from the file */
int loadMap(const char *filename, Map *map);

void printMap(const Map *map);

#endif
