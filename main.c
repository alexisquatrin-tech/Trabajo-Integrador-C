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
        printf("1. Iniciar nueva simulacion\n");
        printf("2. Consultar parametros del escenario\n");
        printf("0. Salir\n\n");

        if (!leer_entero("Seleccione una opcion: ", 0, 2, &opcion)) {
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

            case 0:
                printf("\nHasta luego.\n");
                break;
        }

    } while (opcion != 0);

    return EXIT_SUCCESS;
}
