#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stddef.h>

/* Lee hasta salto o EOF sin aceptar prefijos truncados. Devuelve 1 al leer,
 * 0 ante EOF sin datos/error, -1 si excede capacidad-1, -2 si contiene NUL.
 * La linea se consume por completo; el salto no se incluye en buffer. */
int leer_linea_segura(char *buffer, size_t capacidad);

// Lee un numero entero dentro de un rango.
int leer_entero(const char *mensaje, int minimo,
                int maximo, int *valor);

// Lee un numero real dentro de un rango inclusivo.
int leer_real(const char *mensaje, double minimo,
              double maximo, double *valor);

// Lee un numero real estrictamente mayor que cero.
int leer_real_positivo(const char *mensaje, double *valor);

#endif
