#include "simulador.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Parte 3: flujo, estados y eventos. */

static const char *const NOMBRES_ETAPA[] = {
    "Reingreso atmosferico",
    "Apertura de paracaidas",
    "Amerizaje",
    "Mision finalizada"
};

static const char *const TEXTOS_RESULTADO[] = {
    "Mision en curso.",
    "MISION EXITOSA: amerizaje dentro de la rapidez segura.",
    "MISION FALLIDA: angulo de entrada demasiado bajo (rebote atmosferico).",
    "MISION FALLIDA: angulo de entrada demasiado alto (desaceleracion excesiva).",
    "MISION FALLIDA: rapidez de impacto mayor que la segura.",
    "Mision abortada por el usuario.",
    "Mision cancelada: datos de entrada invalidos.",
    "Mision cancelada: error en un calculo."
};

#define CANTIDAD(arreglo) (sizeof(arreglo) / sizeof((arreglo)[0]))

ConfigMision config_consigna(void)
{
    ConfigMision config;

    config.masa_kg = 10400.0;
    config.angulo_grados = 6.0;
    config.velocidad_entrada_ms = 11000.0;
    config.altitud_entrada_m = 120000.0;
    config.velocidad_paracaidas_ms = 25.0;
    config.altitud_paracaidas_m = 2500.0;
    config.duracion_frenado_s = 400.0;
    config.velocidad_impacto_ms = 8.0;
    config.hundimiento_m = 1.5;

    return config;
}

int config_valida(const ConfigMision *config)
{
    if (config == NULL) {
        return 0;
    }

    if (clasificar_angulo(config->angulo_grados) == ANGULO_INVALIDO) {
        return 0;
    }

    if (!isfinite(config->masa_kg) || config->masa_kg <= 0.0 ||
        !isfinite(config->velocidad_entrada_ms) ||
        !isfinite(config->velocidad_paracaidas_ms) ||
        !isfinite(config->velocidad_impacto_ms) ||
        !isfinite(config->altitud_entrada_m) ||
        !isfinite(config->altitud_paracaidas_m) ||
        !isfinite(config->duracion_frenado_s) ||
        !isfinite(config->hundimiento_m)) {
        return 0;
    }

    if (config->velocidad_paracaidas_ms < 0.0 ||
        config->velocidad_paracaidas_ms >= config->velocidad_entrada_ms ||
        config->altitud_paracaidas_m <= 0.0 ||
        config->altitud_paracaidas_m >= config->altitud_entrada_m ||
        config->duracion_frenado_s <= 0.0 ||
        config->velocidad_impacto_ms < 0.0 ||
        config->hundimiento_m <= 0.0) {
        return 0;
    }

    return 1;
}

void iniciar_estado(EstadoCapsula *estado, const ConfigMision *config)
{
    if (estado == NULL || config == NULL) {
        return;
    }

    memset(estado, 0, sizeof *estado);

    estado->etapa = ETAPA_REINGRESO;
    estado->resultado = MISION_EN_CURSO;
    estado->velocidad_ms = config->velocidad_entrada_ms;
    estado->altitud_m = config->altitud_entrada_m;
    estado->clasificacion_angulo = ANGULO_INVALIDO;
}

const char *nombre_etapa(EtapaMision etapa)
{
    if ((size_t)etapa >= CANTIDAD(NOMBRES_ETAPA)) {
        return "Etapa desconocida";
    }

    return NOMBRES_ETAPA[etapa];
}

const char *describir_resultado(ResultadoMision resultado)
{
    if ((size_t)resultado >= CANTIDAD(TEXTOS_RESULTADO)) {
        return "Resultado desconocido.";
    }

    return TEXTOS_RESULTADO[resultado];
}

static void finalizar(EstadoCapsula *estado, ResultadoMision resultado)
{
    estado->resultado = resultado;
    estado->etapa = ETAPA_FINALIZADA;
}

int avanzar_etapa(EstadoCapsula *estado, const ConfigMision *config)
{
    DatosReingreso datos_reingreso;
    DatosAmerizaje datos_amerizaje;

    if (estado == NULL || config == NULL ||
        estado->etapa == ETAPA_FINALIZADA) {
        return 0;
    }

    if (!config_valida(config)) {
        finalizar(estado, MISION_DATOS_INVALIDOS);
        return 0;
    }

    switch (estado->etapa) {

        case ETAPA_REINGRESO:
            estado->clasificacion_angulo =
                clasificar_angulo(config->angulo_grados);

            if (estado->clasificacion_angulo == ANGULO_INVALIDO) {
                finalizar(estado, MISION_DATOS_INVALIDOS);
                return 0;
            }

            if (estado->clasificacion_angulo == REBOTE_ATMOSFERICO) {
                finalizar(estado, MISION_FALLIDA_REBOTE);
                return 1;
            }

            if (estado->clasificacion_angulo == DESACELERACION_EXCESIVA) {
                finalizar(estado, MISION_FALLIDA_DESACELERACION);
                return 1;
            }

            datos_reingreso.masa_kg = config->masa_kg;
            datos_reingreso.velocidad_inicial_ms =
                config->velocidad_entrada_ms;
            datos_reingreso.altitud_inicial_m =
                config->altitud_entrada_m;
            datos_reingreso.velocidad_final_ms =
                config->velocidad_paracaidas_ms;
            datos_reingreso.altitud_final_m =
                config->altitud_paracaidas_m;
            datos_reingreso.duracion_s = config->duracion_frenado_s;

            if (!calcular_reingreso(&datos_reingreso, &estado->reingreso)) {
                finalizar(estado, MISION_ERROR_CALCULO);
                return 0;
            }

            estado->velocidad_ms = config->velocidad_paracaidas_ms;
            estado->altitud_m = config->altitud_paracaidas_m;
            estado->etapa = ETAPA_PARACAIDAS;

            return 1;

        case ETAPA_PARACAIDAS:
            estado->velocidad_ms = config->velocidad_impacto_ms;
            estado->altitud_m = 0.0;
            estado->etapa = ETAPA_AMERIZAJE;

            return 1;

        case ETAPA_AMERIZAJE:
            estado->velocidad_impacto_ms = estado->velocidad_ms;

            datos_amerizaje.masa_kg = config->masa_kg;
            datos_amerizaje.velocidad_impacto_ms =
                estado->velocidad_impacto_ms;
            datos_amerizaje.hundimiento_m = config->hundimiento_m;

            if (!calcular_amerizaje(&datos_amerizaje,
                                    &estado->amerizaje)) {
                finalizar(estado, MISION_ERROR_CALCULO);
                return 0;
            }

            if (estado->velocidad_impacto_ms <=
                VELOCIDAD_IMPACTO_SEGURA_MS) {
                finalizar(estado, MISION_EXITOSA);
            } else {
                finalizar(estado, MISION_FALLIDA_IMPACTO);
            }

            estado->velocidad_ms = 0.0;

            return 1;

        case ETAPA_FINALIZADA:
        default:
            return 0;
    }
}

