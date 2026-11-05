#include "simulador.h"

#include <math.h>
#include <stddef.h>

/* Parte de Alexis: incisos c y d del escenario 4. */
int calcular_amerizaje(const DatosAmerizaje *datos,
                      ResultadoAmerizaje *resultado)
{
    ResultadoAmerizaje calculado;
    double fuerza_neta;

    if (datos == NULL || resultado == NULL) {
        return 0;
    }
    if (!isfinite(datos->masa_kg) || datos->masa_kg <= 0.0 ||
        !isfinite(datos->velocidad_impacto_ms) || datos->velocidad_impacto_ms < 0.0 ||
        !isfinite(datos->hundimiento_m) || datos->hundimiento_m <= 0.0) {
        return 0;
    }

    calculado.energia_impacto_j = (0.5 * datos->masa_kg *
                                  datos->velocidad_impacto_ms) *
                                  datos->velocidad_impacto_ms;
    calculado.peso_n = datos->masa_kg * GRAVEDAD_TERRESTRE;
    fuerza_neta = calculado.energia_impacto_j / datos->hundimiento_m;
    /* mg*d - Fagua*d = -Ki: el agua frena y el peso favorece el descenso. */
    calculado.fuerza_agua_n = calculado.peso_n + fuerza_neta;
    calculado.multiplo_peso = calculado.fuerza_agua_n / calculado.peso_n;

    if (!isfinite(calculado.energia_impacto_j) ||
        !isfinite(calculado.peso_n) || calculado.peso_n <= 0.0 ||
        !isfinite(calculado.fuerza_agua_n) ||
        !isfinite(calculado.multiplo_peso)) {
        return 0;
    }
    *resultado = calculado;
    return 1;
}

EstadoReingreso clasificar_angulo(double angulo_grados)
{
    if (!isfinite(angulo_grados) || angulo_grados < 0.0 || angulo_grados > 90.0) {
        return ANGULO_INVALIDO;
    }
    if (angulo_grados < ANGULO_MINIMO_SEGURO) {
        return REBOTE_ATMOSFERICO;
    }
    if (angulo_grados > ANGULO_MAXIMO_SEGURO) {
        return DESACELERACION_EXCESIVA;
    }
    return ANGULO_SEGURO;
}

const char *describir_angulo(EstadoReingreso estado)
{
    switch (estado) {
        case REBOTE_ATMOSFERICO:
            return "Entrada demasiado baja: riesgo de rebote atmosferico.";
        case ANGULO_SEGURO:
            return "Angulo dentro de la ventana de seguridad [5.5, 7.0] grados.";
        case DESACELERACION_EXCESIVA:
            return "Entrada demasiado pronunciada: riesgo de desaceleracion excesiva.";
        case ANGULO_INVALIDO:
        default:
            return "Angulo invalido: debe ser finito y estar entre 0 y 90 grados.";
    }
}
