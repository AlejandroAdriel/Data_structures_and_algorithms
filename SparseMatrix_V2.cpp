#include <iostream>
using namespace std;

struct Nodo
{
    int fil;
    int col;
    int dato;
    Nodo* next;
    Nodo(int f,int c,int v){
     fil=f;
     col=c;
     dato=v;
     next=nullptr;
    }
};
class SparseMatrix
{
private:
Nodo *Mat;
int total_filas;
int total_columnas;

public:
    SparseMatrix(int f,int c){
        total_filas=f;
        total_columnas=c;
        Mat=nullptr;
    }
    ~SparseMatrix(){
        Nodo *tmp;
        while(Mat!=nullptr){
          tmp=Mat;
          Mat=Mat->next;
          delete tmp;
        }
    }
    bool find(int f,int c,Nodo **&p){
        if(f >= total_filas || c >= total_columnas || f < 0 || c < 0) return false;
        for(p=&Mat;*p ;p=&((*p)->next)){
            if((*p)->fil==f && (*p)->col==c)return true;

            if((*p)->fil>f || ((*p)->fil==f && (*p)->col>c))break;  //si no lo encontramos y la fil es mayor al entrado o
        }                                                           //cuando es la misma fila pero ya paso la columna entrada
        return false;

    }
    bool ins(Nodo **p,int f,int c,int val){
         Nodo *nuevo=new Nodo(f,c,val);
         nuevo->next=*p;
         *p=nuevo;
         return true;
    } 
    bool rem(Nodo **p,int f,int c){
        Nodo *aEliminar=*p;
        *p=(*p)->next;
        delete aEliminar;
        return true;
    }
    void set(int fil, int col, int dato){
        Nodo **p=nullptr;
        bool existe=find(fil,col,p);
        if(existe){
            if(dato==0)rem(p,fil,col);   //por def si recibe cero se elimina se la matriz
            else  (*p)->dato=dato;                       //existe entonces debemos reeemplazar el dato
        }
        //no existe entonces lo insertamos mientras no sea cero el dato
        else if(dato!=0){
          ins(p,fil,col,dato);
        }
    }
    int get(int fila, int columna){
        if (fila >= total_filas || columna >= total_columnas || fila < 0 || columna < 0)return 0;
        Nodo **p=nullptr;
        if(find(fila,columna,p))return (*p)->dato;   //si existe retornamos su dato
        return 0;                                    //si no por defincion cero
    }
    void print(){
        for(int i=0;i<total_filas;i++){
            for(int j=0;j<total_columnas;j++){
                cout<<get(i,j)<<"  ";
            }
            cout<<endl;
        }
    }
};

int main() {
    cout << "====================================================\n";
    cout << "      PRUEBA DE MATRIZ ESPARSA (UNICA LISTA)        \n";
    cout << "====================================================\n\n";

    // Creamos una matriz de 5x5 (Filas 0 a 4, Columnas 0 a 4)
    SparseMatrix mat(5, 5);

    cout << "[+] Insertando valores iniciales...\n";
    mat.set(0, 0, 5);   // Inicio absoluto
    mat.set(1, 2, 10);  // Medio
    mat.set(2, 4, 15);  // Final de una fila
    mat.set(4, 0, 20);  // Inicio de la ultima fila
    mat.set(4, 4, 25);  // Final absoluto

    cout << "\nMatriz inicial:\n";
    mat.print();
    cout << "----------------------------------------------------\n";

    cout << "[+] Actualizando el valor en (1, 2) de 10 a 99...\n";
    mat.set(1, 2, 99); 

    cout << "[+] Eliminando el valor en (4, 0) pasandole un 0...\n";
    mat.set(4, 0, 0);

    cout << "[+] Insertando un 42 en (2, 2) para probar el orden...\n";
    mat.set(2, 2, 42);

    cout << "\nMatriz despues de las modificaciones:\n";
    mat.print();
    cout << "====================================================\n";

    return 0;
}