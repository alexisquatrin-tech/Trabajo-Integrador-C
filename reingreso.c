#include "simulador.h"
#include "wroz.h"

#include <math.h>
#include <stddef.h>

/* Reingreso (incisos a/b): integra el calculo energetico de wroz.h.
 * El flujo fue integrado originalmente por Nico; se conservan sus aportes.
 * Modelo: masa y g constantes. El trabajo atmosferico es Delta(K + U).
 * Se toma duracion_s como el intervalo completo entre vi y vf; se informa
 * la magnitud positiva de la desaceleracion, (vi - vf) / duracion_s.
 * Es un promedio, no una aceleracion maxima ni una trayectoria resuelta.
 * Justificacion y limites: docs/modelo-fisico.md. */
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
