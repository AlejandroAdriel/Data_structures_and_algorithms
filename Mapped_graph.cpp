/******************************************************************************

Mapped Graph

*******************************************************************************/

#include <iostream>
#include <vector>
#include <list>
#include <cassert>
 

using namespace std;

struct Point {
    double x, y;

    Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

template<typename E>
class Edge
{
public:
    int from;
    int to;
    E data; //peso

    Edge(int f, int t, E d)
    {
        from = f;
        to = t;
        data = d;
    }
};

template<typename N, typename E>
class Node
{
public:
    int index;
    N data;
    list<Edge<E>> edges;

    Node(int i, N d)
    {
        index = i;
        data = d;
    }

    void addEdge(int to, E d)
    {
        edges.push_back(Edge<E>(index, to, d));
    }

    void removeEdge(int to)
    {
        for (auto it = edges.begin(); it != edges.end(); ++it)
        {
            if (it->to == to) { edges.erase(it); break; }
        }
    }

    bool hasEdge(int to)
    {
        for (auto& e : edges)
            if (e.to == to) return true;
        return false;
    }
};

template<typename N, typename E>
class Graph
{
private:
    int numVertices;
    bool directed;
    vector<Node<N, E>> nodes;

    

public:
    Graph(bool dir = false);

    int         addNode(N data);
    void        removeNode(int u);
    void        addEdge(int u, int v, E data);
    void        removeEdge(int u, int v);
    bool        hasEdge(int u, int v);
    Node<N, E>& getNode(int u);
    int         vertexCount();
    void        print();
    
