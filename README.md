# Trabajo práctico 1: Técnicas algorítmicas

## Instalación

Para instalar y correr el proyecto se necesitan algunas cosas:
- Compilador de C++ (como mínimo C++11)
- Cmake para usar con nuestro CMakeLists.txt
- Node > 10 (opcional, para usar nuestro "runner" de instancias)

Clonar el repositorio y correr cmake, se puede hacer de la siguiente manera:

```
cmake --build ./build --config Release --target ALL_BUILD -j 8 --
```

Esto generará cuatro archivos ejecutables, de nombre `ejercicioX.exe`, en la carpeta "build".

## Correr el ejecutable

El ejecutable recibe algunos parámetros, acá se muestran en el ejemplo:

```
./build/ejercicio1.exe ./instancias/1/brock200_1.clq asc
```

Esto va a correr el ejecutable del ejercicio 1, leyendo la instancia "./instancias/1/brock200_1.clq", ordenando a los actores de menor a mayor influencia. 

La firma en general es así:

```
./build/ejercicio1.exe <file_instancia> <sort_flag> <debug_flag>
```

Si no se pasa un "sort_flag" válido (puede ser `asc` o `desc`), el default es orden descendente (de mayor a menor influencia).

## Estructura del proyecto

El proyecto se divide en algunas carpetas clave:

- `common`, tiene archivos comunes a todos los ejercicios.
- `ejercicios`, dentro tiene 4 carpetas con cada ejercicio particular (allí dentro se pueden encontrar los archivos .cpp del programa en sí, archivos de texto con las mediciones de tiempo, entre otras cosas)
- `instancias`, carpeta con las instancias a utilizar, divididas por ejercicio.
  - Notar que las instancias de 1 y 2 están en carpetas separadas. Esto es para simplificar el proceso de testeo en etapas iniciales, al correr los ejercicios 1 y 2 utilizamos ambas carpetas.

