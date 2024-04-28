#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
using namespace std;
namespace ariel {
    class Graph {
    private:
        std::vector<std::vector<int>> adjacencyMatrix;

    public:
        void loadGraph(const std::vector<std::vector<int>>& matrix);
        void printGraph() const;
        bool isDirected() const;
    };
}

#endif // GRAPH_HPP