/******************************************************************************* 

Heap basado en std::vector 

*******************************************************************************/

#include <iostream>
#include <vector>
#include <functional>
#include <queue>

template<class T>
struct asc {
    bool operator()(T a, T b) {
        return a > b;
    }
};

template<class T>
struct des {
    bool operator()(T a, T b) {
        return a < b;
    }
};

template<typename T, typename Ord>
struct CHeap {

    std::vector<T> arr;
    Ord cmp;

    bool empty() {
        return arr.empty();
    }

    T top() {
        if (arr.empty()) return T();
        return arr.front();
    }

    void push(T x) {
        arr.push_back(x);
        int n = arr.size() - 1;

        while ((n > 0) && (cmp(arr[(n - 1) / 2], arr[n]))) {
            T tmp = arr[n];
            arr[n] = arr[(n - 1) / 2];
            arr[(n - 1) / 2] = tmp;

            n = (n - 1) / 2;
        }
    }

    void pop() {
        if (arr.empty()) return;

        int i = 0;

        T tmp = arr[i];
        arr[i] = arr[arr.size() - 1];
        arr[arr.size() - 1] = tmp;

        arr.pop_back();

        while (i * 2 + 1 < arr.size()) {
            int hijo_elegido = i * 2 + 1;
            int derecho = i * 2 + 2;

            if ((derecho < arr.size()) && (cmp(arr[hijo_elegido], arr[derecho]))) {
                hijo_elegido = derecho;
            }

            if (cmp(arr[i], arr[hijo_elegido])) {
                T tmp = arr[hijo_elegido];
                arr[hijo_elegido] = arr[i];
                arr[i] = tmp;

                i = hijo_elegido;
            }
            else {
                break;
            }
        }
    }

    void print() {
        if (arr.empty()) {
            std::cout << "vacio" << std::endl;
            return;
        }
        for (typename std::vector<T>::iterator it = arr.begin(); it != arr.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    int valores[] = { 45, 20, 14, 31, 7, 11, 13, 2 };
    int n_valores = sizeof(valores) / sizeof(valores[0]);

    std::cout << "===========================================" << std::endl;
    std::cout << "                  MIN HEAP                 " << std::endl;
    std::cout << "===========================================" << std::endl;

    CHeap<int, asc<int>> min_heap;
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

    CHeap<int, des<int>> max_heap;
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