#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "simulador.h"

/* Solicita al usuario una configuración personalizada.
   Devuelve 1 si la configuración es correcta y 0 si se interrumpe. */
int solicitar_configuracion_mision(ConfigMision *config);

/* Muestra los parámetros originales del escenario. */
void mostrar_parametros_escenario(void);

#endif
