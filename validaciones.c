#include "validaciones.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int leer_entero(const char *mensaje, int minimo, int maximo, int *salida)
{
    char linea[128];
    char *fin;
    long valor;
    int caracter;

    if (mensaje == NULL || salida == NULL || minimo > maximo) {
        return 0;
    }
    for (;;) {
        fputs(mensaje, stdout);
        fflush(stdout);
        if (fgets(linea, sizeof linea, stdin) == NULL) {
            return 0;
        }
        /* Consumir el resto evita convertir una linea larga en varias opciones. */
        if (strchr(linea, '\n') == NULL && !feof(stdin)) {
            while ((caracter = getchar()) != '\n' && caracter != EOF) {
                /* Descartar el resto de esta linea. */
            }
            if (ferror(stdin)) {
                return 0;
            }
            puts("Entrada demasiado larga. Intente de nuevo.");
            continue;
        }
        errno = 0;
        valor = strtol(linea, &fin, 10);
        if (fin == linea || errno == ERANGE) {
            puts("Ingrese un numero entero valido.");
            continue;
        }
        while (isspace((unsigned char)*fin)) {
            ++fin;
        }
        if (*fin != '\0' || valor < minimo || valor > maximo) {
            printf("Ingrese un entero entre %d y %d.\n", minimo, maximo);
            continue;
        }
        *salida = (int)valor;
        return 1;
    }
}
