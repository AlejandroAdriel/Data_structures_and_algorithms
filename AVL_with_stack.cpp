/******************************************************************************

AVL con recursividad controlada mediante un stack

*******************************************************************************/

#include <iostream>
#include <algorithm>
#include <stack>

struct Node
{
    int value;
    int height;
    Node* nodes[2];

    Node(int x) {
        value = x;
        height = 1;
        nodes[0] = nodes[1] = nullptr;
    }
};

class AVLTree
{
public:
    AVLTree();
    ~AVLTree();
    
    void clear(Node* n);
    bool ins(int x);
    bool rem(int x);
    
    void print();

    void preorder(Node* n);

private:
    Node* root;

    int height(Node* n) 
    {
        return n ? n->height : 0;
    }

    int getBalance(Node* n) 
    {
        if (!n) return 0;
        return height(n->nodes[0]) - height(n->nodes[1]);
    }

    void updateHeight(Node* n) 
    {
        if (n) 
        {
            int h0 = height(n->nodes[0]);
            int h1 = height(n->nodes[1]);
            n->height = 1 + (h0 > h1 ? h0 : h1);
        }
    }

    void balance(std::stack<Node**>& camino) 
    {
        while (!camino.empty()) 
        {
            Node** p = camino.top();
            camino.pop();

            if (!*p) continue;

            updateHeight(*p);
            int balanceFactor = getBalance(*p);

            if (balanceFactor > 1) 
            {
                if (getBalance((*p)->nodes[0]) < 0) 
                {
                    Node* leftChild = (*p)->nodes[0];
                    Node* temp = leftChild->nodes[1];

                    leftChild->nodes[1] = temp->nodes[0];
                    temp->nodes[0] = leftChild;

                    updateHeight(leftChild);
                    updateHeight(temp);
                    (*p)->nodes[0] = temp;
                }

                Node* t = (*p)->nodes[0];
                (*p)->nodes[0] = t->nodes[1];
                t->nodes[1] = *p;

                updateHeight(*p);
                updateHeight(t);
                *p = t;
            }
            else if (balanceFactor < -1) 
            {
                if (getBalance((*p)->nodes[1]) > 0) 
                {
                    Node* rightChild = (*p)->nodes[1];
                    Node* temp = rightChild->nodes[0];

                    rightChild->nodes[0] = temp->nodes[1];
                    temp->nodes[1] = rightChild;

                    updateHeight(rightChild);
                    updateHeight(temp);
                    (*p)->nodes[1] = temp;
                }

                Node* t = (*p)->nodes[1];
                (*p)->nodes[1] = t->nodes[0];
                t->nodes[0] = *p;

                updateHeight(*p);
                updateHeight(t);
                *p = t;
            }
        }
    }
};

AVLTree::AVLTree()
{
    root = nullptr;
}

AVLTree::~AVLTree()
{
    clear(root);
}

void AVLTree::clear(Node* n) 
{
    if (!n) return;
    clear(n->nodes[0]);
    clear(n->nodes[1]);
    delete n;
}

bool AVLTree::ins(int x)
{
    Node** p;
    std::stack<Node**> camino;

    for (p = &root; *p && (*p)->value != x; ) 
    {
        camino.push(p);
        p = &((*p)->nodes[x > (*p)->value]);
    }

    if (*p && (*p)->value == x) return 0;

    *p = new Node(x);

    balance(camino);

    return 1;
}

bool AVLTree::rem(int x)
{
    Node** p;
    std::stack<Node**> camino;

    for (p = &root; *p && (*p)->value != x; ) 
    {
        camino.push(p);
        p = &((*p)->nodes[x > (*p)->value]);
    }

    if (!*p) return 0;

    if ((*p)->nodes[0] && (*p)->nodes[1]) 
    {
        camino.push(p);
        Node** q = &((*p)->nodes[0]);
        while ((*q)->nodes[1] != nullptr) 
        {
            camino.push(q);
            q = &((*q)->nodes[1]);
        }

        (*p)->value = (*q)->value;
        p = q;
    }

    Node* t = *p;
    *p = (*p)->nodes[(*p)->nodes[0] == nullptr];
    delete t;

    balance(camino);
    return 1;
}

void AVLTree::print()
{
    std::cout << "preorder "; preorder(root); std::cout << "\n";
}

void AVLTree::preorder(Node* n)
{
    if (!n) return;
    std::cout << n->value << " ";
    preorder(n->nodes[0]);
    preorder(n->nodes[1]);
}

int main()
{
    AVLTree t;

    t.ins(30);
    t.ins(20);
    t.ins(10);

    std::cout << "CASO 1: ROTACION SIMPLE A LA DERECHA (II)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 20 10" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 20 10 30" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.ins(40);
    t.ins(50);

    std::cout << "CASO 2: ROTACION SIMPLE A LA IZQUIERDA (DD)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 20 10 40 50" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 20 10 40 30 50" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.ins(5);
    t.ins(8);

    std::cout << "CASO 3: ROTACION DOBLE IZQUIERDA-DERECHA (ID)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 20 10 5 8 40 50" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 20 8 5 10 40 30 50" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.ins(45);
    t.ins(42);

    std::cout << "CASO 4: ROTACION DOBLE DERECHA-IZQUIERDA (DI)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 20 10 5 8 40 50 45 42" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 20 8 5 10 40 30 45 42 50" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.rem(50);

    std::cout << "CASO 5: ELIMINACION NODO HOJA (50)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 20 10 5 8 40 45 42" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 20 8 5 10 40 30 45 42" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.rem(45);

    std::cout << "CASO 6: ELIMINACION NODO CON UN HIJO (45)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 20 10 5 8 40 42" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 20 8 5 10 40 30 42" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.rem(20);

    std::cout << "CASO 7: ELIMINACION NODO CON DOS HIJOS (20)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 10 5 8 40 42" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 10 8 5 40 30 42" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.ins(25);
    t.ins(28);

    std::cout << "CASO 8: INSERCION CON ROTACION EN CASCADA (25, 28)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 10 5 8 25 28 40 42" << std::endl;
    std::cout << "  -> ESPERADO :  preorder 10 8 5 40 28 25 30 42" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.rem(42);
    t.rem(42);

    std::cout << "CASO 9: ELIMINACION QUE PROVOCA ROTACION DOBLE (42)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 10 5 8 25 28 40 " << std::endl;
    std::cout << "  -> ESPERADO :  preorder 10 8 5 28 25 40 30" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    t.rem(10);

    std::cout << "CASO 10: ELIMINACION DE LA RAIZ PRINCIPAL (10)" << std::endl;
    std::cout << "  -> SIN AVL  :  preorder 30 8 5 25 28 40 " << std::endl;
    std::cout << "  -> ESPERADO :  preorder 28 8 5 25 40 30" << std::endl;
    std::cout << "  -> OBTENIDO :  "; t.print();
    std::cout << "--------------------------------------------------------------------------" << std::endl;

    return 0;
}