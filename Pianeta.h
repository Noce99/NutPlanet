#ifndef PIANETA_H
#define PIANETA_H

#include "Vettore.h"
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
	std::string getNome() const {
		return nome;
	}
	double getMassa() const {
		return massa;
	}
	double getRaggio() const {
		return raggio;
	}
	Vettore getPosizione() const {
		return posizione;
	}
	Vettore getVelocita() const {
		return velocita;
	}
	Vettore getAccelerazione() const {
		return accelerazione;
	}
	double getR() const {
		return r;
	}
	double getG() const {
		return g;
	}
	double getB() const {
		return b;
	}
	Vettore getForza() const {
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
	void addForza(double const & f, Vettore const & v=Vettore(0,0)){
		Vettore temp = Vettore(0,0);
		temp.setX(f*(posizione.getX()-v.getX()));
		temp.setY(f*(posizione.getY()-v.getY()));
		forza=forza+temp;
	}
	void calcDati(double const & T){
		accelerazione.setX(-forza.getX()/massa);
		accelerazione.setY(-forza.getY()/massa);
		velocita.setX(velocita.getX() + T*accelerazione.getX());
		velocita.setY(velocita.getY() + T*accelerazione.getY());
		posizione.setX(posizione.getX() + T*velocita.getX());
		posizione.setY(posizione.getY() + T*velocita.getY());
	}
};
#endif
