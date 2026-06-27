// Implementacion de la fija para altura con thread, dividir el arbol a partir de los nodos de un nivel adecuado.
// se uso lambda para mejorar una parte, sus contrapartes estan en los otros archivos con el mismo funcionamiento.

#include <iostream>
#include <deque> //Adicional
#include <queue>
#include <thread>

using namespace std;

struct CNode{
    CNode(int _v)
    {
        v = _v; left = right = 0;
    }
    int v;
    CNode* left, * right;
};

class CBinTree{
public:
    CBinTree();
    ~CBinTree();
    bool find(int x, CNode**& p);
    bool ins(int x);
    void inorder(CNode* n);
    int  Height(CNode* n);
    int  altura();
    void contarNodosPorNivel(CNode* n, int k);
    void inorder_mod(CNode* n, int min, int max);
    void print();
    void printRange(int min, int max);
    void print_nivel();
    

    int Height_Thread_1(CNode* n);
    //int Height_Thread_2(CNode* n);
    //int Height_Thread_3(CNode* n);

    int altura_thread_1();
private:
    CNode* root;
    void deleteTree(CNode* n);
};

CBinTree::CBinTree()
{    root = 0; }

CBinTree::~CBinTree()
{
    deleteTree(root);
}

void CBinTree::deleteTree(CNode* n)
{
    if (!n) return;
    deleteTree(n->left);
    deleteTree(n->right);
    delete n;
}

bool CBinTree::find(int x, CNode**& p)
{
    p = &root;
    while (*p && (*p)->v != x)
        if ((*p)->v < x)
            p = &((*p)->right);
        else
            p = &((*p)->left);
    return *p != 0;
}

bool CBinTree::ins(int x)
{
    CNode** p;
    if (find(x, p)) return 0;
    *p = new CNode(x);
    return 1;
}

void CBinTree::print()
{
    inorder(root);
    std::cout << "\n";
}

void CBinTree::inorder(CNode* n)
{
    if (!n) return;
    inorder(n->left);
    std::cout << n->v << " ";
    inorder(n->right);
}

void CBinTree::inorder_mod(CNode* n, int min, int max)
{
    if (!n) return;
    if (n->v >= min) {
        inorder_mod(n->left, min, max);
    }
    if (n->v >= min && n->v <= max) {
        std::cout << n->v << " ";
    }
    if (n->v <= max) {
        inorder_mod(n->right, min, max);
        //std::cout << n->v << " ";
        //return;
    } return;
}


void CBinTree::contarNodosPorNivel(CNode* n, int k) {

    if (!n) return;
    queue<CNode*> q;
    q.push(n);
    int nivel = 0;

    while (!q.empty()) {
        int cantidad = q.size(); // Aquí tienes el dato        
        // Si es el nivel actual se imprime la cola   
        if (nivel == k) {
            for (int i = 0; i < cantidad; i++) {
                CNode* actual = q.front();
                cout << actual->v << " ";
                q.push(actual);
                q.pop();
            }
            break;
        }
        // operacion ingresar nuevos nodos mientras se borra su cabeza
        for (int i = 0; i < cantidad; i++) {
            CNode* actual = q.front();
            if (actual->left) q.push(actual->left);
            if (actual->right) q.push(actual->right);
            q.pop();
        }
        nivel++;


    }
}

int CBinTree::Height(CNode* n) {
    if (!n) return 0;
    int izq = Height(n->left);
    int der = Height(n->right);
    if (izq >= der) return izq + 1;
    return der + 1;
}

int CBinTree::Height_Thread_1(CNode* n) {

    if (!n) return 0;
    queue<CNode*> q;
    q.push(n);
    int nivel = 0;
    int n_threads = thread::hardware_concurrency();
    int n_nodos = 1;
    while (!q.empty() && n_nodos < n_threads) {
        n_nodos = q.size();
        for (int i = 0; i < n_nodos; i++) {
            CNode* actual = q.front();
            if (actual->left) q.push(actual->left);
            if (actual->right) q.push(actual->right);
            q.pop();
        }
        nivel++;
        n_nodos = q.size();
    }
    // Si la cola esta vacia, es porque altura maxima se encontro
    // muy rapido, el arbol es pequeño
    if (q.size() == 0) return nivel;

    //Se lanzan threads para la cola >= al numero de nucleos
    // n_nodos = q.size();
    cout <<" numero de nodos para los threads " << q.size() << endl;
    vector<thread> ths(q.size());
    vector<int> resultados(q.size());

    for ( int i = 0;  i< q.size(); i++  ) {
        CNode* front = q.front();
        ths[i] = thread([front, i, &resultados, this]() {
            resultados[i] = Height(front) ;
        });
        q.pop();
    }   
    for (int i = 0; i < q.size(); i++) {
        ths[i].join();  
    }
    int cantidad_2 = resultados.size(); int altura_down = nivel;
    for (int i = 0; i < cantidad_2; i++) {
        if (resultados[i] >= altura_down) 
            altura_down = resultados[i];        
    }
    return nivel + altura_down;
}

int CBinTree::altura() {
    return Height(root);
}
int CBinTree::altura_thread_1() {
    return Height_Thread_1(root);
}
void CBinTree::printRange(int min, int max)
{   //cout << " imprimir rango" <<endl;
    inorder_mod(root, min, max);
}

void CBinTree::print_nivel()
{   cout << endl << " imprimir niveles" << endl;
    contarNodosPorNivel(root, 2);
}

void insertarCompleto(CBinTree& t, int inicio, int fin) {
    if (inicio > fin) return;
    // Encontrar el punto medio exacto para que sea la raíz de este subárbol
    int medio = inicio + (fin - inicio) / 2;
    t.ins(medio); // Inserta el valor en tu árbol
    // Llenar recursivamente el lado izquierdo y derecho
    insertarCompleto(t, inicio, medio - 1);
    insertarCompleto(t, medio + 1, fin);
}

void insertarunlado(CBinTree& t, int nivel) {
    if (!nivel) return;
    for (int i = 20; i > 0; i--) {
        t.ins(i);  }
}
int main()
{
    CBinTree t;
    //int totalNodos = 2097151;
    int totalNodos = 524287;
    //insertarCompleto(t, 1, totalNodos);
    insertarunlado(t,20);
    std::cout << "Árbol completo: ";
    //t.print();
   
    //t.print_nivel();
    std::cout << "\n\n";

    // Prueba de altura secuencial
    //t.print();    

    cout <<"Altura secuencial " << t.altura();    
    cout <<"\nAltura multinucleo " << t.altura_thread_1(); // Prueba de altura thread
    std::cout << "\n\n";

    return 0;
}
