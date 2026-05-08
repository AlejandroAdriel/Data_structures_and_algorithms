/******************************************************************************

pila de arrays (with two templates)

*******************************************************************************/
#include <iostream>
using namespace std;

template< class T, int N = 10>
struct PILA{
    int N_ELEM;
    T *arr;
    T *top;
    
    PILA();
    ~PILA();
    
    bool lleno();
    bool vacio();
    bool push(T VAL);
    bool pop(T &VAL);
    
};

template <class T, int N>
PILA<T,N>::PILA(){
    N_ELEM = N;
    arr = new T[N_ELEM];
    top = NULL;
}

template <class T, int N>
bool PILA<T,N>::lleno(){
    return top == arr+N_ELEM-1;
}

template <class T, int N>
bool PILA<T,N>::vacio(){
    return !top;
}

template <class T, int N>
bool PILA<T,N>::push(T VAL){
    if (vacio()) top = arr;
    else{ 
        if (lleno()) return false;
        else top++;
    }
    
    *top = VAL;
    return true;
}

template <class T, int N>
bool PILA<T,N>::pop(T &VAL){
    if (vacio()) return false;
    VAL=*top;
    top --;
    if (top<arr) top = NULL;
    
    return true;
}

template <class T, int N>
PILA<T,N>::~PILA(){
    delete[] arr;
}

int main() {

    return 0;
}