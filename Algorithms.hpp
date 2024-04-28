#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Graph.hpp"
#include <vector>

namespace ariel {
    class Algorithms {
    public:
        static bool isConnected(const Graph& g);
        static std::vector<int> shortestPath(const Graph& g, int start, int end);
        static bool isContainsCycle(const Graph& g);
        static bool isBipartite(const Graph& g);
        static bool negativeCycle(const Graph& g);
    private:
        static void DFS(const Graph& g, int node, std::vector<bool>& visited);
    };
}

#endif // ALGORITHMS_HPP