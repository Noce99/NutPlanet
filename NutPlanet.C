#include "NutPlanet.h"

//Costanti
constexpr double G = 6.67408e-11; //Costante Gravitazionale
double T = 1000; //Delta T ad Ogni Tick
constexpr double M = 1.989e30; //Massa Sole
bool stop = false; //Fa ferare il simulatore se TRUE
double WIDTH = 1366, HEIGHT = 768;


//Nuove Variabili Posizione
double CCX = 0; //Camera Centro X
double CCY = 0; //Camera Centro Y
double MPUGL = 44.59631496e11; //Metri per Unità GL
double WP = 0; //Larghezza in Pixel
double HP = 0; //Altezza in Pixel
double rapp = 1;
double raggioSole = 0.02;
double raggioPianeti = 0.01;

int selected = -1;

//Vari campi il cui accesso è necessario da tutto il programma
GtkLabel *CCXL;
GtkLabel *CCYL;
GtkLabel *PlanetInfoName;
GtkLabel *PlanetInfoMass;
GtkLabel *PlanetInfoRadius;
GtkWidget *PlanetInfoWindow;
GtkWidget *NewPlanetColor;
GtkTextBuffer *NewPlanetNameBuff = gtk_text_buffer_new(NULL);
GtkTextBuffer *NewPlanetMassBuff = gtk_text_buffer_new(NULL);
GtkTextBuffer *NewPlanetRadiusBuff = gtk_text_buffer_new(NULL);
GtkTextBuffer *NewPlanetXPositionBuff = gtk_text_buffer_new(NULL);
GtkTextBuffer *NewPlanetYPositionBuff = gtk_text_buffer_new(NULL);
GtkTextBuffer *NewPlanetXSpeedBuff = gtk_text_buffer_new(NULL);
GtkTextBuffer *NewPlanetYSpeedBuff = gtk_text_buffer_new(NULL);

//Funzioni
static void SU (GtkWidget *, gpointer);
static void GIU (GtkWidget *, gpointer);
static void SX (GtkWidget *, gpointer);
static void DX (GtkWidget *, gpointer);
static void Home (GtkWidget *, gpointer);
static void ZP (GtkWidget *, gpointer);
static void ZM (GtkWidget *, gpointer);
static void ChangeTime (GtkRange *, GtkScrollType, gdouble, gpointer);
static void ChangeRaggioPianeti (GtkRange *, GtkScrollType, gdouble, gpointer);
static void ChangeRaggioSole (GtkRange *, GtkScrollType, gdouble, gpointer);
static void Previus (GtkWidget *, gpointer);
static void Next (GtkWidget *, gpointer);
static void NewPlanet (GtkWidget *, gpointer);
static void Delete(GtkWidget *, gpointer);
void drawCircol(double, double, double, double, double, double ,double, bool);
void drawPointer(double, double, double, double, double);
gboolean motore(gpointer);
void render(int);
void draw();
std::string doubleToString(double);

//Oggetti
class Vettore;
class Pianeta;
struct Esplosione;
std::vector<Pianeta> P;//Vector di Pianeti
std::vector<int> Cestino;//Qui vengono messi i pianeti che vanno distrutti
std::vector<Esplosione> E;//Qui vengono memorizzate le esplosioni!
bool sistema_solare = true;//Differenzia Sistema Solare e Modalità Creativa

//Conversioni
double UTGLX(double x){ //Cordinate to Cordinate OpenGl for x
	return (x-CCX)/MPUGL;
}
double UTGLY(double y){ //Cordinate to Cordinate OpenGl for y
	return (y-CCY)/MPUGL;
}




//INIZIO VARIE DEFINIZINIONI CLASSI STRUCT




//Definizione di Vettore
class Vettore{
	double x;
	double y;
	public:
	Vettore(double x, double y): x(x), y(y){}
	Vettore(){
		x = 0;
		y = 0;
	}
	double getX(){
		return x;
	}
	double getY(){
		return y;
	}
	void setX(double xx){
		x = xx;
	}
	void setY(double yy){
		y = yy;
	}
	/*void getInfo(){
		std::cout << "X: " << x << " Y: " << y;
	}*/
	Vettore operator+(Vettore v){
		return Vettore(x+v.getX(), y+v.getY());
	}
};

