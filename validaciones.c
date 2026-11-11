
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#include "validaciones.h"

#define TAM_BUFFER 128


// FUNCIONES AUXILIARES

// Elimina los espacios al final de una cadena.
static void quitar_espacios_finales(char *cadena)
{
    size_t longitud = strlen(cadena);

    while (longitud > 0 &&
           isspace((unsigned char)cadena[longitud - 1]))
    {
        cadena[longitud - 1] = '\0';
        longitud--;
    }
}


// Lee una linea completa desde teclado.
static int leer_linea(char *buffer, int tamanio)
{
    int caracter;

    if (fgets(buffer, tamanio, stdin) == NULL)
    {
        return 0;
    }

    // Verifica si la entrada supera el tamaño del buffer.
    if (strchr(buffer, '\n') == NULL && !feof(stdin))
    {
        while ((caracter = getchar()) != '\n' && caracter != EOF)
        {
        }

        return -1;
    }

    quitar_espacios_finales(buffer);

    return 1;
}


// Verifica si la cadena esta vacia.
static int cadena_vacia(const char *cadena)
{
    while (*cadena != '\0')
    {
        if (!isspace((unsigned char)*cadena))
        {
            return 0;
        }

        cadena++;
    }

    return 1;
}


// VALIDACION DE NUMEROS ENTEROS

int leer_entero(const char *mensaje, int minimo,
                int maximo, int *valor)
{
    char buffer[TAM_BUFFER];
    char *fin;
    long numero;
    int estado;

    if (valor == NULL || minimo > maximo)
    {
        printf("Error interno en la validacion.\n");
        return 0;
    }

    while (1)
    {
        printf("%s", mensaje);

        estado = leer_linea(buffer, TAM_BUFFER);

        if (estado == 0)
        {
            return 0;
        }

        if (estado == -1)
        {
            printf("Entrada demasiado larga.\n");
            continue;
        }

        if (cadena_vacia(buffer))
        {
            printf("Ingrese un numero entero valido.\n");
            continue;
        }

        errno = 0;
        numero = strtol(buffer, &fin, 10);

        while (isspace((unsigned char)*fin))
        {
            fin++;
        }

        if (buffer == fin || *fin != '\0' ||
            errno == ERANGE ||
            numero < INT_MIN || numero > INT_MAX)
        {
            printf("Ingrese un numero entero valido.\n");
            continue;
        }

        if (numero < minimo || numero > maximo)
        {
            printf("Ingrese un numero entero valido entre %d y %d.\n",
                   minimo, maximo);
            continue;
        }

        *valor = (int)numero;

        return 1;
    }
}


// VALIDACION DE NUMEROS REALES

// Acepta numeros dentro de un rango inclusivo.
int leer_real(const char *mensaje, double minimo,
              double maximo, double *valor)
{
    char buffer[TAM_BUFFER];
    char *fin;
    double numero;
    int estado;

    if (valor == NULL || minimo > maximo ||
        !isfinite(minimo) || !isfinite(maximo))
    {
        printf("Error interno en la validacion.\n");
        return 0;
    }

    while (1)
    {
        printf("%s", mensaje);

        estado = leer_linea(buffer, TAM_BUFFER);

        if (estado == 0)
        {
            return 0;
        }

        if (estado == -1)
        {
            printf("Entrada demasiado larga.\n");
            continue;
        }

        if (cadena_vacia(buffer))
        {
            printf("Ingrese un numero real valido.\n");
            continue;
        }

        errno = 0;
        numero = strtod(buffer, &fin);

        while (isspace((unsigned char)*fin))
        {
            fin++;
        }

        if (buffer == fin || *fin != '\0' ||
            errno == ERANGE || !isfinite(numero))
        {
            printf("Ingrese un numero real valido.\n");
            continue;
        }

        if (numero < minimo || numero > maximo)
        {
            printf("Ingrese un numero real valido entre %.6g y %.6g.\n",
                   minimo, maximo);
            continue;
        }

        *valor = numero;

        return 1;
    }
}


// VALIDACION DE NUMEROS POSITIVOS

// Acepta cualquier numero estrictamente mayor que cero.
int leer_real_positivo(const char *mensaje, double *valor)
{
    char buffer[TAM_BUFFER];
    char *fin;
    double numero;
    int estado;

    if (valor == NULL)
    {
        printf("Error interno en la validacion.\n");
        return 0;
    }

    while (1)
    {
        printf("%s", mensaje);

        estado = leer_linea(buffer, TAM_BUFFER);

        if (estado == 0)
        {
            return 0;
        }

        if (estado == -1)
        {
            printf("Entrada demasiado larga.\n");
            continue;
        }

        if (cadena_vacia(buffer))
        {
            printf("Ingrese un numero real valido.\n");
            continue;
        }

        errno = 0;
        numero = strtod(buffer, &fin);

        while (isspace((unsigned char)*fin))
        {
            fin++;
        }

        if (buffer == fin || *fin != '\0' ||
            errno == ERANGE || !isfinite(numero))
        {
            printf("Ingrese un numero real valido.\n");
            continue;
        }

        if (numero <= 0.0)
        {
            printf("Ingrese un numero mayor que cero.\n");
            continue;
        }

        *valor = numero;

        return 1;
    }
}
