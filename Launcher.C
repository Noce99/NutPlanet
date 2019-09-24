#include <gtk/gtk.h>
#include "NutPlanet.h"
#include <unistd.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <thread>

GtkWidget *window;
int cosafare = -1;

static void RunSistemaSolare (GtkWidget *widget, gpointer   data){
	g_print ("Running Modalità Sistema Solare...\n");
	gtk_widget_destroy(window);
	gtk_main_quit();
	cosafare = 0;
}

static void RunCreativa (GtkWidget *widget, gpointer   data){
	g_print ("Running Modalità Creativa...\n");
	gtk_widget_destroy(window);
	gtk_main_quit();
	cosafare = 1;
}

static void RunCredits(GtkWidget *widget, gpointer   data){
	g_print ("Running Creadits...\n");
	cout << "----------------------------------------------" << endl;
	cout << "-------------------------___________----------" << endl;
	cout << "-|\\--------|-|---------|------|-------||------" << endl;
	cout << "-|-\\-------|-|---------|------|-------||------" << endl;
	cout << "-|--\\------|-|---------|------|-------||------" << endl;
	cout << "-|---\\-----|-|---------|------|-------||------" << endl;
	cout << "-|----\\----|-|---------|------|-------||------" << endl;
	cout << "-|-----\\---|-|---------|------|-------||------" << endl;
	cout << "-|------\\--|-|---------|------|-------||------" << endl;
	cout << "-|-------\\-|-|---------|------|-------||------" << endl;
	cout << "-|--------\\|-\\--------/-------|---------------" << endl;
	cout << "-|---------\\--\\______/--------|-------\\/------" << endl;
	cout << "----------------------------------------------" << endl;
}

static void RunSettings (GtkWidget *widget, gpointer   data){
	g_print ("Running Settings...\n");
}

static void Quit (GtkWidget *widget, gpointer   data){
	gtk_main_quit();
}

void MenuGenerale (){
  	GtkWidget *grid;
  	GtkWidget *button;
  	GtkWidget *label;
  	
  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_title(GTK_WINDOW(window), "Nut Planet");
  	  	
  	//gtk_widget_set_size_request (window, 500, 400);
  	grid = gtk_grid_new ();
  	g_signal_connect(window, "destroy", G_CALLBACK(Quit), NULL);
  	gtk_container_add (GTK_CONTAINER (window), grid);
  	
  	label = gtk_label_new (NULL);
  	gtk_label_set_markup (GTK_LABEL (label), "<span weight=\"ultrabold\">Nut Planet Main Menu</span>");
  	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
  	
  	button = gtk_button_new_with_label ("Modalità Sistema Solare");
    g_signal_connect (button, "clicked", G_CALLBACK (RunSistemaSolare), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);
    
    button = gtk_button_new_with_label ("Modalità Creativa");
    g_signal_connect (button, "clicked", G_CALLBACK (RunCreativa), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);
    
    button = gtk_button_new_with_label ("Quit");
    g_signal_connect (button, "clicked", G_CALLBACK (Quit), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1);
    
    button = gtk_button_new_with_label ("Credits");
    g_signal_connect (button, "clicked", G_CALLBACK (RunCredits), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 4, 1, 1);
    
    /*button = gtk_button_new_with_label ("Settings");
    g_signal_connect (button, "clicked", G_CALLBACK (RunSettings), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 3, 1, 1);*/
    
    int WW = 0, WH = 0;
  	gtk_widget_get_size_request (window,&WW, &WH);
  	cout << WW << " " << WH << endl;
	gtk_widget_set_size_request (window, WW, WH);
	
    gtk_widget_show_all (window);
    g_print ("Hi, this is Nut Planet!\nI hope you will like it!\n\n");
}

int main (int argc, char *argv[]) {
  	gtk_init(&argc, &argv);
  	glutInit(&argc, argv);
  	MenuGenerale();
	//settingup();
	
  	gtk_main();
  	switch (cosafare){
  		case -1:	return 0;
  		case  0:  	createFinestraSimulatore(true);break;
  		case  1:  	createFinestraSimulatore(false);break;
 	}
  	return 0;
}

/*Dati CreateMenuSimulatore(double *){
	return Dati();
}*/

/*void CreateInfoMenu(Dati info){
	GtkWidget *window;
  	GtkWidget *grid;
  	GtkWidget *button;
  	GtkWidget *label;
  	
  	
  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_title (GTK_WINDOW (window), "SSS - Info");
  	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  	grid = gtk_grid_new ();
  	gtk_container_add (GTK_CONTAINER (window), grid);
  	
  	label = gtk_label_new (NULL);
  	gtk_label_set_markup (GTK_LABEL (label), info.nome.c_str());
  	gtk_grid_attach (GTK_GRID (grid), label, 1, 0, 1, 1);
  	
  	button = gtk_button_new_with_label ("Modalità Sistema Solare");
    g_signal_connect (button, "clicked", G_CALLBACK (RunSistemaSolare), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 1, 1, 1);
    
    button = gtk_button_new_with_label ("Modalità Creativa");
    g_signal_connect (button, "clicked", G_CALLBACK (RunCreativa), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 2, 1, 1);
    
    button = gtk_button_new_with_label ("Quit");
    g_signal_connect (button, "clicked", G_CALLBACK (Quit), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1);
    
    button = gtk_button_new_with_label ("Credits");
    g_signal_connect (button, "clicked", G_CALLBACK (RunCredits), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1);
    
    button = gtk_button_new_with_label ("Settings");
    g_signal_connect (button, "clicked", G_CALLBACK (RunSettings), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 3, 1, 1);
    
    gtk_widget_show_all (window);
    sleep(10);
}*/
