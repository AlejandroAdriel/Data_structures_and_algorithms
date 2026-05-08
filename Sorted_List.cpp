/******************************************************************************

Sorted List

*******************************************************************************/
#include <iostream>

struct node {
  int data;
  node* next;
  
  node(int VAL){
      data = VAL;
      next = nullptr;
  }
};

struct Sorted_List{
    node* head;
    
    Sorted_List();
    ~Sorted_List();
    
    bool find(int VAL, node**& p);
    bool ins(int VAL);
    bool rem(int VAL);
    void print();
};

Sorted_List::Sorted_List(){
    head = nullptr;
}

Sorted_List::~Sorted_List(){

    while(head){
    
    node* p = head; 
    head = head -> next;
    delete p;
}

}

bool Sorted_List::find(int VAL, node**& p){
    
    for(p = &head; *p && ((*p) -> data < VAL); p = &((*p) -> next));

    return *p && (*p) -> data == VAL;
}

bool Sorted_List::ins(int VAL){
    node** p;
    if(find(VAL, p)) return false;
    
    node* q = new node(VAL);
    (q -> next) = *p;
    *p = q;
    
    return true;
}

bool Sorted_List::rem(int VAL){
    node** p;
    
    if(!find(VAL, p)) return false;
    
    node* q = *p;
    
    *p = (*p) -> next;
    
    delete q;
    
    return true;
}

void Sorted_List::print(){
    node** p = &head;
    for(p; *p; p = &((*p) -> next)){
        std::cout<< (*p) -> data << " -> ";
    }
}

int main()
{
    Sorted_List arr;
    arr.ins(9);
    arr.ins(2);
    arr.ins(8);
    arr.ins(7);
    arr.ins(4);
    arr.print();

    return 0;
}