/******************************************************************************

AVL Symmetric Pruning

implementar un AVL con metodo Podar() va a coger ese arbol y va a ir borrando las hojas
hasta que todos los nodos tengan factor de balanceo sea 0, acomodar el programa
para que reciba un array y haga inserciones atraves de un for

*******************************************************************************/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class GraphicNode {
public:
    static void draw(sf::RenderWindow& window, float x, float y, int value, int bf, sf::Font& font) {
        float r = 22.0f;
        sf::CircleShape ring(r);
        ring.setOrigin(r, r);
        ring.setPosition(x, y);

        ring.setFillColor(sf::Color(10, 16, 26));
        ring.setOutlineThickness(2.5f);
        ring.setOutlineColor(sf::Color(0, 255, 150));
        window.draw(ring);

        sf::Text coreText;
        coreText.setFont(font);
        coreText.setString(to_string(value));
        coreText.setCharacterSize(16);
        coreText.setFillColor(sf::Color::White);

        sf::FloatRect bounds = coreText.getLocalBounds();
        coreText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        coreText.setPosition(x, y);
        window.draw(coreText);

        string strBF = (bf > 0) ? "+" + to_string(bf) : to_string(bf);
        sf::Text textBF;
        textBF.setFont(font);
        textBF.setString("bf: " + strBF);
        textBF.setCharacterSize(12);
        textBF.setFillColor(sf::Color(255, 0, 128));

        sf::FloatRect boundsBF = textBF.getLocalBounds();
        textBF.setOrigin(boundsBF.left + boundsBF.width / 2.0f, 0);
        textBF.setPosition(x, y + r + 5.0f);
        window.draw(textBF);
    }
};

class GraphicEdge {
public:
    static void draw(sf::RenderWindow& window, float startX, float startY, float endX, float endY) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(startX, startY), sf::Color(0, 255, 150, 80)),
            sf::Vertex(sf::Vector2f(endX, endY), sf::Color(0, 255, 150, 80))
        };
        window.draw(line, 2, sf::Lines);
    }
};

struct Node {
    int data;
    int depth;
    Node* nodes[2];

    float coordX, coordY;

    Node(int val) {
        data = val;
        depth = 1;
        nodes[0] = nullptr;
        nodes[1] = nullptr;
        coordX = coordY = 0.0f;
    }
};

class BalancedTree {
private:
    Node* treeRoot;

    int getDepth(Node* n) {
        return n ? n->depth : 0;
    }

    void refreshDepth(Node* n) {
        if (n) n->depth = 1 + max(getDepth(n->nodes[0]), getDepth(n->nodes[1]));
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->nodes[1];
        x->nodes[1] = y->nodes[0];
        y->nodes[0] = x;
        refreshDepth(x);
        refreshDepth(y);
        return y;
    }

    Node* rotateRight(Node* x) {
        Node* y = x->nodes[0];
        x->nodes[0] = y->nodes[1];
        y->nodes[1] = x;
        refreshDepth(x);
        refreshDepth(y);
        return y;
    }

    Node* rebalance(Node* n) {
        if (!n) return nullptr;
        refreshDepth(n);
        int bFactor = evalBalance(n);

        if (bFactor > 1) {
            if (evalBalance(n->nodes[1]) < 0) n->nodes[1] = rotateRight(n->nodes[1]);
            return rotateLeft(n);
        }
        if (bFactor < -1) {
            if (evalBalance(n->nodes[0]) > 0) n->nodes[0] = rotateLeft(n->nodes[0]);
            return rotateRight(n);
        }
        return n;
    }

    Node* insertNode(Node* n, int val) {
        if (!n) return new Node(val);
        if (val < n->data) n->nodes[0] = insertNode(n->nodes[0], val);
        else if (val > n->data) n->nodes[1] = insertNode(n->nodes[1], val);
        return rebalance(n);
    }

    bool podar(Node*& n) {
        if (!n) return false;

        if (!n->nodes[0] && !n->nodes[1]) {
            delete n;
            n = nullptr;
            return true;
        }

        int currentBF = evalBalance(n);
        bool status = false;

        if (currentBF > 0) {
            status = podar(n->nodes[1]);
        }
        else if (currentBF < 0) {
            status = podar(n->nodes[0]);
        }
        else {
            if (getDepth(n->nodes[1]) >= getDepth(n->nodes[0])) {
                status = podar(n->nodes[1]);
            }
            else {
                status = podar(n->nodes[0]);
            }
        }

        if (status) {
            refreshDepth(n);
        }
        return status;
    }

public:
    BalancedTree() { treeRoot = nullptr; }

