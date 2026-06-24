#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>
#include <deque>
#include <string>

struct Node
{
    int value;
    Node* nodes[2];

    Node(int v) {
        value = v;
        nodes[0] = nullptr;
        nodes[1] = nullptr;
    }
};

class CBinTree
{
public:
    CBinTree();
    ~CBinTree();

    bool find(int x, Node**& p);
    bool ins(int x);
    bool rem(int x);
    Node** rep(Node** p);
    void clear(Node* n);

    void inorder(Node* n);   // Izq - Raíz - Der (Imprime ordenado)
    void preorder(Node* n);  // Raíz - Izq - Der (Útil para clonar)
    void posorder(Node* n);  // Izq - Der - Raíz (Útil para destruir)
    void reverse(Node* n);   // Der - Raíz - Izq (Imprime orden descendente)

    void inorder_stack(Node* n);
    void preorder_stack(Node* n);
    void posorder_stack(Node* n);
    void reverse_stack(Node* n);

    void levels(Node* n);
    int height(Node* n);
    int height_profundidad(Node* n);
    int height_amplitud(Node* n);
    int size(Node* n);
    int count_leaves(Node* n);

    int get_min();
    int get_max();

    void printRange(Node* n, int min, int max);

    void kneighbors(int x, int k);

    void triangle();
    void zigzag();

    /* lca (Lowest Common Ancestor): Encuentra el ancestro común más profundo
     * de los nodos con valores 'p' y 'q'. Aprovecha las propiedades del BST. */
    Node* lca(Node* n, int p, int q);

    /* isBalanced: Verifica si la diferencia de alturas entre el hijo izquierdo
     * y derecho de TODO nodo es como máximo 1. */
    bool isBalanced();

    /* isBST: Verifica estrictamente si un árbol binario cumple la propiedad de BST
     * (todo a la izquierda es menor, todo a la derecha es mayor). */
    bool isBST(Node* n, int min_val, int max_val);

    /* kthSmallest: Encuentra el k-ésimo elemento más pequeño del árbol.
     * Sugerencia: Recorrido inorder llevando un contador por referencia. */
    int kthSmallest(Node* n, int k, int& count);

    /* diameter: Calcula el diámetro del árbol (el camino más largo entre dos
     * nodos cualesquiera, no necesariamente pasando por la raíz). */
    int diameter(Node* n, int& max_diam);

    /* mirror: Invierte el árbol (intercambia el hijo izquierdo con el derecho
     * para todos los nodos). */
    void mirror(Node* n);

    /* pathSum: Retorna true si existe un camino desde la raíz hasta alguna HOJA
     * donde la suma de los valores de los nodos sea exactamente 'targetSum'. */
    bool pathSum(Node* n, int targetSum);

    /* floor / ceil:
     * floor: Encuentra el mayor elemento en el árbol que sea <= x.
     * ceil: Encuentra el menor elemento en el árbol que sea >= x. */
    int get_floor(int x);
    int get_ceil(int x);

    /* serialize: Convierte el árbol a un string (ej. usando preorder y marcando
     * los nulos con un carácter especial como '#').
     * deserialize: Reconstruye el árbol exacto a partir de ese string. */
    std::string serialize(Node* n);
    Node* deserialize(std::string& data);

    void print_tree();

    void print();

    /* iterator: Clase anidada que permite recorrer el árbol sin exponer su estructura interna. */
    class iterator {
    public:
        /* Constructor: Inicializa el iterador.
         * Si p no es nulo, debe cargar el nodo raíz en la pila y ejecutar
         * la lógica para posicionarse en el primer elemento del recorrido (inorder). */
        iterator(Node* p = nullptr);

        /* operator*: Retorna una referencia al valor del nodo actual.
         * En tu caso, debe acceder al nodo que está en el tope de la pila (s.top().second->value). */
        int& operator*();

        /* operator++: Avanza el iterador al siguiente nodo lógico del árbol.
         * Debe incrementar el estado del nodo actual en la pila y llamar a la función
         * de avance para procesar los estados hasta encontrar la siguiente parada. */
        iterator& operator++();

        /* operator!=: Compara dos iteradores para saber si el bucle debe continuar.
         * Esencial para la condición 'it != t.end()'. Generalmente compara si las
         * pilas internas de ambos iteradores están vacías o tienen el mismo tope. */
        bool operator!=(const iterator& other) const;

    private:
        /* s: Estructura de datos que simula la recursividad guardando el nodo y su estado.
         * Cada par representa (estado_actual, puntero_al_nodo). */
        std::stack<std::pair<int, Node*>> s;

