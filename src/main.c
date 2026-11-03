#include "simulacion.h"

int main(void) {
    ReingresoInput caso_base = {
        .masa_kg = 4200.0,
        .velocidad_inicial_ms = 7800.0,
        .velocidad_final_ms = 85.0,
    };

    imprimir_resumen_reingreso(&caso_base);
    return 0;
}
