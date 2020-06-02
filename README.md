Grupo: SEGFAULT

Integrantes:
  1. Raimundo Eyzaguirre. Número alumno: 15633616
  2. Eduardo Montecinos. Número alumno 1563776j
  3. Pilar Parga. Número alumno: 15622258
  4. María Jesús Retamales. Número alumno: 16640772


Principales decisiones de diseño:

El struct para crFile incluye los atributos:
  * size: representa el tamaño en bytes del archivo. Solo incluye los bytes efectivos de datos, no el espacio utilizado en guardar otra información.
  * nombre: guarda el nombre del archivo
  * puntero_a_bloque: el número de bloque en donde se encuentra el bloque índice del archivo
  * cant_hardlinks: numero de hard links asociados al archivo
  * modo: el modo en que se está abriendo el archivo
  * cant_bloques: la cantidad de bloques que utiliza el contenido del archivo
  * existe: si existía previamente
  * byte_leido: el último byte leído desde que se abrió el archivo

Supuestos:

Para la función cr_write se considera un archivo inicial vacío y se comienza a escribir. No hay append.

En la función cr_load, no se consideran los softlinks, ya que no es necesario al descargar el archivo al que hacen referencia.