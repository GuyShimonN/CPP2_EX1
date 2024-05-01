#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility>
using namespace std;
namespace ariel {
    class Graph {
    private:
        std::vector<std::vector<int>> adjacencyMatrix;

    public:
        void loadGraph(const std::vector<std::vector<int>>& matrix);
        void printGraph() const;
        bool isDirected() const;
        std::size_t getNumberOfNodes() const;
        std::vector<std::pair<int, std::pair<int, int>>> getEdges() const;
        std::vector<std::size_t> getNeighbors(std::size_t node) const ;
        int getEdgeWeight(int u, int v) const ;
        bool isEdge(size_t from, size_t to) const {
            return adjacencyMatrix[from][to] != 0;
        }
    };
}

#endif // GRAPH_HPP