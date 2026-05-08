/******************************************************************************

List: Doubly Linked List

*******************************************************************************/
#include <iostream>
#include <cassert>

template <class T>
struct NODE{
    T ELEM;
    NODE<T>* PREV;
    NODE<T>* NEXT;
    
    NODE(T VAL){
        ELEM = VAL;
        PREV = NULL;
        NEXT = NULL;
    }
};

template <class T>
struct cList{
    unsigned long NELEM;
    NODE<T>* HEAD;
    NODE<T>* TAIL;
    
    cList();
    ~cList();
    
    bool vacio();
    T& operator[](unsigned long INDICE);
    bool push_back(T VAL);
    bool pop_back(T& VAL);
    bool push_front(T VAL);
    bool pop_front(T& VAL);
};

template <class T>
cList<T>::cList(){
    NELEM = 0;
    HEAD = NULL;
    TAIL = NULL;
}

template <class T>
cList<T>::~cList(){
    T x;
    while(!vacio()){
        pop_front(x);
    }
}

template <class T>
bool cList<T>::vacio(){
    return !HEAD;
}

template <class T>
T& cList<T>::operator[](unsigned long INDICE){
    assert(INDICE < NELEM);
    
    unsigned long x;
    NODE<T>* p;
    
    if (INDICE <= NELEM/2){
        x = 0;
        p = HEAD;
        
        while(x < INDICE){
        p = p -> NEXT;
        x++;
        }
    }
    
    else{
        x = NELEM-1;
        p = TAIL;
        
        while(x > INDICE){
        p = p -> PREV;
        x--;
        }
    }
    
    return p -> ELEM;
}

template <class T>
bool cList<T>::push_back(T VAL){
    NODE<T>* p = new NODE<T>(VAL);
    if(vacio()){
        HEAD = TAIL = p;
    }
    
    else{
        TAIL -> NEXT = p;
        p -> PREV = TAIL;
        TAIL = p;
    }
    
    NELEM++;
    
    return true;
}

template <class T>
bool cList<T>::pop_back(T& VAL){
    if(vacio()) return false;
    
    VAL = TAIL -> ELEM;
    
    if(TAIL -> PREV == NULL){
        delete TAIL;
        HEAD = TAIL = NULL;
    }
    else{
        TAIL = TAIL -> PREV;
        delete TAIL -> NEXT;
        TAIL -> NEXT = NULL;
    }
    
    NELEM--;
    
    return true;
}

template <class T>
bool cList<T>::push_front(T VAL){
    NODE<T>* p = new NODE<T>(VAL);
    
    if(vacio()){
        HEAD = TAIL = p;
    }
    else{
        HEAD -> PREV = p;
        p -> NEXT = HEAD;
        HEAD = p;
    }
    
    NELEM++;
    
    return true;
}

template <class T>
bool cList<T>::pop_front(T& VAL){
    if(vacio()) return false;
    
    VAL = HEAD -> ELEM;
    
    if(HEAD -> NEXT == NULL){
        delete HEAD;
        HEAD = TAIL = NULL;
    }
    else{
        HEAD = HEAD -> NEXT;
        delete HEAD -> PREV;
        HEAD -> PREV = NULL;
    }
    
    NELEM--;
    
    return true;
}

int main()
{

    return 0;
}