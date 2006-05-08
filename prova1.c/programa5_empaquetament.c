

#include <stdio.h>
#include "gtk/gtk.h"

void
delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit ();
}

/* Hacemos una hbox llena de etiquetas de botón. Los argumentos
 * para las variables que estamos interesados son pasados a esta
 * función. No mostramos la caja, pero hacemos todo lo que
 * queremos. */
GtkWidget *make_box (gint homogeneous, gint spacing,
     gint expand, gint fill, gint padding) 
{
    GtkWidget *box;
    GtkWidget *boton;
    char padstr[80];
    
    /* creamos una nueva caja con los argumentos homogeneous y
     * spacing */
    box = gtk_hbox_new (homogeneous, spacing);
    
    /* crear una serie de botones */
    boton = gtk_button_new_with_label ("gtk_box_pack");
    gtk_box_pack_start (GTK_BOX (box), boton, expand, fill, padding);
    gtk_widget_show (boton);
    
    boton = gtk_button_new_with_label ("(box,");
    gtk_box_pack_start (GTK_BOX (box), boton, expand, fill, padding);
    gtk_widget_show (boton);
    
    boton = gtk_button_new_with_label ("boton,");
    gtk_box_pack_start (GTK_BOX (box), boton, expand, fill, padding);
    gtk_widget_show (boton);
    
    /* Este botón llevará por etiqueta el valor de expand */
    if (expand == TRUE)
    boton = gtk_button_new_with_label ("TRUE,");
    else
    boton = gtk_button_new_with_label ("FALSE,");
    
    gtk_box_pack_start (GTK_BOX (box), boton, expand, fill, padding);
    gtk_widget_show (boton);
    
    /* Este es el mismo caso que el de arriba, pero más compacto */
    boton = gtk_button_new_with_label (fill ? "TRUE," : "FALSE,");
    gtk_box_pack_start (GTK_BOX (box), boton, expand, fill, padding);
    gtk_widget_show (boton);
    
    sprintf (padstr, "%d);", padding);
    
    boton = gtk_button_new_with_label (padstr);
    gtk_box_pack_start (GTK_BOX (box), boton, expand, fill, padding);
    gtk_widget_show (boton);
    
    return box;
}

