#include "BKT.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;
BKT::BKT(int k) {
    this->k = k;
    this->root = nullptr;
}
int BKT::comparar(const uchar* s1, int l1, const uchar* s2, int l2) {
    int i = 0;
    while (i < l1 && i < l2) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i]; 
        }
        i++;
    }
    return l1 - l2; 
}

void BKT::dividirHijo(nodoBKT* padre, int pos) {
    int mitad = k / 2;
    nodoBKT* hijoLleno = padre->hijos[pos];
    nodoBKT* hermanoNuevo = new nodoBKT(k);
    hermanoNuevo->numClaves = k - mitad - 1;

    // Traspasa la mitad de la claves a su hermano
    for (int j = 0; j < hermanoNuevo->numClaves; j++) {
        hermanoNuevo->claves[j] = hijoLleno->claves[j + mitad + 1];
        hijoLleno->claves[j + mitad + 1] = nullptr;
    }
    // si no es un nodo, Traspasa los hijos correspondientes 
    if (hijoLleno->hijos[0] != nullptr) {
        for (int j = 0; j <= hermanoNuevo->numClaves; j++) {
            hermanoNuevo->hijos[j] = hijoLleno->hijos[j + mitad + 1];
            hijoLleno->hijos[j + mitad + 1] = nullptr;
        }
    }
    hijoLleno->numClaves = mitad;

    for (int j = padre->numClaves; j >= pos + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[pos + 1] = hermanoNuevo;
    // Desplaza las claves del padre para subir el elemento 
    for (int j = padre->numClaves - 1; j >= pos; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }
    padre->claves[pos] = hijoLleno->claves[mitad];
    hijoLleno->claves[mitad] = nullptr;
    
    padre->numClaves++;
}

void BKT::eliminarNodos(nodoBKT* t) {
    if (t != nullptr) {
        for (int i = 0; i <= t->numClaves; i++) {
            eliminarNodos(t->hijos[i]);
        }
        delete t; 
    }
}

BKT::~BKT() {
    eliminarNodos(root);
}
bool BKT::search(nodoBKT* t, const uchar* palabra, int largo) {
    if (t == nullptr) {
        return false; 
    }
    int pos = 0;
    while (pos < t->numClaves && comparar(palabra, largo, t->claves[pos], largo) > 0) {
        pos++;
    }
    if (pos < t->numClaves && comparar(palabra, largo, t->claves[pos], largo) == 0) {
        return true;
    }
    return search(t->hijos[pos], palabra, largo);
}

bool BKT::insert(nodoBKT** t, const uchar* palabra, int largo) {
    // si el Árbol esta vacío
    if (*t == nullptr) {
        *t = new nodoBKT(k);
        memoriaUsadaBytes += sizeof(nodoBKT);
        memoriaUsadaBytes += (k * sizeof(uchar*));       // Arreglo de claves
        memoriaUsadaBytes += ((k + 1) * sizeof(nodoBKT*)); // Arreglo de hijos
        memoriaUsadaBytes += (largo + 1) * sizeof(uchar);
        (*t)->claves[0] = new uchar[largo + 1];
        for (int i = 0; i < largo; i++) {
            (*t)->claves[0][i] = palabra[i];
        }
        (*t)->claves[0][largo] = '\0';
        (*t)->numClaves = 1;
        return true;
    }
    //El nodo raíz está lleno
    if ((*t) == root && root->numClaves == k) {
        nodoBKT* nuevaRaiz = new nodoBKT(k);
        memoriaUsadaBytes += sizeof(nodoBKT) + (k * sizeof(uchar*)) + ((k + 1) * sizeof(nodoBKT*));
        nuevaRaiz->hijos[0] = root;
        dividirHijo(nuevaRaiz, 0);
        root = nuevaRaiz;
        *t = nuevaRaiz; 
    }
    nodoBKT* p = *t;
    int pos = 0;
    while (pos < p->numClaves && comparar(palabra, largo, p->claves[pos], largo) > 0) {
        pos++;
    }
    //Clave duplicada 
    if (pos < p->numClaves && comparar(palabra, largo, p->claves[pos], largo) == 0) {
        return false;
    }
    // si el hijo al que va a descender está lleno, lo dividimos
    if (p->hijos[pos] != nullptr && p->hijos[pos]->numClaves == k) {
        dividirHijo(p, pos);
        if (comparar(palabra, largo, p->claves[pos], largo) > 0) {
            pos++;
        }
    }
    if (p->hijos[pos] == nullptr) {
        for (int i = p->numClaves; i > pos; i--) {
            p->claves[i] = p->claves[i - 1];
        }
        p->claves[pos] = new uchar[largo + 1];
        for (int i = 0; i < largo; i++) {
            p->claves[pos][i] = palabra[i];
        }
        p->claves[pos][largo] = '\0';
        p->numClaves++;
        return true;
    }
    return insert(&(p->hijos[pos]), palabra, largo);
}
bool BKT::remove(nodoBKT** t, const uchar* palabra, int largo) {
    if (*t == nullptr) {
        return false;
    }
    nodoBKT* p = *t;
    int pos = 0;
    while (pos < p->numClaves && comparar(palabra, largo, p->claves[pos], largo) > 0) {
        pos++;
    }
    if (pos < p->numClaves && comparar(palabra, largo, p->claves[pos], largo) == 0) {
        delete[] p->claves[pos];
        for (int i = pos; i < p->numClaves - 1; i++) {
            p->claves[i] = p->claves[i + 1];
            p->hijos[i] = p->hijos[i + 1];
        }
        p->hijos[p->numClaves - 1] = p->hijos[p->numClaves];
        p->hijos[p->numClaves] = nullptr;
        p->numClaves--;
        return true;
    }
    return remove(&(p->hijos[pos]), palabra, largo);
}

void ConstruirArbol(BKT &arbol, const string &directorio){
    //constructor vector inicial con las palabras de D1.txt
    ifstream archivo(directorio);
    if(!archivo.is_open()){
        cout << "Error: no se pudo abrir el archivo en el directorio: " << directorio << endl;
        return;
    }

    string palabra;
    while(archivo >> palabra){
        int largo = palabra.length();
        arbol.insert(&arbol.root, (const uchar*)palabra.c_str(), largo);
    }
    archivo.close();
}

vector<string> DesordernarDiccionarios(const string& directorio){
    ifstream archivo(directorio);
    vector<string> palabras;
    string palabra;
    while(archivo >> palabra){
        palabras.push_back(palabra);
    }
    archivo.close();

    //Desorden de D2.txt para evitar sesgos en la medicion de tiempos 
    int n = palabras.size();

    for(int i = n - 1; i > 0; i--){
        int j = rand()%(i + 1);
        string temporal = palabras[i];
        palabras[i] = palabras[j];
        palabras[j] = temporal;
    }

    return palabras;
}
//Imprime los elementos: I - R - D
void BKT::printInorder(nodoBKT* t) {
    if (t != nullptr) {
        int i;
        for (i = 0; i < t->numClaves; i++) {
            printInorder(t->hijos[i]);
            if(t->claves[i] != nullptr){
                cout << t->claves[i] <<" ";
            }
        }
        printInorder(t->hijos[i]);
    }
}

int main(int argc, char **argv){
    int k;
    cout << "Ingrese valor de K: ";
    cin >> k;
    cout << "-------------------------------------------------"<< endl;

    //Construccion Arbol 
    BKT Arbol(k);
    clock_t t_inicio_construccion = clock();
    ConstruirArbol(Arbol, "diccionarios/dicionarios/D1.txt");
    clock_t t_fin_construccion = clock();
    float segundos = float(t_fin_construccion - t_inicio_construccion)/CLOCKS_PER_SEC;
    cout << "Tiempo en construir Arbol k+1-ario(solo D1.txt): " << segundos <<" segundos."<< endl;
    float MemoriaUsada = Arbol.memoriaUsadaBytes/1024.0;
    cout << "Memoria Total: " << MemoriaUsada << " bytes" << endl;
    cout << "-------------------------------------------------"<< endl;

    //diccionario 2
    vector<string>DesordenD2 = DesordernarDiccionarios("diccionarios/dicionarios/D2.txt");
    bool insertar = true; //Para cumplir con D2/2 inserciones y D2/2 Eliminaciones, true inserta, false elimina
    int insercionesExitosas = 0;
    int EliminacionesExitosas = 0;
    float TiempoInsercion = 0;
    float TiempoEliminacion = 0;
    for(const string& palabra : DesordenD2){
        int largo = palabra.length();
        if(insertar){
            clock_t t_inicio_insercion = clock();
            if(Arbol.insert(&Arbol.root, (const uchar*)palabra.c_str(),largo)){
                insercionesExitosas++;
            }
            clock_t t_fin_insercion = clock();
            TiempoInsercion += float(t_fin_insercion-t_inicio_insercion)/CLOCKS_PER_SEC;
        }else{
            clock_t t_inicio_eliminacion = clock();
            if(Arbol.remove(&Arbol.root,(const uchar*)palabra.c_str(),largo)){
                EliminacionesExitosas++;
            }
            clock_t t_fin_eliminacion = clock();
            TiempoEliminacion += float(t_fin_eliminacion-t_inicio_eliminacion)/CLOCKS_PER_SEC;
        }
        insertar = !insertar;
    }

    cout << "Inserciones Existosas: " << insercionesExitosas << endl;
    cout << "Tiempo De Insercion: " << TiempoInsercion << endl;
    cout << "Eliminaciones Exitosas: " << EliminacionesExitosas << endl;
    cout << "Tiempo De Eliminacion: " << TiempoEliminacion << endl;
    cout << "-------------------------------------------------"<< endl;


    //Busqueda claves de D1 aleatorias
    vector<string> DesordenD1 = DesordernarDiccionarios("diccionarios/dicionarios/D1.txt");
    int REP = DesordenD1.size();
    int encontradas = 0;
    float TiempoBusqueda = 0;
    clock_t t_inicio_busqueda = clock();
    for(int i = 0; i < REP; i++){
        int largo = DesordenD1[i].length();
        if(Arbol.search(Arbol.root, (const uchar*)DesordenD1[i].c_str(), largo)){
            encontradas++;
        }
    }
    clock_t t_fin_busqueda = clock();
    cout << "Palabras encontradas exitosamente (de D1.txt): " << encontradas << endl;
    cout << "Tiempo promedio busqueda: " << ((float)(t_fin_busqueda - t_inicio_busqueda)/CLOCKS_PER_SEC)/10000 << "s" << endl;



    
    return 0;
}