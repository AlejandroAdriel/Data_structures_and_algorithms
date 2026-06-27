#include <iostream>
struct Node{
    int fil;
    int col;
    int dato;
    Node *next;
    Node(int f,int c,int v){
       fil=f;
       col=c;
       next=nullptr;
       dato=v;
    }
};

class SparseMatrix{
    private:
    Node **rows;
    int filasTotales;
    int colTotales;
    public:
    SparseMatrix(int f ,int c){
     filasTotales=f;
     colTotales=c;
     rows=new Node*[f];
     //reservamos solo f espacios 
     for(int i=0;i<f;i++)rows[i]=nullptr;
    }
    ~SparseMatrix(){
        //  borramos todos los nodos de cada fila
    for (int i = 0; i < filasTotales; i++) {
        Node *tmp;
        while (rows[i] != nullptr) {
            tmp = rows[i];
            rows[i] = rows[i]->next;
            delete tmp;
        }
    }
        delete [] rows;
    }
    bool find(Node **&p,int f ,int c){   //la busqueda por fila es o1 y por columna on
       if(f>=filasTotales || c >= colTotales || f<0 || c<0)return false;

       for(p=&rows[f];*p && (*p)->col<c;p=&((*p)->next));  //condicion mientra p no sea nulo y el nodo no pase la columna pedida
                                                           //se detiene cuando sobrepasa la columna o es igual a la pedida
       if((*p)&&(*p)->col==c) return true ;
       return false;                                                   
      /*otra version del for
       for(p=&rows[f];*p ;p=&((*p)->next)){
       if((*p)->col==c)return true;
       if((*p)->col>c)break;
       }
       return false
      */
    }

    bool ins(Node **p,int f,int c,int valor){
       Node *nuevo=new Node(f,c,valor);
       nuevo->next=*p;
       *p=nuevo;
       return true;
    }
    bool rem(Node **p,int f,int c){
        Node *aEliminar=*p;
        *p=(*p)->next;
        delete aEliminar;
        return true;
    }
    void set(int f,int c,int v){
       Node **p=nullptr;
       bool existe=find(p,f,c);
       if(existe){
         if(v==0)rem(p,f,c);               //por def de matriz esparsa los ceros no se guardan se eliminan
         else (*p)->dato=v;                             //si v no es cero solo se reemplaza el valor   
       }
       else if(v!=0){                      //si el nodo no existe y v !=0 entonces necesita insercion
          ins(p,f,c,v);                                 //si v es cero no se hace nada por definicion de esparza  
       }
    }
    int get(int f,int c){
    if (f >= filasTotales || c >= colTotales || f < 0 || c < 0)return 0;
    Node **p=nullptr;
    if(find(p,f,c))return (*p)->dato;                 //retorna el dato del nodo si existe si no cero por def de esparza
    return 0;
    }

    void print(){
        for(int i=0;i<filasTotales;i++){
            for(int j=0;j<colTotales;j++){
                std::cout<<get(i,j)<<"  ";
            }
            std::cout<<std::endl;
        }
    }
};
int main() {
    using namespace std;

    cout << "====================================================\n";
    cout << "   PRUEBA: MATRIZ ESPARSA (ARREGLO DE LISTAS)       \n";
    cout << "====================================================\n\n";

    // Creamos una matriz de 4x4
    int filas = 4, columnas = 4;
    SparseMatrix mat(filas, columnas);

    cout << "[+] Insertando datos...\n";
    mat.set(0, 0, 5);   // Fila 0
    mat.set(0, 3, 10);  // Fila 0 (Misma fila, probando insercion al final)
    mat.set(1, 1, 15);  // Fila 1
    mat.set(2, 2, 20);  // Fila 2
    mat.set(3, 0, 25);  // Fila 3

    

    cout << "\nMatriz Inicial:\n";
    mat.print();

    cout << "[+] Actualizando (1, 1) de 15 a 99...\n";
    mat.set(1, 1, 99);

    cout << "[+] Insertando 8 en (0, 1) (Intercalado en fila 0)...\n";
    mat.set(0, 1, 8);

    cout << "[+] Eliminando el 20 en (2, 2) pasandole un 0...\n";
    mat.set(2, 2, 0);

    cout << "\nMatriz despues de las modificaciones:\n";
     mat.print();

    cout << "====================================================\n";
    cout << "Limpieza de memoria (Destructor) ejecutandose al salir...\n";

    return 0;
}