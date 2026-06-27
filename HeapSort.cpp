#include <iostream>
#include <vector>
class MaxHeap{
  private:
  std::vector<int>heap;
  
  int padre(int i){return (i-1)/2;}
  int hiz(int i){return i*2+1;}
  int hder(int i){return i*2+2;}

  void hundir(int i,int n){
  while(i<n){
     int mayor =i;
     int iz=hiz(i);
     int der=hder(i);
     if(iz<n &&heap[mayor]<heap[iz])mayor=iz;
     if(der <n &&heap[mayor]<heap[der])mayor=der;
     if(mayor==i)break;
     std::swap(heap[i],heap[mayor]);
     i=mayor;}
  }
  void flotar(int i){
     //se compara con su padre
     int n=heap.size();
   while(i>0 && i<n){
     if(heap[i]>heap[padre(i)]){
        std::swap(heap[i],heap[padre(i)]);
        i=padre(i);
     }
     else break;
    }
  }
 
  public:

  void ins(int x){
     heap.push_back(x);
     flotar(heap.size()-1);
  }
  void pop(){
    if(heap.empty())return;
     std::swap(heap[0],heap[heap.size()-1]);
     heap.pop_back();

     if(!heap.empty())hundir(0,heap.size());
  }
   bool empty(){
    return heap.empty();
  }
  int top(){
    return heap[0];
  }

  void heapSorted(){ //ordenara de mayor a menos al veceetor 
   int n=heap.size();
   for(int i=n-1;i>0;i--){
    //pasamos el mayor al ultimo 
    std::swap(heap[0],heap[i]);
    //el mayor ya esta donde debe estar entonces lo ignoramos en la siguiente iteracion
    //por eso hundimos la nueva raiz hasta i osea ignorando el ultimo pos
    //por q se hunde usualmente hasta el limite de heap pero ahora i=n-1 osea 
    //osea hunir recibe como limite i ya no el tamaño por lo cual se actualiza cada iteracion
    hundir(0,i);
   }
   
  }
  void print(){
    if(heap.empty())return;
    for(int i=0;i<heap.size();i++){
        std::cout<<heap[i]<<" ";
        
    }
    std::cout<<std::endl;
  }
};


int main() {
    MaxHeap mi_heap;
    
    // Insertamos elementos (se ordenarán como Max-Heap automáticamente gracias a flotar)
    mi_heap.ins(4);
    mi_heap.ins(10);
    mi_heap.ins(3);
    mi_heap.ins(5);
    mi_heap.ins(1);
    mi_heap.ins(14);
    mi_heap.ins(2);
    
    std::cout << "Estado interno (Max-Heap): ";
    mi_heap.print(); // Verás que el 10 está en la raíz
    
    mi_heap.heapSorted();
    
    std::cout << "Después de heapSorted(): ";
    mi_heap.print(); // Verás: 1 3 4 5 10
    
    return 0;
}