    Node* getRoot() { return treeRoot; }

    int evalBalance(Node* n) {
        return n ? getDepth(n->nodes[1]) - getDepth(n->nodes[0]) : 0;
    }

    void buildWithFor(const vector<int>& elements) {
        for (size_t i = 0; i < elements.size(); ++i) {
            treeRoot = insertNode(treeRoot, elements[i]);
        }
    }

    bool checkSymmetry(Node* n) {
        if (!n) return true;
        if (evalBalance(n) != 0) return false;
        return checkSymmetry(n->nodes[0]) && checkSymmetry(n->nodes[1]);
    }

    bool isSymmetric() {
        if (!treeRoot) return true;
        return checkSymmetry(treeRoot);
    }

    void podar() {
        if (treeRoot && !isSymmetric()) {
            podar(treeRoot);
        }
    }

    void mapCoordinates(Node* p, float x, float y, float dist) {
        if (!p) return;
        p->coordX = x;
        p->coordY = y;
        mapCoordinates(p->nodes[0], x - dist, y + 90.0f, dist * 0.5f);
        mapCoordinates(p->nodes[1], x + dist, y + 90.0f, dist * 0.5f);
    }
};

void drawTreeLines(sf::RenderWindow& window, Node* root) {
    if (!root) return;

    for (int i = 0; i < 2; ++i) {
        if (root->nodes[i]) {
            GraphicEdge::draw(window, root->coordX, root->coordY, root->nodes[i]->coordX, root->nodes[i]->coordY);
            drawTreeLines(window, root->nodes[i]);
        }
    }
}

void drawTreeNodes(sf::RenderWindow& window, Node* root, sf::Font& font, BalancedTree& core) {
    if (!root) return;

    GraphicNode::draw(window, root->coordX, root->coordY, root->data, core.evalBalance(root), font);

    drawTreeNodes(window, root->nodes[0], font, core);
    drawTreeNodes(window, root->nodes[1], font, core);
}

int main() {
    BalancedTree tree;
    vector<int> dataArray = { 12, 8, 18, 5, 11, 17, 4, 2, 6, 20, 25, 30 };
    tree.buildWithFor(dataArray);

    sf::RenderWindow window(sf::VideoMode(1200, 800), "AVL Symmetric Pruning Visualizer");
    window.setFramerateLimit(60);

    sf::View view = window.getDefaultView();
    view.setSize(1200.0f, 800.0f);
    view.setCenter(600.0f, 400.0f);
    view.zoom(1.1f);

    bool isDragging = false;
    sf::Vector2i oldMousePos;

    sf::Font appFont;
    if (!appFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        if (!appFont.loadFromFile("arial.ttf")) {
            return -1;
        }
    }

    sf::Text header;
    header.setFont(appFont);
    header.setCharacterSize(16);
    header.setPosition(20.0f, 20.0f);
    header.setFillColor(sf::Color(0, 180, 255));

    sf::Clock timer;
    float timeDelay = 2.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) view.zoom(0.8f);
                else if (event.mouseWheelScroll.delta < 0) view.zoom(1.2f);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = true;
                oldMousePos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = false;
            }
            if (event.type == sf::Event::MouseMoved && isDragging) {
                sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
                sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, view) - window.mapPixelToCoords(newMousePos, view);
                view.move(delta);
                oldMousePos = newMousePos;
            }
        }

        if (timer.getElapsedTime().asSeconds() >= timeDelay) {
            if (!tree.isSymmetric()) {
                tree.podar();
            }
            timer.restart();
        }

        if (tree.getRoot()) {
            tree.mapCoordinates(tree.getRoot(), 600.0f, 100.0f, 280.0f);
        }

        if (!tree.getRoot()) {
            header.setString("STATUS: Arbol vacio.");
        }
        else if (tree.isSymmetric()) {
            header.setString("STATUS: Arbol simetrico.");
        }
        else {
            header.setString("STATUS: Cortando hojas...");
        }

        window.clear(sf::Color(8, 10, 18));

        window.setView(view);
        drawTreeLines(window, tree.getRoot());
        drawTreeNodes(window, tree.getRoot(), appFont, tree);

        window.setView(window.getDefaultView());
        window.draw(header);

        window.display();
    }

    return 0;
}
