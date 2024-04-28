//ID- 209306513
//Email- guyguy845@gmail.com

#include "Graph.hpp"

using namespace ariel;
using namespace std;

void printGraph() const {
    int numVertices = adjacencyMatrix.size();
    int numEdges = 0;
    for (const auto &row: adjacencyMatrix) {
        for (const auto &element: row) {
            numEdges += element;
        }
    }
    // If the graph is undirected, each edge is counted twice
    if (!isDirected) {
        numEdges /= 2;
    }

    std::cout << "Graph with " << numVertices << " vertices and " << numEdges << " edges.\n";

    for (const auto &row: adjacencyMatrix) {
        for (const auto &element: row) {
            std::cout << element << ' ';
        }
        std::cout << '\n';
    }
}

bool isDirected() const {
    for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); ++j) {
            if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) {
                return true;
            }
        }
    }
    return false;
}
int getNumberOfNodes() const {
    return adjacencyMatrix.size();
}
std::vector<std::pair<int, std::pair<int, int>>> getEdges() const {
    std::vector<std::pair<int, std::pair<int, int>>> edges;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        for (int j = 0; j < adjacencyMatrix[i].size(); ++j) {
            if (adjacencyMatrix[i][j] != 0) {
                edges.push_back({i, {j, adjacencyMatrix[i][j]}});
            }
        }
    }
    return edges;
}


