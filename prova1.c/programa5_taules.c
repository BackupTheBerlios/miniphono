

#include <gtk/gtk.h>

/* La respuesta, que además se imprime en stdout. */
void callback (GtkWidget *widget, gpointer data)
{
    g_print ("Hello again - %s was pressed\n", (char *) data);
}

/* Con esta otra respuesta terminamos el programa. */
void delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
	g_print("Has clicat el boto de sortida\n"); 
	gtk_main_quit ();
}

int main (int argc, char *argv[])
{
    GtkWidget *ventana;
    GtkWidget *boton;
    GtkWidget *table;

    gtk_init (&argc, &argv);

    ventana = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW (ventana), "Table");

    gtk_signal_connect (GTK_OBJECT (ventana), "delete_event",
                        GTK_SIGNAL_FUNC (delete_event), NULL);

    gtk_container_border_width (GTK_CONTAINER (ventana), 20);

    table = gtk_table_new (2, 2, TRUE);

    gtk_container_add (GTK_CONTAINER (ventana), table);

    boton = gtk_button_new_with_label ("botón 1");

    gtk_signal_connect (GTK_OBJECT (boton), "clicked",
              GTK_SIGNAL_FUNC (callback), (gpointer) "botón 1");

    gtk_table_attach_defaults (GTK_TABLE(table), boton, 0, 1, 0, 1);

    gtk_widget_show (boton);

    boton = gtk_button_new_with_label ("botón 2");

    gtk_signal_connect (GTK_OBJECT (boton), "clicked",
              GTK_SIGNAL_FUNC (callback), (gpointer) "botón 2");
    gtk_table_attach_defaults (GTK_TABLE(table), boton, 1, 2, 0, 1);

    gtk_widget_show (boton);

    boton = gtk_button_new_with_label ("Quit");

    gtk_signal_connect (GTK_OBJECT (boton), "clicked",
                        GTK_SIGNAL_FUNC (delete_event), NULL);
    gtk_table_attach_defaults (GTK_TABLE(table), boton, 0, 2, 1, 2);

    gtk_widget_show (boton);

    gtk_widget_show (table);
    gtk_widget_show (ventana);

    gtk_main ();

    return 0;
}
