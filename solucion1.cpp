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

}

int main(){

    return 0;
}