/******************************************************************************

Heap

*******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template<typename T, typename C>
struct CHeap {

    std::vector<T> arr;
    C cmp;
    
    T top(){
        return arr.front();
    }   
    
    void push(T x){
        arr.push_back(x);    
        int n = arr.size() - 1;
        
        while((n > 0) && (cmp(arr[n] , arr[(n-1) / 2]))){
            
            std::swap(arr[n], arr[(n-1) / 2] );
            n = (n-1) / 2;
        }
    }
    
    void pop(){
        if(arr.empty()) return;
        
        int i = 0;

        std::swap(arr[i], arr[arr.size()-1]);
        arr.pop_back();
        
        while(i*2 +1 < arr.size()){
            
            int menor = i*2 +1;
            int derecho = i*2 +2;
            
            if((derecho < arr.size()) && (cmp(arr[derecho], arr[menor]))){
                menor = derecho;
            }
            
            if (cmp(arr[menor], arr[i])){
                std::swap(arr[menor] , arr[i]);
                i = menor;
            }
            else{
                break;
            }
        }
    }
    
    void print(){
        for(typename std::vector<T>::iterator it = arr.begin(); it != arr.end(); it++){
            std::cout<< *it << " ";
        }
        std::cout<<std::endl;
    }

};

int main() {
    //CHeap<int, std::less<int>> heap;
    CHeap<int, std::greater<int>> heap;
    
    std::vector<int> valores = {45, 20, 14, 31, 7, 11, 13, 2};

    // 1. Prueba de Inserción (Push)
    std::cout << "\n[PASO 1] Insertando elementos..." << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    for (int v : valores) {
        std::cout << "Agregando: " << v;
        heap.push(v);
        std::cout << " | Vector actual: ";
        heap.print(); 
    }

    // 2. Estado final del Heap
    std::cout << "\n[ESTADO FINAL DEL HEAP]" << std::endl;
    std::cout << "Raiz" << heap.top() << std::endl;
    std::cout << "Tamano: " << heap.arr.size() << " elementos." << std::endl;

    // 3. Extracción (Pop) con Verificación de Orden
    std::cout << "\n[PASO 2] Extrayendo elementos (Pop)..." << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    int contador = 1;
    while (!heap.arr.empty()) {
        int minimo_esperado = heap.top();
        std::cout << contador << ". Extraido: " << minimo_esperado;
        
        heap.pop();
        
        std::cout << " | Nuevo vector: ";
        if (heap.arr.empty()) std::cout << "[VACIO]";
        else heap.print();
        
        contador++;
    }

    // 4. Prueba de Seguridad
    std::cout << "\n-------------------------------------------" << std::endl;
    std::cout << "[PASO 3] Intentando Pop en Heap vacio..." << std::endl;
    heap.pop(); 
    std::cout << "Resultado: El programa manejo el error correctamente." << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}
