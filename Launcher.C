#include "NutPlanet.h"

GtkWidget *window;
int cosafare = -1;

static void RunSistemaSolare (GtkWidget *widget, gpointer   data){
	std::cout <<  ("Running Modalità Sistema Solare...\n");
	gtk_widget_destroy(window);
	gtk_main_quit();
	cosafare = 0;
}

static void RunCreativa (GtkWidget *widget, gpointer   data){
	std::cout <<  ("Running Modalità Creativa...\n");
	gtk_widget_destroy(window);
	gtk_main_quit();
	cosafare = 1;
}

static void RunCredits(GtkWidget *widget, gpointer   data){
	std::cout <<  ("Running Creadits...\n");
	std::cout << "This program was made by:" << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "-------------------------___________----------" << std::endl;
	std::cout << "-|\\--------|-|---------|------|-------||------" << std::endl;
	std::cout << "-|-\\-------|-|---------|------|-------||------" << std::endl;
	std::cout << "-|--\\------|-|---------|------|-------||------" << std::endl;
	std::cout << "-|---\\-----|-|---------|------|-------||------" << std::endl;
	std::cout << "-|----\\----|-|---------|------|-------||------" << std::endl;
	std::cout << "-|-----\\---|-|---------|------|-------||------" << std::endl;
	std::cout << "-|------\\--|-|---------|------|-------||------" << std::endl;
	std::cout << "-|-------\\-|-|---------|------|-------||------" << std::endl;
	std::cout << "-|--------\\|-\\--------/-------|---------------" << std::endl;
	std::cout << "-|---------\\--\\______/--------|-------\\/------" << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
}

static void Quit (GtkWidget *widget, gpointer   data){
	gtk_main_quit();
}

void MenuGenerale (){
  	GtkWidget *button;
  	GtkLabel *label;
  	
  	GtkBuilder *builder;
	GError *error = NULL;
	
	builder = gtk_builder_new ();
  	if (gtk_builder_add_from_file (builder, "./Builder/BuilderLauncher.ui", &error) == 0){
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
    }
    
  	window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_title(GTK_WINDOW(window), "Nut Planet");  	
  	g_signal_connect(window, "destroy", G_CALLBACK(Quit), NULL);
  	
  	label = GTK_LABEL(gtk_builder_get_object (builder, "ProgramNameLabel"));
  	gtk_widget_set_size_request (GTK_WIDGET(label), 200, 50);
  	gtk_label_set_markup (label, "<span weight=\"ultrabold\">Nut Planet Main Menu</span>");
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "SolarSystemModButton"));
    g_signal_connect (button, "clicked", G_CALLBACK (RunSistemaSolare), NULL);
    
    button = GTK_WIDGET(gtk_builder_get_object (builder, "CreativityModButton"));
    g_signal_connect (button, "clicked", G_CALLBACK (RunCreativa), NULL);
    
    button = GTK_WIDGET(gtk_builder_get_object (builder, "QuitButton"));
    g_signal_connect (button, "clicked", G_CALLBACK (Quit), NULL);
    
    button = GTK_WIDGET(gtk_builder_get_object (builder, "CreditsButton"));
    g_signal_connect (button, "clicked", G_CALLBACK (RunCredits), NULL);
    	
    gtk_widget_show_all (window);
    std::cout <<  ("Hi, this is Nut Planet!\nI hope you will like it!\n\n");
}

int main (int argc, char *argv[]) {
  	gtk_init(&argc, &argv);
  	glutInit(&argc, argv);
  	MenuGenerale();
	
  	gtk_main();
  	switch (cosafare){
  		case -1:	return 0;
  		case  0:  	createFinestraSimulatore(true);break;
  		case  1:  	createFinestraSimulatore(false);break;
 	}
  	return 0;
}
