#include <gtk/gtk.h>


void close(
GtkWidget *widget,gpointer *data)

{


gtk_main_quit();

}


void clicked(GtkWidget *widget,gpointer
*data)

{

        g_print("Button Clicked\n");

}

int main(
int   argc,char *argv[] )

{

    GtkWidget
*window,*button;


    gtk_init (&argc, &argv);



window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect
(GTK_OBJECT (window), "destroy",


GTK_SIGNAL_FUNC (close), NULL);



button=gtk_button_new_with_label("Button");


gtk_container_add(GTK_CONTAINER(window),button);


gtk_signal_connect(GTK_OBJECT(button),"clicked",

                       GTK_SIGNAL_FUNC(clicked),NULL);

    gtk_widget_show(button);


    gtk_widget_show(window);


    gtk_main ();


    return(0);

}