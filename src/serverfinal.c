#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <gtk/gtk.h>
#include <glib.h>	
#include <cairo.h>
#include "utils.h"

#include "Trip.h"
#include "Money.h"
#include "Parser.h"
#include "ParseAid.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int Shutdown		/* keep running until Shutdown == 1 */
= 0;
float xp=0;
float yp=0;
TAXILIST *taxlist = NULL;
Map *town = NULL;
GtkWidget *map1	/* drawing widget displaying analog clock */
= NULL;

gboolean DrawAreaExposed(	/* drawing area is displayed, */
	GtkWidget *Widget,	/* render a map    */
	GdkEventExpose *Event,
	gpointer Data)
{
	GtkWidget *DrawArea;
	gint Width, Height;
	cairo_t *cr;

/*    DrawArea = Widget;
	int width = 4;
	int height = 3;*/
    /*char mapName[256] = NULL;*/
    /*Map *town = newMap();*/
    Landmark *land=NULL;
    TaxiStand *taxi=NULL;
    /*loadMap("NewIrvine.map", town);*/
    DrawArea = Widget;
    float width = town->col;
    float height = town->row; 


	/* prepare a cairo context for the drawing */
	cr = gdk_cairo_create(DrawArea->window);
	/* draw a white rectangular background */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_rectangle(cr, 0, 0, 600, 600);
	cairo_fill(cr);

	/*make grid*/

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    float dx;
    float dy;
    dy=(400.00/(height-1));
    dx=(400.00/(width-1));
	int x, y;
	for (y = 0; y<height;y++) {
		cairo_move_to(cr,170, ((y * dy) + 120));
		cairo_line_to(cr, 570, ((y * dy) + 120));
		cairo_set_line_width(cr, 2);
		cairo_stroke(cr);

	}
	for (x = 0; x<width; x++) {
		cairo_move_to(cr, ((x * dx) + 170), 120);
		cairo_line_to(cr, ((x * dx) + 170), 520);
		cairo_set_line_width(cr, 2);
		cairo_stroke(cr);
	}

	/*make text*/
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);

	cairo_select_font_face(cr, "Purisa",
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);
	cairo_move_to(cr, 10, 20);
	cairo_show_text(cr, town->title);
    /*this is the horizontal text*/
int i = 0;
int *placehold = town->rowNames;
	for (y = 0; y<height; y++) {
		cairo_move_to(cr, 10, (y * dy) + 122);
        cairo_show_text(cr,*(town->rowNames++));

		/*cairo_show_text(cr,*(town->rowNames++));*/

	}
    town->rowNames = placehold;
	 float a;
	a=M_PI/2;
    placehold = town->colNames;
    for (x=0;x<width;x++){
        
        cairo_save(cr);
	    cairo_move_to(cr,( (x*dx)+170), 10);

	    cairo_rotate(cr,a);

	    cairo_show_text(cr,*(town->colNames++));
    
    /*unrotate*/
        cairo_restore(cr);
    }
    town->colNames = placehold;
        int x1,y1;
        for(x=0;x<town->landmarks->size;x++){ 
        land = town->landmarks->items + x * town->landmarks->itemSize;    
	    /*make a landcape*/
        x1=land->topLeft.x;
        y1=land->topLeft.y;
        /*printf("%d %d\n",x1,y1);*/
	    cairo_set_line_width(cr, 10);
	    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	    cairo_rectangle(cr,y1*dx+170,x1*dy+120,((land->botRight.y )-(land->topLeft.y))*dx,((land->botRight.x )-(land->topLeft.x))*dy);
        cairo_fill(cr);

        cairo_arc(cr, (land->mark.y*dx)+170,(land->mark.x*dy)+120, 3, 0, 2*M_PI);
        
        cairo_set_source_rgb(cr, 0.5, 0.5, 1);
        cairo_fill(cr);


        
        cairo_set_source_rgb(cr, 1, 0, 0.5);
        
            cairo_move_to(cr, (land->mark.y*dx)+170,(land->mark.x*dy+120));
            cairo_show_text(cr, land->name);
        
    }
    /*taxi loop*/
    for(x=0;x<town->stands->size;x++){

       taxi = town->stands->items + x * town->stands->itemSize;
       cairo_arc(cr, (taxi->mark.y*dx)+170,(taxi->mark.x*dy)+120, 3, 0, 2*M_PI);
       cairo_set_source_rgb(cr, 0.5, 0.5, 1);
       cairo_fill(cr);
        cairo_set_source_rgb(cr, 1, 0, 0.5);
        cairo_save(cr);

        cairo_move_to(cr, (taxi->mark.y*dx)+170,(taxi->mark.x*dy+120));
        cairo_rotate(cr,a);
        cairo_show_text(cr, taxi->name);
        cairo_restore(cr);

    } 
    TAXIENTRY *e = taxlist->taxifirst;
    TAXIENTRY *n = NULL;
    while(e) {
    n = e->taxinext;
    cairo_arc(cr, (e->cab->location->x*dx) + 170,(e->cab->location->y*dy) + 120,3,0,2*M_PI);
    cairo_set_source_rgb(cr,0.5,0.5,1);
    cairo_fill(cr);
    cairo_set_source_rgb(cr,1,0,0.5);
    cairo_save(cr);

    e = n;
    }

    cairo_arc(cr, xp+170,yp+120, 3, 0, 2*M_PI);
    cairo_set_source_rgb(cr, 1, 0, 0.5);
    cairo_fill(cr);
	cairo_destroy(cr);
	
