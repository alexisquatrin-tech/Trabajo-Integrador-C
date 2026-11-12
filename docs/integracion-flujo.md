# Integración del flujo de Nico con el menú

## Qué quedó integrado

La opción 1 de `main.c` llama a `ejecutar_simulacion()`. Esta función recorre reingreso, paracaídas y amerizaje con `config_consigna()`; el usuario presiona Enter en cada etapa, puede abortar con 0 y vuelve al menú al terminar. La rapidez final queda en 0 y la de impacto se conserva en `EstadoCapsula.velocidad_impacto_ms`.

Se conservó `validaciones.c/.h` de main, sin el segundo main de la versión antigua. `reingreso.c` es provisional y debe revisarse con Ezequiel. La clasificación de éxito por impacto <= 8 m/s es una regla simplificada documentada, no una garantía estructural.

## Para Ignacio: ¿las funciones están listas?

Sí: las funciones de flujo están conectadas al menú y sus pruebas pasan. Las de lectura `leer_entero`, `leer_real` y `leer_real_positivo` también existen; esta integración conserva sus firmas. Todavía falta conectar los datos personalizados.

- `leer_entero(mensaje, minimo, maximo, &valor)` lee opciones.
- `leer_real(mensaje, minimo, maximo, &valor)` permite, por ejemplo, ángulos en [0, 90]. Aceptar también ángulos peligrosos dentro de ese dominio para poder analizarlos.
- `leer_real_positivo(mensaje, &valor)` sirve para masa, duración y hundimiento.
- Después de leer, validar relaciones entre magnitudes con `config_valida`.
- Agregar una función de configuración en el módulo apropiado y permitir que la ejecución reciba la configuración obtenida. Hoy `ejecutar_simulacion(void)` crea sus datos internamente, así que no basta con leer variables locales en main.
- Mantener el flujo y los cálculos fuera de main; coordinar la nueva firma o función de ejecución con Nico y el historial con Alex.
- Conservar el retorno al menú ante finalización/cancelación y terminar sin bucles al llegar a fin de entrada.

`config_valida` comprueba ángulos y varios rangos/relaciones; no deduce la dinámica del paracaídas ni sustituye la revisión de coherencia física de las entradas personalizadas.

## Pruebas de esta integración

Desde la raíz: `make all check`. Resultado en Linux: 55 comprobaciones de amerizaje, 56 de flujo y 11 pruebas del menú correctas. Estas últimas cubren misión completa, repetición, entradas inválidas, cancelación y fin de entrada en las tres etapas. No se verificó Windows.

Prueba manual: `make run`, ingresar 1, Enter tres veces y luego 0 al volver al menú. Deben aparecer 323786.67 N, 3.1769 y MISION EXITOSA. Para abortar: ingresar 1, luego 0 en la primera pausa; debe volver al menú sin indicar éxito.

La integración no significa que el trabajo esté listo para entregar o grabar el caso final: faltan configuración personalizada, historial/reportes, revisión definitiva de a/b y actualización de la documentación final del equipo.
