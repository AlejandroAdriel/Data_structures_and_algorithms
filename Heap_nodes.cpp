/******************************************************************************

Heap basado en nodos

*******************************************************************************/

#include <iostream>
#include <functional>
#include <deque>

template <typename T>
struct Node {
    T value;
    Node* nodes[2];

    Node(T val) {
        value = val;
        nodes[0] = nodes[1] = nullptr;
    }
};

template <typename T, typename Ord>
class CHeap {
private:
    Node<T>* root;
    std::deque<Node<T>*> tree_nodes;
    Ord cmp;

    void clear(Node<T>* n) {
        if (!n) return;
        clear(n->nodes[0]);
        clear(n->nodes[1]);
        delete n;
    }

    void swap_nodes(int idx_hijo, int idx_padre) {
        Node<T>* hijo = tree_nodes[idx_hijo];
        Node<T>* padre = tree_nodes[idx_padre];

        Node<T>* h_izq = hijo->nodes[0];
        Node<T>* h_der = hijo->nodes[1];

        Node<T>* abuelo = nullptr;
        if (idx_padre > 0) {
            int idx_abuelo = (idx_padre - 1) / 2;
            abuelo = tree_nodes[idx_abuelo];
        }

        int lado_hijo = (padre->nodes[1] == hijo) ? 1 : 0;
        Node<T>* hermano = padre->nodes[1 - lado_hijo];

        padre->nodes[0] = h_izq;
        padre->nodes[1] = h_der;

        hijo->nodes[lado_hijo] = padre;
        hijo->nodes[1 - lado_hijo] = hermano;

        if (abuelo) {
            if (abuelo->nodes[0] == padre) abuelo->nodes[0] = hijo;
            else abuelo->nodes[1] = hijo;
        }
        else {
            root = hijo;
        }

        Node<T>* temp = tree_nodes[idx_hijo];
        tree_nodes[idx_hijo] = tree_nodes[idx_padre];
        tree_nodes[idx_padre] = temp;
    }

    void sift_up(int n) {
        while (n > 0 && cmp(tree_nodes[n]->value, tree_nodes[(n - 1) / 2]->value)) {
            int p_idx = (n - 1) / 2;
            swap_nodes(n, p_idx);
            n = p_idx;
        }
    }

    void sift_down(int i) {
        int size = tree_nodes.size();
        while (i * 2 + 1 < size) {
            int hijo_elegido = i * 2 + 1;
            int derecho = i * 2 + 2;

            if (derecho < size && cmp(tree_nodes[derecho]->value, tree_nodes[hijo_elegido]->value)) {
                hijo_elegido = derecho;
            }

            if (cmp(tree_nodes[hijo_elegido]->value, tree_nodes[i]->value)) {
                swap_nodes(hijo_elegido, i);
                i = hijo_elegido;
            }
            else {
                break;
            }
        }
    }

public:
    CHeap() : root(nullptr) {}

    ~CHeap() {
        clear(root);
        tree_nodes.clear();
    }

    T top() {
        if (tree_nodes.empty()) return T();
        return root->value;
    }

    bool empty() {
        return tree_nodes.empty();
    }

    int size() {
        return tree_nodes.size();
    }

    void push(T x) {
        Node<T>* t = new Node<T>(x);
        tree_nodes.push_back(t);

        if (tree_nodes.size() == 1) {
            root = t;
            return;
        }

        int n = tree_nodes.size() - 1;
        int p_idx = (n - 1) / 2;

        Node<T>* parentNode = tree_nodes[p_idx];

        if (n % 2 != 0) {
            parentNode->nodes[0] = t;
        }
        else {
            parentNode->nodes[1] = t;
        }

        sift_up(n);
    }

    void pop() {
        if (tree_nodes.empty()) return;

        if (tree_nodes.size() == 1) {
            delete root;
            root = nullptr;
            tree_nodes.pop_back();
            return;
        }

        int last_idx = tree_nodes.size() - 1;
        swap_nodes(last_idx, 0);

        Node<T>* last = tree_nodes.back();
        
        int n = tree_nodes.size() - 1;
        int p_idx = (n - 1) / 2;
        
        Node<T>* parentNode = tree_nodes[p_idx];

        if (n % 2 != 0) {
            parentNode->nodes[0] = nullptr;
        }
        else {
            parentNode->nodes[1] = nullptr;
        }

        delete last;
        
        tree_nodes.pop_back();

        sift_down(0);
    }
};

int main() {
    int valores[] = { 45, 20, 14, 31, 7, 11, 13, 2 };
    int n_valores = sizeof(valores) / sizeof(valores[0]);

    std::cout << "===========================================" << std::endl;
    std::cout << "                  MIN HEAP                 " << std::endl;
    std::cout << "===========================================" << std::endl;

    CHeap<int, std::less<int>> min_heap;

    for (int i = 0; i < n_valores; i++) {
        min_heap.push(valores[i]);
    }

    int esperados_min[] = { 2, 7, 11, 13, 14, 20, 31, 45 };
    int idx = 0;

    while (!min_heap.empty()) {
        int esperado = esperados_min[idx++];
        int real = min_heap.top();

        std::cout << "Esperado: " << esperado << " | Obtenido: " << real << std::endl;

        min_heap.pop();
    }

    std::cout << "\n===========================================" << std::endl;
    std::cout << "                  MAX HEAP                    " << std::endl;
    std::cout << "===========================================" << std::endl;

    CHeap<int, std::greater<int>> max_heap;

    for (int i = 0; i < n_valores; i++) {
        max_heap.push(valores[i]);
    }

    int esperados_max[] = { 45, 31, 20, 14, 13, 11, 7, 2 };
    idx = 0;

    while (!max_heap.empty()) {
        int esperado = esperados_max[idx++];
        int real = max_heap.top();

        std::cout << "Esperado: " << esperado << " | Obtenido: " << real << std::endl;

        max_heap.pop();
    }

    return 0;
}