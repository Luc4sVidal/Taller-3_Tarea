#include <iostream>
using namespace std;

typedef unsigned char uchar;

struct Nodo{
    uchar *clave; //palabra guardada 
    Nodo *siguiente; 
    Nodo *anterior;
    Nodo *abajo;

    Nodo(const uchar* palabraI) {
        int largo = 0;
        while(palabraI[largo]){
            largo++;
        }
        
        clave = new uchar[largo + 1];
        for(int i = 0; i <= largo; i++){
            clave[i] = palabraI[i];
        }

        siguiente = nullptr;
        anterior = nullptr;
        abajo = nullptr;
    }
};

struct nivel {
    Nodo** niveles; //arreglo de punteros para niveles[0] es L1, niveles[1] es L2, etc
    int total;   //total L de niveles
    int k;  //cuanntos nodos se agrupa
};

int comparar(const uchar* s1, const uchar* s2) {
    while (*s1 && (*s1 == *s2)){ 
        s1++; s2++; 
    }
    return *s1 - *s2;
}

Nodo *buscar(nivel &mi_grilla, const uchar* palabra_a_buscar) {
    Nodo *actual = mi_grilla.niveles[mi_grilla.total - 1];

    //comenzamos a recorrer desde el nivel L 
    for (int i = mi_grilla.total - 1; i >= 0; i--) {
        while (actual->siguiente != nullptr && comparar(actual->siguiente->clave, palabra_a_buscar) <= 0) {
            actual = actual->siguiente;
        }

        //si es la palabra exacta en ese nivel, se retorna
        if (comparar(actual->clave, palabra_a_buscar) == 0) {
            return actual;
        }

        //si no es la palabra y no podemos avanzar mas, bajamos al nivel de abajo
        if (i > 0 && actual->abajo != nullptr) {
            actual = actual->abajo;
        }
    }
    

    return nullptr; 
}

void construir(nivel &g, Nodo* cabeza_L1) {
    g.niveles[0] = cabeza_L1; 
    //
}
