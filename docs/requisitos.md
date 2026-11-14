# Requisitos de la consigna y evidencia

Fuente: documento original «Trabajo Final Integrador C.docx», aportado por el usuario y leído completo en la revisión del 14/11/2026. Lista de seguimiento del escenario 4; no sustituye el documento de la cátedra. «Parcial» no significa que el requisito esté aprobado.

| Requisito | Estado actual | Evidencia o trabajo pendiente |
| --- | --- | --- |
| Grupo de 3 a 5, conformado en aula virtual | Pendiente de verificación | Los cinco nombres y legajos están registrados. Grupo C 34, comisión k1.4, informados por el usuario; falta verificar el alta de los cinco integrantes en aula virtual. |
| C nativo por consola | Base verificada en Linux | `make run`; sin servicios externos. Windows tiene un registro personal de Alexis en README; esta revisión se ejecutó en Linux. |
| Condicionales y repeticiones | Presentes en la base | Menú, validaciones y recorrido de ángulos. Flujo por eventos integrado. |
| Modularización .c/.h | Presente en la base | `main.c`, `simulador.h/.c`, `validaciones.h/.c`, `amerizaje.c`. |
| Estructuras y punteros | Presentes | Datos y resultados de amerizaje y actualización por puntero. |
| Arreglos funcionales | Implementados y probados | Historial acotado de cinco registros, consulta y comparación desde el menú. |
| Validación exhaustiva y manejo de errores | Casos representativos probados | Enteros/reales, rangos, relaciones, fin de entrada, líneas largas y de capacidad exacta, NUL, NaN/infinito, desbordamientos y capacidad. No demuestra ausencia de todos los errores posibles. |
| Menú principal | Integrado | Referencia, parámetros, personalizada, historial, comparación y salida. |
| Simulación por turnos o eventos | Implementada con datos fijos y personalizados | Flujo de Nico integrado: Enter avanza, 0 aborta. |
| Reportes finales de desempeño | Integrados y probados | Opción 4: entradas/resultados; opción 5: comparación, incluyendo energía cero. |
| Escenario 4 y variación de entradas | Parcial | Entrada personalizada y a/b/c/d conectados al flujo; falta revisión estudiantil definitiva de a/b. |
| Inciso a: trabajo atmosférico | Provisional | Adaptador agregado por Nico; revisar con Ezequiel. |
| Inciso b: desaceleración y g | Provisional | Revisar con Ezequiel el supuesto del intervalo de 400 s. |
| Inciso c: fuerza del agua y peso | Implementado y probado | `amerizaje.c`, 323.786,67 N y 3,1769 con valores base; Registro de verificación personal de Alexis en README; falta defensa integral. |
| Inciso d: ventana y consecuencias | Implementado y probado | Ventana inclusiva y mensajes cualitativos. |
| Repositorio público | Configuración externa por verificar | El propietario comprueba visibilidad en GitHub. |
| Historial progresivo de los cinco | Evidencia local presente | Git registra autores de los cinco integrantes; confirmar identidad, participación real y revisión compartida. No se evalúa equidad contando commits. |
| Ramas y Pull Requests | Recomendación preparada | `CONTRIBUTING.md` y plantilla de PR. |
| Makefile o scripts | Implementado y probado | Compilación estricta de ejecutables separados. |
| README completo | Actualizado, datos académicos pendientes | Flujo, reglas, Grupo C 34 y comisión k1.4 documentados; resta completar el registro de IA únicamente con usos y verificaciones reales. |
| Registro de IA | En curso | README con consultas, tareas y validación de la herramienta; cada estudiante debe completar su revisión. |
| Video de hasta 5 minutos | Pendiente | Aparición y voz real de todos, arquitectura, ejecución exitosa y errores. |
| Defensa individual del sistema | Pendiente de preparación | Todos deben comprender todo el código, no solo su módulo. |
| Entrega por aula virtual | Pendiente | Confirmar fecha y procedimiento en aula; no se especifica una fecha en el PDF. |

## Condiciones para considerar lista la entrega

- Compilar desde un clon limpio y ejecutar el flujo completo con entradas personalizadas.
- Verificar un caso exitoso y escenarios peligrosos sin inventar modelos no documentados.
- Probar entradas mal formadas, rangos, límites, fin de entrada e historial lleno.
- Completar y revisar los datos y documentos del equipo.
- Confirmar visibilidad pública y aportes progresivos de todos.
- Revisar el video y la comprensión compartida antes de enviar el enlace por aula virtual.

La evidencia actual y los límites de esta revisión figuran en [revision-entrega.md](revision-entrega.md).
