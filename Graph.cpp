//guy shimon:
//mail:guyguy845@gmail.com
#include "Graph.hpp"
#include <iostream>
using namespace ariel;
using namespace std;

void Graph::loadGraph(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) {
        throw std::invalid_argument("The graph cannot be empty");
    }
    // Check if the matrix is square
    size_t size = matrix.size();
    for (const auto& row : matrix) {
        if (row.size() != size) {
            throw std::invalid_argument("Invalid graph: The graph is not a square matrix.");
        }
    }
    this->directed=this->isDirected();
    // If the matrix is square, load it into the adjacencyMatrix
    this->adjacencyMatrix = matrix;
}
void Graph::printGraph() const {
    size_t numVertices = getNumberOfNodes();
    int numEdges = 0;
    for (const auto &row: adjacencyMatrix) {
        for (const auto &element: row) {
            if (element != 0){
                numEdges++;}
        }
    }
    // If the graph is undirected, each edge is counted twice
    if (getDirected()) {
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

bool Graph::isDirected() const {
    for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); ++j) {
            if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) {
                return true;
            }
        }
    }
    return false;
}

size_t Graph::getNumberOfNodes() const {
    return adjacencyMatrix.size();
}

std::vector<std::pair<size_t , std::pair<size_t, int>>> Graph::getEdges() const {
    std::vector<std::pair<size_t , std::pair<size_t , int>>> edges;
    for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); ++j) {
            if (adjacencyMatrix[i][j] != 0) {
                edges.push_back({i, {j, adjacencyMatrix[i][j]}});
            }
        }
    }
    return edges;
}


std::vector<size_t> Graph::getNeighbors(size_t node) const {
    std::vector<size_t> neighbors;
    if (node >= adjacencyMatrix.size()) {
        throw std::out_of_range("Node index out of range");
    }
    for (size_t i = 0; i < adjacencyMatrix[node].size(); ++i) {
        if (adjacencyMatrix[node][i] != 0) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

int Graph::getEdgeWeight(size_t ver1, size_t ver2) const {
    if ( static_cast<size_t>(ver1) >= adjacencyMatrix.size()  || static_cast<size_t>(ver2) >= adjacencyMatrix.size()) {
        throw std::out_of_range("Node index out of range");
    }
    return adjacencyMatrix[static_cast<size_t>(ver1)][static_cast<size_t>(ver2)];
}


