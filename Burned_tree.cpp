/******************************************************************************

Burned tree

*******************************************************************************/
#include <iostream>
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

struct BurnState {
    int value;
    int distance;
};

class GraphicNode {
public:
    static void draw(sf::RenderWindow& window, float x, float y, int value, sf::Font& font, int distance, int maxDistance) {

        sf::Color fillColor;
        sf::Color outlineColor;
        sf::Color textColor;

        if (distance == -1) {
            fillColor = sf::Color(10, 16, 26);
            outlineColor = sf::Color(0, 255, 242, 120);
            textColor = sf::Color(0, 255, 242);
        }
        else {
            float t = 0.0f;
            if (maxDistance > 0) {
                t = static_cast<float>(distance) / static_cast<float>(maxDistance);
            }
            t = std::min(1.0f, std::max(0.0f, t));

            sf::Uint8 r = static_cast<sf::Uint8>(15 + (237 * t));
            sf::Uint8 g = static_cast<sf::Uint8>(10 + (242 * t * t));
            sf::Uint8 b = static_cast<sf::Uint8>(30 + (140 * t));

            fillColor = sf::Color(r, g, b);
            outlineColor = sf::Color(r, g, b, 255);

            if (t > 0.55f) {
                textColor = sf::Color(15, 10, 30);
            }
            else {
                textColor = sf::Color(255, 255, 255);
            }
        }

        sf::CircleShape circle(24.f);
        circle.setFillColor(fillColor);
        circle.setOutlineColor(outlineColor);
        circle.setOutlineThickness(2.f);
        circle.setOrigin(24.f, 24.f);
        circle.setPosition(x, y);
        window.draw(circle);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(value));
        text.setCharacterSize(15);
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
        sf::Color edgeColor(60, 65, 75, 90); 

        sf::Vector2f dir(x2 - x1, y2 - y1);
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        sf::Vector2f normal(-dir.y / len, dir.x / len);

        float offsets[] = { -0.4f, 0.0f, 0.4f };

        for (float offset : offsets) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x1 + normal.x * offset, y1 + normal.y * offset), edgeColor),
                sf::Vertex(sf::Vector2f(x2 + normal.x * offset, y2 + normal.y * offset), edgeColor)
            };
            window.draw(line, 2, sf::Lines);
        }
    }
};

class CBinTree
{
public:
    CBinTree();
    ~CBinTree();

    bool find(int x, Node**& p);
    bool ins(int x);
    void clear(Node* n);

    std::vector<BurnState> burn(int targetValue, int& maxDistance);
    void draw(sf::RenderWindow& window, sf::Font& font, const std::vector<BurnState>& burnedNodes, int maxDistance);

private:
    Node* root;
    void mapParents(Node* curr, Node* parent, std::vector<std::pair<Node*, Node*>>& parentPairs);
    void drawRecursive(sf::RenderWindow& window, Node* n, float x, float y, float h_space, sf::Font& font, const std::vector<BurnState>& burnedNodes, int maxDistance, int level);
};

CBinTree::CBinTree() { root = nullptr; }
CBinTree::~CBinTree() { clear(root); }

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

void CBinTree::clear(Node* n) {
    if (!n) return;
    clear(n->nodes[0]);
    clear(n->nodes[1]);
    delete n;
}

void CBinTree::mapParents(Node* curr, Node* parent, std::vector<std::pair<Node*, Node*>>& parentPairs) {
    if (!curr) return;
    if (parent) {
        parentPairs.push_back({ curr, parent });
    }
    mapParents(curr->nodes[0], curr, parentPairs);
    mapParents(curr->nodes[1], curr, parentPairs);
}

