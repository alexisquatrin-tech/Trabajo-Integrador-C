#include <stdio.h>

double DA(double vf,double vi,double hi, double hf){
    double Velocidad  = (vf * vf) - (vi * vi);
    double Distancia = hi - hf;
    double Aceleración = Velocidad / (Distancia * 2);
    return Aceleración;
}
