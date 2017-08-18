/* Team 15  */
/* Original author: Mark Leyva */
/* Server.c file */

#include <stdio.h>
#include <stdlib.h>
/* Network communication headers below */
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include "Trip.h"
#include "Money.h"
/* Team module headers */
#include "Parser.h"
#include "ParseAid.h"

int main(int argc, char *argv[])
{
    /* socketfd, newsocket: Socket file descriptors         */
    /* portnumero: Port Number                              */ 
    /* client_size: Size of the client address              */ 
    /* return_val: Return value of read() and write() calls */
    time_t reftime = 0;
    time_t dumtime = 0;
    int socketfd, newsocketfd, portnumero;
    socklen_t client_size;
    int n;
    int *trip_IDs = malloc(sizeof(int));
    *trip_IDs = 0;
    double *totalmoney = malloc(sizeof(double));
    *totalmoney = 0;
    TRIPLIST *l = newTripList();
    Map *town = newMap();
    loadMap(argv[2],town);
    TAXILIST *taxlist = NULL;
    taxlist = AllTaxi(town);

    /*loadMap("TinyTown.map",town);*/
    /* The char array to hold the messages from the client */
    char buffer[256];

    /* The socket addresses for the server and client */
    struct sockaddr_in server_addr, client_addr;

    /* Check to see that the user inputed a port number */
    if(argc < 3)
    {
        perror("Error! Please enter portnumber nameof.map\n");
        exit(1);
    }

    /* Initialize the socket file deescriptor with the socket() command */
    /*      We use AF_INET for Internet address type and SOCK_STREAM    */
    /*      for cotinuous data reading                                  */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) { perror("Error opening socket!\n"); }

    /* We zero out the server address */
    bzero( (char *) &server_addr, sizeof(server_addr));

    /* Convert the inputted port number from char to int */
    portnumero = atoi(argv[1]);

    /* Initialize the server address struct */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; /* INADDR_ANY is the symbolic constant for the server's address */
    server_addr.sin_port = htons(portnumero); /* sin_port takes in some network byte order so we have to convert using htons() */

    /* Bind the socket file descriptor from above to the server address and check if it works */
    if(bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error with binding the socket to address!\n");
        exit(1);
    }

    while(1)
    {
        /* Begin listening on the socket file deiscriptor */
        listen(socketfd, 5);

        /* Set the client_size to the size of the client_addr */
        client_size = sizeof(client_addr);
        /* Begin accepting requests to the newsocket file descriptor from the client address */
        newsocketfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_size);
        /* Check for errors */
        if(newsocketfd < 0) { perror("Error with accepting request!\n"); }

        /* Zero out the char array */
        bzero(buffer, 256);

        /* Read and check for errors */
        n = read(newsocketfd, buffer, 255);
        if(n < 0) { perror("Error with reading message!\n"); }
        if(strcmp(buffer,"end\n") == 0)
        {
            close(newsocketfd);
            break;
        }
        ServResponse(buffer,255,l,taxlist,town,trip_IDs,totalmoney,&reftime,&dumtime);
        /* Write and check for errors */
        n = write(newsocketfd, buffer, 100);
        if (n < 0) { perror("Error with writing message!\n"); }

        /* Close sockets */
        close(newsocketfd);
    }
    free(trip_IDs);
    deleteTaxiList(taxlist);
    DeleteTripList(l,NULL);
    deleteMap(town);
    free(totalmoney);
    close(socketfd);
    return 0;
}
