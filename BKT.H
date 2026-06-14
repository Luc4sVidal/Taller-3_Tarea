#ifndef BKT_H
#define BKT_H
#include <iostream>
using namespace std;

typedef unsigned char uchar;

struct nodoBKT {
    uchar** claves; 
    nodoBKT** hijos; 
    int numClaves; 

    nodoBKT(int k) {
        claves = new uchar*[k];
        hijos = new nodoBKT*[k + 1];
        numClaves = 0;
        for (int i = 0; i < k; i++) 
            claves[i] = nullptr;
        for (int i = 0; i <= k; i++) 
            hijos[i] = nullptr;
    }
};

class BKT {
public:
    nodoBKT* root;
    int k; 
    long long memoriaUsadaBytes = 0;
    BKT(int k);
    void eliminarNodos(nodoBKT* t);
    virtual ~BKT();
    bool insert(nodoBKT **t, const uchar* palabra, int largo);
    bool search(nodoBKT *t, const uchar* palabra, int largo);
    bool remove(nodoBKT **t, const uchar* palabra, int largo);
    void dividirHijo(nodoBKT* padre, int pos);
    int comparar(const uchar* s1, int l1, const uchar* s2, int l2);
    void printInorder(nodoBKT *t);
    void ConstruirArbol(BKT &arbol, const string &directorio);
};
#endif
