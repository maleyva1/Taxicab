/*money.c*/

#include "Trip.h"

double calcFare(COORDINATE *c, COORDINATE *d)
{
    
    double fare = 3.75 + 0.5  * distancedif(c,d);   
    return fare;
}

/*double moneyEarned()
{
    double money = calcFare(base, block.2 *
    return money;
}*/
