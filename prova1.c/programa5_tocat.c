#include <gtk/gtk.h>


void close(
GtkWidget *widget,gpointer *data)

{


gtk_main_quit();

}

void clicat(GtkWidget *widget,gpointer
*data)

{
	
	g_print("M'HAS PITJAT");
	
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
*window,*button, *window2, *button2;


    gtk_init (&argc, &argv);



window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL); 

    gtk_signal_connect
(GTK_OBJECT (window), "destroy",GTK_SIGNAL_FUNC (close), NULL);
    
    gtk_signal_connect 
(GTK_OBJECT (window2), "destroy",GTK_SIGNAL_FUNC (close), NULL);


button=gtk_button_new_with_label("Buto");
button2=gtk_button_new_with_label("boto2");


gtk_container_add(GTK_CONTAINER(window),button);
gtk_container_add(GTK_CONTAINER(window2),button2);


gtk_signal_connect(GTK_OBJECT(button),"clicked",

                        GTK_SIGNAL_FUNC(clicked),NULL);

gtk_signal_connect(GTK_OBJECT(button2),"clicat",
		
			GTK_SIGNAL_FUNC(clicat),NULL);

    gtk_widget_show(button);
gtk_widget_show(button2);

    gtk_widget_show(window);
	gtk_widget_show(window2);

    gtk_main ();


    return(0);

}
