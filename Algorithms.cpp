#include "Graph.hpp"
#include <queue>
#include <limits>
#include <stdexcept>
#include "Algorithms.hpp"
#include <climits>
#include <algorithm>
using namespace ariel;
using namespace std;

void Algorithms::DFS(const Graph &g, int node, std::vector<bool> &visited) {
    visited[node] = true;
    const auto &neighbors = g.getNeighbors(node);
    for (int neighbor: neighbors) {
        if (!visited[neighbor]) {
            DFS(g, neighbor, visited);
        }
    }
}

bool Algorithms::isConnected(const Graph &g) {
    std::vector<bool> visited(g.getNumberOfNodes(), false);
    DFS(g, 0, visited);
    for (bool visit: visited) {
        if (!visit) {
            return false;
        }
    }
    return true;
}

std::string Algorithms::shortestPath(const Graph& g, int start, int end) {
    int n = g.getNumberOfNodes();
    std::vector<int> dist(n, INT_MAX), pred(n, -1);
    dist[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (const auto& edge : g.getEdges()) {
            int u = edge.first;
            int v = edge.second.first;
            int w = edge.second.second;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pred[v] = u;
            }
        }
    }

    for (const auto& edge : g.getEdges()) {
        int u = edge.first;
        int v = edge.second.first;
        int w = edge.second.second;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            throw std::runtime_error("Graph contains a negative-weight cycle");
        }
    }

    if (dist[end] == INT_MAX) {
        return "No path";
    } else {
        std::vector<int> path;
        for (int v = end; v != -1; v = pred[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());

        std::string pathStr = std::to_string(path[0]);
        for (int i = 1; i < path.size(); ++i) {
            pathStr += "->" + std::to_string(path[i]);
        }
        return pathStr;
    }
}
std::string Algorithms::isContainsCycle(const Graph &g) {
    int numNodes = g.getNumberOfNodes();
    std::vector<bool> visited(numNodes, false);
    std::vector<bool> recStack(numNodes, false);
    std::vector<int> parent(numNodes, -1);

    for(int i = 0; i < numNodes; i++)
        if (!visited[i] && isCyclicUtil(i, visited, recStack, parent, g)) {
            std::vector<int> cycle;
            for (int v = i; v != -1; v = parent[v]) {
                cycle.push_back(v);
            }
            cycle.push_back(i);
            std::reverse(cycle.begin(), cycle.end());

            std::string cycleStr = "The cycle is: ";
            for (int i = 0; i < cycle.size(); ++i) {
                cycleStr += std::to_string(cycle[i]);
                if (i != cycle.size() - 1) {
                    cycleStr += "->";
                }
            }
            return cycleStr;
        }

    return "0";
}

bool Algorithms::isCyclicUtil(int v, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<int>& parent, const Graph& g) {
    visited[v] = true;
    recStack[v] = true;

    std::vector<int> neighbors = g.getNeighbors(v);
    for(int i = 0; i < neighbors.size(); i++) {
        if (!visited[neighbors[i]]) {
            parent[neighbors[i]] = v;
            if (isCyclicUtil(neighbors[i], visited, recStack, parent, g)) {
                return true;
            }
        } else if (recStack[neighbors[i]]) {
            parent[neighbors[i]] = v;
            return true;
        }
    }

    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}

std::string Algorithms::isBipartite(const Graph &g) {
    int numNodes = g.getNumberOfNodes();
    std::vector<int> colorArr(numNodes, -1);
    std::vector<int> setA, setB;

    for (int i = 0; i < numNodes; i++) {
        if (colorArr[i] == -1) {
            std::queue<int> q;
            q.push(i);
            colorArr[i] = 1;
            setA.push_back(i);

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                std::vector<int> neighbors = g.getNeighbors(u);
                for (int v : neighbors) {
                    if (colorArr[v] == -1) {
                        colorArr[v] = 1 - colorArr[u];
                        q.push(v);
                        if (colorArr[v] == 1) {
                            setA.push_back(v);
                        } else {
                            setB.push_back(v);
                        }
                    } else if (colorArr[v] == colorArr[u]) {
                        return "0";
                    }
                }
            }
        }
    }

    std::string result = "The graph is bipartite: A={";
    for (int i = 0; i < setA.size(); i++) {
        result += std::to_string(setA[i]);
        if (i != setA.size() - 1) {
            result += ", ";
        }
    }
    result += "}, B={";
    for (int i = 0; i < setB.size(); i++) {
        result += std::to_string(setB[i]);
        if (i != setB.size() - 1) {
            result += ", ";
        }
    }
    result += "}";

    return result;
}

std::string Algorithms::negativeCycle(const Graph &g) {
    int numNodes = g.getNumberOfNodes();
    std::vector<int> dist(numNodes, INT_MAX);
    std::vector<int> parent(numNodes, -1);
    int source = 0; // Let's take 0 as the source node
    dist[source] = 0;

    // Relax all edges V - 1 times
    for (int i = 0; i < numNodes - 1; i++) {
        for (int u = 0; u < numNodes; u++) {
            std::vector<int> neighbors = g.getNeighbors(u);
            for (int v : neighbors) {
                int weight = g.getEdgeWeight(u, v);
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < numNodes; u++) {
        std::vector<int> neighbors = g.getNeighbors(u);
        for (int v : neighbors) {
            int weight = g.getEdgeWeight(u, v);
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                // Negative cycle found, construct the cycle path
                std::vector<int> cycle;
                for (int v = u; ; v = parent[v]) {
                    cycle.push_back(v);
                    if (v == u && cycle.size() > 1)
                        break;
                }
                std::reverse(cycle.begin(), cycle.end());

                std::string cycleStr = "The cycle is: ";
                for (int i = 0; i < cycle.size(); ++i) {
                    cycleStr += std::to_string(cycle[i]);
                    if (i != cycle.size() - 1) {
                        cycleStr += "->";
                    }
                }
                return cycleStr;
            }
        }
    }

    return "0";
}