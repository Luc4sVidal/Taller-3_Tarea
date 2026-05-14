#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
using namespace std;
typedef unsigned char uchar;

struct Vectores{
    uchar** vec; //Arreglo dinamico del tipo uchar
    int capacidad_max;
    int tam = 0; //tamaño del arreglo inicia en 0
    float overhead; //porcentaje de celdas adicionales 
};

int comparar(const uchar* p1, const uchar* p2){
    while (*p1 && (*p1 == *p2)){
        p1++;
        p2++;
    }
    if (*p1 == *p2) return 0;   
    if (*p1 > *p2)  return 1;   
    return -1;                  

}

void insercion(Vectores &v, const char* nuevaPalabra){
    uchar** nuevoVec;
    //Redimensionar el vector una vez se haya alcanzado la capacidad maxima de claves 
    if(v.tam == v.capacidad_max){ 
        int nueva_cap = v.capacidad_max + (v.capacidad_max*v.overhead);
        nuevoVec = new uchar*[nueva_cap];

        for(int i = 0; i < v.tam; i++){
            nuevoVec[i] = v.vec[i];
        }

        delete[] v.vec;
        v.vec = nuevoVec;
        v.capacidad_max = nueva_cap;
    }

    //Encontrar la posicion en orden alfabetico
    int posicion = 0;
    while (posicion < v.tam && comparar(v.vec[posicion], (const uchar*)nuevaPalabra) < 0)
        posicion++;
    for (int i = v.tam; i > posicion; i--) {
        v.vec[i] = v.vec[i - 1];
    }
    //Desplazar los elementos a la derecha
    for (int i = v.tam; i > posicion; i--) {
        v.vec[i] = v.vec[i - 1];
    }

    uchar* copia = new uchar[strlen(nuevaPalabra) + 1];
    strcpy((char*)copia, nuevaPalabra);
    v.vec[posicion] = copia;
    
    v.tam++;

}

int busquedaBinaria(Vectores &v, const char* claveBuscar) {
    int l = 0;
    int r = v.tam - 1;

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
     if (clave == -1){
        return false; 
     }
    delete[] v.vec[clave];
    //Hay que cerrar el espacio
    for (int i = clave; i < v.tam - 1; i++) {
        v.vec[i] = v.vec[i + 1];
    }
    v.tam--;
    return true;
}

void ConstruirVector(Vectores &v, const string &directorio){
    ifstream archivo(directorio);

    if(!archivo.is_open()){
        cout << "Error: no se pudo abrir el archivo en el directorio: " << directorio << endl;
    }

    string palabra;
    while(archivo >> palabra){
        insercion(v, palabra.c_str());
    }

    archivo.close();
}

void Imprimir(Vectores &v){
    if(v.tam == 0){
        cout << "Vector vacio" << endl;
    }

    for(int i = 0; i < v.tam; i++){
        cout << v.vec[i] << " ";
    }
    cout << endl;
}

int main(int argc, char **argv){
    Vectores v;
    cout << "Ingrese la Capacidad Maxima del vector: ";
    cin >> v.capacidad_max;

    cout << "Ingrese el porcentaje de celdas adicionales: ";
    cin >> v.overhead;

    v.vec = new uchar*[v.capacidad_max];
    v.tam = 0;

    ConstruirVector(v, "diccionarios/dicionarios/D1.txt");

    Imprimir(v);

    return 0;
}