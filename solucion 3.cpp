#include "BKT.h"
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
        (*t)->claves[0] = new uchar[largo];
        for (int i = 0; i < largo; i++) {
            (*t)->claves[0][i] = palabra[i];
        }
        (*t)->numClaves = 1;
        return true;
    }
    //El nodo raíz está lleno
    if ((*t) == root && root->numClaves == k) {
        nodoBKT* nuevaRaiz = new nodoBKT(k);
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
        cout << "Clave ya existe !!" << endl;
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
        p->claves[pos] = new uchar[largo];
        for (int i = 0; i < largo; i++) {
            p->claves[pos][i] = palabra[i];
        }
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
//Imprime los elementos: I - R - D
void BKT::printInorder(nodoBKT* t) {
    if (t != nullptr) {
        int i;
        for (i = 0; i < t->numClaves; i++) {
            printInorder(t->hijos[i]);
            cout << "[Clave] ";
        }
        printInorder(t->hijos[i]);
    }
}