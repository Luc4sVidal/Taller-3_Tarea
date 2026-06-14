
#include "BKT_H"
using namespace std;

//Constructor por parámetro
BKT::BKT(int K) {
    this->k = K;
    this->root = nullptr;
}

int BKT::comparar(const uchar* s1, const uchar* s2) {
    while (*s1 && (*s1 == *s2)) { 
        s1++; s2++; 
    }
    return *s1 - *s2;
}

bool BKT::search(nodoBKT* t, const uchar* palabra){
    if (t == nullptr) {
        return false;//Si el arbol esta vacio
    }
    int i = 0;
    while (i < t->numClaves && comparar(palabra, t->claves[i]) > 0) {
        i++;
    }
    if (i < t->numClaves && comparar(palabra, t->claves[i]) == 0) {
        return true;//Si encontramos en la posicion actual
    }
    return search(t->hijos[i], palabra);
}

bool BKT::insert(nodoBKT **t, const uchar* palabra) {
    if(*t == nullptr) {
        *t = new nodoBKT(k);
        int largo = 0;
        while(palabra[largo]) largo++;
        (*t)->claves[0] = new uchar[largo + 1];

        for(int i=0;i<=largo;i++) {
            (*t)->claves[0][i] = palabra[i];
        }
        (*t)->numClaves = 1;
        return true;
    }

    int pos = 0;

    while(pos < (*t)->numClaves &&comparar((*t)->claves[pos], palabra) < 0) {
        pos++;
    }
    //ya existe
    if(pos < (*t)->numClaves && comparar((*t)->claves[pos], palabra) == 0) {
        return false;
    }
    //hay espacio en este nodo
    if((*t)->numClaves < k) {
        for(int i = (*t)->numClaves; i > pos; i--) {
            (*t)->claves[i] = (*t)->claves[i-1];
        }

        int largo = 0;
        while(palabra[largo]) largo++;
        (*t)->claves[pos] = new uchar[largo + 1];

        for(int i=0;i<=largo;i++) {
            (*t)->claves[pos][i] = palabra[i];
        }
        (*t)->numClaves++;

        return true;
    }

    //nodo lleno (bajar al hijo correspondiente)
    return insert(&((*t)->hijos[pos]), palabra);
}
