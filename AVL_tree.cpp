/*******************************************************************************

AVL tree

*******************************************************************************/

#include <iostream>
#include <algorithm>

struct Node
{
    int value;
    Node* nodes[2];
    int height;

    Node(int x) {
        value = x;
        nodes[0] = nodes[1] = nullptr;
        height = 1;
    }
};

class AVLTree
{
public:
    AVLTree();
    ~AVLTree();

    void clear(Node* n);
    bool find(int x);
    bool ins(int x);
    bool rem(int x);
    void print();
    void preorder(Node* n);

private:
    Node* root;

    int getHeight(Node* n);
    int getBalance(Node* n);
    Node* balance(Node* n);

    Node* insRecursive(Node* n, int x, bool& inserted);
    Node* remRecursive(Node* n, int x, bool& removed);
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

bool AVLTree::find(int x)
{
    Node* curr = root;
    while (curr) {
        if (curr->value == x) return true;
        curr = curr->nodes[x > curr->value];
    }
    return false;
}

bool AVLTree::ins(int x)
{
    bool inserted = false;
    root = insRecursive(root, x, inserted);
    return inserted;
}

bool AVLTree::rem(int x)
{
    bool removed = false;
    root = remRecursive(root, x, removed);
    return removed;
}

void AVLTree::print()
{
    std::cout << "preorder ";
    preorder(root);
    std::cout << "\n";
}

void AVLTree::preorder(Node* n)
{
    if (!n) return;
    std::cout << n->value << " ";
    preorder(n->nodes[0]);
    preorder(n->nodes[1]);
}

int AVLTree::getHeight(Node* n)
{
    return n ? n->height : 0;
}

int AVLTree::getBalance(Node* n)
{
    return n ? getHeight(n->nodes[0]) - getHeight(n->nodes[1]) : 0;
}

Node* AVLTree::balance(Node* n)
{
    if (!n) return nullptr;

    int h0 = getHeight(n->nodes[0]);
    int h1 = getHeight(n->nodes[1]);
    n->height = 1 + std::max(h0, h1);

    int balanceFactor = h0 - h1;

    if (balanceFactor > 1) {
        if (getBalance(n->nodes[0]) < 0) {
            Node* leftChild = n->nodes[0];
            Node* temp = leftChild->nodes[1];

            leftChild->nodes[1] = temp->nodes[0];
            temp->nodes[0] = leftChild;

            leftChild->height = 1 + std::max(getHeight(leftChild->nodes[0]), getHeight(leftChild->nodes[1]));
            temp->height = 1 + std::max(getHeight(temp->nodes[0]), getHeight(temp->nodes[1]));
            n->nodes[0] = temp;
        }
        Node* t = n->nodes[0];
        n->nodes[0] = t->nodes[1];
        t->nodes[1] = n;

        n->height = 1 + std::max(getHeight(n->nodes[0]), getHeight(n->nodes[1]));
        t->height = 1 + std::max(getHeight(t->nodes[0]), getHeight(t->nodes[1]));
        return t;
    }

    if (balanceFactor < -1) {
        if (getBalance(n->nodes[1]) > 0) {
            Node* rightChild = n->nodes[1];
            Node* temp = rightChild->nodes[0];

            rightChild->nodes[0] = temp->nodes[1];
            temp->nodes[1] = rightChild;

            rightChild->height = 1 + std::max(getHeight(rightChild->nodes[0]), getHeight(rightChild->nodes[1]));
            temp->height = 1 + std::max(getHeight(temp->nodes[0]), getHeight(temp->nodes[1]));
            n->nodes[1] = temp;
        }
        Node* t = n->nodes[1];
        n->nodes[1] = t->nodes[0];
        t->nodes[0] = n;

        n->height = 1 + std::max(getHeight(n->nodes[0]), getHeight(n->nodes[1]));
        t->height = 1 + std::max(getHeight(t->nodes[0]), getHeight(t->nodes[1]));
        return t;
    }

    return n;
}

Node* AVLTree::insRecursive(Node* n, int x, bool& inserted)
{
    if (!n) {
        inserted = true;
        return new Node(x);
    }
    if (x == n->value) return n;

    int dir = x > n->value;
    n->nodes[dir] = insRecursive(n->nodes[dir], x, inserted);

    return balance(n);
}

Node* AVLTree::remRecursive(Node* n, int x, bool& removed)
{
    if (!n) return nullptr;

    if (x < n->value) {
        n->nodes[0] = remRecursive(n->nodes[0], x, removed);
    }
    else if (x > n->value) {
        n->nodes[1] = remRecursive(n->nodes[1], x, removed);
    }
    else {
        removed = true;

        if (!n->nodes[0] || !n->nodes[1]) {
            Node* temp = n->nodes[0] ? n->nodes[0] : n->nodes[1];
            delete n;
            return temp;
        }

        Node* temp = n->nodes[0];
        while (temp->nodes[1] != nullptr) {
            temp = temp->nodes[1];
        }

        n->value = temp->value;
        n->nodes[0] = remRecursive(n->nodes[0], temp->value, removed);
    }

    return balance(n);
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
