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

/* Base actual: demuestra c y d; el flujo completo por eventos esta pendiente. */
void ejecutar_simulacion(void);

#endif
