#include <iostream>
#include "Graph.hpp"
#include <queue>
#include <limits>
#include <stdexcept>
#include "Algorithms.hpp"
#include <climits>
#include <algorithm>
#include <unordered_set>
using namespace ariel;
using namespace std;
void Algorithms::DFS(const Graph &g, size_t node, std::vector<bool> &visited) {
    visited[node] = true;
    std::vector<size_t> neighbors = g.getNeighbors(node);
    for (size_t neighbor : neighbors) {
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
//    std::cout << true << std::endl;
    return true;
}

std::string Algorithms::shortestPath(const Graph& g, size_t start, size_t end) {
    size_t n = g.getNumberOfNodes();
    std::vector<int> dist(n, INT_MAX), pred(n, -1);
    dist[start] = 0;

    for (size_t i = 0; i < n - 1; i++) {
        for (const auto& edge : g.getEdges()) {
            size_t u = static_cast<size_t>(edge.first);
            size_t v = static_cast<size_t>(edge.second.first);
            int w = edge.second.second;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pred[v] = u;
            }
        }
    }

    for (const auto& edge : g.getEdges()) {
        size_t u = static_cast<size_t>(edge.first);
        size_t v = static_cast<size_t>(edge.second.first);
        int w = edge.second.second;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            throw std::runtime_error("Graph contains a negative-weight cycle");
        }
    }

    if (dist[end] == INT_MAX) {
        return "No path";
    } else {
        std::vector<int> path;
        for (size_t v = end; v != static_cast<size_t>(-1); v = static_cast<size_t>(pred[v])) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());

        std::string pathStr = std::to_string(path[0]);
        for (size_t i = 1; i < path.size(); ++i) {
            pathStr += "->" + std::to_string(path[i]);
        }

        return pathStr;
    }
}
bool Algorithms::isCyclicUtil(size_t v, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<size_t >& parent, const Graph& g, std::vector<size_t >& cycle) {
    visited[v] = true;
    recStack[v] = true;
    bool isDirected = g.isDirected();

    size_t numNodes = g.getNumberOfNodes();
    for (size_t i = 0; i < numNodes; i++) {
        if (g.isEdge(v, i)) {  // Check if there is an edge from v to i
            if (!visited[i]) {
                parent[i] = v;
                if (isCyclicUtil(i, visited, recStack, parent, g, cycle)) {
                    return true;
                }
            } else if ((isDirected && recStack[i]) || (!isDirected && recStack[i] && parent[v] != i)) {
                // If a cycle is detected, trace back to print the cycle
                cycle.push_back(i);
                for (size_t p = v; p != i; p = parent[p]) {
                    cycle.push_back(p);
                }
                cycle.push_back(i);  // Complete the cycle by adding the start node again
                std::reverse(cycle.begin(), cycle.end());

                // Print the cycle path
                std::cout << "Cycle detected: ";
                for (size_t j = 0; j < cycle.size(); j++) {
                    std::cout << cycle[j];
                    if (j < cycle.size() - 1) std::cout << " -> ";
                }
                std::cout << std::endl;

                return true;
            }
        }
    }

    recStack[v] = false;
    return false;
}

bool Algorithms::isContainsCycle(const Graph& g) {
    size_t numNodes = g.getNumberOfNodes();
    std::vector<bool> visited(numNodes, false);
    std::vector<bool> recStack(numNodes, false);
    std::vector<size_t > parent(numNodes, SIZE_MAX);
    std::vector<size_t> cycle;  // To store the path of the cycle

    for (size_t i = 0; i < numNodes; i++) {
        if (!visited[i]) {
            if (isCyclicUtil(i, visited, recStack, parent, g, cycle)) {
                return true;
            }
        }
    }
    return false;
}
std::string Algorithms::isBipartite(const Graph &g) {
    size_t numNodes = g.getNumberOfNodes();
    std::vector<int> colorArr(numNodes, -1);
    std::vector<size_t> setA, setB;

    for (size_t i = 0; i < numNodes; i++) {
        if (colorArr[i] == -1) {
            std::queue<size_t> q;
            q.push(i);
            colorArr[i] = 1;
            while (!q.empty()) {
                size_t u = q.front();
                q.pop();
                std::vector<size_t> neighbors = g.getNeighbors(u);
                for (size_t v : neighbors) {
                    if (colorArr[v] == -1) {
                        colorArr[v] = 1 - colorArr[u];
                        q.push(v);
                    } else if (colorArr[v] == colorArr[u]) {
//                        std::cout << "0" << std::endl;
                        return "0";
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < numNodes; i++) {
        if (colorArr[i] == 1) {
            setA.push_back(i);
        } else {
            setB.push_back(i);
        }
    }

    std::string result = "The graph is bipartite: A={";
    for (size_t i = 0; i < setA.size(); i++) {
        result += std::to_string(setA[i]);
        if (i != setA.size() - 1) {
            result += ", ";
        }
    }
    result += "}, B={";
    for (size_t i = 0; i < setB.size(); i++) {
        result += std::to_string(setB[i]);
        if (i != setB.size() - 1) {
            result += ", ";
        }
    }
    result += "}";
//    std::cout << result << std::endl;
    return result;
}


std::string Algorithms::negativeCycle(const Graph &g) {
    size_t numNodes = g.getNumberOfNodes();
    std::vector<int> dist(numNodes, INT_MAX);
    std::vector<int> parent(numNodes, -1);
    size_t source = 0; // Let's take 0 as the source node
    dist[source] = 0;

    // Relax all edges V - 1 times
    for (size_t i = 0; i < numNodes - 1; i++) {
        for (size_t u = 0; u < numNodes; u++) {
            std::vector<size_t> neighbors = g.getNeighbors(u);
            for (size_t v : neighbors) {
                int weight = g.getEdgeWeight(u, v);
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (size_t u = 0; u < numNodes; u++) {
        std::vector<size_t> neighbors = g.getNeighbors(u);
        for (size_t v : neighbors) {
            int weight = g.getEdgeWeight(u, v);
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                // Negative cycle found, construct the cycle path
                std::vector<int> cycle;
                for (size_t v = u; ; v = static_cast<size_t>(parent[v])) {
                    cycle.push_back(v);
                    if (v == u && cycle.size() > 1)
                        break;
                }
                std::reverse(cycle.begin(), cycle.end());

                std::string cycleStr = "The cycle is: ";
                for (size_t i = 0; i < cycle.size(); ++i) {
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