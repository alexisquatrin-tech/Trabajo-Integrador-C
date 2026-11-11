#include "simulador.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Parte 3 (Nico): flujo, estados y eventos.
 *
 * Idea general: la capsula tiene un ESTADO (EstadoCapsula) que cambia a
 * medida que ocurren EVENTOS. Cada llamada a avanzar_etapa() ejecuta el
 * evento de la etapa actual (reingreso -> paracaidas -> amerizaje), llama a
 * las funciones de calculo de los otros modulos y deja el estado listo para
 * la etapa siguiente. ejecutar_simulacion() solo pide "continuar" al usuario
 * entre evento y evento y muestra lo que paso.
 */

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
    /* Coherencia: se entra mas rapido y mas alto de lo que se abre el
     * paracaidas, que se abre sobre el nivel del mar. */
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

/* Cierra la mision con un resultado y marca el fin del flujo. */
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
            estado->clasificacion_angulo = clasificar_angulo(config->angulo_grados);
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
            datos_reingreso.velocidad_inicial_ms = config->velocidad_entrada_ms;
            datos_reingreso.altitud_inicial_m = config->altitud_entrada_m;
            datos_reingreso.velocidad_final_ms = config->velocidad_paracaidas_ms;
            datos_reingreso.altitud_final_m = config->altitud_paracaidas_m;
            datos_reingreso.duracion_s = config->duracion_frenado_s;
            if (!calcular_reingreso(&datos_reingreso, &estado->reingreso)) {
                finalizar(estado, MISION_ERROR_CALCULO);
                return 0;
            }
            /* Al terminar el frenado la capsula esta en el punto de apertura. */
            estado->velocidad_ms = config->velocidad_paracaidas_ms;
            estado->altitud_m = config->altitud_paracaidas_m;
            estado->etapa = ETAPA_PARACAIDAS;
            return 1;

        case ETAPA_PARACAIDAS:
            /* La rapidez de impacto es un dato de la consigna: no se modela
             * la dinamica del paracaidas, solo el estado al llegar al agua. */
            estado->velocidad_ms = config->velocidad_impacto_ms;
            estado->altitud_m = 0.0;
            estado->etapa = ETAPA_AMERIZAJE;
            return 1;

        case ETAPA_AMERIZAJE:
            datos_amerizaje.masa_kg = config->masa_kg;
            datos_amerizaje.velocidad_impacto_ms = estado->velocidad_ms;
            datos_amerizaje.hundimiento_m = config->hundimiento_m;
            if (!calcular_amerizaje(&datos_amerizaje, &estado->amerizaje)) {
                finalizar(estado, MISION_ERROR_CALCULO);
                return 0;
            }
            if (estado->velocidad_ms <= VELOCIDAD_IMPACTO_SEGURA_MS) {
                finalizar(estado, MISION_EXITOSA);
            } else {
                finalizar(estado, MISION_FALLIDA_IMPACTO);
            }
            return 1;

        case ETAPA_FINALIZADA:
        default:
            return 0;
    }
}

/* ---- Parte interactiva: pausas y mensajes ---- */

/* Devuelve 1 si el usuario quiere continuar (Enter) y 0 si aborta
 * (escribe 0 o se acaba la entrada). Repite ante cualquier otra cosa. */
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

/* Muestra lo que ocurrio en la etapa que se acaba de ejecutar. */
static void mostrar_evento(EtapaMision ejecutada, const EstadoCapsula *estado)
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
                       estado->velocidad_ms, estado->altitud_m);
            }
            break;
        case ETAPA_PARACAIDAS:
            printf("Paracaidas desplegados. Llegada al agua a %.2f m/s.\n",
                   estado->velocidad_ms);
            break;
        case ETAPA_AMERIZAJE:
            printf("Energia de impacto: %.2f J\nPeso: %.2f N\n"
                   "Fuerza promedio del agua: %.2f N\nMultiplo del peso: %.4f\n",
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

void ejecutar_simulacion(void)
{
    const ConfigMision config = config_consigna();
    EstadoCapsula estado;
    EtapaMision ejecutada;

    puts("\nSimulacion por eventos: reingreso, paracaidas y amerizaje.");
    printf("Datos de la consigna: masa %.0f kg, entrada a %.0f m/s y %.0f m,"
           " angulo %.1f grados.\n",
           config.masa_kg, config.velocidad_entrada_ms,
           config.altitud_entrada_m, config.angulo_grados);

    iniciar_estado(&estado, &config);
    while (estado.etapa != ETAPA_FINALIZADA) {
        ejecutada = estado.etapa;
        printf("\nEtapa: %s\n", nombre_etapa(ejecutada));
        if (!pedir_continuar()) {
            estado.resultado = MISION_ABORTADA;
            estado.etapa = ETAPA_FINALIZADA;
            break;
        }
        if (!avanzar_etapa(&estado, &config)) {
            break;
        }
        mostrar_evento(ejecutada, &estado);
    }
    printf("\n%s\n", describir_resultado(estado.resultado));
}
