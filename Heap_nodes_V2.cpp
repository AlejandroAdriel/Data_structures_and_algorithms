/******************************************************************************

Heap basado en nodos (Con parent y con deque de incompletos)

*******************************************************************************/


#include <iostream>
#include <functional>
#include <deque>
#include <queue>

template <typename T>
struct Node {
    T value;
    Node* nodes[2];
    Node* parent;  

    Node(T val) {
        value = val;
        nodes[0] = nodes[1] = parent = nullptr;
    }
};

template <typename T, typename Ord>
class CHeap {
private:
    Node<T>* root;
    std::deque<Node<T>*> incomplete_nodes;
    Ord cmp;
    int tamano;

    void clear(Node<T>* n) {
        if (!n) return;
        clear(n->nodes[0]);
        clear(n->nodes[1]);
        delete n;
    }

    void swap_nodes(Node<T>* hijo, Node<T>* padre) {
        if (!hijo || !padre) return;

        Node<T>* h_izq = hijo->nodes[0];
        Node<T>* h_der = hijo->nodes[1];
        Node<T>* abuelo = padre->parent;

        int lado_hijo = (padre->nodes[1] == hijo) ? 1 : 0;
        Node<T>* hermano = padre->nodes[1 - lado_hijo];

        padre->nodes[0] = h_izq;
        if (h_izq) h_izq->parent = padre;

        padre->nodes[1] = h_der;
        if (h_der) h_der->parent = padre;

        padre->parent = hijo;

        hijo->nodes[lado_hijo] = padre;
        hijo->nodes[1 - lado_hijo] = hermano;
        if (hermano) hermano->parent = hijo;

        hijo->parent = abuelo;

        if (abuelo) {
            if (abuelo->nodes[0] == padre) abuelo->nodes[0] = hijo;
            else abuelo->nodes[1] = hijo;
        }
        else {
            root = hijo;
        }
    }

    void sift_up(Node<T>* node) {
        while (node->parent && cmp(node->parent->value, node->value)) {
            swap_nodes(node, node->parent);
        }
    }

    void sift_down(Node<T>* node) {
        if (!node) return;
        while (node->nodes[0] || node->nodes[1]) {
            Node<T>* best = node;

            if (node->nodes[0] && cmp(best->value, node->nodes[0]->value)) {
                best = node->nodes[0];
            }
            if (node->nodes[1] && cmp(best->value, node->nodes[1]->value)) {
                best = node->nodes[1];
            }

            if (best == node) break;

            swap_nodes(best, node);
        }
    }

public:
    CHeap() : root(nullptr), tamano(0) {}

    ~CHeap() {
        clear(root);
        incomplete_nodes.clear();
    }

    T top() {
        if (!root) return T();
        return root->value;
    }

    bool empty() {
        return tamano == 0;
    }

    int size() {
        return tamano;
    }

    void push(T x) {
        tamano++;
        Node<T>* t = new Node<T>(x);

        if (!root) {
            root = t;
            incomplete_nodes.push_back(root); 
            return;
        }

        Node<T>* insertionPoint = incomplete_nodes.front();
        t->parent = insertionPoint;

        if (!insertionPoint->nodes[0]) {
            insertionPoint->nodes[0] = t;
        }
        else {
            insertionPoint->nodes[1] = t;
            incomplete_nodes.pop_front(); 
        }

        incomplete_nodes.push_back(t);

        sift_up(t);
    }

    void pop() {
        if (!root) return;

        if (tamano == 1) {
            delete root;
            root = nullptr;
            incomplete_nodes.clear();
            tamano = 0;
            return;
        }

        Node<T>* curr = root;
        while (curr->nodes[0] || curr->nodes[1]) {
            Node<T>* best = curr;

            if (curr->nodes[0] && cmp(best->value, curr->nodes[0]->value)) {
                best = curr->nodes[0];
            }
            if (curr->nodes[1] && cmp(best->value, curr->nodes[1]->value)) {
                best = curr->nodes[1];
            }

            if (best == curr) {
                best = curr->nodes[0] ? curr->nodes[0] : curr->nodes[1];
            }

            swap_nodes(best, curr);
        }

        Node<T>* p = curr->parent;
        if (p) {
            if (p->nodes[1] == curr) {
                p->nodes[1] = nullptr;
            }
            else {
                p->nodes[0] = nullptr;
            }
        }

        for (auto it = incomplete_nodes.begin(); it != incomplete_nodes.end(); ++it) {
            if (*it == curr) {
                incomplete_nodes.erase(it);
                break;
            }
        }

        if (p) {
            bool enc = false;
            for (auto n : incomplete_nodes) {
                if (n == p) { enc = true; break; }
            }
            if (!enc) {
                incomplete_nodes.push_front(p);
            }
        }

        delete curr;
        tamano--;

        sift_down(root);
    }
};

int main() {
    int valores[] = { 45, 20, 14, 31, 7, 11, 13, 2 };
    int n_valores = sizeof(valores) / sizeof(valores[0]);

    std::cout << "===========================================" << std::endl;
    std::cout << "                  MIN HEAP                 " << std::endl;
    std::cout << "===========================================" << std::endl;

    CHeap<int, std::greater<int>> min_heap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> std_min_heap;

    for (int i = 0; i < n_valores; i++) {
        min_heap.push(valores[i]);
        std_min_heap.push(valores[i]);
    }

    int esperados_min[] = { 2, 7, 11, 13, 14, 20, 31, 45 };
    int idx = 0;

    while (!min_heap.empty()) {
        int esperado = esperados_min[idx++];
        int mi_heap_real = min_heap.top();
        int std_heap_real = std_min_heap.top();

        std::cout << "Esperado: " << esperado
            << " | Mi Heap: " << mi_heap_real
            << " | STL Heap: " << std_heap_real << std::endl;

        min_heap.pop();
        std_min_heap.pop();
    }

    std::cout << "\n===========================================" << std::endl;
    std::cout << "                  MAX HEAP                    " << std::endl;
    std::cout << "===========================================" << std::endl;

    CHeap<int, std::less<int>> max_heap;
    std::priority_queue<int> std_max_heap;

    for (int i = 0; i < n_valores; i++) {
        max_heap.push(valores[i]);
        std_max_heap.push(valores[i]);
    }

    int esperados_max[] = { 45, 31, 20, 14, 13, 11, 7, 2 };
    idx = 0;

    while (!max_heap.empty()) {
        int esperado = esperados_max[idx++];
        int mi_heap_real = max_heap.top();
        int std_heap_real = std_max_heap.top();

        std::cout << "Esperado: " << esperado
            << " | Mi Heap: " << mi_heap_real
            << " | STL Heap: " << std_heap_real << std::endl;

        max_heap.pop();
        std_max_heap.pop();
    }

    return 0;
};
