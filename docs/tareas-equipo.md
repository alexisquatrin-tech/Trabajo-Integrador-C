# Plan de trabajo del grupo

Estado: base compartida. Responsabilidades confirmadas: Ezequiel (Navarro Ezequiel Leandro, legajo 31372), parte 1; Alexis (Quatrin Oliver Alexis Javier, legajo 27880), parte 2; Nico (Julian Nicolas Alvarez Carruega, legajo 29342), parte 3; Ignacio (Juan Ignacio Pabon, legajo 28583), parte 4. Alex (Merlo Alex Gabriel, legajo 24518), distinto de Alexis, realiza la parte 5: historial, reportes y compilación. Ignacio conserva el menú y las validaciones. Los cinco integrantes tienen sus datos y responsabilidades registrados; falta completar la comisión.

## 1. Reingreso y frenado — Ezequiel

Revisar la implementación provisional de `reingreso.c` que Nico agregó para probar el flujo con los incisos a/b. Acordar en `simulador.h` las estructuras y prototipos antes de programar.

- Incluir masa, rapidez inicial/final, altitud inicial/final y duración del intervalo elegido.
- Trabajo atmosférico, con gravedad: `Wroz = (m/2)*(vf² - vi²) - m*g*(hi - hf)`.
- Desaceleración media: documentar la interpretación de los 400 s; si corresponden al intervalo vi → vf, `a = (vf - vi)/dt`, con magnitud `fabs(a)` y múltiplo `fabs(a)/g`.
- Separar aceleración media de fuerza aerodinámica: no son la misma magnitud.
- Rechazar duraciones no positivas, valores no finitos y estados fuera del modelo. No elegir límites arbitrarios sin explicarlos.
- Probar resultados manuales, coherencia de unidades, signos y errores.

Terminado cuando funciones, pruebas y explicación estén revisadas e integradas. La ambigüedad del intervalo de 400 s se debe confirmar con la cátedra.

## 2. Amerizaje y ángulo — Alexis

`amerizaje.c` y su sección de `simulador.h` ya implementados; `tests/test_alexis.c` pasa 55 comprobaciones. Fórmulas y supuestos en `parte-alexis.md`.

Pendiente: revisión personal, acuerdo del modelo físico, conexión con entrada de datos y reportes y prueba dentro del flujo completo. No volver a definir las mismas funciones en `simulador.c`.

## 3. Flujo y estados — Nico

El flujo corregido de Nico ya está integrado en `simulador.c`, con datos fijos. Pasan 56 comprobaciones del flujo y 11 pruebas del menú; falta conectar la configuración personalizada y los reportes.

- Proponer estado y etapas: configuración, reingreso, paracaídas, amerizaje y reporte.
- Permitir avance interactivo por eventos; coordinar cálculo y actualización por punteros.
- Consumir las funciones de a/b y c/d sin duplicar fórmulas.
- Definir qué ocurre ante riesgo de ángulo o error de cálculo, sin afirmar trayectorias cuantitativas no modeladas.
- Probar casos completos, repetición y salida o cancelación.

Terminado cuando una simulación personalizada recorra etapas coherentes y termine con reporte.

## 4. Entradas y navegación — Ignacio Pabon

Ya existen `leer_entero`, `leer_real` y `leer_real_positivo`, y el menú inicia la simulación por eventos. Ver `integracion-flujo.md` para completar la configuración personalizada. Ampliar `validaciones.c/.h` y `main.c`.

- Agregar lectura segura de reales y opciones/caracteres que realmente necesite la interfaz.
- Elegir valores de referencia o personalizados, con unidades visibles.
- Validar tipo, rango y relaciones físicas; no descartar como inválido un ángulo peligroso pero físicamente representable.
- Documentar si se acepta punto, coma o ambos como separador decimal. No aceptar parcialmente una cadena mal formada.
- Tratar entradas vacías, sufijos, líneas largas, fin de entrada y errores de lectura.
- Mantener solo `main`, menú e inicialización en `main.c`.

Terminado cuando el flujo personalizado no se rompe ni entra en bucle ante errores de usuario y existen pruebas representativas.

## 5. Historial, reportes y compilación — Alex (Merlo Alex Gabriel)

Crear `reportes.c/.h` y extender el Makefile existente, coordinando contratos.

- Arreglo acotado de estructuras para resultados de varias simulaciones y contador válido.
- Definir capacidad y comportamiento cuando se llena, sin escribir fuera del arreglo.
- Reporte por simulación con unidades, cálculos a/b/c, clasificación d y advertencias del modelo.
- Consulta y comparación útil del historial. No hace falta guardar archivos: la consigna no lo exige.
- Probar vacío, lleno, varias simulaciones y compilación desde un clon limpio.

Terminado cuando los resultados se almacenan, recuperan y muestran correctamente dentro del programa completo.

## Acuerdos comunes y orden

1. Completar responsables/comisión y acordar interfaces y supuestos físicos.
2. Revisar e integrar esta base en `main` y abrir ramas por tarea.
3. Desarrollar cada parte con pruebas y commits reales.
4. Integrar temprano un recorrido por eventos con todas las partes disponibles.
5. Probar ejecución exitosa, entradas erróneas, fronteras y repetición.
6. Completar README y registro de IA, grabar video de hasta 5 minutos y practicar defensa cruzada.

Cada persona escribe la explicación de su parte. Todos revisan el programa completo y deben poder explicar funciones, estructuras, punteros, arreglos y flujo.
