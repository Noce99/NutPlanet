#ifndef VETTORE_H
#define VETTORE_H
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
	double getX() const {
		return x;
	}
	double getY() const {
		return y;
	}
	void setX(double const & xx){
		x = xx;
	}
	void setY(double const & yy){
		y = yy;
	}
	/*void getInfo(){
		std::cout << "X: " << x << " Y: " << y;
	}*/
	Vettore operator+(Vettore const & v) const {
		return Vettore(x+v.getX(), y+v.getY());
	}
};
#endif
