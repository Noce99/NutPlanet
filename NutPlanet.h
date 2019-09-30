#ifndef NUTHEADER_H
#define NUTHEADER_H
#include <iostream> // Necessaria per l'output su terminale
#include <cmath> // Necessaria per alcuni calcoli matematici
#include <vector> // Necessaria per la creazione di un Vector di Pianeti
#include <thread> //Necessaria per avere glut e gtk che funzionanao contemporaneamente
#include <sstream> //Necessaria per alcune conversioni tra tipi
#include "GL/freeglut.h" //Necessaria per disegnare i pianeti e l'universo
#include <gtk/gtk.h> //Necessaria per la creazione di menu, bottoni, label ecc
#include "Vettore.h"
#include "Pianeta.h"
#include "Esplosione.h"
void createFinestraSimulatore(bool const &);
#endif
