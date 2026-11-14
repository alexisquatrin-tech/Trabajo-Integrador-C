#include <stdio.h>
#include <float.h>

#include "interfaz.h"
#include "validaciones.h"

static void mostrar_guia_configuracion(void)
{
    printf("\n========================================\n");
    printf("       GUIA DE CONFIGURACION\n");
    printf("========================================\n");

    printf("\n--- Datos de la capsula ---\n");
    printf("Masa: mayor que 0 kg. Referencia: 10400 kg.\n");

    printf("\n--- Datos del reingreso ---\n");
    printf("Angulo: entre 0 y 90 grados.\n");
    printf("  Menor a 5.5 grados: rebote atmosferico.\n");
    printf("  Entre 5.5 y 7.0 grados: ventana segura.\n");
    printf("  Mayor a 7.0 grados: desaceleracion excesiva.\n");
    printf("Velocidad inicial: mayor que 0 m/s.\n");
    printf("Altitud inicial: mayor que la altitud de despliegue.\n");

    printf("\n--- Datos del paracaidas ---\n");
    printf("Velocidad: desde 0 hasta un valor menor que la inicial.\n");
    printf("Altitud: mayor que 0 y menor que la inicial.\n");
    printf("Duracion del frenado: mayor que 0 segundos.\n");

    printf("\n--- Datos del amerizaje ---\n");
    printf("Velocidad de impacto: igual o mayor que 0 m/s.\n");
    printf("Hundimiento: mayor que 0 metros.\n");

    printf("\n--- Valores originales de referencia ---\n");
    printf("Masa: 10400 kg | Angulo: 6 grados\n");
    printf("Velocidad inicial: 11000 m/s\n");
    printf("Altitud inicial: 120000 m\n");
    printf("Velocidad del paracaidas: 25 m/s\n");
    printf("Altitud de despliegue: 2500 m\n");
    printf("Duracion del frenado: 400 segundos\n");
    printf("Velocidad de impacto: 8 m/s\n");
    printf("Hundimiento: 1.50 m\n");

    printf("\n========================================\n");
    printf("IMPORTANTE: los valores fuera de la ventana\n");
    printf("segura del angulo tambien pueden ingresarse\n");
    printf("para analizar posibles fallas de la mision.\n");
    printf("========================================\n");
}

int solicitar_configuracion_mision(ConfigMision *config)
{
    ConfigMision temporal;

    if (config == NULL) {
        return 0;
    }

    mostrar_guia_configuracion();

    printf("\n========================================\n");
    printf("       CONFIGURACION PERSONALIZADA\n");
    printf("========================================\n");
    printf("Ingrese los valores para la nueva mision.\n");
    printf("Los valores entre corchetes son referencias.\n");

    while (1) {
        temporal = config_consigna();

        printf("\n--- Datos de la capsula ---\n");

        if (!leer_real_positivo(
                "Masa de la capsula (kg) [Referencia: 10400]: ",
                &temporal.masa_kg)) {
            return 0;
        }

        if (!leer_real(
                "Angulo de reingreso (grados, de 0 a 90) [Referencia: 6]: ",
                0.0, 90.0, &temporal.angulo_grados)) {
            return 0;
        }

        printf("\n--- Datos del reingreso ---\n");

        if (!leer_real_positivo(
                "Velocidad inicial (m/s) [Referencia: 11000]: ",
                &temporal.velocidad_entrada_ms)) {
            return 0;
        }

        if (!leer_real_positivo(
                "Altitud inicial (m) [Referencia: 120000]: ",
                &temporal.altitud_entrada_m)) {
            return 0;
        }

        printf("\n--- Despliegue del paracaidas ---\n");

        if (!leer_real(
                "Velocidad al desplegar el paracaidas (m/s) [Referencia: 25]: ",
                0.0, DBL_MAX,
                &temporal.velocidad_paracaidas_ms)) {
            return 0;
        }

        if (!leer_real_positivo(
                "Altitud de despliegue (m) [Referencia: 2500]: ",
                &temporal.altitud_paracaidas_m)) {
            return 0;
        }

        if (!leer_real_positivo(
                "Duracion del frenado (segundos) [Referencia: 400]: ",
                &temporal.duracion_frenado_s)) {
            return 0;
        }

        printf("\n--- Datos del amerizaje ---\n");

        if (!leer_real(
                "Velocidad de impacto (m/s) [Referencia: 8]: ",
                0.0, DBL_MAX,
                &temporal.velocidad_impacto_ms)) {
            return 0;
        }

        if (!leer_real_positivo(
                "Hundimiento en el agua (m) [Referencia: 1.50]: ",
                &temporal.hundimiento_m)) {
            return 0;
        }

        if (config_valida(&temporal)) {
            *config = temporal;
            printf("\nConfiguracion ingresada correctamente.\n");
            return 1;
        }

        printf("\nERROR: los datos ingresados no son coherentes.\n");
        printf("\nVerifique las siguientes condiciones:\n");
        printf("- La altitud inicial debe ser mayor que la de despliegue.\n");
        printf("- La velocidad inicial debe ser mayor que la del paracaidas.\n");
        printf("- La altitud de despliegue debe ser mayor que 0.\n");
        printf("- La duracion del frenado debe ser mayor que 0.\n");
        printf("- La masa y el hundimiento deben ser mayores que 0.\n");
        printf("- El angulo debe encontrarse entre 0 y 90 grados.\n");
        printf("\nLos angulos fuera de la ventana segura no son\n");
        printf("datos invalidos: representan posibles fallas de mision.\n");
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

    printf("\nDisponibles: datos personalizados, historial y comparaciones.\n");
    printf("Modelo simplificado: impacto seguro hasta 8 m/s; no garantiza seguridad estructural.\n");
    printf("========================================\n");
}
