# Parte de Alexis: amerizaje y ángulo de reingreso

Esta entrega implementa los incisos c y d. Es un módulo para integrar al simulador del grupo, no el trabajo integrador completo.

## 1. Física del amerizaje

Modelo simplificado: movimiento vertical, masa constante, gravedad constante de 9,80 m/s² y detención tras un desplazamiento descendente d. La fuerza calculada es el promedio respecto del desplazamiento, no la fuerza máxima ni una simulación hidrodinámica. Representa la acción total del agua durante la detención; no se añade otra fuerza de flotación separada.

Tomando hacia abajo como sentido positivo, el peso realiza trabajo positivo y el agua negativo:

```
Ki = m * v² / 2
Kf = 0
Wpeso + Wagua = Kf - Ki
m*g*d - Fagua*d = -m*v²/2
Fagua = m*g + m*v²/(2*d)
Multiplo del peso = Fagua/(m*g)
```

Con m = 10.400 kg, v = 8 m/s y d = 1,50 m:

| Magnitud | Resultado |
| --- | --- |
| Energía cinética de impacto | 332.800 J |
| Peso | 101.920 N |
| Fuerza neta promedio de frenado, módulo | 221.866,67 N |
| Fuerza promedio del agua, hacia arriba | 323.786,67 N |
| Fuerza del agua / peso | 3,1769 |

La consigna pide la fuerza del agua: no confundirla con la fuerza neta. Si la cátedra espera despreciar el trabajo del peso, debe explicitarse ese supuesto y revisar el resultado esperado; aquí se incluye. No se calcula una fuerza máxima ni se garantiza la integridad estructural: no se proporcionan límites de resistencia.

Una rapidez cero se acepta como caso límite algebraico: Fagua = peso. No representa un impacto con penetración real.

## 2. Ángulo de reingreso

Se interpreta gamma como la magnitud del ángulo descendente respecto de la horizontal, en grados. Se adopta el dominio geométrico [0, 90]. Los ángulos fuera de ese dominio, NaN e infinito son inválidos; un ángulo físicamente representable pero peligroso debe clasificarse, no rechazarse como error de ingreso.

| Rango | Clasificación y consecuencia cualitativa |
| --- | --- |
| 0 ≤ gamma < 5,5 | Entrada demasiado baja: riesgo de rebote atmosférico; puede no disiparse la energía necesaria para completar el descenso previsto. |
| 5,5 ≤ gamma ≤ 7,0 | Dentro de la ventana de seguridad del enunciado. |
| 7,0 < gamma ≤ 90 | Entrada demasiado pronunciada: frenado más brusco, riesgo de desaceleración excesiva y mayor exigencia térmica y estructural. |

Los límites 5,5 y 7,0 están incluidos. Esta clasificación es cualitativa: no calcula trayectoria, temperatura ni fuerzas en función del ángulo, porque no hay un modelo de atmósfera o de resistencia aerodinámica. La ventana segura no garantiza por sí sola un amerizaje seguro.

## 3. Cómo se organiza el código

- `simulador.h`: constantes, estructuras, enumeración de estados y prototipos públicos. El equipo puede ampliarlo con los otros módulos.
- `amerizaje.c`: funciones de cálculo y clasificación de esta parte. No lee el teclado ni imprime mensajes.
- `simulador.c`: coordina la demostración; se ampliará con el flujo por eventos.
- `main.c`: menú base del equipo.
- `ejemplos/demo_alexis.c`: ejecutable separado que llama a la demostración con valores fijos.
- `tests/test_alexis.c`: pruebas con arreglos de casos y comprobaciones numéricas.
- `Makefile`: compila y ejecuta la demo o las pruebas.

`DatosAmerizaje` agrupa las entradas y `ResultadoAmerizaje` agrupa las salidas. El puntero `const DatosAmerizaje *` permite leer los datos sin modificarlos. El puntero `ResultadoAmerizaje *` permite devolver varios resultados al llamador. No hay memoria dinámica: no hacen falta malloc ni free para este módulo.

`calcular_amerizaje` devuelve 1 si tiene éxito y 0 si falla. Comprueba punteros nulos, valores no finitos, masa y distancia no positivas, rapidez negativa y resultados no representables. Calcula primero en una estructura local y solo escribe la salida al finalizar correctamente. El llamador debe comprobar el retorno antes de usarla.

`clasificar_angulo` devuelve un valor de `EstadoReingreso`; `describir_angulo` devuelve un texto constante que no debe modificarse ni liberarse.

Ejemplo de integración dentro de una función del simulador:

```c
DatosAmerizaje datos = {10400.0, 8.0, 1.5};
ResultadoAmerizaje resultado;
if (calcular_amerizaje(&datos, &resultado)) {
    /* Entregar resultado al módulo de reportes. */
} else {
    /* La interfaz informa el error y solicita corregir los datos. */
}
EstadoReingreso estado = clasificar_angulo(6.0);
/* La interfaz puede mostrar describir_angulo(estado). */
```

## 4. Compilar y verificar

Desde la raíz del repositorio, con GCC y Make:

```sh
make demo
make test
```

La demo usa los datos del enunciado y varios ángulos. Las pruebas verifican valores manuales, balance de energía, proporcionalidad con la masa, efecto de la distancia, límites de la ventana, entradas inválidas y conservación de la salida ante un error. No se reemplazan las pruebas de entrada por consola del futuro módulo de validaciones.

## 5. Integración pendiente del equipo

Faltan los incisos a y b, el flujo por eventos, el menú final con datos personalizados, validaciones de las magnitudes físicas, historial y reportes. Ya existe un menú base con lectura segura de enteros. El arreglo de casos de prueba no sustituye el uso de arreglos en el simulador final; se propone el historial de simulaciones para ese requisito. Actualizar el Makefile cuando se agreguen esos archivos, sin enlazar simultáneamente los main de demo, pruebas y programa final.

El equipo debe completar integrantes y comisión en el README, aportar commits progresivos, revisar las interfaces compartidas y preparar el video y la defensa. Antes de integrar, Alexis debe leer el código, ejecutar los comandos y poder justificar la fórmula, el uso de punteros, los retornos de error y los límites inclusivos.
