
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

// Lee un numero entero dentro de un rango.
int leerEntero(const char *mensaje, int *valor,
               int minimo, int maximo);

// Lee un numero real dentro de un rango inclusivo.
int leerReal(const char *mensaje, double *valor,
             double minimo, double maximo);

// Lee un numero real estrictamente mayor que cero.
int leerRealPositivo(const char *mensaje, double *valor);

#endif
