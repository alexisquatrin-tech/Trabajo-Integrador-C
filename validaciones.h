#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Lee una linea y un entero completo dentro de [minimo, maximo].
 * Reintenta ante entrada invalida. Devuelve 0 al terminar la entrada,
 * ante error de lectura o parametros invalidos; no modifica la salida.
 */
int leer_entero(const char *mensaje, int minimo, int maximo, int *salida);

#endif
