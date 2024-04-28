#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Graph.hpp"
#include <vector>
#include <string>

namespace ariel {
    class Algorithms {
    public:
        static bool isConnected(const Graph& g);
        static std::string shortestPath(const Graph& g, int start, int end);
        static std::string isContainsCycle(const Graph& g);
        static std::string isBipartite(const Graph& g);
        static std::string negativeCycle(const Graph& g);
    private:
        static void DFS(const Graph& g, int node, std::vector<bool>& visited);
        static bool isCyclicUtil(int v, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<int>& parent, const Graph& g);    };
}

#endif // ALGORITHMS_HPP