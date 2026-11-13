#include <stdio.h>

double DA(double vf,double vi,double tiempo){
    double Velocidad  = vf - vi;
    double Aceleración = Velocidad / tiempo;
    return Aceleración / 9.81;
}
