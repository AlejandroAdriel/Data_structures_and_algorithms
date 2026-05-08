/******************************************************************************

Lista de libros donde el orden se pueda elegir de forma ascendente o 
descendente segun el campo seleccionado

*******************************************************************************/
#include <iostream>
#include <string>

using namespace std;

struct Libro {
    string Titulo;
    string Autor;
    double Precio;
    int Calificacion;

    bool operator==(const Libro& otro) const {
        return Titulo == otro.Titulo;
    }

    friend ostream& operator<<(ostream& os, const Libro& l) {
        os << l.Titulo << " (" << l.Autor << ") - $" << l.Precio << " - " << l.Calificacion << " pts";
        return os;
    }
};

struct ComparadorLibro {
    int modo;
    bool ascendente;

    ComparadorLibro(int m = 1, bool asc = true) : modo(m), ascendente(asc) {}

    bool operator()(const Libro& a, const Libro& b) {
        switch (modo) {
            case 1: 
                return ascendente ? (a.Titulo < b.Titulo) : (a.Titulo > b.Titulo);
            case 2: 
                return ascendente ? (a.Autor < b.Autor) : (a.Autor > b.Autor);
            case 3: 
                return ascendente ? (a.Precio < b.Precio) : (a.Precio > b.Precio);
            case 4: 
                return ascendente ? (a.Calificacion < b.Calificacion) : (a.Calificacion > b.Calificacion);
            default:
                return a.Titulo < b.Titulo;
        }
    }
};

template <typename T>
struct CNode {
    CNode(T v) { value = v; next = nullptr; }
    T value;
    CNode<T>* next; 
};

template <typename T, typename O>
class CSortedList {
public:
    CSortedList(O crit) : head(nullptr), criterio(crit) {}
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
CSortedList<T, O>::~CSortedList() {
    CNode<T>* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T, typename O>
bool CSortedList<T, O>::find(T x, CNode<T>**& p) {
    for ( p = &head; *p && criterio((*p)->value, x);
          p = &( (*p)->next ) );
    
    return *p && (*p)->value == x;
}

template <typename T, typename O>
bool CSortedList<T, O>::ins(T x) {
    CNode<T>** p;
    if ( find(x, p) ) return false;
    CNode<T>* t = new CNode<T>(x);
    t->next = *p;
    *p = t;
    return true;
}

template <typename T, typename O>
bool CSortedList<T, O>::rem(T x) {
    CNode<T>** p;
    if ( !find(x, p) ) return false;
    CNode<T>* t = *p;
    *p = t->next;
    delete t;
    return true;
}

template <typename T, typename O>
void CSortedList<T, O>::print() {
    for ( CNode<T>* t = head; t; t = t->next )
        cout << t->value << endl;
}

int main() {
    int m, ord;
    cout << "Ordenar por (1:Titulo, 2:Autor, 3:Precio, 4:Calificacion): "; cin >> m;
    cout << "Sentido (1:Ascendente, 0:Descendente): "; cin >> ord;

    ComparadorLibro Criterio(m, ord == 1);
    
    CSortedList<Libro, ComparadorLibro> biblioteca(Criterio);

    biblioteca.ins({"El Quijote", "Miguel de Cervantes", 40.34, 4});
    biblioteca.ins({"Metamorfosis", "Franz Kafka", 20.72, 3});
    biblioteca.ins({"Cien anos de soledad", "G. Garcia Marquez", 50.21, 5});
    biblioteca.ins({"Hamlet", "William Shakespeare", 30.99, 5});
    biblioteca.ins({"Crimen y Castigo", "Fiodor Dostoievski", 42.45, 5});

    cout << "\nLista de libros\n";
    cout << "\n"; 
    biblioteca.print();

    return 0;
}