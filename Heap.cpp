/******************************************************************************

Heap basado en std::vector 

*******************************************************************************/
#include <iostream>
#include <vector>
#include <functional>

template<class T>
struct asc {
    bool operator()(T a, T b){
        return a > b;
    }
};

template<class T>
struct des {
    bool operator()(T a, T b){
        return a < b;
    }
};

template<typename T, typename Ord>
struct CHeap {

    std::vector<T> arr;
    Ord cmp;
    
    T top(){
        if(arr.empty()) return T();
        return arr.front();
    }   
    
    void push(T x){
        arr.push_back(x);    
        int n = arr.size() - 1;
        
        while((n > 0) && (cmp(arr[n], arr[(n-1) / 2]))){
            T temp = arr[n];
            arr[n] = arr[(n-1) / 2];
            arr[(n-1) / 2] = temp;
            
            n = (n-1) / 2;
        }
    }
    
    void pop(){
        if(arr.empty()) return;
        
        int i = 0;
        
        T temp_pop = arr[i];
        arr[i] = arr[arr.size()-1];
        arr[arr.size()-1] = temp_pop;
        
        arr.pop_back();
        
        while(i * 2 + 1 < arr.size()){
            int menor = i * 2 + 1;
            int derecho = i * 2 + 2;
            
            if((derecho < arr.size()) && (cmp(arr[derecho], arr[menor]))){
                menor = derecho;
            }
            
            if (cmp(arr[menor], arr[i])){
                T temp_shift = arr[menor];
                arr[menor] = arr[i];
                arr[i] = temp_shift;
                
                i = menor;
            }
            else{
                break;
            }
        }
    }
    
    void print(){
        if(arr.empty()) {
            std::cout << "vacio" << std::endl;
            return;
        }
        for(typename std::vector<T>::iterator it = arr.begin(); it != arr.end(); it++){
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    int valores[] = {45, 20, 14, 31, 7, 11, 13, 2};
    int n_valores = sizeof(valores) / sizeof(valores[0]);

    std::cout << "===========================================" << std::endl;
    std::cout << "                  MIN HEAP                 " << std::endl;
    std::cout << "===========================================" << std::endl;
    
    CHeap<int, std::less<int>> min_heap;
    //CHeap<int, des<int> > min_heap;
    
    for (int i = 0; i < n_valores; i++) {
        min_heap.push(valores[i]);
    }

    int esperados_min[] = {2, 7, 11, 13, 14, 20, 31, 45};
    int idx = 0;

    while (!min_heap.arr.empty()) {
        int esperado = esperados_min[idx++];
        int real = min_heap.top();
        
        std::cout << "Esperado: " << esperado << " | Obtenido: " << real << std::endl;
        
        min_heap.pop();
    }


    std::cout << "\n===========================================" << std::endl;
    std::cout << "                  MAX HEAP                    " << std::endl;
    std::cout << "===========================================" << std::endl;

    CHeap<int, std::greater<int>> max_heap;
    //CHeap<int, asc<int>> max_heap;
    
    for (int i = 0; i < n_valores; i++) {
        max_heap.push(valores[i]);
    }

    int esperados_max[] = {45, 31, 20, 14, 13, 11, 7, 2};
    idx = 0;

    while (!max_heap.arr.empty()) {
        int esperado = esperados_max[idx++];
        int real = max_heap.top();
        
        std::cout << "Esperado: " << esperado << " | Obtenido: " << real << std::endl;
        
        max_heap.pop();
    }

    return 0;
}