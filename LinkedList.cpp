/******************************************************************************

Forward list: Linked list

*******************************************************************************/
#include <iostream>
#include <cassert>

template<class T>
struct NODE {
	T ELEM;
	NODE<T>* NEXT;
	
	NODE(T VAL);
};

template<class T>
NODE<T>::NODE(T VAL){
    ELEM = VAL;
    NEXT = NULL;
}

template<class T>
struct cForwardList {
	unsigned long NELEM;
	NODE<T>* HEAD;

    cForwardList();
    ~cForwardList();
    
	bool vacio();
	T& operator[](unsigned long INDICE);
	bool push_back(T VAL);
	bool pop_back(T& VAL);
	bool push_front(T VAL);
	bool pop_front(T& VAL);
};

template<class T>
cForwardList<T>::cForwardList() {
    NELEM = 0;
    HEAD = NULL;
}

template<class T>
cForwardList<T>::~cForwardList(){
    T x;
    while(!vacio()){
        pop_front(x);
    }
}

template<class T>
bool cForwardList<T>::vacio() {
	return !HEAD;
}

template <class T>
T& cForwardList<T>::operator[](unsigned long INDICE) {
    assert(INDICE < NELEM);
    
    unsigned long i;
    NODE<T>* p;
    
    for(p = HEAD, i = 0; i < INDICE; p = p -> NEXT, i++);
    
    return p -> ELEM;
}

template <class T>
bool cForwardList<T>::push_back(T VAL) {
    NODE<T>* p = new NODE<T>(VAL);
    
    if(vacio()){
        HEAD = p;
    }
    
    else{
        unsigned long i;
        NODE<T>* q;
        
        for(q = HEAD, i = 0; i < NELEM-1; q = q -> NEXT, i++);
        q -> NEXT = p;
    }
    
    NELEM++;
    
	return true;
}

template <class T>
bool cForwardList<T>::pop_back(T& VAL) {
    if(vacio()) return false;
    
    if(HEAD -> NEXT == NULL){
        VAL = HEAD -> ELEM;
        delete HEAD;
        HEAD = NULL;
    }
    
    else{
        unsigned long i;
        NODE<T>* p;
        for(p = HEAD, i = 0; i < NELEM-2; p = p -> NEXT, i++);
        VAL = (p -> NEXT) -> ELEM;
        delete (p -> NEXT);
        p -> NEXT = NULL;
    }
    
    NELEM--;
    
    return true;
}

template <class T>
bool cForwardList<T>::push_front(T VAL) {
    NODE<T>* p = new NODE<T>(VAL);
    p -> NEXT = HEAD;
	HEAD = p;
	NELEM++;

	return true;
}

template<class T>
bool cForwardList<T>::pop_front(T& VAL){
    if (vacio()) return false;
    
    VAL = HEAD -> ELEM;
    
    NODE<T>* p = HEAD;
    HEAD = HEAD -> NEXT;
    delete p;
    
    NELEM--;
    
    return true;
}

int main() {

    return 0;
}