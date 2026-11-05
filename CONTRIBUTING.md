# Cómo trabajar en equipo

## Primer acceso

El propietario debe comprobar en GitHub que el repositorio sea público e invitar a los otros cuatro integrantes desde Settings → Collaborators. Cada integrante acepta su invitación y trabaja con su propia cuenta. No compartir contraseñas ni tokens.

```sh
git clone https://github.com/alexisquatrin-tech/Trabajo-Integrador-C.git
cd Trabajo-Integrador-C
git switch main
git pull --ff-only origin main
```

La base propuesta debe integrarse primero a `main`; si todavía está en una rama de preparación, revisar ese PR antes de iniciar partes dependientes.

## Cada tarea

```sh
git switch main
git pull --ff-only origin main
git switch -c feat/reingreso
# Editar los archivos de la tarea y agregar pruebas.
make check
git status
# Elegir explícitamente los archivos que corresponden a esta tarea.
git add reingreso.c
# Agregar también las pruebas, cabecera y documentación que se modificaron.
git commit -m "Implementa calculo del trabajo atmosferico"
git push -u origin feat/reingreso
```

El ejemplo corresponde al responsable a/b; cada persona usa una rama descriptiva propia. No ejecutar `git add` sobre un archivo que aún no existe. Los nombres de rama pueden ser `feat/amerizaje`, `feat/flujo`, `feat/validaciones` y `feat/reportes`.

Abrir un Pull Request hacia `main` explicando qué se hizo, cómo se probó y qué queda pendiente. Pedir revisión a otro integrante y verificar CI antes de integrar. No usar force-push sobre `main`, no sobrescribir cambios ajenos y no falsificar autores o commits para aparentar participación.

La cantidad de commits no debe inflarse: registrar avances reales y pequeños. Cada integrante documenta y puede explicar sus decisiones. Para resolver un conflicto de cabecera, acordar los tipos y conservar las declaraciones necesarias de ambos módulos.

## Contratos entre módulos

- `main.c` conserva únicamente `main`, el menú y la inicialización general.
- Los cálculos físicos no leen teclado ni imprimen reportes; devuelven resultados y errores.
- El flujo llama a los módulos y decide qué etapa sigue. No inventar umbrales de seguridad sin documentar su origen.
- La lectura usa líneas completas y conversión controlada; evitar `fflush(stdin)`, `gets` y lecturas que dejen datos inválidos en el buffer.
- Centralizar constantes y tipos en la cabecera compartida. No duplicar funciones ni cabeceras con versiones incompatibles.
- Reportes recibe resultados ya calculados. Un arreglo de estructuras puede almacenar el historial con una capacidad y política de llenado documentadas.
- El Makefile separa programa, demo y pruebas: cada ejecutable tiene un solo `main`.

## Antes de integrar

Compilar sin advertencias, ejecutar pruebas relevantes y `make check`, revisar que `build/` no se agregue a Git, documentar supuestos y actualizar el README/registro de IA. Los nombres y la comisión no se inventan. Al cambiar casos de prueba, actualizar las cantidades reportadas si corresponde.

La protección de `main` es opcional y recomendable: el propietario puede exigir PR, revisión y CI desde Settings → Rules. Este documento no activa esas reglas.
