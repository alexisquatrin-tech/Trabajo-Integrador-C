#include <math.h>
#include <stdio.h>

#include "simulacion.h"

double calcular_energia_cinetica(double masa_kg, double velocidad_ms) {
    if (masa_kg <= 0 || velocidad_ms < 0) {
        return NAN;
    }

    return 0.5 * masa_kg * velocidad_ms * velocidad_ms;
}

double calcular_trabajo_frenado(double masa_kg, double velocidad_inicial_ms, double velocidad_final_ms) {
    if (velocidad_final_ms > velocidad_inicial_ms) {
        return NAN;
    }

    return calcular_energia_cinetica(masa_kg, velocidad_final_ms) -
           calcular_energia_cinetica(masa_kg, velocidad_inicial_ms);
}

void imprimir_resumen_reingreso(const ReingresoInput *input) {
    const double energia_inicial_j = calcular_energia_cinetica(input->masa_kg, input->velocidad_inicial_ms);
    const double energia_final_j = calcular_energia_cinetica(input->masa_kg, input->velocidad_final_ms);
    const double trabajo_j = calcular_trabajo_frenado(
        input->masa_kg,
        input->velocidad_inicial_ms,
        input->velocidad_final_ms
    );

    printf("=== Simulación inicial: Reingreso atmosférico ===\n");
    printf("Masa de la cápsula: %.2f kg\n", input->masa_kg);
    printf("Velocidad inicial: %.2f m/s\n", input->velocidad_inicial_ms);
    printf("Velocidad final: %.2f m/s\n", input->velocidad_final_ms);
    printf("Energía cinética inicial: %.2f J\n", energia_inicial_j);
    printf("Energía cinética final: %.2f J\n", energia_final_j);
    printf("Trabajo de frenado: %.2f J\n", trabajo_j);
}
