#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
typedef unsigned char uchar;

struct Vectores{
    uchar** vec; //Arreglo dinamico del tipo uchar
    int capacidad_max;
    int tam = 0; //tamaño del arreglo inicia en 0
    float overhead; //porcentaje de celdas adicionales 
    int indices[256];
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

void actualizarIndices(Vectores &vl){
    for(int i = 0; i < 256; i++){
        vl.indices[i] = -1;
    }

    for(int i = 0; i < vl.tam; i++){
        uchar inicial = vl.vec[i][0];
        if(vl.indices[inicial] == -1){
            vl.indices[inicial] = i;
        }
    }
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
    while (posicion < v.tam && comparar(v.vec[posicion], (const uchar*)nuevaPalabra) < 0){
        posicion++;
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
    int l = v.indices[(uchar)claveBuscar[0]];
    int r = v.tam - 1;
    //En caso de que la letra inicial no exista en el arreglo
    if(l == -1){ 
        return -1;
    }
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
            r = medio - 1;
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
    //constructor vector inicial con las palabras de D1.txt
    ifstream archivo(directorio);
    if(!archivo.is_open()){
        cout << "Error: no se pudo abrir el archivo en el directorio: " << directorio << endl;
        return;
    }

    string palabra;
    while(archivo >> palabra){
        insercion(v, palabra.c_str());
    }
    actualizarIndices(v);
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
    for(int i = 0; i < 256; i++){
        v.indices[i] = -1;
    }

    clock_t t_inicio = clock();
    ConstruirVector(v, "diccionarios/dicionarios/D1.txt");
    clock_t t_fin = clock();
    float segundos = float(t_fin - t_inicio)/CLOCKS_PER_SEC;
    cout << "Tiempo en construir vector inicial (solo D1.txt): " << segundos <<" segundos."<< endl;
    cout << "-------------------------------------------------"<< endl;
    
    //Imprimir(v);
    clock_t t_inicio_busqueda = clock();
    int resultado = busquedaBinaria(v, "writer");
    if(resultado != -1){
        cout << "Palabra encontrada en la posicion: " << resultado << endl;
    }else{
        cout << "Palabra no encontrada." << endl;
    }
    clock_t t_fin_busqueda = clock();
    float segundos_busqueda = float(t_fin_busqueda - t_inicio_busqueda)/CLOCKS_PER_SEC;
    cout << "Tiempo en buscar una palabra de D1.txt: " << segundos_busqueda << " segundos."<< endl;
    cout << "-------------------------------------------------"<< endl;

    //Carga de D2.txt para realizar operaciones insercion y eliminacion 
    ifstream archivo("diccionarios/dicionarios/D2.txt");
    if(!archivo.is_open()){
        cout << "Error: No se pudo abrir el archivo" << endl;
    }

    vector<string> diccionario2;
    string palabra;
    int contador = 0;
    while(archivo >> palabra && contador <= 50000){
        diccionario2.push_back(palabra);
        contador++;
    }
    archivo.close();

    //Desorden de D2.txt para evitar sesgos en la medicion de tiempos 
    int n = diccionario2.size();
    srand(time(NULL));//Para que el desorden sea diferente cada que se ejecute
    for(int i = n - 1; i > 0; i--){
        int j = rand()%(i + 1);
        string temporal = diccionario2[i];
        diccionario2[i] = diccionario2[j];
        diccionario2[j] = temporal;
    }
    //Insercion primeras 5000 paralabras 
    clock_t t_inicio_insercion = clock();
    for(int i = 0; i < diccionario2.size(); i++){
        insercion(v, diccionario2[i].c_str());
    }
    actualizarIndices(v);
    clock_t t_fin_insercion = clock();
    float segundos_insercion = float(t_fin_insercion - t_inicio_insercion)/CLOCKS_PER_SEC;
    cout << "Tiempo en insertar 50000 palabras desde D2.txt: " << segundos_insercion << " segundos."<< endl;
    cout << "-------------------------------------------------"<< endl;


    clock_t t_inicio_eliminar = clock();
    bool resultado2 = eliminar(v, "writer");
    if(resultado2 == true){
        cout << "La palabra ha sido eliminada con exito."<< endl;
    }else{
        cout << "No se encontro la palabra"<<endl;
    }
    clock_t t_fin_eliminar = clock();
    float segundos_eliminar = float(t_fin_eliminar - t_inicio_eliminar)/CLOCKS_PER_SEC;
    cout << "Tiempo en eliminar una palabra: " << segundos_eliminar << " segundos."<< endl;
    cout << "-------------------------------------------------"<< endl;

    //Imprimir(v);

    for (int i = 0; i < v.tam; i++) {
        delete[] v.vec[i];
    }
    delete[] v.vec;

    return 0;
}