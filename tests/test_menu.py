
"""Pruebas del ejecutable real: Python solo es necesario para esta verificacion."""
from pathlib import Path
import subprocess
import sys
import unittest

APP = str(Path(sys.argv.pop(1)).resolve())


class MenuTests(unittest.TestCase):
    def ejecutar(self, entrada):
        proceso = subprocess.run([APP], input=entrada, text=True,
                                 capture_output=True, timeout=5)
        self.assertEqual(proceso.returncode, 0, proceso.stderr)
        return proceso.stdout

    def test_demo_y_salida(self):
        salida = self.ejecutar('1\n\n\n\n0\n')
        self.assertIn('323786.67 N', salida)
        self.assertIn('3.1769', salida)
        self.assertIn('MISION EXITOSA', salida)
        self.assertIn('Hasta luego.', salida)

    def test_tipo_formato_rango_y_recuperacion(self):
        salida = self.ejecutar('abc\n\n1abc\n1.5\n-1\n6\n0\n')
        self.assertGreaterEqual(salida.count('Ingrese'), 6)
        self.assertNotIn('323786.67 N', salida)
        self.assertIn('Hasta luego.', salida)

    def test_linea_larga_no_ejecuta_sufijo(self):
        salida = self.ejecutar('x' * 200 + '1\n0\n')
        self.assertIn('Entrada demasiado larga', salida)
        self.assertNotIn('323786.67 N', salida)

    def test_desbordamiento_entero(self):
        salida = self.ejecutar('9' * 80 + '\n0\n')
        self.assertIn('Ingrese un numero entero valido', salida)
        self.assertIn('Hasta luego.', salida)

    def test_fin_entrada(self):
        self.assertIn('Fin de entrada', self.ejecutar(''))
        self.assertIn('Fin de entrada', self.ejecutar('abc\n'))

    def test_espacios_crlf_y_sin_salto_final(self):
        self.assertIn('Hasta luego.', self.ejecutar(' 0 \r\n'))
        self.assertIn('Hasta luego.', self.ejecutar('0'))

    def test_estado_y_repeticion(self):
        salida = self.ejecutar('2\n1\n\n\n\n1\n\n\n\n0\n')
        self.assertIn('Disponibles: datos personalizados', salida)
        self.assertEqual(salida.count('323786.67 N'), 2)
        self.assertEqual(salida.count('MISION EXITOSA'), 2)

    def test_abortar_en_cada_etapa_y_volver_al_menu(self):
        for pasos in range(3):
            with self.subTest(pasos=pasos):
                salida = self.ejecutar('1\n' + '\n' * pasos + '0\n2\n0\n')
                self.assertIn('Mision abortada por el usuario', salida)
                self.assertIn('PARAMETROS DEL ESCENARIO 4', salida)
                self.assertNotIn('MISION EXITOSA', salida)

    def test_fin_entrada_en_cada_etapa(self):
        for pasos in range(3):
            with self.subTest(pasos=pasos):
                salida = self.ejecutar('1\n' + '\n' * pasos)
                self.assertIn('Mision abortada por el usuario', salida)
                self.assertIn('Fin de entrada', salida)
                self.assertNotIn('MISION EXITOSA', salida)

    def test_recuperar_pausa_invalida(self):
        salida = self.ejecutar('1\nabc\n00\n\n\n\n0\n')
        self.assertEqual(salida.count('Error: presione solo Enter'), 2)
        self.assertIn('MISION EXITOSA', salida)

    def test_linea_larga_en_pausa(self):
        salida = self.ejecutar('1\n' + 'x' * 200 + '\n\n\n\n0\n')
        self.assertIn('Error: entrada demasiado larga', salida)
        self.assertEqual(salida.count('323786.67 N'), 1)
        self.assertIn('MISION EXITOSA', salida)


    def personalizada(self, angulo='6', impacto='8', masa='10400'):
        return ('3\n' + '\n'.join([masa, angulo, '11000', '120000',
                '25', '2500', '400', impacto, '1.5']) + '\n')

    def test_personalizada_y_reportes(self):
        salida = self.ejecutar(self.personalizada() + '\n\n\n4\n5\n0\n')
        self.assertIn('Configuracion ingresada correctamente', salida)
        self.assertIn('Simulacion #1:', salida)
        self.assertIn('Se necesitan al menos 2', salida)
        self.assertIn('323786.67 N', salida)

    def test_historial_vacio_lleno_y_comparacion(self):
        salida = self.ejecutar('4\n5\n' + '1\n\n\n\n' * 6 + '4\n5\n0\n')
        self.assertIn('historial de simulaciones esta vacio', salida)
        self.assertIn('No hay simulaciones registradas', salida)
        self.assertIn('Historial lleno', salida)
        self.assertIn('Total analizadas: 5', salida)
        self.assertNotIn('Simulacion #6:', salida)

    def test_personalizada_riesgos_y_cero_en_reporte(self):
        entrada = self.personalizada(angulo='4') + '\n'
        entrada += self.personalizada(angulo='8') + '\n'
        entrada += self.personalizada(impacto='12') + '\n\n\n'
        entrada += self.personalizada(impacto='0') + '\n\n\n4\n5\n0\n'
        salida = self.ejecutar(entrada)
        self.assertIn('rebote atmosferico', salida)
        self.assertIn('angulo de entrada demasiado alto', salida)
        self.assertIn('rapidez de impacto mayor', salida)
        self.assertIn('Energia 0.00 J', salida)
        self.assertIn('Menor energia de impacto en agua  : Sim #4 con 0.00 J', salida)

    def test_reales_invalidos_y_recuperacion(self):
        errores = 'abc\n\nnan\ninf\n1e999\n1e-999\n1,5\n2kg\n0\n-1\n' + 'x' * 200 + '\n'
        salida = self.ejecutar(self.personalizada(masa=errores + '10400') + '\n\n\n0\n')
        self.assertIn('Entrada demasiado larga', salida)
        self.assertIn('Ingrese un numero mayor que cero', salida)
        self.assertIn('MISION EXITOSA', salida)

    def test_personalizada_incoherente_y_fin_entrada(self):
        entrada = self.personalizada().replace('120000\n', '2000\n')
        salida = self.ejecutar(entrada + self.personalizada()[2:] + '\n\n\n0\n')
        self.assertIn('datos ingresados no son coherentes', salida)
        self.assertIn('MISION EXITOSA', salida)
        campos = self.personalizada()[2:].splitlines()
        for n in range(len(campos)):
            with self.subTest(campos=n):
                salida = self.ejecutar('3\n' + ''.join(c + '\n' for c in campos[:n]))
                self.assertIn('Fin de entrada', salida)
                self.assertNotIn('MISION EXITOSA', salida)

    def test_linea_de_capacidad_exacta_con_y_sin_salto(self):
        for final in ('', '\n'):
            with self.subTest(final=repr(final)):
                salida = self.ejecutar(' ' * 126 + '0' + final)
                self.assertIn('Hasta luego.', salida)
                self.assertNotIn('Entrada demasiado larga', salida)
        salida = self.ejecutar('1\n' + ' ' * 62 + '0')
        self.assertNotIn('entrada demasiado larga', salida)
        self.assertNotIn('Trabajo del rozamiento', salida)

    def test_nul_no_acepta_prefijo_ni_avanza_etapas(self):
        salida = self.ejecutar('1\x00basura')
        self.assertNotIn('Simulacion por eventos', salida)
        salida = self.ejecutar('1\n\x00basura')
        self.assertNotIn('Trabajo del rozamiento', salida)
        salida = self.ejecutar(self.personalizada(masa='10400\x00basura\n10400') + '\n\n\n0\n')
        self.assertIn('MISION EXITOSA', salida)

    def test_valores_personalizados_cambian_resultados(self):
        entrada = self.personalizada(masa='5200').replace('400\n', '200\n')
        salida = self.ejecutar(entrada + '\n\n\n4\n0\n')
        self.assertIn('Fuerza promedio del agua: 161893.33 N', salida)
        self.assertIn('Trabajo del rozamiento atmosferico: -320586175000.00 J', salida)
        self.assertIn('Desaceleracion media: 54.88 m/s^2', salida)
        self.assertIn('Frenado 200.00 s', salida)

    def test_error_calculo_no_informa_exito_ni_resultados_ficticios(self):
        salida = self.ejecutar(self.personalizada(masa='1e308') + '\n4\n0\n')
        self.assertIn('error en un calculo', salida)
        self.assertNotIn('MISION EXITOSA', salida)
        self.assertNotIn('  Amerizaje :', salida)

    def test_aborto_no_se_compara_como_impacto_cero(self):
        salida = self.ejecutar('1\n0\n1\n\n\n\n4\n5\n0\n')
        self.assertIn('Exitosas: 1 | Fallidas: 0 | Abortadas: 1', salida)
        self.assertIn('Menor energia de impacto en agua  : Sim #2 con 332800.00 J', salida)


if __name__ == '__main__':
    unittest.main()
