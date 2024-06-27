//guy shimon:

//mail:guyguy845@gmail.com
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
void Algorithms::relaxEdges(const Graph &grp, vector<int> &dist, vector<size_t> &pred) {
    auto edges = grp.getEdges();
    for (auto &edge : edges) {
        size_t first = edge.first;
        size_t second = edge.second.first;
        int weight = edge.second.second;
        if (grp.isDirected()){
            if (dist[first] != numeric_limits<int>::max() && dist[first] + weight < dist[second]) {
                dist[second] = dist[first] + weight;
                pred[second] = first;
            }
        }
        else {
            if (dist[first] != numeric_limits<int>::max() && dist[first] + weight < dist[second]&&pred[first]!=second) {
                dist[second] = dist[first] + weight;
                pred[second] = first;
            }
        }
    }
}

string Algorithms::reconstructPath(size_t start, size_t end, const vector<size_t> &pred) {
    if (pred[end] == numeric_limits<size_t>::max()) {
        return "-1";  // No valid path exists
    }

    vector<size_t> path;
    for (size_t at = end; at != start; at = pred[at]) {
        if (at == numeric_limits<size_t>::max()) {
            return "-1";  // No valid path exists
        }
        path.push_back(at);
    }
    path.push_back(start);

    reverse(path.begin(), path.end());
    string pathStr = to_string(path[0]);
    for (size_t i = 1; i < path.size(); ++i) {
        pathStr += "->" + to_string(path[i]);
    }
    return pathStr;
}
string ariel::Algorithms::shortestPath(const Graph& grp, size_t start, size_t end) {
    size_t numNodes = grp.getNumberOfNodes();
    vector<int> dist(numNodes, numeric_limits<int>::max());
    vector<size_t> pred(numNodes, numeric_limits<size_t>::max());
    dist[start] = 0;

    // Relax all edges |V| - 1 times for shortest path finding
    for (size_t i = 0; i < numNodes - 1; i++) {
        relaxEdges(grp, dist, pred);
    }

    // Additional iteration to check for negative-weight cycles
    vector<int> distCheck = dist;  // Copy dist to check against
    relaxEdges(grp, distCheck, pred);
    if (dist != distCheck) {
        throw runtime_error("Graph contains a negative-weight cycle");
    }

    return reconstructPath(start, end, pred);
}

std::vector<size_t> Algorithms::handleCycle(ariel::StartNode startNode, ariel::EndNode endNode, std::vector<size_t>& parent) {
    std::vector<size_t> cycle;
    for (size_t i = startNode; i != endNode; i = parent[i]) {
        cycle.push_back(i);
    }
    cycle.push_back(endNode);  // Complete the cycle by adding the start node again
    std::reverse(cycle.begin(), cycle.end());
    return cycle;
}

bool Algorithms::isCyclicUtil(size_t ver, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<size_t>& parent, const Graph& grp, std::vector<size_t>& cycle) {
    visited[ver] = true;
    recStack[ver] = true;
    bool isDirected = grp.isDirected();
    size_t numNodes = grp.getNumberOfNodes();

    for (size_t i = 0; i < numNodes; i++) {
        if (grp.isEdge(ver, i)) {
            if (!visited[i]) {
                parent[i] = ver;
                if (isCyclicUtil(i, visited, recStack, parent, grp, cycle)) {
                    return true;
                }
            } else if ((isDirected && recStack[i]) || (!isDirected && recStack[i] && parent[ver] != i)) {
                cycle = handleCycle(ver, i, parent);
                return true;
            }
        }
    }
    recStack[ver] = false;
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
bool Algorithms::tryColorGraph(const Graph& grp, std::vector<int>& colorArr) {
    size_t numNodes = grp.getNumberOfNodes();
    for (size_t i = 0; i < numNodes; i++) {
        if (colorArr[i] == -1) {  // Not colored yet
            std::queue<size_t> queue;
            queue.push(i);
            colorArr[i] = 1;  // Start coloring with 1
            while (!queue.empty()) {
                size_t ver = queue.front();
                queue.pop();
                for (size_t neighbor : grp.getNeighbors(ver)) {
                    if (colorArr[neighbor] == -1) {  // If not colored, color with opposite color
                        colorArr[neighbor] = 1 - colorArr[ver];
                        queue.push(neighbor);
                    } else if (colorArr[neighbor] == colorArr[ver]) {  // If same color as parent
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

std::string Algorithms::buildBipartiteResult(const std::vector<int>& colorArr) {
    std::vector<size_t> setA;
    std::vector<size_t> setB;
    for (size_t i = 0; i < colorArr.size(); i++) {
        if (colorArr[i] == 1) {
            setA.push_back(i);
        }
        else{
            setB.push_back(i);
        }
    }

    std::string result = "The graph is bipartite: A={";
    for (size_t i = 0; i < setA.size(); i++) {
        result += std::to_string(setA[i]);
        if (i != setA.size() - 1) {result += ", ";}
    }
    result += "}, B={";
    for (size_t i = 0; i < setB.size(); i++) {
        result += std::to_string(setB[i]);
        if (i != setB.size() - 1) {result += ", ";}
    }
    result += "}";
    return result;
}

std::string Algorithms::isBipartite(const Graph &grp) {
    size_t numNodes = grp.getNumberOfNodes();
    std::vector<int> colorArr(numNodes, -1);  // -1 indicates uncolored

    if (!tryColorGraph(grp, colorArr)) {
        return "0";  // Not bipartite
    }
    return buildBipartiteResult(colorArr);
}

std::vector<size_t> Algorithms::traceCycle(const std::vector<size_t> &parent, size_t start) {
    std::vector<size_t> cycle;
    size_t current = start;
    do {
        cycle.push_back(current);
        current = parent[current];
    } while (current != start && current != std::numeric_limits<size_t>::max());

    cycle.push_back(start);  // Complete the cycle by adding the start node again
    std::reverse(cycle.begin(), cycle.end());
    return cycle;
}

std::string Algorithms::formatCycle(const std::vector<size_t> &cycle) {
    std::string cycleStr = "The cycle is: ";
    for (size_t i = 0; i < cycle.size(); ++i) {
        cycleStr += std::to_string(cycle[i]);
        if (i != cycle.size() - 1) {cycleStr += "->";}
    }
    return cycleStr;
}

std::string Algorithms::detectAndConstructCycle(const Graph &grp, const std::vector<int> &dist, const std::vector<size_t> &parent) {
    size_t numNodes = grp.getNumberOfNodes();
    for (size_t ver = 0; ver < numNodes; ver++) {
        std::vector<size_t> neighbors = grp.getNeighbors(ver);
        for (size_t neighbor : neighbors) {
            int weight = grp.getEdgeWeight(ver, neighbor);
            if (dist[ver] != std::numeric_limits<int>::max() && dist[ver] + weight < dist[neighbor]) {
                std::vector<size_t> cycle = traceCycle(parent, neighbor);
                return formatCycle(cycle);
            }
        }
    }
    return "0";
}
std::string Algorithms::negativeCycle(const Graph &grp) {
    size_t numNodes = grp.getNumberOfNodes();
    std::vector<int> dist(numNodes, INT_MAX);
    std::vector<size_t > parent(numNodes, SIZE_MAX);
    size_t source = 0;  // Let's take 0 as the source node
    dist[source] = 0;

    relaxEdges(grp, dist, parent);  // Relax all edges V - 1 times
    return detectAndConstructCycle(grp, dist, parent);  // Check for negative-weight cycles
}
