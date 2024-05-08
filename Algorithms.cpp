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
//(grp.getDirected() ? grp.getEdgesUndirected() : grp.getEdges() )
void Algorithms::DFS(const Graph &grp, size_t node, std::vector<bool> &visited) {
    // Check if the graph is empty
    if (grp.getNumberOfNodes() == 0) {
        return;
    }
    visited[node] = true;
    std::vector<size_t> neighbors = grp.getNeighbors(node);
    for (size_t neighbor: neighbors) {
        if (!visited[neighbor]) {
            DFS(grp, neighbor, visited);
        }
    }
}

bool Algorithms::isConnected(const Graph &grp) {
    std::vector<bool> visited(grp.getNumberOfNodes(), false);
    DFS(grp, 0, visited);
    return std::all_of(visited.begin(), visited.end(), [](bool ver) { return ver; });
}
string ariel::Algorithms::shortestPath(const Graph& grp, size_t start, size_t end) {
    size_t numNodes = grp.getNumberOfNodes();
    vector<int> dist(numNodes, numeric_limits<int>::max());
    vector<size_t> pred(numNodes, numeric_limits<size_t>::max());
    dist[start] = 0;

    // Relax all edges |V| - 1 times
    for (size_t i = 0; i < numNodes - 1; i++) {
        for (auto edge : (grp.isDirected() ? grp.getEdges() : grp.getEdgesUndirected())) {
            size_t first = static_cast<size_t>(edge.first);
            size_t secend = static_cast<size_t>(edge.second.first);
            int weight = edge.second.second;
            if (dist[first] != numeric_limits<int>::max() && dist[first] + weight < dist[secend]&&pred[secend]!=first) {
                dist[secend] = dist[first] + weight;
                pred[secend] = first;
            }
        }
    }

    // Additional iteration to check for negative-weight cycles
    for (auto edge : (grp.isDirected() ? grp.getEdges() : grp.getEdgesUndirected())) {
        size_t first = static_cast<size_t>(edge.first);
        size_t secend = static_cast<size_t>(edge.second.first);
        int weight = edge.second.second;
        if (dist[first] != numeric_limits<int>::max() && dist[first] + weight < dist[secend]&&pred[secend]!=first) {
            throw runtime_error("Graph contains a negative-weight cycle");
        }
    }

    // Construct the path from start to end
    if (dist[end] == numeric_limits<int>::max()) {
        return "-1";  // No path found
    }

    vector<size_t> path;
    for (size_t at = end; at != start; at = pred[at]) {
        if (at == numeric_limits<size_t>::max() || pred[at] == numeric_limits<size_t>::max()) {
            return "-1";  // No valid path exists
        }
        path.push_back(at);
    }
    path.push_back(start);  // Add the start node at the end
    reverse(path.begin(), path.end());

    // Create a string representation of the path
    string pathStr = to_string(path[0]);
    for (size_t i = 1; i < path.size(); ++i) {
        pathStr += "->" + to_string(path[i]);
    }

    return pathStr;
}

std::vector<size_t> Algorithms::handleCycle(size_t startNode, size_t endNode, std::vector<size_t>& parent) {
    std::vector<size_t> cycle;
    for (size_t p = startNode; p != endNode; p = parent[p]) {
        cycle.push_back(p);
    }
    cycle.push_back(endNode);  // Complete the cycle by adding the start node again
    std::reverse(cycle.begin(), cycle.end());
    return cycle;
}

bool Algorithms::isCyclicUtil(size_t v, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<size_t>& parent, const Graph& grp, std::vector<size_t>& cycle) {
    visited[v] = true;
    recStack[v] = true;
    bool isDirected = grp.isDirected();
    size_t numNodes = grp.getNumberOfNodes();

    for (size_t i = 0; i < numNodes; i++) {
        if (grp.isEdge(v, i)) {
            if (!visited[i]) {
                parent[i] = v;
                if (isCyclicUtil(i, visited, recStack, parent, grp, cycle)) {
                    return true;
                }
            } else if ((isDirected && recStack[i]) || (!isDirected && recStack[i] && parent[v] != i)) {
                cycle = handleCycle(v, i, parent);
                return true;
            }
        }
    }
    recStack[v] = false;
    return false;
}

std::string Algorithms::isContainsCycle(const Graph& grp) {
    size_t numNodes = grp.getNumberOfNodes();
    std::vector<bool> visited(numNodes, false);
    std::vector<bool> recStack(numNodes, false);
    std::vector<size_t> parent(numNodes, SIZE_MAX);
    std::vector<size_t> cycle;

    for (size_t i = 0; i < numNodes; i++) {
        if (!visited[i]) {
            if (isCyclicUtil(i, visited, recStack, parent, grp, cycle)) {
                std::string cycleStr;
                for (size_t j = 0; j < cycle.size(); ++j) {
                    cycleStr += std::to_string(cycle[j]);
                    if (j != cycle.size() - 1) {
                        cycleStr += "->";
                    }
                }
                cycleStr+= "->" + std::to_string(cycle[0]);  // Complete the cycle by adding the start node again
                return cycleStr;
            }
        }
    }
    return "-1";  // Return "-1" if no cycle is found
}
std::string Algorithms::isBipartite(const Graph &grp) {
    size_t numNodes = grp.getNumberOfNodes();
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
                std::vector<size_t> neighbors = grp.getNeighbors(u);
                for (size_t v: neighbors) {
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


std::string Algorithms::negativeCycle(const Graph &grp) {
    size_t numNodes = grp.getNumberOfNodes();
    std::vector<int> dist(numNodes, INT_MAX);
    std::vector<int> parent(numNodes, -1);
    size_t source = 0; // Let's take 0 as the source node
    dist[source] = 0;

    // Relax all edges V - 1 times
    for (size_t i = 0; i < numNodes - 1; i++) {
        for (size_t u = 0; u < numNodes; u++) {
            std::vector<size_t> neighbors = grp.getNeighbors(u);
            for (size_t v: neighbors) {
                int weight = grp.getEdgeWeight(u, v);
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (size_t u = 0; u < numNodes; u++) {
        std::vector<size_t> neighbors = grp.getNeighbors(u);
        for (size_t v: neighbors) {
            int weight = grp.getEdgeWeight(u, v);
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                // Negative cycle found, construct the cycle path
                std::vector<int> cycle;
                for (size_t v = u;; v = static_cast<size_t>(parent[v])) {
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