/* Devuelve 1 si continua y 0 si aborta. */
static int pedir_continuar(void)
{
    char linea[64];
    const char *p;
    int c;

    while (1) {
        printf("Enter = continuar, 0 = abortar: ");

        if (fgets(linea, sizeof linea, stdin) == NULL) {
            puts("\nFin de entrada: se aborta la mision.");
            return 0;
        }

        if (strchr(linea, '\n') == NULL && !feof(stdin)) {
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            puts("Error: entrada demasiado larga.");
            continue;
        }

        p = linea;

        while (isspace((unsigned char)*p) && *p != '\n') {
            p++;
        }

        if (*p == '\n' || *p == '\0') {
            return 1;
        }

        if (*p == '0') {
            p++;

            while (isspace((unsigned char)*p)) {
                p++;
            }

            if (*p == '\0') {
                return 0;
            }
        }

        puts("Error: presione solo Enter o escriba 0.");
    }
}

static void mostrar_evento(EtapaMision ejecutada,
                           const EstadoCapsula *estado)
{
    switch (ejecutada) {

        case ETAPA_REINGRESO:
            puts(describir_angulo(estado->clasificacion_angulo));

            if (estado->resultado == MISION_EN_CURSO) {
                printf("Trabajo del rozamiento atmosferico: %.2f J\n"
                       "Desaceleracion media: %.2f m/s^2 (%.2f g)\n",
                       estado->reingreso.trabajo_rozamiento_j,
                       estado->reingreso.desaceleracion_ms2,
                       estado->reingreso.desaceleracion_g);

                printf("Paracaidas listos: %.1f m/s a %.1f m de altitud.\n",
                       estado->velocidad_ms,
                       estado->altitud_m);
            }
            break;

        case ETAPA_PARACAIDAS:
            printf("Paracaidas desplegados. Llegada al agua a %.2f m/s.\n",
                   estado->velocidad_ms);
            break;

        case ETAPA_AMERIZAJE:
            printf("Energia de impacto: %.2f J\n"
                   "Peso: %.2f N\n"
                   "Fuerza promedio del agua: %.2f N\n"
                   "Multiplo del peso: %.4f\n",
                   estado->amerizaje.energia_impacto_j,
                   estado->amerizaje.peso_n,
                   estado->amerizaje.fuerza_agua_n,
                   estado->amerizaje.multiplo_peso);
            break;

        case ETAPA_FINALIZADA:
        default:
            break;
    }
}

/* Simulacion con configuracion personalizada o predeterminada. */
void ejecutar_simulacion_configurada(const ConfigMision *config)
{
    ConfigMision configuracion;
    EstadoCapsula estado;
    EtapaMision ejecutada;

    if (config == NULL || !config_valida(config)) {
        puts("\nNo se puede iniciar la simulacion: configuracion invalida.");
        return;
    }

    configuracion = *config;

    puts("\nSimulacion por eventos: reingreso, paracaidas y amerizaje.");

    printf("Datos de la mision: masa %.2f kg, entrada a %.2f m/s y %.2f m,"
           " angulo %.2f grados.\n",
           configuracion.masa_kg,
           configuracion.velocidad_entrada_ms,
           configuracion.altitud_entrada_m,
           configuracion.angulo_grados);

    iniciar_estado(&estado, &configuracion);

    while (estado.etapa != ETAPA_FINALIZADA) {
        ejecutada = estado.etapa;

        printf("\nEtapa: %s\n", nombre_etapa(ejecutada));

        if (!pedir_continuar()) {
            estado.resultado = MISION_ABORTADA;
            estado.etapa = ETAPA_FINALIZADA;
            break;
        }

        if (!avanzar_etapa(&estado, &configuracion)) {
            break;
        }

        mostrar_evento(ejecutada, &estado);
    }

    printf("\n%s\n", describir_resultado(estado.resultado));

    if (estado.resultado == MISION_ABORTADA) {
        puts("Implementados: flujo por eventos");
    }
}

/* Mantiene compatibilidad con el menu y las pruebas anteriores. */
void ejecutar_simulacion(void)
{
    ConfigMision config = config_consigna();

    ejecutar_simulacion_configurada(&config);
}
