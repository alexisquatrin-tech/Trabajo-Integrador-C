

#ifndef VALIDACIONES_H
#define VALIDACIONES_H

// Lee un numero entero dentro de un rango.
int leer_entero(const char *mensaje, int minimo,
                int maximo, int *valor);

// Lee un numero real dentro de un rango inclusivo.
int leer_real(const char *mensaje, double minimo,
              double maximo, double *valor);

// Lee un numero real estrictamente mayor que cero.
int leer_real_positivo(const char *mensaje, double *valor);

#endif

