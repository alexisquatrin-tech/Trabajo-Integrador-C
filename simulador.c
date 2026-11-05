#include "simulador.h"
#include <stdio.h>

/* Punto de integracion para el responsable del flujo por eventos. */
void ejecutar_simulacion(void)
{
    const DatosAmerizaje datos = {10400.0, 8.0, 1.5};
    ResultadoAmerizaje resultado;
    const double angulos[] = {5.0, 5.5, 6.0, 7.0, 7.5};
    size_t i;

    puts("\nDemostracion parcial: amerizaje y angulo (incisos c y d).");
    puts("Datos fijos: masa 10400 kg, impacto 8 m/s, hundimiento 1.50 m.");
    if (!calcular_amerizaje(&datos, &resultado)) {
        puts("No se pudo calcular el amerizaje con los datos disponibles.");
        return;
    }
    printf("Energia de impacto: %.2f J\nPeso: %.2f N\n"
           "Fuerza promedio del agua: %.2f N\nMultiplo del peso: %.4f\n",
           resultado.energia_impacto_j, resultado.peso_n,
           resultado.fuerza_agua_n, resultado.multiplo_peso);
    for (i = 0; i < sizeof angulos / sizeof angulos[0]; ++i) {
        printf("%.1f grados: %s\n", angulos[i],
               describir_angulo(clasificar_angulo(angulos[i])));
    }
    puts("Pendiente: entradas personalizadas, reingreso/frenado, eventos e historial.");
}
