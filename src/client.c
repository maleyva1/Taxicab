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
/*#include <cairo.h>*/

/* Setups up the client networking */
/*void setupClient(int *argc, char *argv[]);*/

/* Prints how to use the program*/
void howTo();

gchar *text1,*text2,*text3,*text4;
GtkWidget *status_bar;

static void push_item( GtkWidget *widget,
                       gpointer   data )
{
        gchar *buff;
        static int count = 1;
        buff = g_strdup_printf ("$ %d",++count);
        gtk_statusbar_push (GTK_STATUSBAR (status_bar), GPOINTER_TO_INT (data), buff);
        g_free (buff);
}
	
void enter_callback(GtkWidget *entry,const gchar *entrytext)
{
        /*you can just use this function to get the text from the gtk*/
        entrytext = gtk_entry_get_text (GTK_ENTRY (entry));
        printf("%s",entrytext);
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
        printf("%d",data);
    } else {

        /* If control reaches here, the toggle button is up */
        data = 0;
    }
}

confirm_button(GtkWidget *widget,gint data)
{
	printf("Cab Confirmed!\n");
}

int main(int argc, char *argv[])
{
    /* GTK init */
	gtk_init(&argc,&argv);

    /* Networking stuff */
	int xpick,ypick,xdest,ydest,sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /* GUI stuff */
    GtkWidget *fixed;
    char buffer[256];
    int case1=0;
    int w,h;

    /*printf("\nPlease enter the X value you would like to be picked up at:");
    scanf("%d",&xpick);
    printf("PLease enter the Y value you would like to be piecked up at:");
    scanf("%d",&ypick);
    printf("Please enter the X value of your final destination:");
    scanf("%d",&xdest);
    printf("Please enter the Y value of your final destination:");
    scanf("%d",&ydest);
    sprintf(buffer,"REQUEST_TAXI %d %d TO %d %d",xpick,ypick,xdest,ydest);
    printf("\nPLEASE CONFIRM YOUR TAXI REQUEST NEXT. YOUR CONFIRMATION NUMBER IS GIVEN IN THE NEXT LINE AT THE END\n");

    printf("\nPlease enter your confirmation number to confirm taxi request: ");
    scanf("%d",&case1);
    sprintf(buffer,"CONFIRM #%d",case1);

    printf("\nPlease enter the taxi number: ");
    scanf("%d",&case1);
    sprintf(buffer,"REQUEST_POSITION Taxi%d",case1);*/

    close(sockfd);

    /*ghar client_input;*/

/*add debug flags*/
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
        return 0;
}


/*void setupClient(int *argc, char *argv[])
{
    if (argc < 3)
    {
        perror("Must enter: %s 'hostname' '#port' in the command line\n", argv[0]);
        return 0;
    }
    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        peror("ERROR opening socket");
        return 0;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        perror("ERROR, no such host\n");
        return 0;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    bzero(buffer,256);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
      perror("ERROR connecting");
      return 0;
    }
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return 0;
    }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        return 0;
    }
    printf("%s\n",buffer);
}*/

void howTo()
{
    /* Print how to use the program*/
    printf("\nTo request a taxi, enter 1\nTo confirm a taxi request, enter 2\n");
    printf("To request the location of a taxi, enter 3\nAll locations are currently represented by a x and y coordinate system\n");
    printf("All taxi start from the location (10,10)\n");
    /* */
}
