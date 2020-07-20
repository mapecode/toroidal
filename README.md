# Red Toroide con MPI

Dado un archivo con nombre datos.dat, cuyo contenido es una lista de valores
separados por comas, nuestro programa realizará lo siguiente:
El proceso de rank 0 destribuirá a cada uno de los nodos de un toroide de
lado L, los L x L numeros reales que estarán contenidos en el archivo datos.dat.

En caso de que no se hayan lanzado suficientes elementos de proceso para
los datos del programa, éste emitirá un error y todos los procesos finalizarán.
En caso de que todos los procesos han recibido su correspondiente
elemento,comenzará el proceso normal del programa.

Se pide calcular el elemento menor de toda la red, el elemento de
proceso con rank 0 mostrará en su salida estándar el valor obtenido.
La complejidad del algoritmo no superará O(raiz_cuadrada(n)) Con n número de elementos de la red.

Más información en la [documentación](https://github.com/mapecode/toroidal/blob/master/Memoria.pdf)

## Requisitos
* Distribución de Linux
* Instalar [Open-MPI] (http://www.open-mpi.org/software/ompi)

## Ejecución
Por defecto, en el Makefile el número de nodos es 16, pero este valor se puede modificar.

* **make all**: ejecución de todas las reglas necesarias
* Ejecución paso a paso:
    * **make dirs**: crea los directorios para los binarios.
    * **make data**: compila el archivo que genera aleatoriamente los valores a utilizar.
    * **make toroidal**: compila el programa principal.
    * **make new_data**: genera nuevos valores.
    * **make solution**: genera la solución.
* **make clean**: eliminar archivos de ejecución.
