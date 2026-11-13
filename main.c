
#include <stdio.h>
#include <stdlib.h>

#include "simulador.h"
#include "validaciones.h"
#include "interfaz.h"

int main(void)
{
    int opcion;
    int subopcion;
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

            printf("\nFin de la entrada. Cerrando el programa.\n");
            break;
        }

        switch (opcion) {

            case 1:
                do {
                    printf("\n========================================\n");
                    printf("        CONFIGURACION DE MISION\n");
                    printf("========================================\n");
                    printf("1. Utilizar valores predeterminados\n");
                    printf("2. Ingresar valores personalizados\n");
                    printf("0. Volver al menu principal\n\n");

                    if (!leer_entero("Seleccione una opcion: ",
                                     0, 2, &subopcion)) {
                        if (ferror(stdin)) {
                            fprintf(stderr,
                                    "\nError al leer la entrada.\n");
                            return EXIT_FAILURE;
                        }

                        printf("\nFin de la entrada. Cerrando el programa.\n");
                        return EXIT_SUCCESS;
                    }

                    switch (subopcion) {

                        case 1:
                            mision = config_consigna();

                            printf("\nSe utilizaran los valores originales "
                                   "del escenario.\n");

                            ejecutar_simulacion_configurada(&mision);
                            break;

                        case 2:
                            if (solicitar_configuracion_mision(&mision)) {
                                ejecutar_simulacion_configurada(&mision);
                            } else {
                                if (ferror(stdin)) {
                                    fprintf(stderr,
                                            "\nError al leer la entrada.\n");
                                    return EXIT_FAILURE;
                                }

                                printf("\nCarga de datos interrumpida.\n");
                                return EXIT_SUCCESS;
                            }
                            break;

                        case 0:
                            printf("\nVolviendo al menu principal...\n");
                            break;
                    }

                } while (subopcion != 0);

                break;

            case 2:
                mostrar_parametros_escenario();
                break;

            case 0:
                printf("\nFinalizando el simulador Artemis...\n");
                break;
        }

    } while (opcion != 0);

    return EXIT_SUCCESS;
}
