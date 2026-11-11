#include "simulador.h"

#include <math.h>
#include <stdio.h>

static int correctas = 0;
static int fallidas = 0;

#define VERIFICAR(condicion)                                              \
    do {                                                                  \
        if (condicion) {                                                  \
            ++correctas;                                                  \
        } else {                                                          \
            ++fallidas;                                                   \
            printf("FALLA (linea %d): %s\n", __LINE__, #condicion);       \
        }                                                                 \
    } while (0)

static int cercano(double a, double b, double tolerancia)
{
    return fabs(a - b) <= tolerancia;
}

/* Recorre la mision completa y devuelve el estado final. */
static EstadoCapsula recorrer(const ConfigMision *config)
{
    EstadoCapsula estado;
    int pasos = 0;

    iniciar_estado(&estado, config);
    while (estado.etapa != ETAPA_FINALIZADA && pasos < 10) {
        if (!avanzar_etapa(&estado, config)) {
            break;
        }
        ++pasos;
    }
    return estado;
}

static void prueba_recorrido_exitoso(void)
{
    const ConfigMision config = config_consigna();
    EstadoCapsula estado;

    iniciar_estado(&estado, &config);
    VERIFICAR(estado.etapa == ETAPA_REINGRESO);
    VERIFICAR(estado.resultado == MISION_EN_CURSO);
    VERIFICAR(cercano(estado.velocidad_ms, 11000.0, 1e-9));

    VERIFICAR(avanzar_etapa(&estado, &config) == 1);
    VERIFICAR(estado.etapa == ETAPA_PARACAIDAS);
    VERIFICAR(estado.clasificacion_angulo == ANGULO_SEGURO);
    VERIFICAR(cercano(estado.velocidad_ms, 25.0, 1e-9));
    VERIFICAR(cercano(estado.altitud_m, 2500.0, 1e-9));
    VERIFICAR(estado.reingreso.trabajo_rozamiento_j < 0.0);
    VERIFICAR(cercano(estado.reingreso.desaceleracion_ms2, 27.4375, 1e-9));
    VERIFICAR(cercano(estado.reingreso.desaceleracion_g, 27.4375 / 9.8, 1e-9));

    VERIFICAR(avanzar_etapa(&estado, &config) == 1);
    VERIFICAR(estado.etapa == ETAPA_AMERIZAJE);
    VERIFICAR(cercano(estado.velocidad_ms, 8.0, 1e-9));
    VERIFICAR(cercano(estado.altitud_m, 0.0, 1e-9));

    VERIFICAR(avanzar_etapa(&estado, &config) == 1);
    VERIFICAR(estado.etapa == ETAPA_FINALIZADA);
    VERIFICAR(estado.resultado == MISION_EXITOSA);
    VERIFICAR(cercano(estado.amerizaje.fuerza_agua_n, 323786.67, 0.01));
    VERIFICAR(cercano(estado.amerizaje.multiplo_peso, 3.1769, 0.0001));

    /* Una mision finalizada no avanza mas. */
    VERIFICAR(avanzar_etapa(&estado, &config) == 0);
    VERIFICAR(estado.resultado == MISION_EXITOSA);
}

static void prueba_angulos(void)
{
    ConfigMision config = config_consigna();
    EstadoCapsula estado;

    config.angulo_grados = 5.0;
    estado = recorrer(&config);
    VERIFICAR(estado.etapa == ETAPA_FINALIZADA);
    VERIFICAR(estado.resultado == MISION_FALLIDA_REBOTE);
    VERIFICAR(cercano(estado.velocidad_ms, 11000.0, 1e-9));

    config.angulo_grados = 7.5;
    estado = recorrer(&config);
    VERIFICAR(estado.resultado == MISION_FALLIDA_DESACELERACION);

    config.angulo_grados = 5.5;
    VERIFICAR(recorrer(&config).resultado == MISION_EXITOSA);
    config.angulo_grados = 7.0;
    VERIFICAR(recorrer(&config).resultado == MISION_EXITOSA);

    config.angulo_grados = 91.0;
    iniciar_estado(&estado, &config);
    VERIFICAR(avanzar_etapa(&estado, &config) == 0);
    VERIFICAR(estado.resultado == MISION_DATOS_INVALIDOS);
    VERIFICAR(estado.etapa == ETAPA_FINALIZADA);
}

static void prueba_impacto(void)
{
    ConfigMision config = config_consigna();

    config.velocidad_impacto_ms = 8.0;
    VERIFICAR(recorrer(&config).resultado == MISION_EXITOSA);
    config.velocidad_impacto_ms = 8.5;
    VERIFICAR(recorrer(&config).resultado == MISION_FALLIDA_IMPACTO);
}

static void prueba_datos_invalidos(void)
{
    ConfigMision config = config_consigna();
    EstadoCapsula estado;

    VERIFICAR(config_valida(&config) == 1);
    VERIFICAR(config_valida(NULL) == 0);

    config.masa_kg = 0.0;
    VERIFICAR(config_valida(&config) == 0);
    iniciar_estado(&estado, &config);
    VERIFICAR(avanzar_etapa(&estado, &config) == 0);
    VERIFICAR(estado.resultado == MISION_DATOS_INVALIDOS);

    config = config_consigna();
    config.altitud_paracaidas_m = 130000.0;
    VERIFICAR(config_valida(&config) == 0);
    config = config_consigna();
    config.velocidad_paracaidas_ms = 12000.0;
    VERIFICAR(config_valida(&config) == 0);
    config = config_consigna();
    config.hundimiento_m = NAN;
    VERIFICAR(config_valida(&config) == 0);
    config = config_consigna();
    config.duracion_frenado_s = -1.0;
    VERIFICAR(config_valida(&config) == 0);
}

static void prueba_punteros_nulos_y_textos(void)
{
    const ConfigMision config = config_consigna();
    EstadoCapsula estado;
    ResultadoReingreso resultado;
    const DatosReingreso datos = {10400.0, 11000.0, 120000.0, 25.0, 2500.0, 400.0};

    VERIFICAR(avanzar_etapa(NULL, &config) == 0);
    iniciar_estado(&estado, &config);
    VERIFICAR(avanzar_etapa(&estado, NULL) == 0);
    iniciar_estado(NULL, &config);
    iniciar_estado(&estado, NULL);

    VERIFICAR(calcular_reingreso(NULL, &resultado) == 0);
    VERIFICAR(calcular_reingreso(&datos, NULL) == 0);
    VERIFICAR(calcular_reingreso(&datos, &resultado) == 1);

    VERIFICAR(nombre_etapa(ETAPA_AMERIZAJE) != NULL);
    VERIFICAR(nombre_etapa((EtapaMision)99) != NULL);
    VERIFICAR(describir_resultado(MISION_EXITOSA) != NULL);
    VERIFICAR(describir_resultado((ResultadoMision)99) != NULL);
}

int main(void)
{
    prueba_recorrido_exitoso();
    prueba_angulos();
    prueba_impacto();
    prueba_datos_invalidos();
    prueba_punteros_nulos_y_textos();

    printf("Flujo: %d comprobaciones correctas, %d fallidas.\n", correctas, fallidas);
    return fallidas == 0 ? 0 : 1;
}
