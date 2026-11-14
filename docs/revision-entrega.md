# Revisión para la entrega — 14 de noviembre de 2026

Se leyó completo el documento original «Trabajo Final Integrador C.docx» aportado por el usuario y se cotejaron sus secciones 3 a 9 y el escenario 4 con todo el código fuente, cabeceras, Makefile, CI, pruebas y documentación. Esta revisión técnica no acredita aprobación de la cátedra ni revisión personal del grupo.

## Versión revisada y preservación

Se consultó `origin/main` mediante `git fetch origin main`: el último commit remoto observado es `b024e48e0b7a67c6a4ac32efd5cdda02eff6a7ee` (Update Makefile). Coincide con HEAD local; la rama de trabajo es `work`. El entorno ya contenía correcciones locales de una revisión anterior: se respaldaron y se conservaron, sin volver a aplicarlas ni sobrescribir aportes.

Se compiló y ejecutó la suite de una copia exacta de ese commit remoto: 55 comprobaciones de amerizaje, 56 de flujo, pruebas de historial y 11 pruebas de menú aprobadas. Ese resultado corresponde a main sin las correcciones locales; las cifras de abajo corresponden a las fuentes corregidas del directorio de trabajo. La revisión se preparó localmente; su integración posterior en GitHub fue solicitada por el usuario. No se publicó el entorno durante esta revisión.

## Resultado y cambios

La base compila y permite ejecutar el escenario 4 con datos originales o personalizados. Presenta condicionales, repeticiones, structs, punteros, arreglo funcional de registros, modularización, menú, eventos y reportes. Se preservaron los módulos, fórmulas, nombres y atribuciones existentes; no se reescribió el historial Git ni se atribuyeron verificaciones personales a estudiantes.

- Se conectaron al menú las funciones existentes de historial (opción 4) y comparación (5). Antes se almacenaban registros pero no podían consultarse desde el programa.
- Se corrigieron reportes y comparaciones que ignoraban amerizajes con energía cero. Se muestran también todas las entradas y la desaceleración en m/s² y g.
- El registro rechaza estados sin finalizar y configuraciones inválidas; mantiene la capacidad de cinco sin sobrescritura. Las misiones abortadas se conservan y los datos se pierden al cerrar.
- Se alinearon los controles de reingreso con el modelo de descenso y frenado. Las salidas se conservan ante datos inválidos o desbordamientos.
- Se retiró el tope arbitrario de 10^9 m/s en la lectura; se aceptan reales representables y los cálculos verifican resultados finitos. Esto no implica que toda magnitud representable sea físicamente realista.
- Se corrigió la lectura de líneas de capacidad exacta (127 caracteres para números, 63 en pausas) con o sin salto final, y el rechazo de NUL incrustado sin ejecutar prefijos ni avanzar etapas. Se comparte `leer_linea_segura` en validaciones y se actualizaron dependencias del Makefile, incluida la demo. Las nuevas pruebas reprodujeron los fallos antes de aplicar la corrección.
- Se corrigieron mensajes y documentación desactualizados sobre configuración e historial y se ampliaron las pruebas de regresión.

## Evidencia ejecutada

En Linux, GCC 14.2.0, GNU Make 4.4.1 y Python 3.12.14:

- `make clean` seguido de `make -j2 all build/demo_alexis check`: compilación C17 con `-Wall -Wextra -Wpedantic -Werror`; sin advertencias ni fallos.
- 55 comprobaciones de amerizaje y 67 de flujo/reingreso aprobadas; pruebas de historial con aserciones activas aprobadas.
- 21 pruebas Python de menú aprobadas. Incluyen misión personalizada, riesgos de ángulo e impacto, entradas mal formadas, NaN/infinito, desbordamientos, líneas largas, recuperación, incoherencia entre alturas, EOF durante la carga y cada etapa, abortos, repetición, historial vacío/lleno, comparación de impacto cero, abortos excluidos de extremos de impacto, cambios efectivos de masa/duración y errores de cálculo sin resultados ficticios. También cubren líneas de capacidad exacta y NUL incrustado.
- Se copiaron las fuentes versionadas actuales, incluyendo modificaciones de esta revisión, a un directorio temporal sin binarios. Allí se compilaron aplicación, demo y pruebas con AddressSanitizer y UndefinedBehaviorSanitizer y se ejecutó la misma suite: todo pasó, sin diagnósticos. Es evidencia desde fuentes limpias de artefactos, no desde un clon remoto de cambios todavía no publicados.
- La demo en esa copia terminó con `MISION EXITOSA`, fuerza `323786.67 N` y múltiplo `3.1769`.

