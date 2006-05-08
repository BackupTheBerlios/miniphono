#include <gtk/gtk.h>


void close(
GtkWidget *widget,gpointer *data)

{


gtk_main_quit();

}


int main( int   argc,char *argv[]
)

{

    GtkWidget *window;


    gtk_init (&argc,
&argv);


    window = gtk_window_new
(GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect (GTK_OBJECT (window),
"destroy",

                        GTK_SIGNAL_FUNC (close),
NULL);

    gtk_widget_show  (window);


    gtk_main
();


    return(0);

}