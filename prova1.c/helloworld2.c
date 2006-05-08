

#include <gtk/gtk.h>

/* Nuestra respuesta mejorada. Los argumentos de la función se
 * imprimen en el stdout.*/
void callback (GtkWidget *widget, gpointer data)
{
    g_print ("Hello again - %s was pressed\n", (char *) data);
}

/* otra respuesta*/
void delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit ();
}

int main (int argc, char *argv[])
{
    /* GtkWidget es el tipo de almacenamiento usado para los wigtes*/
    GtkWidget *ventana;
    GtkWidget *boton;
    GtkWidget *caja1;
    
    /* Esta llamada está presente en todas las aplicaciones basadas
     * en GTK. Los argumentos introducidos a la aplicación*/
    gtk_init (&argc, &argv);

    /* creamos una nueva ventana*/
    ventana = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    /* Esta función es nueva, pone como título de la ventana
     * "¡Hola botones!"*/ 

    gtk_window_set_title (GTK_WINDOW (ventana), "¡Hola botones!");

    /* Establecemos el controlador para la llamada delete_event que
     * termina la aplicación inmediatamente. */

    gtk_signal_connect (GTK_OBJECT (ventana), "delete_event",
GTK_SIGNAL_FUNC (delete_event), NULL);

    
    /* Establecemos el ancho del borde de la ventana.*/
    gtk_container_border_width (GTK_CONTAINER (ventana), 10);

    /* Creamos una caja donde empaquetaremos los widgets. El
     * procedimiento de empaquetamiento se describe en detalle en la
     * sección correspondiente. La caja no se ve realmente, sólo
     * sirve para introducir los widgets. */
    caja1 = gtk_hbox_new(FALSE, 0);

    /* ponemos la caja en la ventana principal */
    gtk_container_add (GTK_CONTAINER (ventana), caja1);

    /* Creamos un nuevo botón con la etiqueta "Botón 1". */
    boton = gtk_button_new_with_label ("Botón 1");
    
    /* Cada vez que el botón sea pulsado llamamos a la función
     * "callback" con un puntero a "botón 1" como argumento. */
    gtk_signal_connect (GTK_OBJECT (boton), "clicked",
GTK_SIGNAL_FUNC (callback), (gpointer) "botón 1");
    
    /* En lugar de gtk_container_add empaquetamos el botón en la
     * caja invisible, que a su vez ha sido empaquetado en la
     * ventana. */
    gtk_box_pack_start(GTK_BOX(caja1), boton, TRUE, TRUE, 0);

    /* Siempre se debe realizar este paso. Sirve para decirle a GTK
     * que los preparativos del botón ya se han finalizado y que
     * por tanto puede ser mostrado. */
    gtk_widget_show(boton);

    /* hacemos lo mismo para crear un segundo botón. */
    boton = gtk_button_new_with_label ("Botón 2");

    /* Llamamos a la misma función de respuesta pero con diferente
     * argumento: un puntero a "botón 2". */
    gtk_signal_connect (GTK_OBJECT (boton), "clicked",
GTK_SIGNAL_FUNC (callback), (gpointer) "botón 2");

    gtk_box_pack_start(GTK_BOX(caja1), boton, TRUE, TRUE, 0);

    /* El orden en que mostramos los botones no es realmente
     * importante, pero se recomienda mostrar la ventana la última
     * para que todo aparezca de golpe. */
    gtk_widget_show(boton);

    gtk_widget_show(caja1);

    gtk_widget_show (ventana);

    /* Esperamos en gtk_main a que comience el espectáculo.*/
    gtk_main ();

    return 0;
}
