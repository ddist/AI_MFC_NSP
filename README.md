# Minimal Forward Checking for the Nurse Scheduling Problem

Autor: Diego Pérez T.<br>
ROL: 201173045-3<br>

### Requisitos
El programa puede utilizarse solo en sistemas operativos
Linux, y con un compilador que soporte C++11 (e.g: GCC 
4.8.x+).


### Compilación

Se incluye un archivo Makefile. El comando make compilará 
todos los archivos necesarios, y generará un binario de
nombre "nsp". 


### Uso

Para ejecutar el programa, ingresa en la terminal

>	$ ./nsp ARCHIVO

reemplazando ARCHIVO por el archivo que contenga la
instancia del NSP. El programa se ejecutara por un máximo
de dos horas, o hasta encontrar todas las soluciones, o
hasta que se termine manualmente con CTRL + C. 

No importa la forma en que se termine el proceso, se 
entregará un resumen de la ejecución al término.
