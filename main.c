#include "simulador.h"
#include "validaciones.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int opcion;

    puts("ARTEMIS - Escenario 4 | Base de desarrollo del equipo");
    do {
        puts("\n1. Simulacion por eventos (datos de la consigna)");
        puts("2. Consultar estado del desarrollo");
        puts("0. Salir");
        if (!leer_entero("Opcion: ", 0, 2, &opcion)) {
            if (ferror(stdin)) {
                fputs("Error al leer la entrada.\n", stderr);
                return EXIT_FAILURE;
            }
            puts("\nFin de entrada. Hasta luego.");
            return EXIT_SUCCESS;
        }
        switch (opcion) {
            case 1:
                ejecutar_simulacion();
                break;
            case 2:
                puts("Implementados: flujo por eventos, calculos a/b provisionales y c/d.");
                puts("Pendientes: datos personalizados, revision de a/b e historial/reportes.");
                break;
            case 0:
                puts("Hasta luego.");
                break;
        }
    } while (opcion != 0);
    return EXIT_SUCCESS;
}