std::vector<BurnState> CBinTree::burn(int targetValue, int& maxDistance) {
    std::vector<BurnState> nodesBurned;
    maxDistance = 0;

    Node** targetPtr;
    if (!find(targetValue, targetPtr)) return nodesBurned;
    Node* targetNode = *targetPtr;

    std::vector<std::pair<Node*, Node*>> parentPairs;
    mapParents(root, nullptr, parentPairs);

    auto getParent = [&](Node* n) -> Node* {
        for (const auto& pair : parentPairs) {
            if (pair.first == n) return pair.second;
        }
        return nullptr;
        };

    std::vector<Node*> queue;
    std::vector<int> distances;

    queue.push_back(targetNode);
    distances.push_back(0);
    nodesBurned.push_back({ targetNode->value, 0 });

    size_t head = 0;
    while (head < queue.size()) {
        Node* curr = queue[head];
        int currentDist = distances[head];
        head++;

        if (currentDist > maxDistance) maxDistance = currentDist;

        if (curr->nodes[0]) {
            bool visited = false;
            for (Node* v : queue) { if (v == curr->nodes[0]) { visited = true; break; } }
            if (!visited) {
                queue.push_back(curr->nodes[0]);
                distances.push_back(currentDist + 1);
                nodesBurned.push_back({ curr->nodes[0]->value, currentDist + 1 });
            }
        }
        if (curr->nodes[1]) {
            bool visited = false;
            for (Node* v : queue) { if (v == curr->nodes[1]) { visited = true; break; } }
            if (!visited) {
                queue.push_back(curr->nodes[1]);
                distances.push_back(currentDist + 1);
                nodesBurned.push_back({ curr->nodes[1]->value, currentDist + 1 });
            }
        }
        Node* parent = getParent(curr);
        if (parent) {
            bool visited = false;
            for (Node* v : queue) { if (v == parent) { visited = true; break; } }
            if (!visited) {
                queue.push_back(parent);
                distances.push_back(currentDist + 1);
                nodesBurned.push_back({ parent->value, currentDist + 1 });
            }
        }
    }

    std::cout << "\n";
    std::cout << "  Nodo inicial quemado: " << targetValue << "\n";
    std::cout << "  Profundidad maxima: " << maxDistance << " anillos.\n\n";

    for (int r = 0; r <= maxDistance; ++r) {
        std::cout << "  Anillo " << r << " -> Nodos: ";
        for (const auto& state : nodesBurned) {
            if (state.distance == r) {
                std::cout << state.value << " ";
            }
        }
        std::cout << "\n";
    }
    return nodesBurned;
}

void CBinTree::draw(sf::RenderWindow& window, sf::Font& font, const std::vector<BurnState>& burnedNodes, int maxDistance) {
    drawRecursive(window, root, 0.f, 0.f, 720.f, font, burnedNodes, maxDistance, 0);
}

void CBinTree::drawRecursive(sf::RenderWindow& window, Node* n, float x, float y, float h_space, sf::Font& font, const std::vector<BurnState>& burnedNodes, int maxDistance, int level) {
    if (!n) return;

    float vertical_space = 140.f;
    float factor = 2.0f;

    if (level >= 2) factor = 1.8f;
    if (level >= 4) factor = 1.4f;

    if (n->nodes[0]) {
        GraphicEdge::draw(window, x, y, x - h_space, y + vertical_space);
        drawRecursive(window, n->nodes[0], x - h_space, y + vertical_space, h_space / factor, font, burnedNodes, maxDistance, level + 1);
    }
    if (n->nodes[1]) {
        GraphicEdge::draw(window, x, y, x + h_space, y + vertical_space);
        drawRecursive(window, n->nodes[1], x + h_space, y + vertical_space, h_space / factor, font, burnedNodes, maxDistance, level + 1);
    }

    int currentDistance = -1;
    for (const auto& state : burnedNodes) {
        if (state.value == n->value) {
            currentDistance = state.distance;
            break;
        }
    }

    GraphicNode::draw(window, x, y, n->value, font, currentDistance, maxDistance);
}

int main()
{
    CBinTree t;

    t.ins(600);
    t.ins(300); t.ins(150); t.ins(450); t.ins(75);  t.ins(220);
    t.ins(380); t.ins(520); t.ins(35);  t.ins(110); t.ins(180);
    t.ins(260); t.ins(340); t.ins(410); t.ins(480); t.ins(560);
    t.ins(15);  t.ins(50);  t.ins(90);  t.ins(130); t.ins(160);
    t.ins(200); t.ins(240); t.ins(280); t.ins(320); t.ins(360);
    t.ins(395); t.ins(430); t.ins(465); t.ins(495); t.ins(540);
    t.ins(580);
    t.ins(900); t.ins(750); t.ins(1050); t.ins(680); t.ins(820);
    t.ins(980); t.ins(1120); t.ins(640); t.ins(710); t.ins(790);
    t.ins(860); t.ins(940); t.ins(1010); t.ins(1080); t.ins(1160);
    t.ins(620); t.ins(660); t.ins(695); t.ins(730); t.ins(770);
    t.ins(810); t.ins(840); t.ins(880); t.ins(920); t.ins(960);
    t.ins(995); t.ins(1030); t.ins(1065); t.ins(1100); t.ins(1140);
    t.ins(1180);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Burned Tree Visualizer", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

    sf::View view = window.getDefaultView();
    view.setCenter(0.f, 320.f);
    view.zoom(1.4f);

    bool isDragging = false;
    sf::Vector2i oldMousePos;

    int maxDistance = 0;
    std::vector<BurnState> burnedNodes = t.burn(580, maxDistance);

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

        window.clear(sf::Color(8, 10, 15));
        window.setView(view);

        t.draw(window, font, burnedNodes, maxDistance);

        window.display();
    }

    return 0;
}