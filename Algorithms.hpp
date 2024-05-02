#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Graph.hpp"
#include <vector>
#include <string>

namespace ariel {
    class Algorithms {
    public:
        static bool isConnected(const Graph& grp);
        static std::string shortestPath(const Graph& g, size_t start, size_t end);
        static bool isContainsCycle(const Graph& g);
        static std::string isBipartite(const Graph& g);
        static std::string negativeCycle(const Graph& g);
        static void DFS(const Graph& grp, size_t node, std::vector<bool>& visited);

        static bool isCyclicUtil(size_t v, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<size_t >& parent, const Graph& g, std::vector<size_t>& cycle);

    private:
        static bool isEdgeAndNotVisited(size_t v, size_t i, const Graph& grp, std::vector<bool>& visited);};
}

#endif // ALGORITHMS_HPP