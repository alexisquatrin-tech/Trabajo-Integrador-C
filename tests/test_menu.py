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
        salida = self.ejecutar('abc\n\n1abc\n1.5\n-1\n3\n0\n')
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
        self.assertIn('Pendientes: datos personalizados', salida)
        self.assertEqual(salida.count('323786.67 N'), 2)
        self.assertEqual(salida.count('MISION EXITOSA'), 2)

    def test_abortar_en_cada_etapa_y_volver_al_menu(self):
        for pasos in range(3):
            with self.subTest(pasos=pasos):
                salida = self.ejecutar('1\n' + '\n' * pasos + '0\n2\n0\n')
                self.assertIn('Mision abortada por el usuario', salida)
                self.assertIn('Implementados: flujo por eventos', salida)
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


if __name__ == '__main__':
    unittest.main()
