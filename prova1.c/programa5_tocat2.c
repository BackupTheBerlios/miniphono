#include <gtk/gtk.h>

void close(GtkWidget *widget,gpointer *data)

{
	gtk_main_quit();
}
void txt(
GtkWidget *widget,gpointer *data)

{
	
	g_print("has pitjar el boto de tancar\n");
	destroy(data);
	/*gtk_main_quit();*/
}

void mis2(GtkWidget *widget,gpointer
*data)

{
	g_print("HAS PITJAT \n");
	
}


void clicked(GtkWidget *widget,gpointer
*data)

{
	g_print("M'HAS PITJAT el boto d'escriure missatge\n");
	/*gtk_main_quit();*/

}

int main(
int   argc,char *argv[] )

{
GtkWidget  *window2;
GtkWidget *buto2;
GtkWidget *window;
GtkWidget *buto;

gtk_init (&argc, &argv);
/* s'han de matar les 2 finestres o crear un panell i ajuntar-les allà*/
window = gtk_window_new (GTK_WINDOW_TOPLEVEL); 
window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL); 

gtk_signal_connect (GTK_OBJECT (window2), "destroy",GTK_SIGNAL_FUNC (close),NULL);
    
gtk_signal_connect (GTK_OBJECT (window), "destroy",GTK_SIGNAL_FUNC (close),NULL);

buto2=gtk_button_new_with_label("surt");
buto=gtk_button_new_with_label("escriu");


gtk_container_add(GTK_CONTAINER(window2),buto2);
gtk_container_add(GTK_CONTAINER(window),buto);

gtk_signal_connect(GTK_OBJECT(buto2),GTK_SIGNAL_FUNC(txt),G_CALLBACK(close));
gtk_signal_connect(GTK_OBJECT(buto),"clicked",GTK_SIGNAL_FUNC(clicked),NULL);

gtk_widget_show(buto2);
gtk_widget_show(buto);

gtk_widget_show(window2);
gtk_widget_show(window);

gtk_main ();

return(0);

}
