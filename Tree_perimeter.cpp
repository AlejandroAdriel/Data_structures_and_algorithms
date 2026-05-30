/******************************************************************************

Dibujo de un árbol binario con SFML 2.5.1
SFML_VS2019

dibujar un arbol binario y pintar los nodos del contorno de un color

hacer clases que pinten los nodos.
que sea reutilizable. 
nodos aristas, hacer clases que pinten nodos, aristas

*******************************************************************************/
#include <iostream>
#include <stack>
#include <algorithm>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct Node
{
    int value;
    Node* nodes[2];

    Node(int v) {
        value = v;
        nodes[0] = nullptr;
        nodes[1] = nullptr;
    }
};

class GraphicNode {
public:
    static void draw(sf::RenderWindow& window, float x, float y, int value, sf::Font& font, bool isPerimeter) {

        sf::Color fillColor = isPerimeter ? sf::Color(255, 0, 255) : sf::Color(15, 15, 20);

        sf::Color outlineColor = sf::Color(0, 255, 255);

        sf::Color textColor = isPerimeter ? sf::Color(15, 15, 20) : sf::Color(150, 255, 255);

        sf::CircleShape circle(22.f);
        circle.setFillColor(fillColor);
        circle.setOutlineColor(outlineColor);
        circle.setOutlineThickness(2.f);
        circle.setOrigin(22.f, 22.f);
        circle.setPosition(x, y);
        window.draw(circle);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(value));
        text.setCharacterSize(18);
        text.setFillColor(textColor);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(x, y);
        window.draw(text);
    }
};

class GraphicEdge {
public:
    static void draw(sf::RenderWindow& window, float x1, float y1, float x2, float y2) {
 
        sf::Color edgeColor(0, 255, 255, 150);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), edgeColor),
            sf::Vertex(sf::Vector2f(x2, y2), edgeColor)
        };
        window.draw(line, 2, sf::Lines);
    }
};

class CBinTree
{
public:
    CBinTree();
    ~CBinTree();

    bool find(int x, Node**& p);
    bool ins(int x);
    bool rem(int x);
    Node** rep(Node** p);
    void clear(Node* n);

    std::vector<int> getPerimeter();
    void draw(sf::RenderWindow& window, sf::Font& font);

private:
    Node* root;
    bool brep;

    void drawRecursive(sf::RenderWindow& window, Node* n, float x, float y, float h_space, sf::Font& font, const std::vector<int>& perimeterNodes, int level);
};

CBinTree::CBinTree() {
    root = nullptr;
    brep = 0;
}

CBinTree::~CBinTree() {
    clear(root);
}

bool CBinTree::find(int x, Node**& p) {
    for (p = &root; *p && (*p)->value != x; p = &((*p)->nodes[x > (*p)->value]));
    return *p && (*p)->value == x;
}

bool CBinTree::ins(int x) {
    Node** p;
    if (find(x, p)) return false;
    *p = new Node(x);
    return true;
}

bool CBinTree::rem(int x) {
    Node** p;
    if (!find(x, p)) return false;

    if ((*p)->nodes[0] && (*p)->nodes[1]) {
        Node** q = rep(p);
        (*p)->value = (*q)->value;
        p = q;
    }

    Node* t = *p;
    *p = (*p)->nodes[!!(*p)->nodes[1]];
    delete t;
    return true;
}

Node** CBinTree::rep(Node** p) {
    p = &((*p)->nodes[brep]);
    for (;*p;p = &((*p)->nodes[!brep]));
    brep = !brep;
    return p;
}

void CBinTree::clear(Node* n) {
    if (!n) return;
    clear(n->nodes[0]);
    clear(n->nodes[1]);
    delete n;
}

