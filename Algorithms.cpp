#include "Graph.hpp"
#include <queue>
#include <limits>
#include "Algorithms.hpp"

using namespace ariel;
using namespace std;

void DFS(const Graph &g, int node, std::vector<bool> &visited) {
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

std::vector<int> Algorithms::shortestPath(const Graph& g, int start, int end) {
    int n = g.getNumberOfNodes();
    std::vector<int> dist(n, INT_MAX);
    dist[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (const auto& edge : g.getEdges()) {
            int u = edge.first;
            int v = edge.second.first;
            int w = edge.second.second;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
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

    return dist[end] == INT_MAX ? std::vector<int>() : std::vector<int>{dist[end]};
}

static bool isContainsCycle(const Graph &g) {
    // Implement your logic here
    return false;
}

static bool isBipartite(const Graph &g) {
    // Implement your logic here
    return false;
}

static bool negativeCycle(const Graph &g) {
    // Implement your logic here
    return false;
}
