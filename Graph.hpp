#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility>

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
    };
}

#endif // GRAPH_HPP