#include <stdio.h>

#include "interfaz.h"
#include "validaciones.h"

int solicitar_configuracion_mision(ConfigMision *config)
{
    ConfigMision temporal;

    if (config == NULL) {
        return 0;
    }

    printf("\n========================================\n");
    printf("       CONFIGURACION PERSONALIZADA\n");
    printf("========================================\n");
    printf("Ingrese los valores para la nueva mision.\n\n");

    while (1) {
        temporal = config_consigna();

        printf("\n--- Datos de la capsula ---\n");

        if (!leer_real_positivo("Masa de la capsula (kg): ",
                &temporal.masa_kg)) {
            return 0;
        }

        if (!leer_real("Angulo de reingreso (grados, 0 a 90): ",
                0.0, 90.0, &temporal.angulo_grados)) {
            return 0;
        }

        printf("\n--- Datos del reingreso ---\n");

        if (!leer_real_positivo("Velocidad inicial (m/s): ",
                &temporal.velocidad_entrada_ms)) {
            return 0;
        }

        if (!leer_real_positivo("Altitud inicial (m): ",
                &temporal.altitud_entrada_m)) {
            return 0;
        }

        printf("\n--- Despliegue del paracaidas ---\n");

        if (!leer_real("Velocidad al desplegar el paracaidas (m/s): ",
                0.0, 1000000000.0,
                &temporal.velocidad_paracaidas_ms)) {
            return 0;
        }

        if (!leer_real_positivo("Altitud de despliegue del paracaidas (m): ",
                &temporal.altitud_paracaidas_m)) {
            return 0;
        }

        if (!leer_real_positivo("Duracion del frenado (segundos): ",
                &temporal.duracion_frenado_s)) {
            return 0;
        }

        printf("\n--- Datos del amerizaje ---\n");

        if (!leer_real("Velocidad de impacto (m/s): ",
                0.0, 1000000000.0,
                &temporal.velocidad_impacto_ms)) {
            return 0;
        }

        if (!leer_real_positivo("Hundimiento en el agua (m): ",
                &temporal.hundimiento_m)) {
            return 0;
        }

        if (config_valida(&temporal)) {
            *config = temporal;
            printf("\nConfiguracion ingresada correctamente.\n");
            return 1;
        }

        printf("\nERROR: los datos ingresados no son coherentes.\n");
        printf("Verifique especialmente que:\n");
        printf("- La altitud inicial sea mayor que la de despliegue.\n");
        printf("- La velocidad inicial sea mayor que la del paracaidas.\n");
        printf("- Todos los valores sean compatibles con el escenario.\n");
        printf("\nVuelva a ingresar los datos.\n");
    }
}

void mostrar_parametros_escenario(void)
{
    ConfigMision config = config_consigna();

    printf("\n========================================\n");
    printf("       PARAMETROS DEL ESCENARIO 4\n");
    printf("========================================\n");

    printf("Masa de la capsula: %.2f kg\n", config.masa_kg);
    printf("Angulo de reingreso: %.2f grados\n", config.angulo_grados);
    printf("Velocidad inicial: %.2f m/s\n", config.velocidad_entrada_ms);
    printf("Altitud inicial: %.2f m\n", config.altitud_entrada_m);
    printf("Velocidad al desplegar paracaidas: %.2f m/s\n",
           config.velocidad_paracaidas_ms);
    printf("Altitud de despliegue: %.2f m\n",
           config.altitud_paracaidas_m);
    printf("Duracion del frenado: %.2f segundos\n",
           config.duracion_frenado_s);
    printf("Velocidad de impacto: %.2f m/s\n",
           config.velocidad_impacto_ms);
    printf("Hundimiento en el agua: %.2f m\n",
           config.hundimiento_m);

    printf("\nPendientes: datos personalizados\n");
    printf("========================================\n");
}
