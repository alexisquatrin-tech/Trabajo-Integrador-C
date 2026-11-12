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

/* Origen: la consigna habla de un amerizaje a "rapidez final segura" de
 * 8,0 m/s. Se usa como criterio de exito del impacto (a confirmar en equipo). */
#define VELOCIDAD_IMPACTO_SEGURA_MS 8.0

/* Todos los datos de entrada de una mision (SI, angulo en grados).
 * Los valores de la consigna salen de config_consigna(). */
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

/* Contrato con reingreso.c (incisos a y b, responsable: Navarro). */
typedef struct {
    double masa_kg;
    double velocidad_inicial_ms;
    double altitud_inicial_m;
    double velocidad_final_ms;
    double altitud_final_m;
    double duracion_s;
} DatosReingreso;

typedef struct {
    double trabajo_rozamiento_j;   /* Wroz: negativo, el aire frena */
    double desaceleracion_ms2;     /* media durante duracion_s */
    double desaceleracion_g;       /* idem, en multiplos de g */
} ResultadoReingreso;

/* Devuelve 1 si calcula; 0 ante datos invalidos. En error no modifica
 * el resultado. Los punteros no pueden ser NULL. */
int calcular_reingreso(const DatosReingreso *datos,
                       ResultadoReingreso *resultado);

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
    double velocidad_ms;              /* 0 al terminar de detenerse */
    double altitud_m;
    double velocidad_impacto_ms;      /* rapidez al llegar al agua */
    EstadoReingreso clasificacion_angulo;
    ResultadoReingreso reingreso;     /* valido tras la etapa de reingreso */
    ResultadoAmerizaje amerizaje;     /* valido tras la etapa de amerizaje */
} EstadoCapsula;

/* Datos de referencia de la consigna (angulo de ejemplo: 6,0 grados,
 * dentro de la ventana segura; la consigna no da un valor). */
ConfigMision config_consigna(void);

/* Devuelve 1 si todos los datos son finitos y coherentes entre si,
 * incluido el angulo de entrada (entre 0 y 90 grados). */
int config_valida(const ConfigMision *config);

/* Deja la capsula lista para empezar: etapa de reingreso, mision en curso. */
void iniciar_estado(EstadoCapsula *estado, const ConfigMision *config);

/* Ejecuta el evento de la etapa actual, actualiza el estado y pasa a la
 * etapa siguiente. No lee teclado ni imprime.
 * Devuelve 1 si el evento se ejecuto (aunque la mision haya fallado, mirar
 * estado->resultado) y 0 si no pudo ejecutarse (punteros NULL, mision ya
 * finalizada, datos invalidos o error de calculo). */
int avanzar_etapa(EstadoCapsula *estado, const ConfigMision *config);

const char *nombre_etapa(EtapaMision etapa);
const char *describir_resultado(ResultadoMision resultado);

/* Simulacion interactiva por eventos con los datos de la consigna.
 * Es la opcion 1 del menu. */
void ejecutar_simulacion(void);

#endif
