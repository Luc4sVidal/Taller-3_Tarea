# Taller-3_Tarea

1. Descripción del Proyecto
Este repositorio contiene el código fuente y el informe técnico para la primera etapa del taller.
El objetivo es comparar la eficiencia de dos estructuras de datos dinámicas para el
almacenamiento y búsqueda de claves lexicográficas (uchar*).

Solución 1: Arreglo Dinámico Ordenado con búsqueda binaria
Solución 2: Grilla de Listas Enlazadas de L niveles con salto K

2. Estructura de Archivos
solucion1.cpp: Implementación del vector dinámico
solucion2.cpp: Implementación de la grilla multinivel
Informe_Taller_3_2026.pdf: Análisis de resultados y conclusiones
diccionarios/: Carpeta que debe contener D1.txt y D2.txt

3. Instrucciones de Compilación
g++ -O3 solucion1.cpp -o solucion1
g++ -O3 solucion2.cpp -o solucion2

4. Instrucciones de Ejecución
Para la Solución 1: ./solucion1
El programa solicitará la Capacidad Máxima y el procentaje de celdas. En esta ocasión se utlizo 10000 y 0.1 respectivamente

Para la Solución 2: ./solucion2
El programa solicitará el Factor de salto K. En esta ocasión se utilizó K=32 para las pruebas

5. Notas de Implementación
Se incluyó una función de comparar personalizada para optimizar el manejo de
unsigned char*.
Ambas soluciones liberan correctamente la memoria dinámica antes de finalizar la ejecución.