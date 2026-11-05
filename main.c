#include "simulador.h"
#include "validaciones.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int opcion;

    puts("ARTEMIS - Escenario 4 | Base de desarrollo del equipo");
    do {
        puts("\n1. Demostracion de amerizaje y angulo (datos de la consigna)");
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
                puts("Implementados: incisos c/d y menu base con lectura de enteros.");
                puts("Pendientes: a/b, datos personalizados, eventos e historial/reportes.");
                break;
            case 0:
                puts("Hasta luego.");
                break;
        }
    } while (opcion != 0);
    return EXIT_SUCCESS;
}
