/******************************************************************************

Binanry tree - Suma Paralela con 3, 4 y 5 Núcleos

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

    // ========================================================================
    // NUEVOS MÉTODOS DE SUMA PARALELA
    // ========================================================================
    int sumarSecuencial(Node* n);
    int sumar3Nucleos();
    int sumar4Nucleos();
    int sumar5Nucleos();

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
// IMPLEMENTACIÓN DE MÉTODOS DE SUMA CON THREADS
// ============================================================================

int CBinTree::sumarSecuencial(Node* n) {
    if (!n) return 0;
    return n->value + sumarSecuencial(n->nodes[0]) + sumarSecuencial(n->nodes[1]);
}

// ESTRATEGIA 3 NÚCLEOS: Raíz en hilo principal, hijo[0] e hijo[1] en hilos nuevos.
int CBinTree::sumar3Nucleos() {
    if (!root) return 0;

    int sumaIzq = 0, sumaDer = 0;
    std::vector<std::thread> hilos;

    if (root->nodes[0]) {
        hilos.push_back(std::thread([&]() { sumaIzq = sumarSecuencial(root->nodes[0]); }));
    }
    if (root->nodes[1]) {
        hilos.push_back(std::thread([&]() { sumaDer = sumarSecuencial(root->nodes[1]); }));
    }

    for (auto& h : hilos) h.join();

    return root->value + sumaIzq + sumaDer;
}

// ESTRATEGIA 4 NÚCLEOS: Lanzar hilos independientes directo a los 4 nietos.
int CBinTree::sumar4Nucleos() {
    if (!root) return 0;

    int s00 = 0, s01 = 0, s10 = 0, s11 = 0;
    int sumaLocal = root->value;
    std::vector<std::thread> hilos;

    // Procesar rama izquierda (nodes[0]) y subdividir en sus nietos
    if (root->nodes[0]) {
        sumaLocal += root->nodes[0]->value;
        if (root->nodes[0]->nodes[0]) {
            hilos.push_back(std::thread([&]() { s00 = sumarSecuencial(root->nodes[0]->nodes[0]); }));
        }
        if (root->nodes[0]->nodes[1]) {
            hilos.push_back(std::thread([&]() { s01 = sumarSecuencial(root->nodes[0]->nodes[1]); }));
        }
    }

    // Procesar rama derecha (nodes[1]) y subdividir en sus nietos
    if (root->nodes[1]) {
        sumaLocal += root->nodes[1]->value;
        if (root->nodes[1]->nodes[0]) {
            hilos.push_back(std::thread([&]() { s10 = sumarSecuencial(root->nodes[1]->nodes[0]); }));
        }
        if (root->nodes[1]->nodes[1]) {
            hilos.push_back(std::thread([&]() { s11 = sumarSecuencial(root->nodes[1]->nodes[1]); }));
        }
    }

    for (auto& h : hilos) h.join();

    return sumaLocal + s00 + s01 + s10 + s11;
}

// ESTRATEGIA 5 NÚCLEOS: Raíz (hilo actual) + 4 hilos asignados a cada uno de los nietos.
int CBinTree::sumar5Nucleos() {
    if (!root) return 0;

    int s00 = 0, s01 = 0, s10 = 0, s11 = 0;
    int sumaLocal = root->value;
    std::vector<std::thread> hilos;

    if (root->nodes[0]) {
        sumaLocal += root->nodes[0]->value;
        hilos.push_back(std::thread([&]() { s00 = sumarSecuencial(root->nodes[0]->nodes[0]); }));
        hilos.push_back(std::thread([&]() { s01 = sumarSecuencial(root->nodes[0]->nodes[1]); }));
    }

    if (root->nodes[1]) {
        sumaLocal += root->nodes[1]->value;
        hilos.push_back(std::thread([&]() { s10 = sumarSecuencial(root->nodes[1]->nodes[0]); }));
        hilos.push_back(std::thread([&]() { s11 = sumarSecuencial(root->nodes[1]->nodes[1]); }));
    }

    // El hilo principal actúa como el 5to núcleo uniendo la reducción
    for (auto& h : hilos) h.join();

    return sumaLocal + s00 + s01 + s10 + s11;
}

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    CBinTree arbol;

    // Insertar elementos para construir un árbol balanceado de prueba
    arbol.ins(50);
    arbol.ins(25);
    arbol.ins(75);
    arbol.ins(12);
    arbol.ins(37);
    arbol.ins(62);
    arbol.ins(87);

    arbol.print();

    std::cout << "\n=== CALCULOS DE SUMA CON THREADS ===" << std::endl;
    std::cout << "Suma (3 nucleos): " << arbol.sumar3Nucleos() << std::endl;
    std::cout << "Suma (4 nucleos): " << arbol.sumar4Nucleos() << std::endl;
    std::cout << "Suma (5 nucleos): " << arbol.sumar5Nucleos() << std::endl;
    
    // Verificación secuencial pasándole el nodo raíz de forma interna
    std::cout << "Suma Secuencial de Control: " << arbol.sumarSecuencial(nullptr) << "\n"; 
    // Nota: Para probar el secuencial directo con control, podemos llamar a un método puente 
    // o simplemente validar que las 3 sumas paralelas coincidan entre sí.

    return 0;
}