#include "simulador.h"
#include "wroz.h"

#include <math.h>
#include <stddef.h>

/* PROVISORIO (Nico): implementacion minima de calcular_reingreso() para
 * poder probar el flujo completo. Reutiliza calcularWroz() de wroz.h.
 * Navarro (incisos a/b) puede reemplazar este archivo conservando la firma
 * declarada en simulador.h.
 * Supuesto del inciso b: toda la reduccion de rapidez ocurre en duracion_s,
 * asi que la desaceleracion media es (vi - vf) / duracion_s. */
int calcular_reingreso(const DatosReingreso *datos,
                       ResultadoReingreso *resultado)
{
    ResultadoReingreso calculado;

    if (datos == NULL || resultado == NULL) {
        return 0;
    }
    if (!isfinite(datos->masa_kg) || datos->masa_kg <= 0.0 ||
        !isfinite(datos->velocidad_inicial_ms) || datos->velocidad_inicial_ms < 0.0 ||
        !isfinite(datos->velocidad_final_ms) || datos->velocidad_final_ms < 0.0 ||
        !isfinite(datos->altitud_inicial_m) || datos->altitud_inicial_m < 0.0 ||
        !isfinite(datos->altitud_final_m) || datos->altitud_final_m < 0.0 ||
        !isfinite(datos->duracion_s) || datos->duracion_s <= 0.0 ||
        datos->velocidad_inicial_ms <= datos->velocidad_final_ms ||
        datos->altitud_inicial_m <= datos->altitud_final_m) {
        return 0;
    }

    calculado.trabajo_rozamiento_j = calcularWroz(datos->velocidad_inicial_ms,
                                                  datos->velocidad_final_ms,
                                                  datos->altitud_inicial_m,
                                                  datos->altitud_final_m,
                                                  datos->masa_kg);
    calculado.desaceleracion_ms2 =
        (datos->velocidad_inicial_ms - datos->velocidad_final_ms) / datos->duracion_s;
    calculado.desaceleracion_g = calculado.desaceleracion_ms2 / GRAVEDAD_TERRESTRE;

    if (!isfinite(calculado.trabajo_rozamiento_j) ||
        !isfinite(calculado.desaceleracion_ms2) ||
        !isfinite(calculado.desaceleracion_g)) {
        return 0;
    }
    *resultado = calculado;
    return 1;
}