    //nuevos metodos
    vector<int> findPathEuclidean(int from, int to);
    vector<int> findPathAStar(int from, int to);

private:
    void validateVertex(int u);
};

template<typename N, typename E>
Graph<N, E>::Graph(bool dir)
{
    numVertices = 0;
    directed = dir;
}

template<typename N, typename E>
int Graph<N, E>::addNode(N data)
{
    nodes.push_back(Node<N, E>(numVertices, data));
    numVertices++;
    return numVertices - 1;
}

template<typename N, typename E>
void Graph<N, E>::removeNode(int u)
{
    validateVertex(u);

    for (int i = 0; i < numVertices; i++)
    {
        if (i == u) continue;
        for (auto it = nodes[i].edges.begin(); it != nodes[i].edges.end(); )
        {
            if (it->to == u)
                it = nodes[i].edges.erase(it);
            else
            {
                if (it->to > u)
                    it->to--;
                ++it;
            }
        }
    }

    nodes.erase(nodes.begin() + u);
    numVertices--;

    for (int i = u; i < numVertices; i++)
        nodes[i].index = i;
}

template<typename N, typename E>
void Graph<N, E>::addEdge(int u, int v, E data)
{
    validateVertex(u);  validateVertex(v);
    nodes[u].addEdge(v, data);
    if (!directed)
        nodes[v].addEdge(u, data);
}

template<typename N, typename E>
void Graph<N, E>::removeEdge(int u, int v)
{
    validateVertex(u);  validateVertex(v);
    nodes[u].removeEdge(v);
    if (!directed)
        nodes[v].removeEdge(u);
}

template<typename N, typename E>
bool Graph<N, E>::hasEdge(int u, int v)
{
    validateVertex(u);  validateVertex(v);
    return nodes[u].hasEdge(v);
}

template<typename N, typename E>
Node<N, E>& Graph<N, E>::getNode(int u)
{
    validateVertex(u);
    return nodes[u];
}

template<typename N, typename E>
int Graph<N, E>::vertexCount()
{
    return numVertices;
}

template<typename N, typename E>
void Graph<N, E>::print()
{
    for (int u = 0; u < numVertices; u++)
    {
        cout << nodes[u].index << "(" << nodes[u].data << ") -> ";
        for (auto& e : nodes[u].edges)
            cout << e.to << "(" << e.data << ") ";
        cout << "\n";
    }
}

#include <cmath>
#include <queue>     
#include <functional>

template<typename N, typename E>
std::vector<int> Graph<N, E>::findPathEuclidean(int from, int to)
{
    validateVertex(from);  validateVertex(to);

    std::vector<double> dist(numVertices, 0.0);
    std::vector<int> parent(numVertices, -1);
    std::vector<bool> hasDist(numVertices, false); 

    std::priority_queue<std::pair<double, int>, 
                        std::vector<std::pair<double, int>>, 
                        std::greater<std::pair<double, int>>> pq;

    dist[from] = 0.0;
    hasDist[from] = true;
    pq.push({ dist[from], from });

    while (!pq.empty())
    {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (hasDist[u] && d > dist[u]) continue;
        if (u == to) break;

        for (auto& edge : nodes[u].edges)
        {
            int v = edge.to;
            
            // OBTENEMOS LAS COORDENADAS DE AMBOS NODOS
            Point p1 = nodes[u].data; 
            Point p2 = nodes[v].data;

            // CÁLCULO DIRECTO DE LA DISTANCIA EUCLIDIANA AQUÍ MISMO
            double weight = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
            double newDist = d + weight;

            if (!hasDist[v] || newDist < dist[v])
            {
                dist[v] = newDist;
                hasDist[v] = true;
                parent[v] = u;
                pq.push({ dist[v], v });
            }
        }
    }

    // Reconstrucción del camino (idéntica a tu código base)
    std::vector<int> path;
    if (!hasDist[to]) return path; 

    for (int curr = to; curr != -1; curr = parent[curr])
    {
        path.push_back(curr);
    }

    int inicio = 0;
    int fin = path.size() - 1;
    while (inicio < fin)
    {
        int temp = path[inicio];
        path[inicio] = path[fin];
        path[fin] = temp;
        inicio++;
        fin--;
    }

    return path;
}

template<typename N, typename E>
std::vector<int> Graph<N, E>::findPathAStar(int from, int to)
{
    validateVertex(from);  validateVertex(to);

    // g_score[u] guardará la distancia real acumulada desde el inicio hasta 'u'
    std::vector<double> g_score(numVertices, 0.0);
    std::vector<int> parent(numVertices, -1);
    std::vector<bool> hasDist(numVertices, false); 

    // Guardamos las posiciones del punto inicial y destino final para la heurística
    Point p_destination = nodes[to].data;

    // Min-Heap que almacena pares de <f_score, índice_del_nodo>
    // Se ordena automáticamente de menor a mayor f_score
    std::priority_queue<std::pair<double, int>, 
                        std::vector<std::pair<double, int>>, 
                        std::greater<std::pair<double, int>>> pq;

    // El g_score inicial del nodo origen es 0
    g_score[from] = 0.0;
    hasDist[from] = true;

    // f_score inicial = g_score (0) + h_score (distancia euclidiana directa al destino)
    Point p_start = nodes[from].data;
    double initial_h = std::sqrt(std::pow(p_start.x - p_destination.x, 2) + 
                                  std::pow(p_start.y - p_destination.y, 2));
    
    pq.push({ initial_h, from });

    while (!pq.empty())
    {
        double current_f = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Si ya llegamos al destino, el camino óptimo ha sido encontrado
        if (u == to) break;

        // Para cada vecino 'v' del nodo actual 'u'
        for (auto& edge : nodes[u].edges)
        {
            int v = edge.to;
            
            Point p_u = nodes[u].data; 
            Point p_v = nodes[v].data;

            // 1. Calcular el peso real de la arista (distancia euclidiana u -> v)
            double weight = std::sqrt(std::pow(p_u.x - p_v.x, 2) + std::pow(p_u.y - p_v.y, 2));
            
            // 2. g_score tentativo para el vecino
            double tentative_g = g_score[u] + weight;

            // Si es la primera vez que alcanzamos a 'v' o encontramos un camino real más corto ($g$)
            if (!hasDist[v] || tentative_g < g_score[v])
            {
                g_score[v] = tentative_g;
                hasDist[v] = true;
                parent[v] = u;

                // 3. Calcular la Heurística $h(v)$ desde el vecino hasta el destino final
                double h_score = std::sqrt(std::pow(p_v.x - p_destination.x, 2) + 
                                           std::pow(p_v.y - p_destination.y, 2));
                
                // 4. El costo total estimado $f(v) = g(v) + h(v)$
                double f_score = tentative_g + h_score;

                pq.push({ f_score, v });
            }
        }
    }

    // Reconstrucción del camino inverso (idéntica a tus funciones anteriores)
    std::vector<int> path;
    if (!hasDist[to]) return path; 

    for (int curr = to; curr != -1; curr = parent[curr])
    {
        path.push_back(curr);
    }

    int inicio = 0;
    int fin = path.size() - 1;
    while (inicio < fin)
    {
        int temp = path[inicio];
        path[inicio] = path[fin];
        path[fin] = temp;
        inicio++;
        fin--;
    }

    return path;
}

template<typename N, typename E>
void Graph<N, E>::validateVertex(int u)
{
    assert(u >= 0 && u < numVertices);
}

int main()
{
    // Instanciamos el Grafo usando la estructura Point para los datos del Nodo (N)
    // y un entero simple para los metadatos de las aristas (E)
    Graph<Point, int> g;

    // Creamos un mapa de coordenadas (X, Y)
    // Imaginemos un escenario donde queremos ir de (0,0) a (6,4)
    int origen    = g.addNode(Point(0.0, 0.0)); // Nodo 0
    int desvio_ar = g.addNode(Point(2.0, 4.0)); // Nodo 1 (Hacia arriba)
    int centro    = g.addNode(Point(3.0, 2.0)); // Nodo 2 (Cerca de la línea recta)
    int desvio_ab = g.addNode(Point(4.0, 0.0)); // Nodo 3 (Hacia abajo)
    int destino   = g.addNode(Point(6.0, 4.0)); // Nodo 4

    // Conectamos los caminos (las aristas)
    // Nota: El peso entero '1' se ignora porque ambos algoritmos calculan el peso
    // geométrico real dinámicamente usando std::sqrt y std::pow.
    g.addEdge(origen, desvio_ar, 1);
    g.addEdge(origen, centro, 1);
    g.addEdge(origen, desvio_ab, 1);

    g.addEdge(desvio_ar, destino, 1);
    g.addEdge(centro, destino, 1);
    g.addEdge(desvio_ab, destino, 1);

    // Conexión interna extra para ver si toman atajos
    g.addEdge(desvio_ab, centro, 1); 

    cout << "==================================================\n";
    cout << "   PRUEBA DE ALGORITMOS GEOMÉTRICOS EN GRAFOS     \n";
    cout << "==================================================\n";
    cout << "Estructura del Grafo y Coordenadas de los Nodos:\n";
    g.print();
    cout << "--------------------------------------------------\n";

    // 1. PROBAR DIJKSTRA EUCLIDIANO
    cout << "\n[1] Ejecutando Dijkstra con Distancia Euclidiana...\n";
    vector<int> caminoDijkstra = g.findPathEuclidean(origen, destino);

    if (caminoDijkstra.empty()) {
        cout << "Dijkstra: No se encontró un camino disponible.\n";
    } else {
        cout << "Camino óptimo encontrado por Dijkstra:\n";
        for (size_t i = 0; i < caminoDijkstra.size(); ++i) {
            int idx = caminoDijkstra[i];
            cout << "Nodo " << idx << " " << g.getNode(idx).data;
            if (i + 1 < caminoDijkstra.size()) cout << " -> ";
        }
        cout << "\n";
    }

    // 2. PROBAR ALGORITMO A* (A-STAR)
    cout << "\n[2] Ejecutando Algoritmo A* (A-Star)...\n";
    vector<int> caminoAStar = g.findPathAStar(origen, destino);

    if (caminoAStar.empty()) {
        cout << "A*: No se encontró un camino disponible.\n";
    } else {
        cout << "Camino óptimo encontrado por A*:\n";
        for (size_t i = 0; i < caminoAStar.size(); ++i) {
            int idx = caminoAStar[i];
            cout << "Nodo " << idx << " " << g.getNode(idx).data;
            if (i + 1 < caminoAStar.size()) cout << " -> ";
        }
        cout << "\n";
    }
    cout << "==================================================\n";

    return 0;
}