//Definizine di Pianeta
class Pianeta{
	std::string nome;
	double massa;
	double raggio;
	Vettore posizione;
	Vettore velocita;
	Vettore accelerazione;
	Vettore forza;
	double r;
	double g;
	double b;
	public:
	Pianeta(std::string nome, double massa, double raggio, Vettore posizione, Vettore velocita, int color=1, double r=1, double g=1, double b=1):
	 nome(nome), massa(massa), raggio(raggio), posizione(posizione), velocita(velocita), r(r), g(g), b(b){
		accelerazione = Vettore(0,0);
		forza = Vettore(0,0);
	}
	std::string getNome(){
		return nome;
	}
	double getMassa(){
		return massa;
	}
	double getRaggio(){
		return raggio;
	}
	Vettore getPosizione(){
		return posizione;
	}
	Vettore getVelocita(){
		return velocita;
	}
	Vettore getAccelerazione(){
		return accelerazione;
	}
	double getR(){
		return r;
	}
	double getG(){
		return g;
	}
	double getB(){
		return b;
	}
	Vettore getForza(){
		return forza;
	}
	/*void getInfo(){
		std::cout << "Nome: " << nome << "  Massa: " << massa << "  Posizione: ";
		posizione.getInfo();
		std::cout << std::flush;
		std::cout << " Velocita': ";
		velocita.getInfo();
		std::cout << std::flush;
		std::cout << " Accellerazione: ";
		accelerazione.getInfo();
		std::cout << std::flush;
		std::cout << " Forza: ";
		accelerazione.getInfo();
		std::cout << std::flush;
		std::cout << std::endl;
	}*/
	void cancForza(){
		forza.setX(0);
		forza.setY(0);
	}
	Vettore temp = Vettore(0,0);
	void addForza(double f, Vettore v=Vettore(0,0)){
		temp.setX(f*(posizione.getX()-v.getX()));
		temp.setY(f*(posizione.getY()-v.getY()));
		forza=forza+temp;
	}
	void calcDati(){
		accelerazione.setX(-forza.getX()/massa);
		accelerazione.setY(-forza.getY()/massa);
		velocita.setX(velocita.getX() + T*accelerazione.getX());
		velocita.setY(velocita.getY() + T*accelerazione.getY());
		posizione.setX(posizione.getX() + T*velocita.getX());
		posizione.setY(posizione.getY() + T*velocita.getY());
	}
};

//Definizione Esplosione
struct Esplosione{
	Vettore posizione;
	int vita = 400;
	Esplosione(Vettore posizione):posizione(posizione){}
	Vettore getPosizione(){
		return posizione;
	}
};




//FINE VARIE DEFINIZINIONI CLASSI STRUCT


