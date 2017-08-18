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
#include "map.h"
#include "utils.h"

/*#include "Trip.h"
#include "Money.h
#include "Parser.h"
#include "ParseAid.h"*/

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int Shutdown		/* keep running until Shutdown == 1 */
= 0;
float xp=0;
float yp=0;

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
    Map *town = newMap();
    Landmark *land=NULL;
    TaxiStand *taxi=NULL;
    loadMap("NewIrvine.map", town);
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
	for (y = 0; y<height; y++) {
		cairo_move_to(cr, 10, (y * dy) + 122);
		cairo_show_text(cr,*(town->rowNames++));

	}
	 float a;
	a=M_PI/2;
    for (x=0;x<width;x++){
        
        cairo_save(cr);
	    cairo_move_to(cr,( (x*dx)+170), 10);

	    cairo_rotate(cr,a);

	    cairo_show_text(cr,* (town->colNames++));
    
    /*unrotate*/
        cairo_restore(cr);
    }
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

gchar *text1,*text2,*text3,*text4;
GtkWidget *status_bar;

void enter_callback(GtkWidget *entry,const gchar *tmp )
{
        /*you can just use this function to get the text from the gtk*/
        const gchar *entry_text;
        entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
        tmp = entry_text;

        g_print ("Entry contents: %s\n", tmp);
}
static gboolean on_match_select(GtkEntryCompletion *widget, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data){
        GValue value = {0, };
        gtk_tree_model_get_value(model, iter, 0, &value);/*The 0 is for the column value/words displayed*/
        fprintf(stdout, "You have selected %s\n", g_value_get_string(&value));
        g_value_unset(&value);
        return FALSE;
}
void toggle_button_callback (GtkWidget *widget, gint data)
{

        if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
        {
                /* If control reaches here, the toggle button is down */
                data = 1;
                printf("%d\n",data);
        } else {

        /* If control reaches here, the toggle button is up */
                data = 0;
                printf("%d\n",data);
        }
}

void confirm_button(GtkWidget *widget,gint data)
{
        printf("Cab confirmed!\n");
}

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

	/*Aaron's code*/
	GtkWidget *window;
        GtkWidget *hbox,*vbox;
        GtkWidget *check1;
        GtkWidget *button1,*button2;
        GtkWidget *entry1,*entry2,*entry3,*entry4;
        GtkWidget *label;
        GtkEntryCompletion *completion;
        GtkListStore *listStore = gtk_list_store_new(1,G_TYPE_STRING);
        GtkTreeIter iter;
        gint condition;
        const char *name[] = {"a","ab","ac","ad","ae"};
        int size = sizeof(name)/sizeof(const char *);
        int i;

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window),"Client Interface");
        hbox = gtk_hbox_new(FALSE,0);
        gtk_container_add(GTK_CONTAINER(window),hbox);
        vbox = gtk_vbox_new(FALSE,0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox,TRUE,TRUE,0);


        g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
        gtk_widget_set_size_request (GTK_WIDGET (window), 500, 500);

        label = gtk_label_new("Starting location");
        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

        label = gtk_label_new("x coordinate");
        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

        entry1 = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(entry1),50);
        g_signal_connect(entry1,"activate",G_CALLBACK(enter_callback),GINT_TO_POINTER(text1));
        gtk_box_pack_start(GTK_BOX(vbox),entry1,TRUE,TRUE,0);
        completion = gtk_entry_completion_new();
        gtk_entry_completion_set_text_column(completion,0);
        gtk_entry_set_completion(GTK_ENTRY(entry1),completion);
        g_signal_connect(G_OBJECT(completion),"match-selected",G_CALLBACK(on_match_select),NULL);
        for(i=0;i<size;i++)
        {
                gtk_list_store_append(listStore, &iter);
                gtk_list_store_set(listStore,&iter, 0, name[i],-1);
        }
        gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(listStore));

        label = gtk_label_new("y coordinate");
        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

        entry2 = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(entry2),50);
        g_signal_connect(entry2,"activate",G_CALLBACK(enter_callback),GINT_TO_POINTER(text2));
        gtk_box_pack_start(GTK_BOX(vbox),entry2,TRUE,TRUE,0);
        completion = gtk_entry_completion_new();
        gtk_entry_completion_set_text_column(completion,0);
        gtk_entry_set_completion(GTK_ENTRY(entry2),completion);
        g_signal_connect(G_OBJECT(completion),"match-selected",G_CALLBACK(on_match_select),NULL);
        gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(listStore));

        label = gtk_label_new("Ending location");
        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

        label = gtk_label_new("x coordinate");
        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);


       entry3 = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(entry3),50);
        g_signal_connect(entry3,"activate",G_CALLBACK(enter_callback),GINT_TO_POINTER(text3));
        gtk_box_pack_start(GTK_BOX(vbox),entry3,TRUE,TRUE,0);
        completion = gtk_entry_completion_new();
        gtk_entry_completion_set_text_column(completion,0);
        gtk_entry_set_completion(GTK_ENTRY(entry3),completion);
        g_signal_connect(G_OBJECT(completion),"match-selected",G_CALLBACK(on_match_select),NULL);
    gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(listStore));

        label = gtk_label_new("y coordinate");
        gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,0);

        entry4 = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(entry4),50);
        g_signal_connect(entry4,"activate",G_CALLBACK(enter_callback),GINT_TO_POINTER(text4));
        gtk_box_pack_start(GTK_BOX(vbox),entry4,TRUE,TRUE,0);
        completion = gtk_entry_completion_new();
        gtk_entry_completion_set_text_column(completion,0);
        gtk_entry_set_completion(GTK_ENTRY(entry4),completion);
        g_signal_connect(G_OBJECT(completion),"match-selected",G_CALLBACK(on_match_select),NULL);
        gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(listStore));

        check1 = gtk_check_button_new_with_label("Single");
        gtk_box_pack_start(GTK_BOX(vbox),check1,TRUE,TRUE,0);
        toggle_button_callback(check1,condition);


        button1 = gtk_button_new_with_label ("Quit");
        g_signal_connect (button1, "clicked",G_CALLBACK (gtk_main_quit), NULL);
        gtk_box_pack_start(GTK_BOX(vbox),button1,TRUE,TRUE,0);


        button2 = gtk_button_new_with_label ("Confirm Cab");
        g_signal_connect (button2, "clicked",G_CALLBACK (confirm_button), NULL);
        gtk_box_pack_start(GTK_BOX(vbox),button2,TRUE,TRUE,0);


        gtk_widget_show_all(window);
        gtk_main();
        



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




    ServerMainLoop();

	return 0;
} /* end of main */

