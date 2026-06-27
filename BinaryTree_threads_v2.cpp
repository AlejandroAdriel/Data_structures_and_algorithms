/******************************************************************************

Binary tree - Suma Externa con 3, 4 y 5 Núcleos (Sin modificar la clase)

*******************************************************************************/
#include <iostream>
#include <vector>
#include <thread>

struct Node
{
    int value;
    Node* nodes[2];
    
    Node (int x){
        value = x;
        nodes[0] = nodes [1] = nullptr;
    }
};

class CBinTree
{
public:
    CBinTree();
    ~CBinTree();
    
    void clear(Node* n);
    bool find(int x, Node**& p);
    bool ins(int x);
    bool rem(int x);
    Node** rep(Node** p);
    
    void print();
    
    void inorder(Node* n);
private:
    Node* root;
    bool brep;
};

CBinTree::CBinTree()
{   root = 0;  brep = 0; }

CBinTree::~CBinTree()
{
    clear(root);
}

void CBinTree::clear(Node* n){
    if (!n) return;
    clear(n->nodes[0]);
    clear(n->nodes[1]);
    delete n;
}

bool CBinTree::find(int x, Node**& p)
{
    for ( p = &root; *p && (*p)->value != x;
        p = &( (*p)->nodes[ x > (*p)->value ] ) );
    return *p && (*p)->value == x;
}

bool CBinTree::ins(int x)
{
    Node** p;
    if ( find(x,p) ) return 0;
    *p = new Node(x);
    return 1;
}

bool CBinTree::rem(int x)
{
    Node** p;
    if ( !find(x,p) ) return 0;
    
    if ( (*p)->nodes[0] && (*p)->nodes[1] ) 
    {
        Node** q = rep(p);
        (*p)->value = (*q)->value;
        p = q;
    }
    
    Node* t = *p;
    *p = (*p)->nodes[ (*p)->nodes[1] != 0 ];
    delete t;
    return 1;
}

Node** CBinTree::rep(Node** p) {
    p = &((*p)->nodes[brep]);
    while ((*p)->nodes[!brep]) {
        p = &((*p)->nodes[!brep]);
    }
    brep = !brep;
    return p;
}

void CBinTree::print()
{
    std::cout<<"inorder "; inorder(root); std::cout<<"\n";
}

void CBinTree::inorder(Node* n)
{
    if (!n) return;
    inorder(n->nodes[0]);
    std::cout<<n->value<<" ";
    inorder(n->nodes[1]);
}

// ============================================================================
// FUNCIONES EXTERNAS DE SUMA (Sin modificar la clase CBinTree)
// ============================================================================

// Suma secuencial base para procesar los subárboles de los nodos descubiertos
int sumarSecuencialExterna(Node* n) {
    if (!n) return 0;
    return n->value + sumarSecuencialExterna(n->nodes[0]) + sumarSecuencialExterna(n->nodes[1]);
}

// Estrategia para 3 núcleos
int externaSumar3Nucleos(Node* raiz) {
    if (!raiz) return 0;

    int sumaIzq = 0, sumaDer = 0;
    std::vector<std::thread> hilos;

    if (raiz->nodes[0]) {
        hilos.push_back(std::thread([&]() { sumaIzq = sumarSecuencialExterna(raiz->nodes[0]); }));
    }
    if (raiz->nodes[1]) {
        hilos.push_back(std::thread([&]() { sumaDer = sumarSecuencialExterna(raiz->nodes[1]); }));
    }

    for (auto& h : hilos) h.join();

    return raiz->value + sumaIzq + sumaDer;
}

// Estrategia para 4 núcleos
int externaSumar4Nucleos(Node* raiz) {
    if (!raiz) return 0;

    int s00 = 0, s01 = 0, s10 = 0, s11 = 0;
    int sumaLocal = raiz->value;
    std::vector<std::thread> hilos;

    if (raiz->nodes[0]) {
        sumaLocal += raiz->nodes[0]->value;
        if (raiz->nodes[0]->nodes[0]) {
            hilos.push_back(std::thread([&]() { s00 = sumarSecuencialExterna(raiz->nodes[0]->nodes[0]); }));
        }
        if (raiz->nodes[0]->nodes[1]) {
            hilos.push_back(std::thread([&]() { s01 = sumarSecuencialExterna(raiz->nodes[0]->nodes[1]); }));
        }
    }

    if (raiz->nodes[1]) {
        sumaLocal += raiz->nodes[1]->value;
        if (raiz->nodes[1]->nodes[0]) {
            hilos.push_back(std::thread([&]() { s10 = sumarSecuencialExterna(raiz->nodes[1]->nodes[0]); }));
        }
        if (raiz->nodes[1]->nodes[1]) {
            hilos.push_back(std::thread([&]() { s11 = sumarSecuencialExterna(raiz->nodes[1]->nodes[1]); }));
        }
    }

    for (auto& h : hilos) h.join();

    return sumaLocal + s00 + s01 + s10 + s11;
}

// Estrategia para 5 núcleos
int externaSumar5Nucleos(Node* raiz) {
    if (!raiz) return 0;

    int s00 = 0, s01 = 0, s10 = 0, s11 = 0;
    int sumaLocal = raiz->value;
    std::vector<std::thread> hilos;

    if (raiz->nodes[0]) {
        sumaLocal += raiz->nodes[0]->value;
        hilos.push_back(std::thread([&]() { s00 = sumarSecuencialExterna(raiz->nodes[0]->nodes[0]); }));
        hilos.push_back(std::thread([&]() { s01 = sumarSecuencialExterna(raiz->nodes[0]->nodes[1]); }));
    }

    if (raiz->nodes[1]) {
        sumaLocal += raiz->nodes[1]->value;
        hilos.push_back(std::thread([&]() { s10 = sumarSecuencialExterna(raiz->nodes[1]->nodes[0]); }));
        hilos.push_back(std::thread([&]() { s11 = sumarSecuencialExterna(raiz->nodes[1]->nodes[1]); }));
    }

    for (auto& h : hilos) h.join();

    return sumaLocal + s00 + s01 + s10 + s11;
}

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    CBinTree arbol;

    // Insertamos la raíz primero (50) para poder extraerla fácilmente después
    arbol.ins(50); 
    arbol.ins(25);
    arbol.ins(75);
    arbol.ins(12);
    arbol.ins(37);
    arbol.ins(62);
    arbol.ins(87);

    arbol.print();

    // --- TRUCO EXTERNO: Obtener el puntero real de la raíz ---
    Node** punteroARaiz = nullptr;
    arbol.find(50, punteroARaiz); // Buscamos el valor de la raíz
    Node* raizReal = *punteroARaiz;  // Desreferenciamos para obtener el Node*

    std::cout << "\n=== CALCULOS DE SUMA CON THREADS EXTERNOS ===" << std::endl;
    std::cout << "Suma (3 nucleos): " << externaSumar3Nucleos(raizReal) << std::endl;
    std::cout << "Suma (4 nucleos): " << externaSumar4Nucleos(raizReal) << std::endl;
    std::cout << "Suma (5 nucleos): " << externaSumar5Nucleos(raizReal) << std::endl;
    std::cout << "Suma Secuencial:  " << sumarSecuencialExterna(raizReal) << std::endl;

    return 0;
}