
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#define TAM_BUFFER 128


int leerEntero(const char *mensaje, int *valor, int minimo, int maximo);

int leerReal(const char *mensaje, double *valor,
             double minimo, double maximo);

int leerRealPositivo(const char *mensaje, double *valor);



// FUNCIONES AUXILIARES

// Elimina los espacios al final de una cadena.
static void quitarEspaciosFinales(char *cadena)
{
    int longitud = strlen(cadena);

    while (longitud > 0 &&
           isspace((unsigned char)cadena[longitud - 1]))
    {
        cadena[longitud - 1] = '\0';
        longitud--;
    }
}


// Lee una linea completa desde teclado.
static int leerLinea(char *buffer, int tamanio)
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

    quitarEspaciosFinales(buffer);

    return 1;
}


// Verifica si la cadena esta vacia.
static int cadenaVacia(const char *cadena)
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

int leerEntero(const char *mensaje, int *valor,
               int minimo, int maximo)
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

        estado = leerLinea(buffer, TAM_BUFFER);

        if (estado == 0)
        {
            printf("\nFin de la entrada.\n");
            return 0;
        }

        if (estado == -1)
        {
            printf("Error: entrada demasiado larga.\n");
            continue;
        }

        if (cadenaVacia(buffer))
        {
            printf("Error: no puede dejar el campo vacio.\n");
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
            printf("Error: debe ingresar un numero entero valido.\n");
            continue;
        }

        if (numero < minimo || numero > maximo)
        {
            printf("Error: el valor debe estar entre %d y %d.\n",
                   minimo, maximo);
            continue;
        }

        *valor = (int)numero;

        return 1;
    }
}



// VALIDACION DE NUMEROS REALES

// Acepta numeros dentro de un rango inclusivo.
int leerReal(const char *mensaje, double *valor,
             double minimo, double maximo)
{
    char buffer[TAM_BUFFER];
    char *fin;
    double numero;
    int estado;

    if (valor == NULL || minimo > maximo)
    {
        printf("Error interno en la validacion.\n");
        return 0;
    }

    while (1)
    {
        printf("%s", mensaje);

        estado = leerLinea(buffer, TAM_BUFFER);

        if (estado == 0)
        {
            printf("\nFin de la entrada.\n");
            return 0;
        }

        if (estado == -1)
        {
            printf("Error: entrada demasiado larga.\n");
            continue;
        }

        if (cadenaVacia(buffer))
        {
            printf("Error: no puede dejar el campo vacio.\n");
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
            printf("Error: debe ingresar un numero real valido.\n");
            continue;
        }

        if (numero < minimo || numero > maximo)
        {
            printf("Error: el valor debe estar entre %.6g y %.6g.\n",
                   minimo, maximo);
            continue;
        }

        *valor = numero;

        return 1;
    }
}



// VALIDACION DE NUMEROS ESTRICTAMENTE POSITIVOS

// Acepta cualquier numero mayor que cero.
int leerRealPositivo(const char *mensaje, double *valor)
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

        estado = leerLinea(buffer, TAM_BUFFER);

        if (estado == 0)
        {
            printf("\nFin de la entrada.\n");
            return 0;
        }

        if (estado == -1)
        {
            printf("Error: entrada demasiado larga.\n");
            continue;
        }

        if (cadenaVacia(buffer))
        {
            printf("Error: no puede dejar el campo vacio.\n");
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
            printf("Error: debe ingresar un numero real valido.\n");
            continue;
        }

        if (numero <= 0)
        {
            printf("Error: el valor debe ser mayor que cero.\n");
            continue;
        }

        *valor = numero;

        return 1;
    }
}



// PROGRAMA PRINCIPAL DE PRUEBA

int main()
{
    int opcion;
    double masa;
    double angulo;

    printf("------------------------------------\n");
    printf("       ARTEMIS - PRUEBA DE DATOS\n");
    printf("------------------------------------\n\n");

    // PRUEBA DE ENTEROS

    if (!leerEntero("Ingrese una opcion (1 a 3): ",
                    &opcion, 1, 3))
    {
        return 0;
    }

    printf("Opcion ingresada: %d\n\n", opcion);


    // PRUEBA DE NUMEROS POSITIVOS

    if (!leerRealPositivo("Ingrese la masa de la capsula (kg): ",
                         &masa))
    {
        return 0;
    }

    printf("Masa ingresada: %.6f kg\n\n", masa);


    // PRUEBA DE NUMEROS QUE PUEDEN SER CERO

    if (!leerReal("Ingrese el angulo de reingreso (0 a 180 grados): ",
                  &angulo, 0.0, 180.0))
    {
        return 0;
    }

    printf("Angulo ingresado: %.2f grados\n", angulo);

    printf("   TODAS LAS VALIDACIONES CORRECTAS\n");

    return 0;
}