void createFinestraSimulatore(bool ss){
	int WW = 0, WH = 0;
	
	//GdkRectangle workarea = {0};
	//gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()),&workarea);
	
	
	//OPENGL INIZIALIZZAZIONI
		
  	glutInitDisplayMode(GLUT_RGB);
  	glutInitWindowSize(WIDTH-436, HEIGHT);
  	glutInitWindowPosition(0, 0);
  	glutCreateWindow("Nut Planet");
  	glutDisplayFunc(draw);
  	glutTimerFunc(25, render, 0);
  	
  	//-------------------------------
  	
  	//DISTACCO DEL MAIN LOOP DI OPENLGL
  	
  	std::thread motoreAutonomo;
  	motoreAutonomo = std::thread(glutMainLoop);
	motoreAutonomo.detach();
  	
  	//-------------------------------
  	
  	
  	//INIZIALIZZAZIONE VARIABILI UTILI AI MENU
  	
	GtkBuilder *builder;
	GError *error = NULL;
	GtkWidget *window;
	GtkWidget *button;
	
	//-------------------------------
	
	//MENU COMANDI
	
	builder = gtk_builder_new ();
  	if (gtk_builder_add_from_file (builder, "./Builder/BuilderComandi.ui", &error) == 0){
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
    }
	
	window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	gtk_window_set_deletable (GTK_WINDOW(window), false);
	gtk_window_set_title(GTK_WINDOW(window), "Movimento");
	gtk_window_move(GTK_WINDOW(window), WIDTH-436, 0);
	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonZP"));
  	g_signal_connect (button, "clicked", G_CALLBACK (ZP), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonZM"));
  	g_signal_connect (button, "clicked", G_CALLBACK (ZM), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonSX"));
  	g_signal_connect (button, "clicked", G_CALLBACK (SX), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonUP"));
  	g_signal_connect (button, "clicked", G_CALLBACK (SU), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonDX"));
  	g_signal_connect (button, "clicked", G_CALLBACK (DX), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonDW"));
  	g_signal_connect (button, "clicked", G_CALLBACK (GIU), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "buttonBH"));
  	g_signal_connect (button, "clicked", G_CALLBACK (Home), NULL);
  	
  	gtk_widget_show_all (window);
  	
  	//-------------------------------
  	
  	//MENU SETTINGS
  	
	builder = gtk_builder_new ();
  	if (gtk_builder_add_from_file (builder, "./Builder/BuilderSettings.ui", &error) == 0){
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
    }
	
	GtkScale *scale;
	
	window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	gtk_window_set_deletable (GTK_WINDOW(window), false);
	gtk_window_set_title(GTK_WINDOW(window), "Settings");
	gtk_window_move(GTK_WINDOW(window), WIDTH-436, 131);
	
  	scale = GTK_SCALE(gtk_builder_get_object (builder, "Tempo"));
  	gtk_range_set_range (GTK_RANGE(scale), 100, 100000);
  	gtk_widget_set_size_request (GTK_WIDGET(scale), 436, 3);
  	gtk_scale_set_draw_value (scale,false);
  	g_signal_connect (scale, "change-value", G_CALLBACK (ChangeTime), NULL);
  	
  	scale = GTK_SCALE(gtk_builder_get_object (builder, "RaggioPianeti"));
  	gtk_range_set_range (GTK_RANGE(scale), 0.005, 0.2);
  	gtk_widget_set_size_request (GTK_WIDGET(scale), 436, 3);
  	gtk_range_set_round_digits (GTK_RANGE(scale), 10);
  	gtk_scale_set_draw_value (scale,false);
  	g_signal_connect (scale, "change-value", G_CALLBACK (ChangeRaggioPianeti), NULL);
  	
  	scale = GTK_SCALE(gtk_builder_get_object (builder, "RaggioSole"));
  	gtk_range_set_round_digits (GTK_RANGE(scale), 10);
  	gtk_range_set_range (GTK_RANGE(scale), 0.01, 0.4);
  	gtk_widget_set_size_request (GTK_WIDGET(scale), 436, 3);
  	gtk_scale_set_draw_value (scale,false);
  	g_signal_connect (scale, "change-value", G_CALLBACK (ChangeRaggioSole), NULL);
  	
  	gtk_widget_show_all (window);
	
	//-------------------------------
	
	//MENU INFOUNIVERSO
	
	builder = gtk_builder_new ();
  	if (gtk_builder_add_from_file (builder, "./Builder/BuilderInfoU.ui", &error) == 0){
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
    }
	
	window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	gtk_window_set_deletable (GTK_WINDOW(window), false);
	gtk_window_set_title(GTK_WINDOW(window), "Univers Info");
	gtk_window_move(GTK_WINDOW(window), WIDTH-436, 326);
	gtk_widget_get_size_request (GTK_WIDGET(window),&WW, &WH);
	gtk_widget_set_size_request (window, 436, WH);
	
	CCXL = GTK_LABEL(gtk_builder_get_object (builder, "PosxLabel"));
	CCYL = GTK_LABEL(gtk_builder_get_object (builder, "PosyLabel"));
	
	gtk_widget_show_all (window);
	
	//-------------------------------
	
	//MENU INFOPIANETI
	
	builder = gtk_builder_new ();
  	if (gtk_builder_add_from_file (builder, "./Builder/BuilderInfoP.ui", &error) == 0){
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
    }
	
	PlanetInfoWindow = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	gtk_window_set_deletable (GTK_WINDOW(PlanetInfoWindow), false);
	gtk_window_set_title(GTK_WINDOW(PlanetInfoWindow), "Planet Info");
	gtk_window_move(GTK_WINDOW(PlanetInfoWindow), WIDTH-436, 419);
	gtk_widget_get_size_request (GTK_WIDGET(PlanetInfoWindow),&WW, &WH);
	gtk_widget_set_size_request (PlanetInfoWindow, 436, WH);
	
	button = GTK_WIDGET(gtk_builder_get_object (builder, "PreviusButton"));
  	g_signal_connect (button, "clicked", G_CALLBACK (Previus), NULL);
  	
  	button = GTK_WIDGET(gtk_builder_get_object (builder, "NextButton"));
  	g_signal_connect (button, "clicked", G_CALLBACK (Next), NULL);
  	
	PlanetInfoName = GTK_LABEL(gtk_builder_get_object (builder, "NameLabel"));
	PlanetInfoMass = GTK_LABEL(gtk_builder_get_object (builder, "MassLabel"));
	PlanetInfoRadius = GTK_LABEL(gtk_builder_get_object (builder, "RadiusLabel"));

	button = GTK_WIDGET(gtk_builder_get_object (builder, "DeleteButton"));
	g_signal_connect (button, "clicked", G_CALLBACK (Delete), NULL);
	
	gtk_widget_show_all (window);
	
	//-------------------------------
	
	//MENU AGGIUNTA PIANETI
	
	builder = gtk_builder_new ();
  	if (gtk_builder_add_from_file (builder, "./Builder/BuilderAddP.ui", &error) == 0){
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
    }
	
	window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	gtk_window_set_deletable (GTK_WINDOW(window), false);
	gtk_window_set_title(GTK_WINDOW(window), "Add Planet");
	gtk_window_move(GTK_WINDOW(window), WIDTH-436, 830);
	gtk_widget_get_size_request (GTK_WIDGET(window),&WW, &WH);
	gtk_widget_set_size_request (window, 436, 100);
	
	GtkWidget *textview;
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "NameText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetNameBuff);
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "MassText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetMassBuff);
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "RadiusText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetRadiusBuff);
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "PosXText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetXPositionBuff);
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "PosYText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetYPositionBuff);
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "VelXText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetXSpeedBuff);
	
	textview = GTK_WIDGET(gtk_builder_get_object (builder, "VelYText"));
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), NewPlanetYSpeedBuff);
		
	NewPlanetColor = GTK_WIDGET(gtk_builder_get_object (builder, "ColorButton"));
	
	button = GTK_WIDGET(gtk_builder_get_object (builder, "NewButton"));
	g_signal_connect (button, "clicked", G_CALLBACK (NewPlanet), NULL);

	gtk_widget_show_all (window);
	
	//-------------------------------
	
	//TRA QUANTO RICHIAMRE MOTORE IN MICROSECONDI (DOPO L'AVVIO DEL MAIN LOOP DI GTK3
	g_timeout_add (1, motore, NULL);
	
	//PRENDO IN INPUT IL TIPO DI MODALITA' RICHIESTO
	sistema_solare = ss;
	
	//INSERICO TUTTI I PIANETI SE E' STATA SCELTA MODALITA' SISTEMA SOLARE
	if (ss){
		std::cout << "Aggiungo i pianeti del sistema solare!\n";
		P.push_back(Pianeta(std::string("Mercurio"), 3.3011e23,2.4397e6, Vettore(4.6e10, 0), Vettore(0, 58.98e3), 1, 0.63, 0.17, 0));
		P.push_back(Pianeta(std::string("Venere"), 4.8675e24,6.0518e6, Vettore(1.0748e11, 0), Vettore(0, 35.26e3), 1, 0.4, 0.17, 0));
	  	P.push_back(Pianeta(std::string("Terra"), 5.972e24,6.378137e6, Vettore(1.496e11, 0), Vettore(0, 30e3), 1, 0, 0.36, 0.4));
		P.push_back(Pianeta(std::string("Marte"), 6.4185e23,3.40245e6, Vettore(2.06644545e11, 0), Vettore(0, 26.499e3), 1, 0.7, 0.09, 0.03));
		P.push_back(Pianeta(std::string("Giove"), 1.89819e27,7.1492e7, Vettore(7.40742598e11, 0), Vettore(0, 13.712e3), 1, 0.75, 0.75, 0.29));
		P.push_back(Pianeta(std::string("Saturno"), 5.6834e26,6.0268e7, Vettore(13.52550e11, 0), Vettore(0, 10.18e3), 1, 0.75, 0.75, 0.29));
		P.push_back(Pianeta(std::string("Urano"), 86.813e24,2.5559e7, Vettore(27.413e11, 0), Vettore(0, 7.11e3), 0.05, 1, 0.78, 0.76));
		P.push_back(Pianeta(std::string("Nettuno"), 1.0243e26,2.4764e7, Vettore(44.59631496e11, 0), Vettore(0, 5.479e3), 1, 0.01, 0.31, 0.3));
	}else{
		MPUGL = 500;
	}
	//INIZIO LOOP GTK3
	gtk_main();
}



//INIZIO VARIE DEFINIZIONI CALL_BACK GTK3


void updateCameraPosition(){
	gtk_label_set_text (CCXL, doubleToString(CCX).c_str());
	gtk_label_set_text (CCYL, doubleToString(CCY).c_str());
	gtk_text_buffer_set_text (NewPlanetXPositionBuff, doubleToString(CCX).c_str(), doubleToString(CCX).size());
	gtk_text_buffer_set_text (NewPlanetYPositionBuff, doubleToString(CCY).c_str(), doubleToString(CCY).size());
}
static void ZP (GtkWidget *widget, gpointer   data){
	MPUGL = MPUGL - 0.1 * MPUGL;
	updateCameraPosition();
}
static void ZM (GtkWidget *widget, gpointer   data){
	MPUGL = MPUGL + 0.1 * MPUGL;
	updateCameraPosition();
}
static void SX (GtkWidget *widget, gpointer   data){
	CCX = CCX - 0.1 * MPUGL;
	updateCameraPosition();
}
static void SU (GtkWidget *widget, gpointer   data){
	CCY = CCY + 0.1 * MPUGL;
	updateCameraPosition();
}
static void DX (GtkWidget *widget, gpointer   data){
	CCX = CCX + 0.1 * MPUGL;
	updateCameraPosition();
}
static void GIU (GtkWidget *widget, gpointer   data){
	CCY = CCY - 0.1 * MPUGL;
	updateCameraPosition();
}
static void Home (GtkWidget *widget, gpointer   data){
	CCX = 0;
	CCY = 0;
	updateCameraPosition();
}
static void Previus (GtkWidget *widget, gpointer   data){
	if (static_cast<int>(P.size())!=0){
		if (selected != 0 && selected != -1){
			selected--;
		}else{
			selected = P.size()-1;
		}
		gtk_label_set_text (PlanetInfoName, P[selected].getNome().c_str());
		gtk_label_set_text (PlanetInfoMass, doubleToString(P[selected].getMassa()).c_str());
		gtk_label_set_text (PlanetInfoRadius, doubleToString(P[selected].getRaggio()).c_str());
		CCX = P[selected].getPosizione().getX();
		CCY = P[selected].getPosizione().getY();
		updateCameraPosition();
	}else{
		gtk_label_set_text (PlanetInfoName, "-");
		gtk_label_set_text (PlanetInfoMass, "-");
		gtk_label_set_text (PlanetInfoRadius, "-");
	}
}
static void Next (GtkWidget *widget, gpointer   data){
	if (P.size()!=0){
		if (selected!=static_cast<int>(P.size())-1){
			selected++;
		}else{
			selected = 0;
		}
		gtk_label_set_text (PlanetInfoName, P[selected].getNome().c_str());
		gtk_label_set_text (PlanetInfoMass, doubleToString(P[selected].getMassa()).c_str());
		gtk_label_set_text (PlanetInfoRadius, doubleToString(P[selected].getRaggio()).c_str());
		CCX = P[selected].getPosizione().getX();
		CCY = P[selected].getPosizione().getY();
		updateCameraPosition();
	}else{
		gtk_label_set_text (PlanetInfoName, "-");
		gtk_label_set_text (PlanetInfoMass, "-");
		gtk_label_set_text (PlanetInfoRadius, "-");
	}
}
static void Delete (GtkWidget *widget, gpointer data){
	if ((P.size()!=0)&&(selected!=-1)){
		P.erase(P.begin()+selected);
		selected--;
		Next(NULL, NULL);
	}
}
static void NewPlanet (GtkWidget *widget, gpointer   data){
	std::string name;
	std::stringstream ss("");
	double radius, mass, xpos, ypos, xspeed, yspeed;
	GdkRGBA rgba;
	GtkTextIter start, end;
	gtk_text_buffer_get_iter_at_offset  (NewPlanetNameBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetNameBuff,&end, -1);
	name = std::string(gtk_text_buffer_get_text (NewPlanetNameBuff, &start, &end, false));
	
	gtk_text_buffer_get_iter_at_offset  (NewPlanetMassBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetMassBuff,&end, -1);
	ss = std::stringstream(gtk_text_buffer_get_text (NewPlanetMassBuff, &start, &end, false));
	ss >> mass;
	
	gtk_text_buffer_get_iter_at_offset  (NewPlanetRadiusBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetRadiusBuff,&end, -1);
	ss = std::stringstream(gtk_text_buffer_get_text (NewPlanetRadiusBuff, &start, &end, false));
	ss >> radius;
	
	gtk_text_buffer_get_iter_at_offset  (NewPlanetXPositionBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetXPositionBuff,&end, -1);
	ss = std::stringstream(gtk_text_buffer_get_text (NewPlanetXPositionBuff, &start, &end, false));
	ss >> xpos;
	
	gtk_text_buffer_get_iter_at_offset  (NewPlanetYPositionBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetYPositionBuff,&end, -1);
	ss = std::stringstream(gtk_text_buffer_get_text (NewPlanetYPositionBuff, &start, &end, false));
	ss >> ypos;
	
	gtk_text_buffer_get_iter_at_offset  (NewPlanetXSpeedBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetXSpeedBuff,&end, -1);
	ss = std::stringstream(gtk_text_buffer_get_text (NewPlanetXSpeedBuff, &start, &end, false));
	ss >> xspeed;
	
	gtk_text_buffer_get_iter_at_offset  (NewPlanetYSpeedBuff,&start, 0);
	gtk_text_buffer_get_iter_at_offset  (NewPlanetYSpeedBuff,&end, -1);
	ss = std::stringstream(gtk_text_buffer_get_text (NewPlanetYSpeedBuff, &start, &end, false));
	ss >> yspeed;

	gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER(NewPlanetColor), &rgba);
	
	P.push_back(Pianeta(name, mass, radius, Vettore(xpos, ypos), Vettore(xspeed, yspeed), 1, rgba.red, rgba.green, rgba.blue));
}
static void ChangeTime (GtkRange *range, GtkScrollType scroll, gdouble value, gpointer user_data){
	if (value>100000){
		value = 100000;
	}
	if (value<100){
		value = 100;
	}
	gtk_range_set_value (range, value);
	T = value;
}
static void ChangeRaggioSole (GtkRange *range, GtkScrollType scroll, gdouble value, gpointer user_data){
	if (value>0.4){
		value = 0.4;
	}
	if (value<0.01){
		value = 0.01;
	}
	gtk_range_set_value (range, value);
	raggioSole = value;
}
static void ChangeRaggioPianeti (GtkRange *range, GtkScrollType scroll, gdouble value, gpointer user_data){
	if (value>0.2){
		value = 0.2;
	}
	if (value<0.005){
		value = 0.005;
	}
	gtk_range_set_value (range, value);
	raggioPianeti = value;
}

//FINE VARIE DEFINIZIONI CALL_BACK

//INIZIO DEFINIZIONE UNICHE 2 CALLBACK di OPENGL

//Chiamata in caso di resize della finestra (esclusivamente opengl)
void draw(){
	WP = glutGet(GLUT_WINDOW_WIDTH);
	HP = glutGet(GLUT_WINDOW_HEIGHT);
	rapp=WP/HP;
	render(1);
}

//Chiamata ogni 25 microsecondi
void render(int i){
	glutTimerFunc(25, render, 0); //Setta la prossima chiamata della stessa nel opp di OPENGL
	glClearColor(0, 0.025, 0.045, 1); //Setta il colore di sfondo
    glClear(GL_COLOR_BUFFER_BIT); //Mette effettivamente il colore di sfondo
    
    if (sistema_solare){
    	drawCircol(0, 0, raggioSole, 245, 143, 0, 1, false);//Disegna il sole nel caso in cui sia necessario
    }
    
    for (int c=0; c<static_cast<int>(P.size()); c++){
    	drawCircol(P[c].getPosizione().getX(), P[c].getPosizione().getY(), raggioPianeti, P[c].getR(), P[c].getG(), P[c].getB(), 1, false);//Disegna tutti i pianeti
   	}
   	
   	drawPointer(CCX, CCY, 1, 1, 1);//disegna il Puntatore al Centro dello Schermo
   	
    //Controlla se ci sono pianeti da Eliminare
    if (Cestino.size()!=0){
		if (Cestino[0]<Cestino[1]){
			P.erase(P.begin()+Cestino[1]);
			P.erase(P.begin()+Cestino[0]);
		}else{
			P.erase(P.begin()+Cestino[0]);
			P.erase(P.begin()+Cestino[1]);
		}
		Cestino.erase(Cestino.begin());Cestino.erase(Cestino.begin());
	}
	
	//Controlla se ci sono esplosioni da mostrare!
	for (int e=0; e<static_cast<int>(E.size()); e++){
		double raggio;
		raggio = 0.1-0.1/E[e].vita;
		drawCircol(E[e].getPosizione().getX(), E[e].getPosizione().getY(), raggio, 1, 0, 0, 1, false);
		E[e].vita--;
		if (E[e].vita==0){
			E.erase(E.begin()+e);
			e--;
		}
	}
	glutSwapBuffers();	//Mostra nella finestra ciò che si è disegnato!
}


//FINE DEFINIZIONI CALL_BACK OPENGL

//FUNZIONE CHE FA TUTTI I CONTI
gboolean motore(gpointer user_data){
	g_timeout_add (1, motore, NULL);
	double r;
	if (!stop){
		for (int c=0; c<static_cast<int>(P.size()); c++){
			//Calcolo della forza con il SOLE!
			P[c].cancForza();
			if (sistema_solare){
				r = sqrt(pow(P[c].getPosizione().getX(),2) + pow(P[c].getPosizione().getY(),2));
				P[c].addForza((G*M*P[c].getMassa())/(r*r*r));
			}
			//Calcolo della forza con gli altri corpi!
				for (int cc=0; cc<static_cast<int>(P.size()); cc++){
					if (cc!=c){
						r = sqrt(pow(P[c].getPosizione().getX()-P[cc].getPosizione().getX(),2) + pow(P[c].getPosizione().getY()-P[cc].getPosizione().getY(),2));
						if (r<(P[c].getRaggio()+P[cc].getRaggio())){
							if (Cestino.size()==0){
								Cestino.push_back(c);
								Cestino.push_back(cc);
								E.push_back(Esplosione(P[c].getPosizione()));
							}
						}else{
							P[c].addForza((G*P[cc].getMassa()*P[c].getMassa())/(r*r*r), P[cc].getPosizione());
						}
					}
				}
			P[c].calcDati();
		}
	}
	return false;
}

// FUNZIONE CHE DISEGNA UN CERCHIO
void drawCircol(double Ux, double Uy, double Ur, double r, double g, double b, double a, bool rvero){
	double ang = 0;
	if (rvero){
		Ur = Ur/MPUGL;
	}
	while (ang<2*3.14){
	    glColor4f(r, g, b, a);
        glBegin(GL_TRIANGLES);
                glVertex2f(cos(ang)*Ur+UTGLX(Ux), sin(ang)*Ur*rapp+UTGLY(Uy));
                glVertex2f(cos(ang+0.1)*Ur+UTGLX(Ux), sin(ang+0.1)*Ur*rapp+UTGLY(Uy));
                glVertex2f(UTGLX(Ux), UTGLY(Uy));
        glEnd();
        ang+=0.1;
	}
}
// FUNZIONE CHE DISEGNA IL PUNTATORE
void drawPointer(double Ux, double Uy, double r, double g, double b){
	glColor4f(r, g, b, 1);
	glBegin(GL_TRIANGLES);
                glVertex2f(UTGLX(Ux), UTGLY(Uy));
                glVertex2f(UTGLX(Ux)-0.01, UTGLY(Uy)+0.02);
                glVertex2f(UTGLX(Ux)+0.01, UTGLY(Uy)+0.02);
                glVertex2f(UTGLX(Ux), UTGLY(Uy));
                glVertex2f(UTGLX(Ux)-0.02, UTGLY(Uy)+0.01);
                glVertex2f(UTGLX(Ux)-0.02, UTGLY(Uy)-0.01);
                glVertex2f(UTGLX(Ux), UTGLY(Uy));
                glVertex2f(UTGLX(Ux)-0.01, UTGLY(Uy)-0.02);
                glVertex2f(UTGLX(Ux)+0.01, UTGLY(Uy)-0.02);
                glVertex2f(UTGLX(Ux), UTGLY(Uy));
                glVertex2f(UTGLX(Ux)+0.02, UTGLY(Uy)+0.01);
                glVertex2f(UTGLX(Ux)+0.02, UTGLY(Uy)-0.01);
    glEnd();
}
//FUNZIONE CHE CONVERTE DOUBLE IN STRINGHE
std::string doubleToString(double dd){
	std::stringstream ss("");
	ss << dd;
	return ss.str();
}

//MADE by NOCE
//Inizio sviluppo programma: Venerdì 3 Maggio 2019
//Ultima Modifica: 24/09/2019
//Commenti scritti il 24/09/2019
