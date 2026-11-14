# Integración del flujo, entradas e historial

Estado al 14/11/2026. Se conservan los aportes de Nico (flujo), Ignacio (entradas), Alex (historial/reportes), Ezequiel (reingreso) y Alexis (amerizaje).

`main.c` ofrece misión de referencia (1), parámetros (2), personalizada (3), reportes (4), comparación (5) y salida (0). `interfaz.c` carga nueve magnitudes en una estructura temporal y solo la entrega al validar todo. `config_valida` comprueba las relaciones; si fallan se repite la carga. EOF cancela la carga y permite cerrar el programa.

Ambas misiones usan `ejecutar_simulacion_configurada`. `ejecutar_simulacion` conserva la compatibilidad con la demo. Enter avanza cada etapa y 0 aborta. `EstadoCapsula.velocidad_impacto_ms` conserva la rapidez de llegada aunque al finalizar se detenga la cápsula.

El historial de `simulador.c/.h` admite cinco registros finalizados por sesión, también abortados. Al llenarse rechaza nuevas inserciones sin sobrescribir las anteriores; la simulación puede continuar. No hay persistencia en disco. Reportes y comparación reutilizan las funciones globales existentes.

Pruebas: `make all build/demo_alexis check`. Pasan 55 comprobaciones de amerizaje, 67 de flujo/reingreso, las pruebas de historial y 21 pruebas de menú, incluidas carga personalizada, riesgos, cancelación, EOF, desbordamientos, recuperación de entradas, historial lleno y energía cero.

El modelo no calcula trayectoria ni dinámica de paracaídas. La rapidez de impacto es una entrada y el umbral de 8 m/s es una regla simplificada. El alcance del intervalo de 400 s requiere revisión de Ezequiel y, si corresponde, consulta a la cátedra. Ver [revisión de entrega](revision-entrega.md).
