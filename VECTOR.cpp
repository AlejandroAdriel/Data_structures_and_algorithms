/******************************************************************************

VECTOR

*******************************************************************************/
#include <iostream>

template<class T, int N>
struct cVECTOR{
    T *VECTOR;
    int SIZE;
    T *TAIL;
    
    cVECTOR();
    ~cVECTOR();
    
    T& operator[](int indice);
    bool vacio();
    bool lleno();
    bool expand();
    bool collapse(); 
    bool push_back(T VAL);
    bool pop_back(T &VAL);
    bool push_front(T VAL);
    bool pop_front(T &VAL);
};

template<class T, int N>
cVECTOR<T,N>:: cVECTOR(){
    SIZE = N;
    VECTOR = new T[SIZE];
    TAIL = NULL;
}

template<class T, int N>
cVECTOR<T,N>:: ~cVECTOR(){
    delete[] VECTOR;
}

template<class T, int N>
T& cVECTOR<T,N>:: operator[](int INDICE){
    return *(VECTOR+INDICE);
}

template<class T, int N>
bool cVECTOR<T,N>::vacio(){
    return !TAIL;
}

template<class T, int N>
bool cVECTOR<T,N>::lleno(){
    return (TAIL == VECTOR+SIZE);
}

template<class T, int N>
bool cVECTOR<T,N>::expand(){
    T* p = new T[SIZE*2];
    T* a = VECTOR;
    T* b = p;
    
    while(a < VECTOR+SIZE){
        *b = *a;
        a++;
        b++;
    }
    
    delete[] VECTOR;
    TAIL = p + (TAIL - VECTOR);
    VECTOR = p;
    SIZE = SIZE*2;
    
    return true;
}

template<class T, int N>
bool cVECTOR<T,N>::collapse(){
    if (SIZE/2 < N) return false;
    
    T* p = new T[SIZE/2];
    T* a = VECTOR;
    T* b = p;
    
    while(a < TAIL){
        *b = *a;
        a++;
        b++;
    }
    
    delete[] VECTOR;
    TAIL = p + (TAIL - VECTOR);
    VECTOR = p;
    SIZE = SIZE/2;
    
    return true;
}

template<class T, int N>
bool cVECTOR<T,N>::push_back(T VAL){
    if(vacio()) TAIL = VECTOR;
    if(lleno()) expand();
    
    *TAIL = VAL;
    TAIL++;
    
    return true;
}

template<class T, int N>
bool cVECTOR<T,N>::pop_back(T &VAL){
    if (vacio()) return false;
    
    VAL = *(TAIL-1);
    TAIL--;
    
    if (TAIL == VECTOR) {
        TAIL = NULL;
        return true;
    }
    
    if (TAIL <= VECTOR+(SIZE/4)) collapse();
    
    return true;
}


template<class T, int N>
bool cVECTOR<T,N>::push_front(T VAL){
    if (vacio()) TAIL = VECTOR;
    if (lleno()) expand();
    
    T* a = TAIL;
    T* b = (TAIL-1);
    
    while(b >= VECTOR){
        *a = *b;
        b--;
        a--;
    }
    
    *VECTOR = VAL;
    TAIL++;
    
    return true;
}

template<class T, int N>
bool cVECTOR<T,N>::pop_front(T &VAL){
    if (vacio()) return false;
    
    VAL = *VECTOR;
    
    T* a = VECTOR;
    T* b = VECTOR+1;
    
    while(b < TAIL){
        *a = *b;
        b++;
        a++;
    }
    
    TAIL--;
    
    if (TAIL == VECTOR) {
        TAIL = NULL;
        return true;
    }
    if (TAIL <= VECTOR+(SIZE/4)) collapse();
    
    
    return true;
}

int main() {

    return 0;
}