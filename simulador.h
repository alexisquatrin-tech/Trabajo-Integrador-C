#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stddef.h>

/* Unidades del SI, salvo los angulos, expresados en grados. */
#define GRAVEDAD_TERRESTRE 9.80
#define ANGULO_MINIMO_SEGURO 5.5
#define ANGULO_MAXIMO_SEGURO 7.0

/* Datos del amerizaje. */
typedef struct {
    double masa_kg;
    double velocidad_impacto_ms;
    double hundimiento_m;
} DatosAmerizaje;

typedef struct {
    double energia_impacto_j;
    double peso_n;
    double fuerza_agua_n;
    double multiplo_peso;
} ResultadoAmerizaje;

typedef enum {
    ANGULO_INVALIDO,
    REBOTE_ATMOSFERICO,
    ANGULO_SEGURO,
    DESACELERACION_EXCESIVA
} EstadoReingreso;

/* Funciones del modulo de amerizaje. */
int calcular_amerizaje(const DatosAmerizaje *datos,
                       ResultadoAmerizaje *resultado);

EstadoReingreso clasificar_angulo(double angulo_grados);

const char *describir_angulo(EstadoReingreso estado);

/* ------------------------------------------------------------------
 * Parte 3: flujo, estados y eventos.
 * ------------------------------------------------------------------ */

#define VELOCIDAD_IMPACTO_SEGURA_MS 8.0

/* Configuracion completa de una mision. */
typedef struct {
    double masa_kg;
    double angulo_grados;
    double velocidad_entrada_ms;
    double altitud_entrada_m;
    double velocidad_paracaidas_ms;
    double altitud_paracaidas_m;
    double duracion_frenado_s;
    double velocidad_impacto_ms;
    double hundimiento_m;
} ConfigMision;

/* Datos enviados al modulo de reingreso. */
typedef struct {
    double masa_kg;
    double velocidad_inicial_ms;
    double altitud_inicial_m;
    double velocidad_final_ms;
    double altitud_final_m;
    double duracion_s;
} DatosReingreso;

typedef struct {
    double trabajo_rozamiento_j;
    double desaceleracion_ms2;
    double desaceleracion_g;
} ResultadoReingreso;

/* Funcion del modulo de reingreso. */
int calcular_reingreso(const DatosReingreso *datos,
                       ResultadoReingreso *resultado);

/* Etapas de la mision. */
typedef enum {
    ETAPA_REINGRESO,
    ETAPA_PARACAIDAS,
    ETAPA_AMERIZAJE,
    ETAPA_FINALIZADA
} EtapaMision;

/* Resultados posibles de la mision. */
typedef enum {
    MISION_EN_CURSO,
    MISION_EXITOSA,
    MISION_FALLIDA_REBOTE,
    MISION_FALLIDA_DESACELERACION,
    MISION_FALLIDA_IMPACTO,
    MISION_ABORTADA,
    MISION_DATOS_INVALIDOS,
    MISION_ERROR_CALCULO
} ResultadoMision;

/* Estado actual de la capsula. */
typedef struct {
    EtapaMision etapa;
    ResultadoMision resultado;
    double velocidad_ms;
    double altitud_m;
    double velocidad_impacto_ms;
    EstadoReingreso clasificacion_angulo;
    ResultadoReingreso reingreso;
    ResultadoAmerizaje amerizaje;
} EstadoCapsula;

/* Configuracion original del escenario. */
ConfigMision config_consigna(void);

/* Validacion de la configuracion completa. */
int config_valida(const ConfigMision *config);

/* Inicializacion y avance de estados. */
void iniciar_estado(EstadoCapsula *estado, const ConfigMision *config);

int avanzar_etapa(EstadoCapsula *estado, const ConfigMision *config);

/* Descripciones de etapas y resultados. */
const char *nombre_etapa(EtapaMision etapa);

const char *describir_resultado(ResultadoMision resultado);

/* Simulacion con valores originales. */
void ejecutar_simulacion(void);

/* Simulacion con valores personalizados. */
void ejecutar_simulacion_configurada(const ConfigMision *config);

/* ------------------------------------------------------------------
 * Parte 4: Historial acotado de simulaciones y reportes comparativos.
 * ------------------------------------------------------------------ */

#define CAPACIDAD_HISTORIAL 5

/* Registro individual de una simulacion finalizada. */
typedef struct {
    int id;
    ConfigMision config;
    EstadoCapsula estado_final;
} RegistroSimulacion;

/* Contenedor acotado para almacenar el historial. */
typedef struct {
    RegistroSimulacion registros[CAPACIDAD_HISTORIAL];
    size_t cantidad;
    int siguiente_id;
} HistorialSimulaciones;

/* Operaciones de gestion del historial. */
void iniciar_historial(HistorialSimulaciones *historial);
int historial_vacio(const HistorialSimulaciones *historial);
int historial_lleno(const HistorialSimulaciones *historial);
int registrar_simulacion(HistorialSimulaciones *historial,
                         const ConfigMision *config,
                         const EstadoCapsula *estado);

/* Operaciones de acceso al historial global de la sesion. */
void limpiar_historial_global(void);
const HistorialSimulaciones *obtener_historial_global(void);
void mostrar_reporte_global(void);
void mostrar_comparacion_global(void);

/* Generacion de reportes finales y comparaciones. */
void mostrar_reporte_historial(const HistorialSimulaciones *historial);
void mostrar_comparacion_simulaciones(const HistorialSimulaciones *historial);

#endif
