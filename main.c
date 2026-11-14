#include <stdio.h>
#include <stdlib.h>

#include "simulador.h"
#include "validaciones.h"
#include "interfaz.h"

int main(void)
{
    int opcion;
    ConfigMision mision;

    do {
        printf("\n========================================\n");
        printf("                 ARTEMIS\n");
        printf("       SIMULADOR DE MISIONES\n");
        printf("========================================\n");
        printf("              MENU PRINCIPAL\n\n");
        printf("1. Iniciar simulacion predeterminada\n");
        printf("2. Consultar parametros del escenario\n");
        printf("3. Iniciar simulacion personalizada\n");
        printf("4. Consultar historial y reportes\n");
        printf("5. Comparar simulaciones\n");
        printf("0. Salir\n\n");

        if (!leer_entero("Seleccione una opcion: ", 0, 5, &opcion)) {
            if (ferror(stdin)) {
                fprintf(stderr, "\nError al leer la entrada.\n");
                return EXIT_FAILURE;
            }

            printf("\nFin de entrada. Cerrando el programa.\n");
            break;
        }

        switch (opcion) {
            case 1:
                mision = config_consigna();
                ejecutar_simulacion_configurada(&mision);
                break;

            case 2:
                mostrar_parametros_escenario();
                break;

            case 3:
                if (solicitar_configuracion_mision(&mision)) {
                    ejecutar_simulacion_configurada(&mision);
                } else {
                    printf("\nNo se pudo cargar la configuracion.\n");
                }
                break;

            case 4:
                mostrar_reporte_global();
                break;

            case 5:
                mostrar_comparacion_global();
                break;

            case 0:
                printf("\nHasta luego.\n");
                break;
        }

    } while (opcion != 0);

    return EXIT_SUCCESS;
}
