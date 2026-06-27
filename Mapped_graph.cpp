/******************************************************************************

Mapped Graph

*******************************************************************************/

#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <cmath>
#include <queue>     
#include <functional> 

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
    int numVertices; //nro de nodos 
    bool directed;
    vector<Node<N, E>> nodes;

    // Estructuras para Floyd-Warshall (precalculado)
    vector<vector<E>> distMatrix;
    vector<vector<int>> nextMatrix;

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
    vector<int> findPath(int from, int to);
    void        precalcAllPaths();
    vector<int> findPathFromPreCalc(int from, int to);
    
    vector<int> findPathEuclidean(int from, int to);

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



template<typename N, typename E>
std::vector<int> Graph<N, E>::findPath(int from, int to)
{
    validateVertex(from);  validateVertex(to);

    /* Algoritmo para encontrar un camino entre 'from' (origen) y 'to' (destino), recorriendo el grafo.   
    Utiliza el Algoritmo de Dijkstra para encontrar el camino con el menor peso acumulado. */

    std::vector<E> dist(numVertices);
    std::vector<int> parent(numVertices, -1);
    std::vector<bool> hasDist(numVertices, false); // Rastrea si el nodo ya tiene una distancia válida

    // Cola de prioridad: almacena pares de <distancia_acumulada, índice_del_nodo>
    std::priority_queue<std::pair<E, int>, std::vector<std::pair<E, int>>, std::greater<std::pair<E, int>>> pq;

    dist[from] = E(); // Constructor por defecto (inicializa en 0 de forma genérica)
    hasDist[from] = true;
    pq.push({ dist[from], from }); // Se envía la distancia inicial de origen

    while (!pq.empty())
    {
        E d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Si ya encontramos una distancia menor para este nodo, lo ignoramos
        if (hasDist[u] && d > dist[u]) continue;

        // Si ya llegamos al destino, podemos terminar la búsqueda temprano
        if (u == to) break;

        // Para cada vecino conectado al nodo 'u':
        for (auto& edge : nodes[u].edges)
        {
            int v = edge.to;
            E weight = edge.data;
            E newDist = d + weight;

            // ¿Es mejor pasar por 'u' para llegar a 'v' que lo que teníamos guardado?
            // Se evalúa si v no tenía distancia previa asignada, o si la nueva ruta es menor
            if (!hasDist[v] || newDist < dist[v])
            {
                dist[v] = newDist;          // Registramos el nuevo costo mínimo para 'v'
                hasDist[v] = true;
                parent[v] = u;              // Guardamos que a 'v' se llega mejor desde 'u'
                pq.push({ dist[v], v });    // Ponemos al vecino en la lista para revisarlo después
            }
        }
    }

    // Reconstruimos el camino desde el destino hacia el origen
    std::vector<int> path;

    if (!hasDist[to]) return path; // No existe ningún camino accesible

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
void Graph<N, E>::precalcAllPaths()
{
    /* Algoritmo para computar los caminos entre todos los pares de nodos posibles   
        y almacenarlos en una estructura precomputada. Utiliza el algoritmo de Floyd-Warshall. */

    distMatrix.assign(numVertices, std::vector<E>(numVertices, E()));
    nextMatrix.assign(numVertices, std::vector<int>(numVertices, -1)); // -1 indicará que NO hay ruta conocida

    // Paso 1: Inicializar con los caminos directos (aristas base)
    for (int i = 0; i < numVertices; i++)
    {
        distMatrix[i][i] = E(); // Distancia de un nodo a sí mismo es cero (constructor por defecto)
        nextMatrix[i][i] = i;

        for (auto& edge : nodes[i].edges)
        {
            distMatrix[i][edge.to] = edge.data;
            nextMatrix[i][edge.to] = edge.to;
        }
    }

    // Paso 2: Floyd-Warshall incorporando nodos intermediarios 'k'
    for (int k = 0; k < numVertices; k++)
    {
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                // Validación de existencia: Solo evaluamos si hay un camino real de 'i' a 'k' y de 'k' a 'j'
                if (nextMatrix[i][k] != -1 && nextMatrix[k][j] != -1)
                {
                    E newDist = distMatrix[i][k] + distMatrix[k][j];

                    // Si no había ninguna ruta previa entre 'i' y 'j', o si la nueva ruta a través de 'k' es más corta
                    if (nextMatrix[i][j] == -1 || newDist < distMatrix[i][j])
                    {
                        distMatrix[i][j] = newDist;
                        nextMatrix[i][j] = nextMatrix[i][k]; // Mantiene la ruta del siguiente paso
                    }
                }
            }
        }
    }
}

template<typename N, typename E>
vector<int> Graph<N, E>::findPathFromPreCalc(int from, int to)
{
    /* Recupera el camino precomputado entre 'from' y 'to' en O(1) tiempo secuencial por nodo,
       asume que la función precalcAllPaths() fue llamada con anterioridad. */

    vector<int> path;

    // Si no hay camino registrado o las matrices están vacías
    if (nextMatrix.empty() || nextMatrix[from][to] == -1)
        return path;

    // Reconstrucción del camino usando la matriz de reconstrucción dinámica de rutas
    int curr = from;
    path.push_back(curr);

    while (curr != to)
    {
        curr = nextMatrix[curr][to];
        if (curr == -1) return vector<int>(); 
        path.push_back(curr);
    }

    return path;
}

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
void Graph<N, E>::validateVertex(int u)
{
    assert(u >= 0 && u < numVertices);
}

int main()
{
    // Grafo donde cada Nodo guarda un Point, y la arista un entero cualquiera
    Graph<Point, int> g;

    int a = g.addNode(Point(0, 0));
    int b = g.addNode(Point(3, 4)); // Distancia geométrica de A a B es 5
    int c = g.addNode(Point(0, 4));
    int d = g.addNode(Point(6, 4));

    g.addEdge(a, b, 1);
    g.addEdge(a, c, 1);
    g.addEdge(c, b, 1);
    g.addEdge(b, d, 1);

    cout << "Estructura del Grafo Geométrico:";
    g.print();

    vector<int> camino = g.findPathEuclidean(a, d);
    
    cout << "\nCamino más corto (Dijkstra Euclidiano): ";
    for(int idx : camino) {
        cout << idx << " " << g.getNode(idx).data << " -> ";
    }
    cout << "FIN\n";

    return 0;
}
