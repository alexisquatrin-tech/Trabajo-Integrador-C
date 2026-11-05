#include "simulador.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int comprobaciones;
static int fallos;
#define VERIFICAR(condicion) do { \
    ++comprobaciones; \
    if (!(condicion)) { \
        fprintf(stderr, "FALLO linea %d: %s\n", __LINE__, #condicion); \
        ++fallos; \
    } \
} while (0)

static void verificar_rechazo(DatosAmerizaje datos)
{
    ResultadoAmerizaje r = {1.0, 2.0, 3.0, 4.0};
    VERIFICAR(!calcular_amerizaje(&datos, &r));
    VERIFICAR(r.energia_impacto_j == 1.0 && r.peso_n == 2.0 &&
              r.fuerza_agua_n == 3.0 && r.multiplo_peso == 4.0);
}

int main(void)
{
    DatosAmerizaje datos = {10400.0, 8.0, 1.5};
    ResultadoAmerizaje r, doble_masa, mayor_distancia;
    const struct { double angulo; EstadoReingreso esperado; } casos[] = {
        {0.0, REBOTE_ATMOSFERICO}, {5.4999, REBOTE_ATMOSFERICO},
        {5.5, ANGULO_SEGURO}, {6.0, ANGULO_SEGURO}, {7.0, ANGULO_SEGURO},
        {7.0001, DESACELERACION_EXCESIVA}, {90.0, DESACELERACION_EXCESIVA},
        {-1.0, ANGULO_INVALIDO}, {90.1, ANGULO_INVALIDO},
        {NAN, ANGULO_INVALIDO}, {INFINITY, ANGULO_INVALIDO}
    };
    size_t i;

    VERIFICAR(calcular_amerizaje(&datos, &r));
    VERIFICAR(fabs(r.energia_impacto_j - 332800.0) < 1e-6);
    VERIFICAR(fabs(r.peso_n - 101920.0) < 1e-6);
    VERIFICAR(fabs(r.fuerza_agua_n - 323786.6666666667) < 1e-6);
    VERIFICAR(fabs(r.multiplo_peso - 3.17687074829932) < 1e-12);
    /* Verificacion independiente del balance trabajo-energia. */
    VERIFICAR(fabs((r.peso_n - r.fuerza_agua_n) * datos.hundimiento_m
                  + r.energia_impacto_j) < 1e-6);
    datos.masa_kg *= 2.0;
    VERIFICAR(calcular_amerizaje(&datos, &doble_masa));
    VERIFICAR(fabs(doble_masa.fuerza_agua_n - 2.0 * r.fuerza_agua_n) < 1e-6);
    VERIFICAR(fabs(doble_masa.multiplo_peso - r.multiplo_peso) < 1e-12);
    datos.masa_kg = 10400.0;
    datos.hundimiento_m = 3.0;
    VERIFICAR(calcular_amerizaje(&datos, &mayor_distancia));
    VERIFICAR(mayor_distancia.fuerza_agua_n < r.fuerza_agua_n);
    datos.velocidad_impacto_ms = 0.0;
    VERIFICAR(calcular_amerizaje(&datos, &r));
    VERIFICAR(r.multiplo_peso == 1.0);
    verificar_rechazo((DatosAmerizaje){0.0, 8.0, 1.5});
    verificar_rechazo((DatosAmerizaje){-1.0, 8.0, 1.5});
    verificar_rechazo((DatosAmerizaje){10400.0, -8.0, 1.5});
    verificar_rechazo((DatosAmerizaje){10400.0, 8.0, 0.0});
    verificar_rechazo((DatosAmerizaje){10400.0, 8.0, -1.0});
    verificar_rechazo((DatosAmerizaje){NAN, 8.0, 1.5});
    verificar_rechazo((DatosAmerizaje){10400.0, INFINITY, 1.5});
    verificar_rechazo((DatosAmerizaje){10400.0, 8.0, NAN});
    verificar_rechazo((DatosAmerizaje){DBL_MAX, DBL_MAX, 1.5});
    VERIFICAR(!calcular_amerizaje(NULL, &r));
    VERIFICAR(!calcular_amerizaje(&datos, NULL));
    for (i = 0; i < sizeof casos / sizeof casos[0]; ++i) {
        VERIFICAR(clasificar_angulo(casos[i].angulo) == casos[i].esperado);
        VERIFICAR(strlen(describir_angulo(casos[i].esperado)) > 0);
    }
    printf("%d comprobaciones: %d correctas, %d fallidas.\n",
           comprobaciones, comprobaciones - fallos, fallos);
    return fallos == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
