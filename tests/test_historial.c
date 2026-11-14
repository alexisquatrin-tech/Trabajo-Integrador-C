#include "simulador.h"

#include <assert.h>
#include <stdio.h>

/* Ejecuta una mision completa de forma automatica sin pedir Enter por consola. */
static EstadoCapsula simular_automatico(const ConfigMision *config)
{
    EstadoCapsula estado;
    iniciar_estado(&estado, config);
    while (estado.etapa != ETAPA_FINALIZADA) {
        if (!avanzar_etapa(&estado, config)) {
            break;
        }
    }
    return estado;
}

static void probar_historial_vacio(void)
{
    HistorialSimulaciones h;
    iniciar_historial(&h);

    puts("--- Prueba 1: Historial vacio ---");
    assert(historial_vacio(&h) == 1);
    assert(historial_lleno(&h) == 0);
    assert(h.cantidad == 0);

    mostrar_reporte_historial(&h);
    mostrar_comparacion_simulaciones(&h);
}

static void probar_varias_simulaciones(void)
{
    HistorialSimulaciones h;
    ConfigMision c1, c2, c3;
    EstadoCapsula e1, e2, e3;

    iniciar_historial(&h);
    puts("\n--- Prueba 2: Varias simulaciones y comparacion ---");

    /* Sim 1: Mision estandar exitosa */
    c1 = config_consigna();
    e1 = simular_automatico(&c1);
    assert(e1.resultado == MISION_EXITOSA);
    assert(registrar_simulacion(&h, &c1, &e1) == 1);

    /* Sim 2: Impacto fuerte (velocidad de impacto 12 m/s > 8 m/s) y menor duracion */
    c2 = config_consigna();
    c2.velocidad_impacto_ms = 12.0;
    c2.duracion_frenado_s = 320.0;
    c2.hundimiento_m = 1.2;
    e2 = simular_automatico(&c2);
    assert(e2.resultado == MISION_FALLIDA_IMPACTO);
    assert(registrar_simulacion(&h, &c2, &e2) == 1);

    /* Sim 3: Rebote atmosferico (angulo 4.0 grados) */
    c3 = config_consigna();
    c3.angulo_grados = 4.0;
    e3 = simular_automatico(&c3);
    assert(e3.resultado == MISION_FALLIDA_REBOTE);
    assert(registrar_simulacion(&h, &c3, &e3) == 1);

    assert(h.cantidad == 3);
    assert(!historial_vacio(&h));
    assert(!historial_lleno(&h));

    mostrar_reporte_historial(&h);
    mostrar_comparacion_simulaciones(&h);
}

static void probar_historial_lleno(void)
{
    HistorialSimulaciones h;
    ConfigMision c = config_consigna();
    EstadoCapsula e = simular_automatico(&c);
    int i;

    iniciar_historial(&h);
    puts("\n--- Prueba 3: Control de capacidad (Historial lleno) ---");

    for (i = 0; i < CAPACIDAD_HISTORIAL; i++) {
        c.masa_kg = 10000.0 + (i * 250.0);
        e = simular_automatico(&c);
        assert(registrar_simulacion(&h, &c, &e) == 1);
    }

    assert(historial_lleno(&h) == 1);
    assert(h.cantidad == CAPACIDAD_HISTORIAL);

    /* Intentar insertar una simulacion extra superando CAPACIDAD_HISTORIAL */
    assert(registrar_simulacion(&h, &c, &e) == 0);
    assert(h.cantidad == CAPACIDAD_HISTORIAL);

    printf("Capacidad maxima (%d) verificada correctamente: insercion extra bloqueada.\n",
           CAPACIDAD_HISTORIAL);
}

int main(void)
{
    probar_historial_vacio();
    probar_varias_simulaciones();
    probar_historial_lleno();

    puts("\nOK: Todas las pruebas de historial y reportes pasaron con exito.");
    return 0;
}