Esta revisión no ejecutó Windows ni CI remota. Se conserva en README el registro previo de pruebas de Alexis en Windows.

## Modelo físico y decisiones que debe revisar el grupo

El trabajo atmosférico implementado es la diferencia de energía mecánica: `Wroz = (m/2)*(vf²-vi²) + m*g*(hf-hi)`. Con los datos base da `-641172350000 J`; incluye el trabajo de la gravedad. La desaceleración se informa como magnitud positiva `(vi-vf)/dt`: `27.4375 m/s²`, aproximadamente `2.7997 g`. La aceleración con sentido positivo de movimiento sería negativa.

El enunciado dice «fase de mayor desaceleración» de 400 s, pero no aporta otras velocidades para ese subintervalo. El código usa las velocidades de entrada y despliegue para ese intervalo: se mantiene como supuesto explícito y provisional, a revisar por Ezequiel y consultar con la cátedra si corresponde. `CDA.h` se conserva como aporte previo, pero no se enlaza: usa otra convención de signo y 9.81; el programa activo usa 9.80 de forma consistente. `wroz.h` sí se usa desde `reingreso.c`.

Para el agua: `Fagua = mg + m*v²/(2*d)`. Es fuerza media respecto del desplazamiento, con gravedad y masa constantes; no modela el perfil temporal ni la flotación. La ventana angular es inclusiva y las consecuencias son cualitativas. La rapidez de impacto es una entrada, no una predicción de la dinámica de paracaídas. La regla de éxito por impacto <= 8 m/s es una simplificación basada en la referencia de la consigna y no garantiza seguridad estructural. No se añadieron vidas, puntajes ni límites estructurales ajenos al escenario.

## Pendientes para entregar

1. Datos informados por el usuario: Grupo C 34, comisión k1.4, registrados en README. Confirmar que los cinco integrantes figuren correctamente en aula virtual.
2. Ezequiel y el grupo deben revisar y defender los incisos a/b y acordar el supuesto de los 400 s. Cada integrante debe validar personalmente su aporte y comprender el sistema completo, incluidos estos cambios.
3. Completar el registro de uso de IA exclusivamente con herramientas, consultas, adaptación y validación que realmente hayan ocurrido. No atribuir pruebas a estudiantes ni inventar uso de IA de integrantes que no lo hayan utilizado. La evidencia ejecutada por Codex no reemplaza la comprensión individual.
4. Confirmar que el repositorio sea público y que el enlace permita acceder. El historial local registra autores de los cinco integrantes; esto no demuestra por sí solo participación equitativa. Preservar desarrollo progresivo real; ramas y PR son recomendados, no obligatorios.
5. Comprobar la compilación desde el clon de la versión que efectivamente se entregará después de integrar estos cambios.
6. Grabar y revisar video de hasta cinco minutos, con aparición y voz real de todos, arquitectura, caso exitoso y validaciones ante errores. El documento no exige persistir el historial a disco ni un archivo separado llamado reportes.c.
7. Confirmar fecha y procedimiento en aula virtual, subir el enlace al repositorio y al video, y preparar la defensa individual. El documento menciona la fecha indicada por la cátedra, pero no incluye una fecha concreta.

Después de las correcciones no quedan fallos en los casos ejecutados. Los pendientes académicos y la revisión del modelo impiden afirmar que la entrega completa está terminada.
