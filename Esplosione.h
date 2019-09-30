#ifndef ESPLOSIONE_H
#define ESPLOSIONE_H
//Definizione Esplosione
struct Esplosione{
	Vettore posizione;
	int vita = 400;
	Esplosione(Vettore posizione):posizione(posizione){}
	Vettore getPosizione() const {
		return posizione;
	}
};
#endif
