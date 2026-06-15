#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
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
    Nodo *cursor = mi_grilla.niveles[mi_grilla.total - 1];

    //comenzamos a recorrer desde el nivel L 
    for (int i = mi_grilla.total - 1; i >= 0; i--) {
        while (cursor->siguiente != nullptr && comparar(cursor->siguiente->clave, palabra_a_buscar) <= 0) {
            cursor = cursor->siguiente;
        }

        //si es la palabra exacta en ese nivel, se retorna
        if (comparar(cursor->clave, palabra_a_buscar) == 0) {
            return cursor;
        }

        //si no es la palabra y no podemos avanzar mas, bajamos al nivel de abajo
        if (i > 0 && cursor->abajo != nullptr) {
            cursor = cursor->abajo;
        }
    }
    return nullptr; 
}

void construir(nivel &g, Nodo* cabeza_L1) {
    if (cabeza_L1 == nullptr) return;
    g.niveles[0] = cabeza_L1; 
    int nivel = 0;
    int cantNodos = 0;
    Nodo*p = cabeza_L1;
    //contamos cuantos nodos hay en el nivel base
    while(p != nullptr){
        cantNodos++;
        p = p->siguiente;
    }
    while (cantNodos > g.k) {
        Nodo* inferior = g.niveles[nivel]; //el nodo que esta actualmente
        Nodo* nuevoInicio = nullptr; //para gurarda el la direccion del primer nodo
        Nodo* ultimo = nullptr;// guarda el ultimo nodo
        int contador = 0;
        int creados = 0; 

       // Regla de la Grilla: 1 de cada K nodos sube
        while (inferior != nullptr){
            if (contador % g.k == 0){
                Nodo* nuevo = new Nodo(inferior->clave);
                nuevo->abajo = inferior;
                if (nuevoInicio == nullptr){
                    nuevoInicio = nuevo; // Es el primero del nuevo nivel
                } else {
                    ultimo->siguiente = nuevo;
                    nuevo->anterior = ultimo;
                }
                ultimo = nuevo;
                creados++;
            }
            inferior = inferior->siguiente;
            contador++;
        }
        // hay que subir un nivel
        nivel++;
        g.niveles[nivel] =nuevoInicio;
        cantNodos = creados;
    } 
    g.total = nivel + 1;  
}

void limpiarNiveles(nivel &g) {
    for (int i = 1; i < g.total; i++) {
        Nodo* q = g.niveles[i];
        while (q != nullptr) {
            Nodo* aux = q;
            q = q->siguiente;
            delete[] aux->clave;
            delete aux;
        }
        g.niveles[i] = nullptr;
    }
    g.total = 1;
}

void eliminarEnGrilla(nivel &g, const uchar* palabra) {
    //buscamos el nodo en el nivel base (L1)
    Nodo* p = g.niveles[0];
    while (p != nullptr && comparar(p->clave, palabra) != 0) {
        p = p->siguiente;
    }
    //si no existe la palabra, salimos
    if (p == nullptr) return;

    //eliminación física en el nivel base (L1)
    if (p == g.niveles[0]) {
        g.niveles[0] = p->siguiente;
    }
    if (p->anterior != nullptr) p->anterior->siguiente = p->siguiente;
    if (p->siguiente != nullptr) p->siguiente->anterior = p->anterior;

    delete[] p->clave;
    delete p;

    limpiarNiveles(g);
    construir(g, g.niveles[0]);
}

void insercionGrilla(nivel &g, const uchar* nuevaPalabra) {
    Nodo* p= g.niveles[0];
    Nodo* anterior = nullptr;

    while (p != nullptr && comparar(p->clave, nuevaPalabra) < 0) {
        anterior = p;
        p = p->siguiente;
    }
    if (p != nullptr && comparar(p->clave, nuevaPalabra) == 0) return;
    Nodo* nuevo = new Nodo(nuevaPalabra);
    nuevo->siguiente = p;
    nuevo->anterior = anterior;
    if (anterior == nullptr) {
        g.niveles[0] = nuevo;
    } else {
        anterior->siguiente = nuevo;
    }
    if (p != nullptr) {
        p->anterior = nuevo;
    }

    limpiarNiveles(g);
    construir(g, g.niveles[0]);
}

int main() {
    nivel g;
    cout << "Ingrese el factor de salto K: ";
    cin >> g.k;

    g.niveles = new Nodo*[20];
    for(int i=0; i<20; i++) g.niveles[i] = nullptr;
    g.total = 1; 

    ifstream archivoD1("diccionarios/D1.txt"); 
    string palabra;
    if (!archivoD1.is_open()) {
        cout << "Error: No se encontro D1.txt en la carpeta 'diccionarios/'" << endl;
        return 1;
    }

    cout << "Cargando diccionarioD1 y construyendo niveles..." << endl;
    clock_t t_inicio_const = clock();
    while (archivoD1 >> palabra) {
        Nodo* p_act = g.niveles[0];
        Nodo* ant_act = nullptr;
        while (p_act != nullptr && comparar(p_act->clave, (const uchar*)palabra.c_str()) < 0) {
            ant_act = p_act;
            p_act = p_act->siguiente;
        }
        if (p_act != nullptr && comparar(p_act->clave, (const uchar*)palabra.c_str()) == 0) continue;
        
        Nodo* nuevo = new Nodo((const uchar*)palabra.c_str());
        nuevo->siguiente = p_act;
        nuevo->anterior = ant_act;
        if (ant_act == nullptr) g.niveles[0] = nuevo;
        else ant_act->siguiente = nuevo;
        if (p_act != nullptr) p_act->anterior = nuevo;
    }
    construir(g, g.niveles[0]); 
    clock_t t_fin_const = clock();

    cout << "Tiempo de construccion diccionario D1: " << (float)(t_fin_const - t_inicio_const)/CLOCKS_PER_SEC << "s" << endl;

    ifstream archivoD2("diccionarios/D2.txt");
    if (!archivoD2.is_open()) {
        cout << "Error: No se encontro D2.txt" << endl;
        return 1;
    }

    vector<string> d2_palabras;
    while(archivoD2 >> palabra) d2_palabras.push_back(palabra);
    
    float t_ins = 0, t_eli = 0;
    for(size_t i = 0; i < d2_palabras.size(); i++) {
        clock_t t1 = clock();
        if(i % 2 == 0) {
            insercionGrilla(g, (const uchar*)d2_palabras[i].c_str());
        } else {
            eliminarEnGrilla(g, (const uchar*)d2_palabras[i].c_str());
        }
        clock_t t2 = clock();
        
        if(i % 2 == 0) t_ins += (float)(t2 - t1)/CLOCKS_PER_SEC;
        else t_eli += (float)(t2 - t1)/CLOCKS_PER_SEC;
    }

    cout << "Tiempo Insercion total (D2): " << t_ins << "s" << endl;
    cout << "Tiempo Eliminacion total (D2): " << t_eli << "s" << endl;

    clock_t t_bus_ini = clock();
    for(int i = 0; i < 10000 && i < (int)d2_palabras.size(); i++) {
        buscar(g, (const uchar*)d2_palabras[i].c_str());
    }
    clock_t t_bus_fin = clock();
    cout << "Tiempo promedio busqueda: " << ((float)(t_bus_fin - t_bus_ini)/CLOCKS_PER_SEC)/10000 << "s" << endl;

    return 0;
}