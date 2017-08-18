/* Team 15 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Parser.h"
#include "Conversion.h"
#include "Money.h"
#include "Trip.h"

char *ServResponse(char *buffer, int size,TRIPLIST *l,TAXILIST *taxlist,Map *m,int *trip_IDs,double *totalmoney,time_t *reftime, time_t *dumtime) 
{
    if(*reftime == 0) {
        *reftime = time(NULL);
    }
    if(*dumtime == 0) {
        *dumtime = time(NULL);
    } 
 char **ParsedBuf = malloc(sizeof(char) * 256);
  TRIP *taxiTrip = NULL;
  ROUTELIST *r = NULL;
    char temp[10];
    char temp2[10];
  int confirm, find_taxi;
  time_t curtime = time(NULL);
  time_t ptime;
  time_t dtime;
  struct tm *loctime;
  char ptimebuf[80];
  char timebuf[80];
  double fare; 
  ParsedBuf = parseString(ParsedBuf,buffer);
  scaleTime(curtime,dumtime,reftime);
  MoveTaxi(l,curtime);

  if(strcmp(ParsedBuf[0],"REQUEST_TAXI") == 0)
  {
       strcpy(temp,ParsedBuf[1]);
       strcpy(temp2,ParsedBuf[3]);
       COORDINATE *pickup = toNumeric(ParsedBuf[1]);
       COORDINATE *destin = toNumeric(ParsedBuf[3]);
       /* Send an error message*/
       if(pickup->y > m->row || pickup->x > m->col || destin->y > m->row || destin->x > m->col)
       {
           sprintf(buffer, "ERROR Pickup and/or Destination locations are not within city boundaries.");
           return buffer;
       }
       TAXI *taxi = FindTaxi(taxlist,pickup,m);
       r = FullRoute(taxi->location,pickup,destin,m);    
       r = DeleteRepeat(r);
       fare = calcFare(pickup, destin);
       *totalmoney += fare - BlockCount(r) * .2;
       taxiTrip = newTrip(pickup,destin,r);
       taxiTrip->trip_ID = ++*(trip_IDs);
       taxiTrip->taxicab = taxi;
       newTripEntry(taxiTrip,l);
       taxi->available_seats = 0;
       ETAcalc(r,taxi->location,pickup,*dumtime,&ptime);
       ETAcalc(r,pickup,destin,ptime,&dtime);
       taxiTrip->ETA = dtime;
       loctime = localtime(&ptime);
       strftime(ptimebuf,80,"%I:%M",loctime);
       loctime = localtime(&dtime);
       strftime(timebuf,80,"%I:%M",loctime);
       strcpy(taxiTrip->stringp,temp);
       strcpy(taxiTrip->stringd,temp2);
       sprintf(buffer,"OK Taxi%d PICKUP %s %s DROPOFF %s %s $%0.2f CONFIRM #%d",taxi->taxi_ID,temp,ptimebuf,temp2,timebuf,fare,taxiTrip->trip_ID);
      
       free(ParsedBuf);
       DeleteCoordinate(pickup);
       DeleteCoordinate(destin);
       *reftime = curtime;
       return buffer;
  }
  else if (strcmp(ParsedBuf[0],"CONFIRM") == 0) 
  {
       sscanf(ParsedBuf[1],"#%d",&confirm);
       if((taxiTrip = searchConfirm(confirm,l)) != NULL){
            loctime = localtime(&taxiTrip->ETA);
            strftime(timebuf,80,"%I:%M",loctime);
            sprintf(buffer,"OK Taxi%d POSITION %s ETA %s %s",taxiTrip->taxicab->taxi_ID,taxiTrip->stringd,taxiTrip->stringd,timebuf);
            return buffer;
        }
       else{
           sprintf(buffer,"CONFIRM #%d DNE",confirm);
           return buffer;       
        }
  }
  else if (strcmp(ParsedBuf[0],"REQUEST_POSITION") == 0)
  {
      if(strcmp(ParsedBuf[1],"ALL") == 0)
       {
           return buffer;
        }
       else
       {
           sscanf(ParsedBuf[1],"Taxi%d",&find_taxi);
           taxiTrip = FindTaxiCoor(l,find_taxi);
           if(taxiTrip == NULL){
                sprintf(buffer,"Taxi%d DNE",find_taxi);
                return buffer;
           }
           else {
                loctime = localtime(&taxiTrip->ETA);
                strftime(timebuf,80,"%I:%M",loctime);
                strcpy(temp,toAlpha(taxiTrip->taxicab->location));
                strcpy(temp2,toAlpha(taxiTrip->destination));
                sprintf(buffer,"OK Taxi%d POSITION %s ETA %s %s",find_taxi,temp,temp2,timebuf);
                return buffer;
           }
       }
  }
  else
  {
      free(ParsedBuf);
      return buffer;
  }
}
