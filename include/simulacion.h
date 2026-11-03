#ifndef SIMULACION_H
#define SIMULACION_H

typedef struct {
    double masa_kg;
    double velocidad_inicial_ms;
    double velocidad_final_ms;
} ReingresoInput;

double calcular_energia_cinetica(double masa_kg, double velocidad_ms);
double calcular_trabajo_frenado(double masa_kg, double velocidad_inicial_ms, double velocidad_final_ms);
void imprimir_resumen_reingreso(const ReingresoInput *input);

#endif