        /* avanzar: Función privada que ejecuta el switch de estados (0, 1, 2, 3).
         * Debe detenerse (hacer return) únicamente cuando llegue al estado de "lectura" (Estado 1). */
        void avanzar();
    };

    /* begin: Crea un iterador apuntando al primer elemento lógico del árbol.
     * Debe pasar la raíz (root) al constructor del iterador. */
    iterator begin();

    /* end: Crea un iterador que representa el final del recorrido.
     * Generalmente es un iterador inicializado en nullptr o con una pila vacía. */
    iterator end();

private:
    Node* root;
    bool brep;
};

CBinTree::CBinTree() {
    root = nullptr;
    brep = 0;
}

CBinTree::~CBinTree() {
    clear(root);
}

/*  * find: Busca 'x'. 'p' termina apuntando al puntero que conecta al nodo
    * (si existe) o al puntero nulo donde debería ir (si no existe). */
bool CBinTree::find(int x, Node**& p) {
    for (p = &root; *p && (*p)->value != x; p = &((*p)->nodes[x > (*p)->value]));
    return *p && (*p)->value == x;
}

/* ins: Inserta 'x' usando find y punteros dobles. */
bool CBinTree::ins(int x) {
    Node** p;
    if (find(x, p)) return false;
    *p = new Node(x);
    return true;
}

/* rem: Elimina 'x'. Maneja casos de 0, 1 o 2 hijos. Libera memoria. */
bool CBinTree::rem(int x) {
    Node** p;
    if (!find(x, p)) return false;

    if ((*p)->nodes[0] && (*p)->nodes[1]) {
        Node** q = rep(p);
        (*p)->value = (*q)->value;
        p = q;
    }

    Node* t = *p;
    *p = (*p)->nodes[!!(*p)->nodes[1]];
    delete t;
    return true;
}

/* rep: Encuentra el nodo reemplazo (predecesor o sucesor) usando 'brep'
 para alternar y mantener el árbol balanceado heurísticamente, sin salirse al puntero nulo */
Node** CBinTree::rep(Node** p) {
    p = &((*p)->nodes[brep]);
    while ((*p)->nodes[!brep]) {
        p = &((*p)->nodes[!brep]);
    }
    brep = !brep;
    return p;
}

/* clear: Recorre el árbol en postorden liberando la memoria de cada nodo. */
void CBinTree::clear(Node* n) {
    if (!n) return;
    clear(n->nodes[0]);
    clear(n->nodes[1]);
    delete n;
}

// Izq - Raíz - Der (Imprime ordenado)
void CBinTree::inorder(Node* n) {
    if (!n) return;
    inorder(n->nodes[0]);
    std::cout << n->value << " ";
    inorder(n->nodes[1]);
}

// Raíz - Izq - Der (Útil para clonar)
void CBinTree::preorder(Node* n) {
    if (!n) return;
    std::cout << n->value << " ";
    preorder(n->nodes[0]);
    preorder(n->nodes[1]);
}

// Izq - Der - Raíz (Útil para destruir)
void CBinTree::posorder(Node* n) {
    if (!n) return;
    posorder(n->nodes[0]);
    posorder(n->nodes[1]);
    std::cout << n->value << " ";
}

// Der - Raíz - Izq (Imprime orden descendente)
void CBinTree::reverse(Node* n) {
    if (!n) return;
    reverse(n->nodes[1]);
    std::cout << n->value << " ";
    reverse(n->nodes[0]);
}

// Simulando la pila de llamadas

void CBinTree::inorder_stack(Node* n) {
    if (!n) return;
    std::stack< std::pair<int, Node*> > s;
    s.push(std::make_pair(0, n));

    while (!s.empty()) {
        switch (s.top().first)
        {
        case 0:
            if (!s.top().second) {
                s.pop();
            }
            else {
                s.top().first++;
                s.push(std::make_pair(0, s.top().second->nodes[0]));
            }
            break;
        case 1:
            s.top().first++;
            std::cout << s.top().second->value << " ";
            break;
        case 2:
            s.top().first++;
            s.push(std::make_pair(0, s.top().second->nodes[1]));
            break;
        case 3:
            s.pop();
            break;
        }
    }
}