#ifdef DEBUG
	printf("%s: DrawAreaExposed callback done.\n", Program);
#endif
	return TRUE; /* area drawn, done with expose event */
} /* end of DrawAreaExposed */

void ShutdownClicked(		/* shutdown button was clicked */
	GtkWidget *Widget,
	gpointer Data)
{
#ifdef DEBUG
	printf("%s: ShutdownClicked callback starting...\n", Program);
#endif
	Shutdown = 1;	/* initiate server shutdown */
	map1 = NULL;
#ifdef DEBUG
	printf("%s: ShutdownClicked callback done.\n", Program);
#endif
} /* end of ShutdownClicked */


GtkWidget *CreateWindow(	/* create the server window */
	int *argc,
	char **argv[])
{
	GtkWidget *Window;
    GtkWidget *VBox, *DrawArea, *Frame, *Label;
    GtkWidget *ShutdownButton;
    

	/* initialize the GTK libraries */
	gtk_init(argc, argv);

	/* create the main, top level window */
	Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(Window), "Domer Roamer Server");
	gtk_window_set_default_size(GTK_WINDOW(Window),650,650 );
	gtk_container_set_border_width(GTK_CONTAINER(Window), 10);

	/* overall vertical arrangement in the window */
	VBox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(Window), VBox);
	
    /* on the top, put a drawing area */
	DrawArea = gtk_drawing_area_new();
	gtk_widget_set_size_request(DrawArea, 500, 500);
	gtk_container_add(GTK_CONTAINER(VBox), DrawArea);


	/* on the bottom, a button to shutdown the server and quit */
	ShutdownButton = gtk_button_new_with_label("Shutdown Server and Quit");
	gtk_container_add(GTK_CONTAINER(VBox), ShutdownButton);

	/* make sure that everything becomes visible */
	gtk_widget_show_all(Window);

	/* connect drawing area with function to render a clock */
	g_signal_connect(DrawArea, "expose_event",
		G_CALLBACK(DrawAreaExposed), NULL);

	/* make clock widgets public */
	map1 = DrawArea;

	/* connect window-close with function terminating this server */
	g_signal_connect(Window, "destroy",
		G_CALLBACK(ShutdownClicked), NULL);

	/* connect shutdown button with function terminating this server */
	g_signal_connect(ShutdownButton, "clicked",
		G_CALLBACK(ShutdownClicked), NULL);

	return(Window);
} /* end of CreateWindow */

void UpdateWindow(void)		/* render the window on screen */
{
	/* this server has it's own main loop for handling client connections;
	* as such, it can't have the usual GUI main loop (gtk_main);
	* instead, we call this UpdateWindow function on regular basis
	*/
	while (gtk_events_pending())
	{
		gtk_main_iteration();
	}
} /* end of UpdateWindow */



void ServerMainLoop() {
	while (!Shutdown)
	{
		UpdateWindow();	/* update the GUI window */
        if (map1)    /* if analog clock is functional, */
                {
                        gdk_window_invalidate_rect(map1->window, /* update it! */
                                        &map1->allocation, /* invalidate entire region */
                                                FALSE); /* no children */
                            }
	}

	


}




int main(			/* the main function */
	int argc,
	char *argv[])
{
    town = newMap();
    loadMap(argv[2],town);

    taxlist = AllTaxi(town);

	GtkWidget *Window;	/* the client window */	
#ifdef DEBUG
	printf(" Creating the server window...\n");
#endif
	Window = CreateWindow(&argc, &argv);
	if (!Window)
	{
		printf("cannot create GUI window\n");
		exit(10);
	}
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

    while(!Shutdown)
    {
        
        UpdateWindow();
        if(map1)
        {
            gdk_window_invalidate_rect(map1->window, /* update it! */
                                        &map1->allocation, /* invalidate entire region */
                                                FALSE); /* no children */
                            }
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
        printf("total money earned = %.02f",*totalmoney); 

    }
    
    free(trip_IDs);
    deleteTaxiList(taxlist);
    DeleteTripList(l,NULL);
    deleteMap(town);
    free(totalmoney);
    close(socketfd);
    return 0;
 /* end of main */
}

