#include "simulador.h"
#include "validaciones.h"

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

/* Historial global de la sesion interactiva. */
static HistorialSimulaciones historial_global;
static int historial_global_iniciado = 0;

static void asegurar_historial_global(void)
{
    if (!historial_global_iniciado) {
        iniciar_historial(&historial_global);
        historial_global_iniciado = 1;
    }
}

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
    int lectura;

    while (1) {
        printf("Enter = continuar, 0 = abortar: ");

        lectura = leer_linea_segura(linea, sizeof linea);
        if (lectura == 0) {
            puts(ferror(stdin) ? "\nError al leer: se aborta la mision." :
                                "\nFin de entrada: se aborta la mision.");
            return 0;
        }
        if (lectura < 0) {
            puts(lectura == -1 ? "Error: entrada demasiado larga." :
                                "Error: entrada contiene caracteres invalidos.");
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

/* ------------------------------------------------------------------
 * Parte 4: Historial acotado de simulaciones y reportes comparativos.
 * ------------------------------------------------------------------ */

void iniciar_historial(HistorialSimulaciones *historial)
{
    if (historial == NULL) {
        return;
    }

    memset(historial, 0, sizeof *historial);
    historial->siguiente_id = 1;
}

int historial_vacio(const HistorialSimulaciones *historial)
{
    if (historial == NULL) {
        return 1;
    }

    return historial->cantidad == 0;
}

int historial_lleno(const HistorialSimulaciones *historial)
{
    if (historial == NULL) {
        return 0;
    }

    return historial->cantidad >= CAPACIDAD_HISTORIAL;
}

int registrar_simulacion(HistorialSimulaciones *historial,
                         const ConfigMision *config,
                         const EstadoCapsula *estado)
{
    RegistroSimulacion *reg;

    if (historial == NULL || config == NULL || estado == NULL) {
        return 0;
    }

    if (!config_valida(config) || estado->etapa != ETAPA_FINALIZADA ||
        estado->resultado <= MISION_EN_CURSO ||
        estado->resultado > MISION_ERROR_CALCULO) {
        return 0;
    }

    if (historial_lleno(historial)) {
        return 0;
    }

    reg = &historial->registros[historial->cantidad];
    reg->id = historial->siguiente_id++;
    reg->config = *config;
    reg->estado_final = *estado;
    historial->cantidad++;

    return 1;
}

void limpiar_historial_global(void)
{
    iniciar_historial(&historial_global);
    historial_global_iniciado = 1;
}

const HistorialSimulaciones *obtener_historial_global(void)
{
    asegurar_historial_global();
    return &historial_global;
}

void mostrar_reporte_historial(const HistorialSimulaciones *historial)
{
    size_t i;
    const RegistroSimulacion *r;

    if (historial_vacio(historial)) {
        puts("\n[Reporte] El historial de simulaciones esta vacio.");
        return;
    }

    printf("\n=== REPORTE DE SIMULACIONES (%zu/%d registradas) ===\n",
           historial->cantidad, CAPACIDAD_HISTORIAL);

    for (i = 0; i < historial->cantidad; i++) {
        r = &historial->registros[i];
        printf("\nSimulacion #%d:\n", r->id);
        printf("  Entrada   : Masa %.1f kg | Angulo %.2f deg | Vel. entrada %.1f m/s\n",
               r->config.masa_kg,
               r->config.angulo_grados,
               r->config.velocidad_entrada_ms);
        printf("  Parametros : Altitud entrada %.2f m | Paracaidas %.2f m/s a %.2f m\n"
               "               Frenado %.2f s | Impacto previsto %.2f m/s | Hundimiento %.2f m\n",
               r->config.altitud_entrada_m, r->config.velocidad_paracaidas_ms,
               r->config.altitud_paracaidas_m, r->config.duracion_frenado_s,
               r->config.velocidad_impacto_ms, r->config.hundimiento_m);
        printf("  Resultado : %s\n",
               describir_resultado(r->estado_final.resultado));

        if (r->estado_final.reingreso.desaceleracion_g > 0.0) {
            printf("  Reingreso : Trabajo rozamiento %.2f J | Desaceleracion %.4f m/s^2 (%.2f g)\n",
                   r->estado_final.reingreso.trabajo_rozamiento_j,
                   r->estado_final.reingreso.desaceleracion_ms2,
                   r->estado_final.reingreso.desaceleracion_g);
        }

        if (r->estado_final.resultado == MISION_EXITOSA ||
            r->estado_final.resultado == MISION_FALLIDA_IMPACTO) {
            printf("  Amerizaje : Vel. impacto %.2f m/s | Energia %.2f J | Fuerza agua %.2f N (%.2f x peso)\n",
                   r->estado_final.velocidad_impacto_ms,
                   r->estado_final.amerizaje.energia_impacto_j,
                   r->estado_final.amerizaje.fuerza_agua_n,
                   r->estado_final.amerizaje.multiplo_peso);
        }
    }
}

void mostrar_comparacion_simulaciones(const HistorialSimulaciones *historial)
{
    size_t i;
    size_t exitosas = 0;
    size_t fallidas = 0;
    size_t abortadas = 0;
    const RegistroSimulacion *menor_g = NULL;
    const RegistroSimulacion *mayor_g = NULL;
    const RegistroSimulacion *menor_impacto = NULL;
    const RegistroSimulacion *mayor_fuerza = NULL;
    const RegistroSimulacion *r;

    if (historial_vacio(historial)) {
        puts("\n[Comparacion] No hay simulaciones registradas para comparar.");
        return;
    }

    if (historial->cantidad < 2) {
        puts("\n[Comparacion] Se necesitan al menos 2 simulaciones para comparar resultados.");
        return;
    }

    for (i = 0; i < historial->cantidad; i++) {
        r = &historial->registros[i];

        if (r->estado_final.resultado == MISION_EXITOSA) {
            exitosas++;
        } else if (r->estado_final.resultado == MISION_ABORTADA) {
            abortadas++;
        } else {
            fallidas++;
        }

        if (r->estado_final.reingreso.desaceleracion_g > 0.0) {
            if (menor_g == NULL ||
                r->estado_final.reingreso.desaceleracion_g <
                menor_g->estado_final.reingreso.desaceleracion_g) {
                menor_g = r;
            }
            if (mayor_g == NULL ||
                r->estado_final.reingreso.desaceleracion_g >
                mayor_g->estado_final.reingreso.desaceleracion_g) {
                mayor_g = r;
            }
        }

        if (r->estado_final.resultado == MISION_EXITOSA ||
            r->estado_final.resultado == MISION_FALLIDA_IMPACTO) {
            if (menor_impacto == NULL ||
                r->estado_final.amerizaje.energia_impacto_j <
                menor_impacto->estado_final.amerizaje.energia_impacto_j) {
                menor_impacto = r;
            }
            if (mayor_fuerza == NULL ||
                r->estado_final.amerizaje.fuerza_agua_n >
                mayor_fuerza->estado_final.amerizaje.fuerza_agua_n) {
                mayor_fuerza = r;
            }
        }
    }

    puts("\n=== COMPARACION ENTRE SIMULACIONES ===");
    printf("Total analizadas: %zu (Exitosas: %zu | Fallidas: %zu | Abortadas: %zu)\n",
           historial->cantidad, exitosas, fallidas, abortadas);

    if (menor_g != NULL && mayor_g != NULL) {
        printf("- Menor desaceleracion en reingreso : Sim #%d con %.2f g (angulo %.2f deg)\n",
               menor_g->id,
               menor_g->estado_final.reingreso.desaceleracion_g,
               menor_g->config.angulo_grados);
        printf("- Mayor desaceleracion en reingreso : Sim #%d con %.2f g (angulo %.2f deg)\n",
               mayor_g->id,
               mayor_g->estado_final.reingreso.desaceleracion_g,
               mayor_g->config.angulo_grados);
    }

    if (menor_impacto != NULL && mayor_fuerza != NULL) {
        printf("- Menor energia de impacto en agua  : Sim #%d con %.2f J (vel. %.2f m/s)\n",
               menor_impacto->id,
               menor_impacto->estado_final.amerizaje.energia_impacto_j,
               menor_impacto->estado_final.velocidad_impacto_ms);
        printf("- Mayor fuerza de frenado en agua   : Sim #%d con %.2f N (hundimiento %.2f m)\n",
               mayor_fuerza->id,
               mayor_fuerza->estado_final.amerizaje.fuerza_agua_n,
               mayor_fuerza->config.hundimiento_m);
    }
}

void mostrar_reporte_global(void)
{
    asegurar_historial_global();
    mostrar_reporte_historial(&historial_global);
}

void mostrar_comparacion_global(void)
{
    asegurar_historial_global();
    mostrar_comparacion_simulaciones(&historial_global);
}

/* Simulacion con configuracion personalizada o predeterminada. */
void ejecutar_simulacion_configurada(const ConfigMision *config)
{
    ConfigMision configuracion;
    EstadoCapsula estado;
    EtapaMision ejecutada;

    asegurar_historial_global();

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

    if (registrar_simulacion(&historial_global, &configuracion, &estado)) {
        printf("Simulacion guardada en el historial (%zu/%d).\n",
               historial_global.cantidad, CAPACIDAD_HISTORIAL);
    } else {
        puts("Aviso: Historial lleno. No se pudo registrar esta simulacion.");
    }
}

/* Mantiene compatibilidad con el menu y las pruebas anteriores. */
void ejecutar_simulacion(void)
{
    ConfigMision config = config_consigna();

    ejecutar_simulacion_configurada(&config);
}
