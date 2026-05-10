#include <iostream>
#include <vector>
using namespace std;
typedef unsigned char uchar;

struct Vectores{
    uchar** vec; //Arreglo dinamico del tipo uchar
    int capacidad_max;
    int tamaño;
    float overhead; //porcentaje de celdas adicionales 
};

void insercion(Vectores &v, const char* nuevaPalabra){
    uchar** nuevoVec;
    //Redimensionar el vector una vez se haya alcanzado la capacidad maxima de claves 
    if(v.tamaño == v.capacidad_max){ 
        int nueva_cap = v.capacidad_max + (v.capacidad_max*v.overhead);
        nuevoVec = new uchar*[nueva_cap];

        for(int i = 0; i < v.tamaño; i++){
            nuevoVec[i] = v.vec[i];
        }

        delete[] v.vec;
        v.vec = nuevoVec;
        v.capacidad_max = nueva_cap;
    }

//Encontrar la posicion en orden alfabetico
int posicion = 0;
while (posicion < v.tamaño && comparar(v.vec[posicion], (const uchar*)nuevaPalabra) < 0)
    posicion++;
for (int i = v.tamaño; i > posicion; i--) {
        v.vec[i] = v.vec[i - 1];
    }
//Desplazar los elementos a la derecha
for (int i = v.tamaño; i > posicion; i--) {
        v.vec[i] = v.vec[i - 1];
    }

v.vec[posicion] = (uchar*)nuevaPalabra;
    
    v.tamaño++;

}

int comparar(const uchar* p1, const uchar* p2){
    while (*p1 && (*p1 == *p2)){
        p1++;
        p2++;
    }
    if (*p1 == *p2) return 0;   
    if (*p1 > *p2)  return 1;   
    return -1;                  

    }

int busquedaBinaria(Vectores &v, const char* claveBuscar) {
    int l = 0;
    int r = v.tamaño - 1;

    while(l<=r){
        int medio = l +(r-l)/2;
        int res = comparar(v.vec[medio], (const uchar*)claveBuscar);
        if (res == 0) {
            return medio; 
        }
        if (res < 0) {
            l = medio + 1;
        }
        else {
            r = medio -1;
        }
    }
    return -1;
}

bool eliminar(Vectores &v, const char* claveEliminar) {
    int clave = busquedaBinaria(v, claveEliminar);
    //si no existe
     if (clave = -1){
        return false;
     }
    delete[] v.vec[clave];
    //Hay que cerrar el espacio
    for (int i = clave; i < v.tamaño - 1; i++) {
        v.vec[i] = v.vec[i + 1];
    }
    v.tamaño--;
    return true;
}

int main(){

    return 0;
}