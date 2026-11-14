# Revisión y cierre de entrega — 14 de noviembre de 2026

Se cotejó el documento original «Trabajo Final Integrador C.docx», secciones 3 a 9 y escenario 4, con el código, las cabeceras, Makefile, pruebas y README. La verificación técnica no sustituye la evaluación de la cátedra.

## Versión publicada y verificación

La revisión anterior se integró en `main` mediante `8318f81260d424f97d79114a541ee65bb02d10ae`. Se verificó por `git fetch origin main` y se creó un clon limpio desde el repositorio público. El directorio de trabajo original estaba limpio antes del cierre.

En ese clon se ejecutó `make -j2 all build/demo_alexis check` en Linux:

- Compilación C17 con `-Wall -Wextra -Wpedantic -Werror`, sin errores.
- 55 comprobaciones de amerizaje y 67 de flujo/reingreso aprobadas.
- Pruebas de historial y reportes aprobadas.
- 21 pruebas automáticas de menú aprobadas, incluidas entradas personalizadas, riesgos, errores y fronteras de lectura.
- Ejecución de misión de referencia hasta `MISION EXITOSA`, consulta de reporte y salida normal.

Las salidas de referencia incluyeron trabajo atmosférico `-641172350000 J`, desaceleración `27.4375 m/s²`, fuerza del agua `323786.67 N` y relación al peso `3.1769`.

## Cierre del repositorio

- Se agregaron al inicio del README los enlaces al repositorio y al video terminado informado por el equipo.
- Se documentó en `modelo-fisico.md` el fundamento de los incisos a/b/c/d, sus signos, unidades, resultados y límites.
- Se conservó el modelo existente: los 400 s se interpretan como intervalo completo entre las velocidades de entrada y despliegue. No se calcula una aceleración máxima ni una trayectoria continua.
- Se reemplazó el comentario de implementación provisional de `reingreso.c` por la explicación del modelo activo, conservando fórmulas, interfaces y atribuciones.
- Se actualizaron requisitos y textos de planificación que describían como pendientes funcionalidades ya integradas o la grabación del video ya terminado.
- Se completó la descripción de una actividad del registro de IA sin inventar su consulta literal y se registraron las solicitudes y el alcance de esta asistencia.
- Se mantuvieron los registros históricos de uso de IA y pruebas personales; no se atribuyeron verificaciones de la herramienta a estudiantes.

Tras el cierre se volvió a ejecutar la compilación del programa y demo y la suite completa: 55 comprobaciones de amerizaje, 67 de flujo/reingreso, historial y 21 pruebas de menú aprobadas en Linux.

## Confirmación del equipo y entrega formal

El usuario informó «ya verificamos todo». Se registra como confirmación del equipo sobre las verificaciones finales del trabajo, video y aspectos académicos. La visibilidad pública y el historial de Git se verificaron técnicamente; Codex no inspeccionó el contenido audiovisual ni operó el aula virtual.

Enlaces de entrega:

- Repositorio: https://github.com/alexisquatrin-tech/Trabajo-Integrador-C
- Video: https://youtu.be/oiT_YOoU5U0

El envío formal de ambos enlaces se realiza por el aula virtual de la cátedra. El DOCX no contiene una fecha concreta. La consigna exige un video de hasta 5 minutos con aparición y voz real de los integrantes, arquitectura, ejecución exitosa y errores. La eventual defensa individual exige comprender todo el programa.
