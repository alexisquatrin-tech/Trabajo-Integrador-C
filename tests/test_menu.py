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
        salida = self.ejecutar('1\n0\n')
        self.assertIn('323786.67 N', salida)
        self.assertIn('3.1769', salida)
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
        salida = self.ejecutar('2\n1\n1\n0\n')
        self.assertIn('Pendientes: a/b', salida)
        self.assertEqual(salida.count('323786.67 N'), 2)


if __name__ == '__main__':
    unittest.main()
