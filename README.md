# Simulador Artemis — Escenario 4

Trabajo Final Integrador de **Algoritmos y Estructuras de Datos**, Ingeniería en Sistemas de Información, UTN Facultad Regional Resistencia. Programa en C, nativo por consola.

> **Base de desarrollo compartida, no entrega final.** Funcionan el menú, el flujo por eventos de Nico y los cálculos de amerizaje. Hay una implementación provisional de a/b para ejecutar el recorrido completo con datos fijos. Faltan datos personalizados, revisión definitiva de a/b e historial/reportes.

## Equipo y comisión

Comisión: **pendiente de completar**.

| Integrante | Legajo | Responsabilidad confirmada |
| --- | --- | --- |
| Quatrin Oliver Alexis Javier | 27880 | Amerizaje y ángulo: incisos c/d |
| Navarro Ezequiel Leandro | 31372 | Parte 1: reingreso y frenado (a/b) |
| Julian Nicolas Alvarez Carruega | 29342 | Parte 3: flujo, estados y eventos |
| Juan Ignacio Pabon | 28583 | Parte 4: entradas, validaciones y menú |
| Merlo Alex Gabriel | 24518 | Parte 5: historial, reportes y compilación |

| Responsable | Parte | Archivo principal / entrega |
| --- | --- | --- |
| Navarro Ezequiel Leandro | Reingreso y frenado: incisos a/b | `reingreso.c` (provisional; revisar), pruebas y explicación física |
| **Quatrin Oliver Alexis Javier (Alexis)** | Amerizaje y ángulo: incisos c/d | `amerizaje.c`, pruebas y explicación física |
| Julian Nicolas Alvarez Carruega (Nico) | Flujo, estados y eventos | `simulador.c`, contratos en `simulador.h` |
| Juan Ignacio Pabon | Entradas, validaciones y menú | `validaciones.c/.h` y `main.c` |
| Merlo Alex Gabriel (Alex) | Arreglo de historial, reportes y compilación | `reportes.c/.h` (por crear), Makefile |

Cada integrante programa, prueba y documenta su aporte. README, revisión, Git, video y preparación del coloquio son responsabilidades compartidas.

## Escenario y modelo propuesto

Orion regresa a la Tierra y debe disipar energía antes de amerizar. Valores de referencia de la consigna:

| Dato | Valor |
| --- | --- |
| Masa de la cápsula | 10.400 kg |
| Rapidez inicial / altitud de entrada | 11.000 m/s / 120.000 m |
| Rapidez / altitud al abrir paracaídas | 25 m/s / 2.500 m |
| Duración indicada para la mayor desaceleración | 400 s |
| Rapidez de impacto / hundimiento | 8 m/s / 1,50 m |
| Ventana segura del ángulo de entrada | 5,5° a 7,0°, inclusive |

Se propone un simulador por eventos: configurar datos, evaluar reingreso, avanzar a paracaídas, evaluar amerizaje y emitir/guardar el reporte. El usuario podrá repetir simulaciones y compararlas. **El recorrido reingreso → paracaídas → amerizaje ya funciona con datos fijos; configuración personalizada e historial están pendientes**; los modelos y reglas definitivos deben acordarse en equipo. No agregar vidas, puntajes o límites estructurales como si fueran requisitos de la cátedra.

Modelo actual c/d: movimiento vertical de detención, masa y gravedad constantes, fuerza del agua promediada respecto del desplazamiento. Incluye el trabajo del peso: `Fagua = m*g + m*v²/(2*d)`. Con los datos de referencia: **323.786,67 N**, **3,1769 veces el peso**. El ángulo se clasifica cualitativamente; no se calcula trayectoria ni temperatura y la ventana no garantiza toda la misión. [Fórmulas y límites](docs/parte-alexis.md).

## Compilación y ejecución

Requisitos: GCC compatible con C17 y GNU Make; biblioteca matemática estándar. Python 3 solo es necesario para las pruebas automáticas del menú, no para compilar o usar el simulador. Sin servicios externos, claves ni paquetes adicionales de C.

