
#ifndef SIMULADOR_H
#define SIMULADOR_H

/* Unidades del SI, salvo los angulos, expresados en grados. */
#define GRAVEDAD_TERRESTRE 9.80
#define ANGULO_MINIMO_SEGURO 5.5
#define ANGULO_MAXIMO_SEGURO 7.0

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

/* Devuelve 1 si calcula; 0 ante datos invalidos o fuera del rango numerico.
 * Los punteros deben apuntar a objetos validos. NULL se rechaza.
 * En caso de error no modifica el resultado.
 * masa y hundimiento > 0; rapidez >= 0; todos finitos.
 */
int calcular_amerizaje(const DatosAmerizaje *datos,
                       ResultadoAmerizaje *resultado);

/* Magnitud del angulo descendente respecto de la horizontal: [0, 90]. */
EstadoReingreso clasificar_angulo(double angulo_grados);
const char *describir_angulo(EstadoReingreso estado);

/* ------------------------------------------------------------------
 * Parte 3 (Nico): flujo, estados y eventos.
 * ------------------------------------------------------------------ */

/* Rapidez maxima considerada segura para el amerizaje. */
#define VELOCIDAD_IMPACTO_SEGURA_MS 8.0

/* Todos los datos de entrada de una mision (SI, angulo en grados).
 * Los valores originales de la consigna salen de config_consigna(). */
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

/* Contrato con reingreso.c (incisos a y b). */
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

/* Etapas por las que pasa la capsula, en orden. */
typedef enum {
    ETAPA_REINGRESO,
    ETAPA_PARACAIDAS,
    ETAPA_AMERIZAJE,
    ETAPA_FINALIZADA
} EtapaMision;

/* Como termino (o en que punto esta) la mision. */
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

/* Estado de la capsula en cada momento de la simulacion. */
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

/* Configuracion original de la consigna.
 * Se utiliza un angulo de ejemplo de 6 grados porque no se especifica. */
ConfigMision config_consigna(void);

/* Comprueba que todos los datos sean finitos y coherentes. */
int config_valida(const ConfigMision *config);

/* Deja la capsula lista para empezar. */
void iniciar_estado(EstadoCapsula *estado, const ConfigMision *config);

/* Ejecuta el evento de la etapa actual y actualiza el estado. */
int avanzar_etapa(EstadoCapsula *estado, const ConfigMision *config);

const char *nombre_etapa(EtapaMision etapa);
const char *describir_resultado(ResultadoMision resultado);

/* Ejecuta la simulacion con los valores originales de la consigna. */
void ejecutar_simulacion(void);

/* Ejecuta la simulacion utilizando la configuracion recibida.
 * Devuelve el control al menu al finalizar o abortar. */
void ejecutar_simulacion_configurada(const ConfigMision *config);

#endif