std::vector<int> CBinTree::getPerimeter() {
    std::vector<int> perim;
    if (!root) return perim;

    perim.push_back(root->value);

    Node* tmp1 = root->nodes[0];
    while (tmp1 && (tmp1->nodes[0] || tmp1->nodes[1])) {
        perim.push_back(tmp1->value);
        tmp1 = tmp1->nodes[0] ? tmp1->nodes[0] : tmp1->nodes[1];
    }

    std::stack<Node*> s_leaves;
    s_leaves.push(root);

    while (!s_leaves.empty()) {
        Node* n = s_leaves.top();
        s_leaves.pop();

        if (!n->nodes[0] && !n->nodes[1]) {
            if (n != root) perim.push_back(n->value);
        }
        else {
            if (n->nodes[1]) s_leaves.push(n->nodes[1]);
            if (n->nodes[0]) s_leaves.push(n->nodes[0]);
        }
    }

    std::stack<int> s;
    Node* tmp2 = root->nodes[1];

    while (tmp2 && (tmp2->nodes[0] || tmp2->nodes[1])) {
        s.push(tmp2->value);
        tmp2 = tmp2->nodes[1] ? tmp2->nodes[1] : tmp2->nodes[0];
    }

    while (!s.empty()) {
        perim.push_back(s.top());
        s.pop();
    }

    return perim;
}

void CBinTree::draw(sf::RenderWindow& window, sf::Font& font) {
    std::vector<int> perimeterNodes = getPerimeter();

    drawRecursive(window, root, 0.f, 0.f, 1200.f, font, perimeterNodes, 0);
}

void CBinTree::drawRecursive(sf::RenderWindow& window, Node* n, float x, float y, float h_space, sf::Font& font, const std::vector<int>& perimeterNodes, int level) {
    if (!n) return;

    float vertical_space = 180.f;

    float factor = 2.0f;
    if (level >= 4) factor = 1.6f;
    if (level >= 8) factor = 1.2f;
    if (level >= 12) factor = 0.8f; 

    if (n->nodes[0]) {
        GraphicEdge::draw(window, x, y, x - h_space, y + vertical_space);
        drawRecursive(window, n->nodes[0], x - h_space, y + vertical_space, h_space / factor, font, perimeterNodes, level + 1);
    }
    if (n->nodes[1]) {
        GraphicEdge::draw(window, x, y, x + h_space, y + vertical_space);
        drawRecursive(window, n->nodes[1], x + h_space, y + vertical_space, h_space / factor, font, perimeterNodes, level + 1);
    }

    bool isPerim = std::find(perimeterNodes.begin(), perimeterNodes.end(), n->value) != perimeterNodes.end();

    GraphicNode::draw(window, x, y, n->value, font, isPerim);
}

int main()
{
    CBinTree t;

    t.ins(500);

    t.ins(250); t.ins(750);

    t.ins(125); t.ins(375); t.ins(625); t.ins(875);

    t.ins(60);  t.ins(180); t.ins(310); t.ins(430);
    t.ins(570); t.ins(690); t.ins(810); t.ins(940);

    t.ins(30);  t.ins(90);  t.ins(150); t.ins(210); t.ins(280); t.ins(340); t.ins(410); t.ins(460);
    t.ins(540); t.ins(590); t.ins(660); t.ins(720); t.ins(780); t.ins(840); t.ins(910); t.ins(970);

    t.ins(140); t.ins(160); t.ins(330); t.ins(350); t.ins(650); t.ins(670); t.ins(830); t.ins(850);
    t.ins(135); t.ins(145); t.ins(155); t.ins(165); t.ins(645); t.ins(655);
    t.ins(132); t.ins(138); t.ins(642); t.ins(648);

    t.ins(45);  t.ins(105); t.ins(225); t.ins(295); t.ins(525); t.ins(605); t.ins(705); t.ins(925);
    t.ins(42);  t.ins(48);  t.ins(102); t.ins(108); t.ins(522); t.ins(528); t.ins(922); t.ins(928);

    t.ins(15);  t.ins(22);  t.ins(18);  t.ins(20);  t.ins(19);
    t.ins(8);   t.ins(4);   t.ins(6);   t.ins(5);

    t.ins(985); t.ins(978); t.ins(982); t.ins(980); t.ins(981);
    t.ins(992); t.ins(996); t.ins(994); t.ins(995);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Visor de Arbol Binario", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

    sf::View view = window.getDefaultView();
   
    view.setCenter(0.f, 400.f);
    view.zoom(1.5f);

    bool isDragging = false;
    sf::Vector2i oldMousePos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0)
                    view.zoom(0.8f);
                else if (event.mouseWheelScroll.delta < 0)
                    view.zoom(1.2f);
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

        window.clear(sf::Color(10, 10, 15));

        window.setView(view);

        t.draw(window, font);

        window.display();
    }

    return 0;
}