void CBinTree::preorder_stack(Node* n) {
    if (!n) return;
    std::stack< std::pair<int, Node*> > s;
    s.push(std::make_pair(0, n));

    while (!s.empty()) {
        switch (s.top().first)
        {
        case 0:
            if (!s.top().second) {
                s.pop();
            }
            else {
                s.top().first++;
                std::cout << s.top().second->value << " ";
            }
            break;
        case 1:
            s.top().first++;
            s.push(std::make_pair(0, s.top().second->nodes[0]));
            break;
        case 2:
            s.top().first++;
            s.push(std::make_pair(0, s.top().second->nodes[1]));
            break;
        case 3:
            s.pop();
            break;
        }
    }
}

void CBinTree::posorder_stack(Node* n) {
    if (!n) return;
    std::stack< std::pair<int, Node*> > s;
    s.push(std::make_pair(0, n));

    while (!s.empty()) {
        switch (s.top().first) {
        case 0:
            if (!s.top().second) {
                s.pop();
            }
            else {
                s.top().first++;
                s.push(std::make_pair(0, s.top().second->nodes[0]));
            }
            break;
        case 1:
            s.top().first++;
            s.push(std::make_pair(0, s.top().second->nodes[1]));
            break;
        case 2:
            s.top().first++;
            std::cout << s.top().second->value << " ";
            break;
        case 3:
            s.pop();
            break;
        }
    }
}

void CBinTree::reverse_stack(Node* n) {
    if (!n) return;
    std::stack< std::pair<int, Node*> > s;
    s.push(std::make_pair(0, n));

    while (!s.empty()) {
        switch (s.top().first)
        {
        case 0:
            if (!s.top().second) {
                s.pop();
            }
            else {
                s.top().first++;
                s.push(std::make_pair(0, s.top().second->nodes[1]));
            }
            break;
        case 1:
            s.top().first++;
            std::cout << s.top().second->value << " ";
            break;
        case 2:
            s.top().first++;
            s.push(std::make_pair(0, s.top().second->nodes[0]));
            break;
        case 3:
            s.pop();
            break;
        }
    }
}

// Recorre por niveles usando std::queue
void CBinTree::levels(Node* n) {
    if (!n) return;
    std::queue<Node*> q;
    q.push(n);

    while (!q.empty())
    {
        int a = q.size();
        for (int i = 0; i < a; i++) {
            Node* curr = q.front();
            if (curr->nodes[0]) q.push(curr->nodes[0]);
            if (curr->nodes[1]) q.push(curr->nodes[1]);

            std::cout << curr->value << " ";
            q.pop();
        }
        std::cout << "\n";
    }
}

/* height: Retorna la profundidad máxima del árbol (camino más largo a una hoja). */
int CBinTree::height(Node* n) {
    if (!n) return 0;
    return std::max(height(n->nodes[0]), height(n->nodes[1])) + 1;
}

//usando preorder
int CBinTree::height_profundidad(Node* n) {
    if (!n) return 0;

    std::stack<std::pair<Node*, int> > s;
    s.push(std::make_pair(n, 1));

    int max_h = 0;

    while (!s.empty())
    {
        Node* tmp = s.top().first;
        int h = s.top().second;
        s.pop();

        if (tmp) {
            s.push(std::make_pair(tmp->nodes[0], h + 1));
            s.push(std::make_pair(tmp->nodes[1], h + 1));

            max_h = std::max(h, max_h);
        }
    }
    return max_h;
}

int CBinTree::height_amplitud(Node* n) {
    if (!n) return 0;
    std::queue<Node*> q;
    q.push(n);

    int max_h = 0;

    while (!q.empty())
    {
        int x = q.size();

        for (int i = 0; i < x; i++) {
            Node* curr = q.front();
            if (curr->nodes[0]) q.push(curr->nodes[0]);
            if (curr->nodes[1]) q.push(curr->nodes[1]);

            q.pop();
        }
        max_h++;
    }
    return max_h;
}

/* size: Retorna la cantidad total de nodos en el árbol. */
int CBinTree::size(Node* n) {
    if (!n) return 0;
    std::stack<Node*> s;
    s.push(n);
    int contador = 0;
    while (!s.empty()) {
        Node* tmp = s.top();
        s.pop();
        if (tmp) {
            contador++;
            s.push(tmp->nodes[0]);
            s.push(tmp->nodes[1]);
        }
    }
    return contador;
}

/* count_leaves: Retorna la cantidad de nodos que no tienen hijos. */
int CBinTree::count_leaves(Node* n) {
    if (!n) return 0;
    if (!n->nodes[0] && !n->nodes[1]) {
        return 1;
    }
    return count_leaves(n->nodes[0]) + count_leaves(n->nodes[1]);
}