Desde la raíz del repositorio, en Linux o una terminal MSYS2 UCRT64 en Windows:

```sh
make          # Compila build/artemis (artemis.exe en Windows)
make run      # Ejecuta el menú actual
make demo     # Ejecuta el recorrido por eventos; Enter avanza, 0 aborta
make test     # Ejecuta 55 comprobaciones de amerizaje y 56 del flujo
make check    # Ejecuta las pruebas C y las 11 pruebas de menú con Python
make clean    # Elimina los ejecutables generados
```

Si Python se invoca como `python` en tu equipo: `make check PYTHON=python`.

### Windows sin VS Code

Instalar [MSYS2](https://www.msys2.org/) en la ubicación habitual. Abrir **MSYS2 UCRT64**, actualizar siguiendo sus indicaciones y ejecutar:

```sh
pacman -Syu
# Si solicita cerrar la terminal, reabrir UCRT64 y repetir la actualización.
pacman -S mingw-w64-ucrt-x86_64-gcc make git
cd "/c/Users/TU_USUARIO/Downloads/Trabajo-Integrador-C"
make run
```

Ajustar la ruta a la carpeta real. La terminal UCRT64 evita depender de una configuración previa de PowerShell o VS Code. Las instrucciones Windows están documentadas; la validación ejecutada en este entorno fue en Linux. Para `make check`, instalar además Python 3 o usar un Python ya disponible.

### Compilar y realizar pruebas con VS Code en Windows

VS Code es el editor; GCC compila y Make ejecuta las tareas del proyecto. Instalar [VS Code](https://code.visualstudio.com/) y MSYS2 como se indica arriba. La extensión **C/C++ de Microsoft** ayuda con edición y autocompletado, pero no instala GCC ni Make.

**1. Preparar las herramientas.** Desde **MSYS2 UCRT64**, después de actualizar MSYS2, instalar:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc make mingw-w64-ucrt-x86_64-python
```

Python es necesario para las pruebas del menú. No hace falta instalar bibliotecas de Python con pip.

**2. Abrir la carpeta completa.** En VS Code elegir **Archivo → Abrir carpeta** y seleccionar la carpeta que contiene `Makefile`, `main.c` y `README.md`. Si se descargó un ZIP, extraerlo antes; el nombre habitual de esa carpeta es `Trabajo-Integrador-C-main`.

**3. Configurar la terminal UCRT64 dentro de VS Code.** Abrir la paleta con `Ctrl+Shift+P`, buscar **Preferences: Open User Settings (JSON)** y agregar este perfil a la configuración de usuario. Conservar los ajustes existentes; si ya existe `terminal.integrated.profiles.windows`, agregar la entrada dentro de ese objeto, sin duplicar la propiedad.

```json
{
  "terminal.integrated.profiles.windows": {
    "MSYS2 UCRT64": {
      "path": "C:\\msys64\\usr\\bin\\bash.exe",
      "args": ["--login", "-i"],
      "env": {
        "MSYSTEM": "UCRT64",
        "CHERE_INVOKING": "1"
      }
    }
  },
  "terminal.integrated.defaultProfile.windows": "MSYS2 UCRT64"
}
```

El ejemplo supone MSYS2 instalado en `C:\msys64`; ajustar `path` si se instaló en otra ubicación. `MSYSTEM` activa las herramientas UCRT64 y `CHERE_INVOKING` conserva el directorio desde el que se abre la terminal.

**4. Crear una terminal nueva.** Cerrar la terminal anterior y elegir **Terminal → Nueva terminal**. Comprobar las herramientas y la carpeta actual:

```sh
pwd
ls Makefile
gcc --version
make --version
python --version
```

Si `ls Makefile` no encuentra el archivo, entrar a la carpeta real del proyecto, por ejemplo:

```sh
cd "/c/Users/TU_USUARIO/Downloads/Trabajo-Integrador-C-main"
```

**5. Ejecutar desde esa terminal.** Usar estos comandos para compilar todos los módulos necesarios, con el Makefile del equipo:

```sh
make run                  # Menú interactivo: 1 simulación, 2 estado, 0 salir
make demo                 # Demostración directa con los datos del enunciado
make test                 # Pruebas C del módulo de Alexis
make check PYTHON=python  # Pruebas C y pruebas automáticas del menú
```

Resultados esperados en la base actual: la demo muestra **323786.67 N** y **3.1769**; `make test` informa **55 comprobaciones de amerizaje y 56 de flujo correctas, sin fallos**; las pruebas del menú informan **11 pruebas y OK**. Las cantidades pueden cambiar cuando el equipo amplíe las pruebas. Registrar siempre la salida real, no copiar estos resultados como si se hubieran ejecutado personalmente.

Para probar manualmente las validaciones del menú, ejecutar `make run`, ingresar `abc`, una línea vacía y `9`: debe informar el error y volver a pedir la opción. Luego ingresar `1`, presionar Enter en cada una de las tres etapas y, al volver al menú, ingresar `0` para salir. Dentro de una etapa, `0` aborta la misión y vuelve al menú. Los datos físicos de la demo siguen siendo fijos; esta prueba no valida todavía su ingreso personalizado.

Si una herramienta aparece como `command not found`, verificar que se abrió una terminal **MSYS2 UCRT64** nueva y que la instalación del paso 1 terminó. Si cambian las herramientas de compilación, ejecutar `make clean` antes de recompilar.

Estas instrucciones de VS Code/Windows están documentadas, pero no se ejecutaron en una máquina Windows desde este entorno. Las comprobaciones previas del proyecto se realizaron en Linux.

### Funcionamiento actual

1. Elegir `1` para iniciar la simulación con los datos de la consigna y un ángulo de ejemplo de 6°.
2. Presionar Enter para avanzar por reingreso, paracaídas y amerizaje. Dentro de una etapa, `0` aborta y vuelve al menú.
3. Elegir `2` para consultar qué está implementado y qué falta.
4. Elegir `0` en el menú para salir. El fin de entrada también termina sin bucles infinitos.

El menú rechaza letras, líneas vacías, números fuera de rango, sufijos y líneas demasiado largas. Aún falta leer y validar las magnitudes físicas ingresadas por el usuario.

## Arquitectura

```text
main.c                 menú e inicialización; una sola función main
validaciones.c/.h      lectura segura de la opción del menú; ampliar para datos
simulador.h            constantes, structs, enum y contratos compartidos
simulador.c            coordinación por eventos y pausas interactivas
amerizaje.c            cálculos y clasificación de Alexis (c/d)
reingreso.c            cálculos a/b provisionales; revisión por Ezequiel
reportes.c/.h          PENDIENTE: historial y reportes finales
```

```mermaid
flowchart TD
    M[main: menú] --> V[validaciones: lectura segura]
    M --> S[simulador: coordinación]
    S --> A[amerizaje: c/d implementados]
    S --> R[reingreso: a/b provisionales]
    S -. pendiente .-> H[historial y reportes]
```

Las demos y pruebas tienen sus propios `main` en ejecutables separados. No compilar todos los `.c` con un comodín: eso enlazaría varios `main`. El Makefile enumera las fuentes explícitamente. Los binarios de `build/` no se versionan.

## Trabajo colaborativo y requisitos de entrega

- [Cómo trabajar con ramas y Pull Requests](CONTRIBUTING.md).
- [Tareas concretas de los cinco integrantes](docs/tareas-equipo.md).
- [Lista de requisitos y evidencias](docs/requisitos.md).
- [Explicación de la parte de Alexis](docs/parte-alexis.md).
- [Integración de flujo y validaciones para Ignacio](docs/integracion-flujo.md).

El repositorio de entrega debe ser **público** y mostrar commits progresivos reales de todos. Ramas y PR son recomendados por la consigna. La protección de `main` y el acceso de colaboradores se configuran en GitHub: los archivos de este proyecto no los activan.

La entrega final requiere código `.c/.h`, Makefile o script, README completo y video de hasta 5 minutos con aparición y voz real de todos, arquitectura, caso exitoso y entradas erróneas. No se permiten voces o participantes sustituidos por IA. Todos deben comprender el sistema para una posible defensa individual. La fecha de entrega no está especificada en el PDF; verificar el aula virtual.

## Registro de uso de IA

Herramienta: ChatGPT/Codex. Registro del 3 de noviembre de 2026.

| Consulta del usuario | Uso de la herramienta |
| --- | --- |
| «Somos un grupo de 5 personas [...] decime como podemos dividirnos el trabajo [...] Nos toco el escenario 4» | Lectura de la consigna y propuesta de distribución de responsabilidades. |
| «armame un doc para pasar al grupo de whatsapp, con la division de partes» | Generación de un documento de organización. |
| «arranquemos con mi parte, teniendo en cuenta todos los requerimientos del trabajo integrador» | Generación del módulo C de amerizaje y ángulo, demostración, pruebas y documentación. |

Validación realizada por la herramienta: compilación C17 con advertencias tratadas como errores, ejecución de la demo y 55 comprobaciones automáticas aprobadas. Se incluyó el trabajo del peso para distinguir fuerza del agua de fuerza neta; se documentaron los supuestos físicos y los límites del modelo. Se separaron las funciones de cálculo de la entrada/salida para integrarlas con el resto del equipo.

Revisión y adaptación por los estudiantes: **en curso**. Las verificaciones realizadas por Alexis se detallan a continuación; cada compañero debe completar su propio registro. La ejecución de pruebas no acredita por sí sola la comprensión integral del código.


### Preparación de la base compartida

Consulta: «armemos el repo que vamos a usar con el grupo para el trabajo integrador, leete de vuelta el pdf del trabajo que te habia mandado».

ChatGPT/Codex releyó la consigna completa, separó c/d en `amerizaje.c`, agregó un menú base, lectura segura de enteros, pruebas del ejecutable, documentación de requisitos/tareas, guía de contribución y configuración de CI. Se mantuvo pendiente la implementación de las partes de los compañeros. Se ejecutaron en Linux la compilación estricta, la demo, 55 comprobaciones C y 7 pruebas del menú; todas pasaron. La configuración de CI se ejecutará en GitHub cuando se publique y no equivale a una ejecución ya observada allí. Windows no fue probado en este entorno.


### Integración del flujo corregido de Nico

Consulta: «integralas al repo». Se integró la rama `flujo/nico` conservando su historial y se aplicaron los tres archivos corregidos del ZIP aportado por Alexis. Se conservaron las validaciones actuales de Ignacio, se actualizaron los textos y las secuencias de pruebas del menú y se agregaron casos de cancelación, fin de entrada y recuperación en pausas. Validación ejecutada por Codex en Linux: compilación estricta desde limpio, 55 comprobaciones de amerizaje, 56 de flujo y 11 pruebas de menú aprobadas. No acredita ejecución personal por los estudiantes ni prueba en Windows.


### Revisión personal de Alexis — 12 de noviembre de 2026

Quatrin Oliver Alexis Javier ejecutó el proyecto en Windows, desde MSYS2 UCRT64 integrado en VS Code. Compartió la salida de los comandos y sus cálculos durante esta conversación con ChatGPT/Codex.

- **Recorrido completo:** ejecutó la opción 1 de `make run`, avanzó por las tres etapas y confirmó `MISION EXITOSA` y el retorno al menú. La salida de amerizaje mostró 332800.00 J, 101920.00 N, 323786.67 N y 3.1769 veces el peso.
- **Pruebas automáticas:** ejecutó `make check PYTHON=python`. Resultado compartido: 55 comprobaciones de amerizaje correctas, 56 comprobaciones del flujo correctas y 11 pruebas del menú aprobadas; ningún fallo.
- **Validación manual del menú:** ingresó `abc` y `9`; el programa rechazó ambas entradas y volvió a solicitar una opción. Inició una misión, la abortó con `0` en la primera pausa, comprobó el regreso al menú sin mensaje de éxito y salió normalmente con `0`.
- **Contraste con calculadora:** para masa 10400 kg, rapidez 8 m/s, hundimiento 1.50 m y gravedad 9.80 m/s², verificó energía cinética de 332800 J, peso de 101920 N, fuerza promedio del agua de 323786.6667 N y relación fuerza/peso de 3.176870748. Corrigió un error inicial de operación al separar la división por la distancia de la suma del peso.

Consultas de esta revisión: «sigamos con mi registro de uso de ia, vamos con las pruebas de mi parte», «vamos con la demo primero» y «avancemos con las pruebas que debo realizar [...] sin vs code». ChatGPT/Codex orientó los comandos, ayudó a configurar la terminal y localizar el proyecto, propuso las secuencias de prueba y explicó la corrección del cálculo manual. Alexis ejecutó los comandos en su equipo y compartió los resultados. Estas acciones verifican los casos indicados; no significan que se hayan probado todas las entradas posibles ni completado la defensa de cada línea de código. La justificación física de la fórmula y la comprensión integral siguen en preparación.


## Registro detallado del uso de Inteligencia Artificial

| Actividad                                              | Prompts y consultas realizadas                                                                                                                                                                                                                                           | Aporte de la IA y solución aplicada                                                                                                                                                                                                                                                                                                                                             |
| ------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|                                                        |                                                                                                                                                                                                                                                                          | Se explicó la importancia de coordinar el menú con las funciones y módulos desarrollados por los demás integrantes. Se revisó cómo conectar el menú principal con la función `ejecutar_simulacion()` sin duplicar la lógica de otros módulos.                                                                                                                                   |
| **Desarrollo de las funciones de validación**          | «Tenemos así organizado, como muestra en la primer imagen y en la segunda muestra el error».<br><br>También se compartió el contenido de `validaciones.c` para revisar por qué fallaba al integrarlo al repositorio.                                                     | Se detectaron problemas relacionados con la organización del archivo de validaciones y la inclusión de una función `main()` de prueba. Se separaron las declaraciones de `validaciones.h` de las implementaciones de `validaciones.c` y se organizaron las funciones `leer_entero()`, `leer_real()` y `leer_real_positivo()`.                                                   |
| **Corrección de errores de compilación e integración** | Se compartieron capturas de la estructura del repositorio y mensajes de error para identificar por qué fallaba la integración de `validaciones.c`.<br><br>«Dale seguimos entonces».                                                                                      | Se revisaron los archivos involucrados, las declaraciones y definiciones de funciones y la estructura del proyecto. Se realizaron correcciones para evitar conflictos de vinculación y mantener una única función `main()`.                                                                                                                                                     |
| **Configuración del entorno de desarrollo**            | Se consultó cómo instalar las herramientas necesarias para compilar desde Visual Studio Code en Windows. Durante el proceso se fueron comprobando los comandos de GCC y Make y se consultó cómo resolver el problema de reconocimiento del comando `make` en PowerShell. | Se indicó cómo instalar MSYS2, GCC y Make, configurar el PATH y comprobar las instalaciones. Se identificó que debía utilizarse `mingw32-make` en lugar de `make` desde PowerShell.                                                                                                                                                                                             |
| **Revisión e integración de los módulos del equipo**   | Se compartieron los archivos `main.c`, `simulador.c`, `simulador.h`, `reingreso.c`, `amerizaje.c` y `validaciones.c` para revisar cómo estaban conectados.<br><br>«¿Qué habría que modificar entonces para que el usuario ingrese valores?»                              | Se analizaron las responsabilidades de cada módulo, las funciones y las estructuras compartidas. Se identificó que la simulación utilizaba los valores de referencia mediante `config_consigna()`. Se propuso agregar una función que permita recibir una estructura `ConfigMision` con valores personalizados. También se revisaron las pruebas automatizadas del repositorio. |
