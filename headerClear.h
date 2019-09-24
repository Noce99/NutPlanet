#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <iostream>
#include <gtk/gtk.h>
using namespace std;
struct Dati{
  bool annullato = false;
  string nome = "-";
  double massa = 1;
  double raggio = 0;
  double * posizione = new double[2]{0,0};
  double * velocita = new double[2]{0,0};
  int colore = 4;
};
void settingup();
void createFinestraSimulatore(bool);
void avviaSimulatore();
Dati CreateMenuSimulatore(double *);
string doubleToString(double);
void CreateInfoMenu(Dati);

//Nuove Funzioni
//static void Quit (GtkWidget *, gpointer);
#endif