/* get_min / get_max: Retornan el valor mínimo y máximo del BST iterativamente. */
int CBinTree::get_min() {
    if (!root) return 0;
    Node* tmp = root;
    while (tmp->nodes[0]) {
        tmp = tmp->nodes[0];
    }
    return tmp->value;
}

int CBinTree::get_max() {
    if (!root) return 0;
    Node* tmp = root;
    while (tmp->nodes[1]) {
        tmp = tmp->nodes[1];
    }
    return tmp->value;
}

/* printRange: Imprime valores en [min, max].
 * DEBE podar ramas (no visitar nodos que lógicamente están fuera del rango). */
void CBinTree::printRange(Node* n, int min, int max) {
    if (!n) return;
    if (n->value > min) {
        printRange(n->nodes[0], min, max);
    }
    if (n->value >= min && n->value <= max) {
        std::cout << n->value << " ";
    }
    if (n->value < max) {
        printRange(n->nodes[1], min, max);
    }
}

/* printTriangle: Imprime el contorno del árbol.
 * Frontera izquierda -> Hojas -> Frontera derecha (de abajo hacia arriba). */
void CBinTree::triangle() {
    if (!root) return;

    std::cout << root->value << " ";

    Node* tmp1 = root->nodes[0];
    while (tmp1 && (tmp1->nodes[0] || tmp1->nodes[1])) {
        std::cout << tmp1->value << " ";
        tmp1 = tmp1->nodes[0] ? tmp1->nodes[0] : tmp1->nodes[1];
    }

    std::stack<Node*> s_leaves;
    s_leaves.push(root);

    while (!s_leaves.empty()) {
        Node* t = s_leaves.top();
        s_leaves.pop();

        if (!t->nodes[0] && !t->nodes[1]) {
            if (t != root) std::cout << t->value << " ";
        }
        else {
            if (t->nodes[1]) s_leaves.push(t->nodes[1]);
            if (t->nodes[0]) s_leaves.push(t->nodes[0]);
        }
    }

    std::stack<int> s;
    Node* tmp2 = root->nodes[1];

    while (tmp2 && (tmp2->nodes[0] || tmp2->nodes[1])) {
        s.push(tmp2->value);
        tmp2 = tmp2->nodes[1] ? tmp2->nodes[1] : tmp2->nodes[0];
    }

    while (!s.empty()) {
        std::cout << s.top() << " ";
        s.pop();
    }
    std::cout << "\n";
}

/* zigzag: Recorrido por niveles pero alternando dirección (Izq->Der, Der->Izq).
 * Sugerencia: Usar dos std::stack. */
void CBinTree::zigzag() {
    if (!root) return;

    std::stack<Node*> s1;
    std::stack<Node*> s2;
    s1.push(root);

    while (!s1.empty() || !s2.empty()) {
        while (!s1.empty()) {
            Node* t = s1.top();
            s1.pop();
            std::cout << t->value << " ";
            if (t->nodes[0]) s2.push(t->nodes[0]);
            if (t->nodes[1]) s2.push(t->nodes[1]);
        }

        while (!s2.empty()) {
            Node* t = s2.top();
            s2.pop();
            std::cout << t->value << " ";
            if (t->nodes[1]) s1.push(t->nodes[1]); // Corregido: pasar a s1 para alternar
            if (t->nodes[0]) s1.push(t->nodes[0]);
        }
    }
    std::cout << "\n";
}

/* lca (Lowest Common Ancestor): Encuentra el ancestro común más profundo
* de los nodos con valores 'p' y 'q'. Aprovecha las propiedades del BST. */
Node* CBinTree::lca(Node* n, int p, int q) {
    while (n) {
        if (n->value > p && n->value > q) n = n->nodes[0];
        else if (n->value < p && n->value < q) n = n->nodes[1];
        else return n;
    }
    return nullptr;
}

/* kthSmallest: Encuentra el k-ésimo elemento más pequeño del árbol. */
int CBinTree::kthSmallest(Node* n, int k, int& count) {
    if (!n) return -1;
    int left = kthSmallest(n->nodes[0], k, count);
    if (left != -1) return left;
    if (++count == k) return n->value;
    return kthSmallest(n->nodes[1], k, count);
}

/* diameter: Calcula el diámetro del árbol. */
int CBinTree::diameter(Node* n, int& max_diam) {
    if (!n) return 0;
    int lh = diameter(n->nodes[0], max_diam);
    int rh = diameter(n->nodes[1], max_diam);
    max_diam = std::max(max_diam, lh + rh);
    return std::max(lh, rh) + 1;
}