int
main (int argc, char *argv[])
{
    GtkWidget *ventana;
    GtkWidget *boton;
    GtkWidget *caja1;
    GtkWidget *caja2;
    GtkWidget *separator;
    GtkWidget *etiqueta;
    GtkWidget *quitbox;
    int which;
    
    /* ¡No olvidar la siguiente llamada! */
    gtk_init (&argc, &argv);
    
    if (argc != 2) {
fprintf (stderr, "usage: packbox num, where num is 1, 2, or 3.\n");
        /* hacemos limpieza en GTK y devolvemos el valor de 1 */
gtk_exit (1);
    }
    
    which = atoi (argv[1]);

    /* Creamos la ventana */
    ventana = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* Siempre hay que conectar la señal de destrucción con la
     * ventana principal. Esto es muy importante para que el
     * comportamiento de la ventana sea intuitivo. */
    gtk_signal_connect (GTK_OBJECT (ventana), "delete_event",
GTK_SIGNAL_FUNC (delete_event), NULL);
    gtk_container_border_width (GTK_CONTAINER (ventana), 10);
    
    /* Creamos una caja vertical donde empaquetaremos las cajas
     * horizontales. Así podemos apilar las cajas horizontales
     * llenas con botones una encima de las otras. */
    caja1 = gtk_vbox_new (FALSE, 0);
    
    /* Aclaramos cúal es el ejemplo a mostrar. Se corresponde con
     * las imágenes anteriores. */
    switch (which) {
    case 1:
      /* creamos una nueva etiqueta. */
etiqueta = gtk_label_new ("gtk_hbox_new (FALSE, 0);");

      /* Alineamos la etiqueta a la izquierda. Está función
       * será discutida en detalle en la sección de los
       * atributos de los widgets. */
gtk_misc_set_alignment (GTK_MISC (etiqueta), 0, 0);

      /* Empaquetamos la etiqueta en la caja vertical (vbox
       * caja1). Siempre hay que recordar que los widgets añadidos a
       * una vbox serán empaquetados uno encimo de otro. */
gtk_box_pack_start (GTK_BOX (caja1), etiqueta, FALSE, FALSE, 0);

      /* mostramos la etiqueta. */
gtk_widget_show (etiqueta);

      /* llamada a la función que hace las cajas. Los argumentos
       * son homogenous = FALSE, expand = FALSE, fill = FALSE,
       * padding = 0 */
caja2 = make_box (FALSE, 0, FALSE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

/* Llamad a la función para hacer cajas -
         * homogeneous = FALSE, spacing = 0, expand = FALSE,
         * fill = FALSE, padding = 0 */
caja2 = make_box (FALSE, 0, TRUE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (FALSE, 0, TRUE, TRUE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);


        /* creamos un separador. Más tarde aprenderemos más cosas
         * sobre ellos, pero son bastante sencillos. */
separator = gtk_hseparator_new ();

        /* empaquetamos el separador el la vbox. Los widgets serán
         * apilados verticalmente. */
gtk_box_pack_start (GTK_BOX (caja1), separator, FALSE, TRUE, 5);
gtk_widget_show (separator);

        /* creamos una nueva etiqueta y la mostramos */
etiqueta = gtk_label_new ("gtk_hbox_new (TRUE, 0);");
gtk_misc_set_alignment (GTK_MISC (etiqueta), 0, 0);
gtk_box_pack_start (GTK_BOX (caja1), etiqueta, FALSE, FALSE, 0);
gtk_widget_show (etiqueta);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (TRUE, 0, TRUE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (TRUE, 0, TRUE, TRUE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

/* un nuevo separador */
separator = gtk_hseparator_new ();
/* Los tres últimos argumentos son: expand, fill, padding. */
gtk_box_pack_start (GTK_BOX (caja1), separator, FALSE, TRUE, 5);
gtk_widget_show (separator);

break;

    case 2:

/* Nueva etiqueta */
etiqueta = gtk_label_new ("gtk_hbox_new (FALSE, 10);");
gtk_misc_set_alignment (GTK_MISC (etiqueta), 0, 0);
gtk_box_pack_start (GTK_BOX (caja1), etiqueta, FALSE, FALSE, 0);
gtk_widget_show (etiqueta);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (FALSE, 10, TRUE, FALSE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (FALSE, 10, TRUE, TRUE, 0);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

separator = gtk_hseparator_new ();
/* Los argumentos son: expand, fill, padding. */
gtk_box_pack_start (GTK_BOX (caja1), separator, FALSE, TRUE, 5);
gtk_widget_show (separator);

etiqueta = gtk_label_new ("gtk_hbox_new (FALSE, 0);");
gtk_misc_set_alignment (GTK_MISC (etiqueta), 0, 0);
gtk_box_pack_start (GTK_BOX (caja1), etiqueta, FALSE, FALSE, 0);
gtk_widget_show (etiqueta);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (FALSE, 0, TRUE, FALSE, 10);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

/* Los argumentos son: homogeneous, spacing, expand, fill,
         * padding */
caja2 = make_box (FALSE, 0, TRUE, TRUE, 10);
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);

separator = gtk_hseparator_new ();
/* Los argumentos son: expand, fill, padding. */
gtk_box_pack_start (GTK_BOX (caja1), separator, FALSE, TRUE, 5);
gtk_widget_show (separator);
break;
    
    case 3:

        /* Con esto demostramos como hay que usar gtk_box_pack_end ()
         * para conseguir que los 
        widgets esten alineados a la izquierda. */
caja2 = make_box (FALSE, 0, FALSE, FALSE, 0);

        /* la última etiqueta*/ 
etiqueta = gtk_label_new ("end");

        /* la empaquetamos usando gtk_box_pack_end(), por lo que se
         * sitúa en el lado derecho de la hbox.*/
gtk_box_pack_end (GTK_BOX (caja2), etiqueta, FALSE, FALSE, 0);

        /* mostrar la etiqueta */
gtk_widget_show (etiqueta);


        /* empaquetamos caja2 en caja1 */
gtk_box_pack_start (GTK_BOX (caja1), caja2, FALSE, FALSE, 0);
gtk_widget_show (caja2);


        /* el separador para la parte de abajo. */
separator = gtk_hseparator_new ();

/* Así se determina el tamaño del separador a 400 pixels
         * de largo por 5 de alto. La hbox también tendrá 400
         * pixels de largo y la etiqueta "end" estará separada de 
         * las demás etiquetas en la hbox. Si no establecemos estos
         * parámetros todos los widgets en la hbox serán
         * empaquetados tan juntos como se pueda.*/ 
gtk_widget_set_usize (separator, 400, 5);

/* Empaquetamos el separador creado al principio de main() en
         * la vbox (caja1). */
gtk_box_pack_start (GTK_BOX (caja1), separator, FALSE, TRUE, 5);
gtk_widget_show (separator);    
    }
    
    /* Creamos otra hbox... recordar que podemos crear tantas como
     * queramos. */
    quitbox = gtk_hbox_new (FALSE, 0);
    
    /* El botón de salida. */
    boton = gtk_button_new_with_label ("Quit");
    
    /* Establecemos la señal de destrucción de la ventana.
     * Recuerde que emitirá la señal de "destroy" que a su vez
     * será procesada por el controlador de señales, tal y como
     * ya hemos visto. */
  
    gtk_signal_connect_object (GTK_OBJECT (boton), "clicked",
       GTK_SIGNAL_FUNC (gtk_main_quit),
       GTK_OBJECT (ventana));
    /* Empaquetamos el botón en la caja de salida (quitbox).
     * los tres últimos argumentos de gtk_box_pack_start
     * son:expand, fill, padding. */
    gtk_box_pack_start (GTK_BOX (caja1), quitbox, FALSE, FALSE, 0);
    
    /* empaquetamos la vbox (caja1) que ya contiene todos los widgets
     * en la ventana principal. */
    gtk_container_add (GTK_CONTAINER (ventana), caja1);
    
    /* mostramos todo aquello que faltaba por mostrar */
    gtk_widget_show (boton);
    gtk_widget_show (quitbox);
    
    gtk_widget_show (caja1);

    /* Si mostramos la ventana lo último todo aparece de golpe. */
    gtk_widget_show (ventana);
  
    /* por supuesto tenemos una función main. */
    gtk_main ();

    /* El programa llega aquí cuando se llama a gtk_main_quit(),
     * pero no cuando se llama a gtk_exit(). */
    return 0;
}
