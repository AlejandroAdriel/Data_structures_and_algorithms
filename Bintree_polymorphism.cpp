#include <iostream>

struct cOP
{
    virtual bool cmp(int p, int q) = 0;
};
struct OP1 : cOP {
    bool cmp(int p, int q) {
        return p > q;
    }
};
struct OP2 : cOP {
    bool cmp(int p, int q) {
        return p < q;
    }
};

struct Node
{
    int value;
    Node* nodes[2];
    Node(int v) {
        value = v;
        nodes[0] = nodes[1] = nullptr;
    }
};

class CBinTree
{
public:
    CBinTree(cOP* op);
    ~CBinTree();
    void clear(Node* x);
    bool find(int x, Node**& p);
    bool ins(int x);
    bool rem(int x);
    Node** rep(Node** p);
    void print();
    void inorder(Node* n);

private:
    Node* root;
    bool brep;
    cOP* op;
};

CBinTree::CBinTree(cOP* OP)
{   root = 0;  
    brep = 0;
    op = OP;
}

CBinTree::~CBinTree()
{
    clear(root);
}

void CBinTree::clear(Node* x) {
    if (!x) return;
    clear(x->nodes[0]);
    clear(x->nodes[1]);
    delete x;
}

bool CBinTree::find(int x, Node**& p)
{
    for ( p = &root; *p && (*p)->value != x; p = &( (*p)->nodes[ op->cmp(x, (*p)->value) ] ) );
    return *p && (*p)->value == x;
}

bool CBinTree::ins(int x)
{
    Node** p;
    if ( find(x,p) ) return 0;
    *p = new Node(x);
    return 1;
}

bool CBinTree::rem(int x)
{
    Node** p;
    if ( !find(x,p) ) return 0;
    
    if ( (*p)->nodes[0] && (*p)->nodes[1] ) 
    {
        Node** q = rep(p);
        (*p)->value = (*q)->value;
        p = q;
    }
    
    Node* t = *p;
    *p = (*p)->nodes[ (*p)->nodes[1] != 0 ];
    delete t;
    return 1;
}

Node** CBinTree::rep(Node** p)
{
    Node** q;
    for ( q = &( (*p)->nodes[!brep] ); (*q)->nodes[brep] != 0;
         q = &( (*q)->nodes[brep] ) );
    brep = !brep;
    return q;
}

void CBinTree::print()
{
    std::cout<<"inorder "; inorder(root); std::cout<<"\n";
}

void CBinTree::inorder(Node* n)
{
    if (!n) return;
    inorder(n->nodes[0]);
    std::cout<<n->value<<" ";
    inorder(n->nodes[1]);
}

int main()
{
    OP1 asc;
    OP2 des;

    //CBinTree t(&asc);
    CBinTree t(&des);

    int valores[] = { 5, 11, 14, 30, 31, 33, 48, 50, 55, 60, 80 };
    for (int v : valores) {
        t.ins(v);
    }

    t.print();

    int a_eliminar[] = { 31, 11, 14, 33, 50, 60, 55, 5, 80, 48 };

    for (int v : a_eliminar) {
        t.rem(v);
        t.print();
    }

    return 0;
}