/* pathSum: Retorna true si existe un camino sumatorio targetSum. */
bool CBinTree::pathSum(Node* n, int targetSum) {
    if (!n) return false;
    if (!n->nodes[0] && !n->nodes[1]) return targetSum == n->value;
    return pathSum(n->nodes[0], targetSum - n->value) || pathSum(n->nodes[1], targetSum - n->value);
}

/* floor / ceil */
int CBinTree::get_floor(int x) {
    Node* res = nullptr;
    Node* tmp = root;

    while (tmp) {
        if (tmp->value == x) return tmp->value;
        if (tmp->value > x) tmp = tmp->nodes[0];
        else {
            res = tmp;
            tmp = tmp->nodes[1];
        }
    }
    return res ? res->value : -1;
}

int CBinTree::get_ceil(int x) {
    Node* res = nullptr;
    Node* tmp = root;

    while (tmp) {
        if (tmp->value == x) return tmp->value;
        if (tmp->value < x) tmp = tmp->nodes[1];
        else {
            res = tmp;
            tmp = tmp->nodes[0];
        }
    }
    return res ? res->value : -1;
}

void CBinTree::kneighbors(int x, int k) {}
bool CBinTree::isBalanced() { return true; }
bool CBinTree::isBST(Node* n, int min_val, int max_val) { return true; }
void CBinTree::mirror(Node* n) {}
std::string CBinTree::serialize(Node* n) { return ""; }
Node* CBinTree::deserialize(std::string& data) { return nullptr; }

CBinTree::iterator::iterator(Node* p) {}
int& CBinTree::iterator::operator*() { static int dummy = 0; return dummy; }
CBinTree::iterator& CBinTree::iterator::operator++() { return *this; }
bool CBinTree::iterator::operator!=(const iterator& other) const { return false; }
void CBinTree::iterator::avanzar() {}
CBinTree::iterator CBinTree::begin() { return iterator(nullptr); }
CBinTree::iterator CBinTree::end() { return iterator(nullptr); }


void CBinTree::print_tree() {

}

void CBinTree::print() {
    std::cout << "Inorder        : "; inorder(root); std::cout << "\n";
    std::cout << "Inorder Stack  : "; inorder_stack(root); std::cout << "\n";
    std::cout << "\n";
    std::cout << "Preorder       : "; preorder(root); std::cout << "\n";
    std::cout << "Preorder Stack : "; preorder_stack(root); std::cout << "\n";
    std::cout << "\n";
    std::cout << "Posorder       : "; posorder(root); std::cout << "\n";
    std::cout << "Posorder Stack : "; posorder_stack(root); std::cout << "\n";
    std::cout << "\n";
    std::cout << "Reverse        : "; reverse(root); std::cout << "\n";
    std::cout << "Reverse Stack  : "; reverse_stack(root); std::cout << "\n";
    std::cout << "\n";
    std::cout << "Niveles        :\n"; levels(root);
    std::cout << "\n";
    int a = height(root); std::cout << "Height         : " << a; std::cout << "\n";
    std::cout << "\n";
    int b = height_profundidad(root); std::cout << "Height Profundidad : " << b; std::cout << "\n";
    std::cout << "\n";
    int c = height_amplitud(root); std::cout << "Height Amplitud : " << c; std::cout << "\n";
    std::cout << "\n";

    int d = size(root); std::cout << "Size         : " << d; std::cout << "\n";
    std::cout << "\n";
    int e = count_leaves(root); std::cout << "Leaves         : " << e; std::cout << "\n";
    std::cout << "\n";
    int f = get_max(); std::cout << "Max          : " << f; std::cout << "\n";
    std::cout << "\n";
    int g = get_min(); std::cout << "Min          : " << g; std::cout << "\n";
    std::cout << "\n";
}

int main()
{
    CBinTree t;

    t.ins(50);
    t.ins(30); t.ins(70);
    t.ins(20); t.ins(40); t.ins(60); t.ins(80);
    t.ins(15); t.ins(25); t.ins(35); t.ins(45);


/*

Forma del árbol después de las inserciones:

               50
            /      \
          /          \
         30            70
       /    \        /    \
     20      40    60      80
    /  \    /  \
  15   25  35   45

Inorder: 15 20 25 30 35 40 45 50 60 70 80

*/

    t.print();

    return 0;
}