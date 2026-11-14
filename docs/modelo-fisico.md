# Modelo físico del escenario 4

El programa permite variar las entradas del escenario de reingreso y amerizaje. Utiliza unidades SI, ángulos en grados, masa constante y gravedad uniforme de 9,80 m/s². No integra una trayectoria ni calcula calentamiento, flotación o cargas máximas.

## a. Trabajo del rozamiento atmosférico

Por trabajo y energía, `Delta K = Wpeso + Wroz`, con `Wpeso = mg(hi-hf)`. Por lo tanto:

```text
Wroz = 0,5*m*(vf²-vi²) + m*g*(hf-hi) = Delta(K + U)
```

`wroz.h`, utilizado por `reingreso.c`, implementa esa diferencia de energía mecánica. El trabajo es negativo porque el rozamiento retira energía. No debe confundirse con su magnitud positiva ni con la variación de energía cinética sola: durante el descenso también trabaja la gravedad.

## b. Desaceleración media

```text
a_media = (vf-vi)/dt
magnitud_desaceleracion = (vi-vf)/dt
multiplo_g = magnitud_desaceleracion/g
```

La consigna menciona 400 s para la fase de mayor desaceleración y no da velocidades adicionales para ese subintervalo. El modelo implementado toma esos 400 s como el intervalo de reducción entre 11000 y 25 m/s. Es un supuesto de cálculo explícito: no demuestra aceleración constante ni permite obtener el pico de carga. El programa informa una magnitud positiva; la aceleración en el sentido inicial del movimiento es negativa. Si se elige otro intervalo, deben ingresarse velocidades y duración compatibles con él.

## c. Fuerza promedio del agua

Durante el hundimiento vertical, `mg*d - Fagua*d = -0,5*m*v²`. Se obtiene:

```text
Fagua = mg + m*v²/(2*d)
multiplo_peso = Fagua/(mg)
```

Es la fuerza del agua promediada respecto del desplazamiento de detención. Incluye el peso, que favorece el descenso. No es la fuerza neta ni una fuerza máxima; no describe el movimiento posterior de flotación.

## d. Ángulo y reglas de misión

| Ángulo de entrada | Clasificación |
| --- | --- |
| Menor que 5,5° | Riesgo de rebote atmosférico; finaliza por riesgo. |
| Entre 5,5° y 7,0°, incluidos | Se permite avanzar. |
| Mayor que 7,0° | Riesgo de desaceleración excesiva; finaliza por riesgo. |

La clasificación es cualitativa. Se admiten entradas angulares de 0° a 90°; fuera de ese dominio se rechazan. La velocidad de impacto es una entrada, no una predicción de la dinámica del paracaídas. El éxito requiere completar las etapas con ángulo dentro de la ventana y rapidez de impacto de hasta 8 m/s, referencia adoptada como regla simplificada. No garantiza seguridad estructural.

## Resultados de referencia

| Magnitud | Resultado |
| --- | --- |
| Trabajo atmosférico | −641172350000 J |
| Aceleración media con signo | −27,4375 m/s² |
| Magnitud de desaceleración | 27,4375 m/s² ≈ 2,7997 g |
| Energía cinética de impacto | 332800 J |
| Peso | 101920 N |
| Fuerza promedio del agua | 323786,6667 N |
| Fuerza del agua / peso | 3,176870748 |

Se usan masa 10400 kg; entrada a 11000 m/s y 120000 m; despliegue a 25 m/s y 2500 m; intervalo 400 s; impacto 8 m/s; hundimiento 1,50 m. El ángulo predeterminado de 6° pertenece a la ventana de seguridad.

## Implementación y defensa

`main.c` coordina el menú; `interfaz.c` carga una `ConfigMision`; `validaciones.c` convierte líneas completas y rechaza errores de tipo, rango y representación. `simulador.c` valida la configuración, actualiza una `EstadoCapsula` por puntero y llama a los cálculos. Los resultados se calculan en estructuras temporales y solo se copian a la salida si son válidos y finitos.

El historial utiliza un arreglo de cinco `RegistroSimulacion`, un contador de tipo `size_t` y comprobación de capacidad antes de insertar. Se conserva en memoria durante la sesión; al llenarse no sobrescribe registros. Los punteros permiten actualizar estructuras y acceder a ellas sin duplicarlas; los parámetros `const` señalan datos de consulta. No se utiliza memoria dinámica.

`CDA.h` es un aporte previo no compilado por el Makefile. Su convención de signo y gravedad 9,81 no definen las salidas del programa activo. Los integrantes deben poder explicar estas decisiones y el flujo completo durante el coloquio.
