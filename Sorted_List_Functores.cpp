/******************************************************************************

Lista enlazada ordenada con Templates y Functores (ASC/DES)

*******************************************************************************/
#include <iostream>

template <typename T>
struct CNode
{
    CNode(T v)
    {   value = v; next = nullptr;   }
    T value;
    CNode<T>* next; 
};

template <typename T>
struct ASC
{
    bool operator()(T a, T b){
        return (a < b);
    }
};

template <typename T>
struct DES
{
    bool operator()(T a, T b){
        return (a > b);
    }
};

template <typename T, typename O>
class CSortedList
{
public:
    CSortedList() : head(nullptr) {}
    ~CSortedList();
    
    bool find(T x, CNode<T>**& p);
    bool ins(T x);
    bool rem(T x);
    void print();

private:
    CNode<T>* head;
    O criterio; 
};

template <typename T, typename O>
CSortedList<T, O>::~CSortedList()
{
    CNode<T>* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T, typename O>
bool CSortedList<T, O>::find(T x, CNode<T>**& p)
{
    for ( p = &head; *p && criterio((*p)->value, x);
          p = &( (*p)->next ) );
    
    return *p && (*p)->value == x;
}

template <typename T, typename O>
bool CSortedList<T, O>::ins(T x)
{
    CNode<T>** p;
    if ( find(x, p) ) return false;
    CNode<T>* t = new CNode<T>(x);
    t->next = *p;
    *p = t;
    return true;
}

template <typename T, typename O>
bool CSortedList<T, O>::rem(T x)
{
    CNode<T>** p;
    if ( !find(x, p) ) return false;
    CNode<T>* t = *p;
    *p = t->next;
    delete t;
    return true;
}

template <typename T, typename O>
void CSortedList<T, O>::print()
{
    for ( CNode<T>* t = head; t; t = t->next )
        std::cout << t->value << " ";
    std::cout << "\n";
}

int main()
{
    CSortedList<int, ASC<int>> l_asc;
    l_asc.ins(3); l_asc.ins(14); l_asc.ins(7); l_asc.ins(11);
    l_asc.print();

    CSortedList<int, DES<int>> l_des;
    l_des.ins(3); l_des.ins(14); l_des.ins(7); l_des.ins(11);
    l_des.print();
    
    return 0;
}