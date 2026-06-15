# Taller-3_Tarea

1. Descripción del Proyecto
Este repositorio contiene el código fuente y el informe técnico para la segunda etapa del taller
El objetivo es comparar la eficiencia de tres estructuras de datos dinámicas para el almacenamiento y búsqueda de claves lexicográficas (uchar*)

Solución 1: Arreglo Dinámico Ordenado con búsqueda binaria
Solución 2: Grilla de Listas Enlazadas de L niveles con salto K
Solución 3: Árbol B balanceado de orden K denominado BKT

2. Estructura de Archivos
solucion1.cpp: Implementación del vector dinámico ordenado
solucion2.cpp: Implementación de la grilla multinivel
solucion3.cpp: Implementación del árbol B balanceado (BKT)
BKT.h: Archivo de cabecera con la estructura del nodoBKT 
Informe_Taller_3_2026.pdf: Análisis de resultados, gráficos comparativos de las 3 soluciones y conclusiones
diccionarios/: Carpeta que contiene los archivos D1.txt y D2.txt

3. Instrucciones de Compilación
g++ -O3 solucion1.cpp -o solucion1
g++ -O3 solucion2.cpp -o solucion2
g++ -O3 solucion2.cpp -o solucion3

4. Instrucciones de Ejecución
Para la Solución 1: ./solucion1
El programa solicitará la Capacidad Máxima y el procentaje de celdas (overhead). 

Para la Solución 2: ./solucion2
El programa solicitará el Factor de salto K. 

Para la Solución 3: ./solucion3
El programa solicitará el grado de ramificación o valor de k para el árbol.

5. Notas de Implementación
Las tres soluciones integran funciones de comparación de caracter por carcater optimizadas para punteros unsigned char*

Todas las solucionees implementan sus respectivas liberacion de memoria antes de finalizar la eejecución