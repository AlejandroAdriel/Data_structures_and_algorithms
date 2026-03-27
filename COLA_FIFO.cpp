/******************************************************************************

COLA FIFO

*******************************************************************************/
#include <iostream>

template <class T, int N = 10 >
struct COLA {
	T* Arr;
	int N_Elem;
	T* Head;
	T* Tail;

	COLA();
	~COLA();

	bool vacio();
	bool lleno();
	bool Push(T elem); 
	bool Pop(T &elem); 
};

template <class T, int N>
COLA<T,N>::COLA() {
    N_Elem = N;
    Arr = new T[N_Elem];
    Head = Tail = NULL;
}

template <class T, int N>
COLA<T,N>::~COLA() {
    delete[] Arr;
}

template <class T, int N>
bool COLA<T,N>::vacio(){
    return !Head;
}

template <class T, int N>
bool COLA<T,N>::lleno(){
    if (Head < Tail) return (Tail==Arr+N_Elem && Head == Arr);
    else{
        return (Tail == Head && Head != NULL);
    }
}

template<class T, int N>
bool COLA<T,N>::Push(T VAL){
    if(Tail==Arr+N_Elem && Head!=Arr) Tail=Arr;
    if(lleno()) return false;
    if(vacio()){
        Head = Tail = Arr;
    }
    
    *Tail = VAL;
    Tail++;

    return true;
}

template<class T, int N>
bool COLA<T,N>::Pop(T &VAL){
    if(vacio()) return false;
    
    VAL = *Head;
    Head++;

    if(Head==Tail){
    Head = Tail = NULL;
    }
    if(Head == Arr+N_Elem){
        Head = Arr;
    }

    return true;
}

int main() {

    return 0;
}