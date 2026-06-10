#ifndef BKT_H
#define BKT_H

#include <iostream>
#include <cstring>

typedef unsigned char uchar;

// Estructura del nodo del árbol B-Key Tree
struct nodoBKT {
    uchar** claves; //Arreglo dinámico de palabras (hasta k claves)
    nodoBKT** hijos; //Arreglo dinámico de punteros a hijos (hasta k+1 hijos)
    int numClaves; //Contador de claves

    nodoBKT(int k) {
        claves = new uchar*[k];
        hijos = new nodoBKT*[k + 1];
        numClaves = 0;;
        
        for (int i = 0; i <= k; i++) {
            hijos[i] = nullptr;
        }
    }

    nodoBKT() {
        for (int i = 0; i < numClaves; i++) {
            delete[] claves[i];
        }
        delete[] claves;
        delete[] hijos;
    }
};