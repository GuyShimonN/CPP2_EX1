#include "doctest.h"
#include "Algorithms.hpp"
#include "Graph.hpp"
#include <stdexcept>
using namespace std;

TEST_CASE("Test isConnected")
{

    ariel::Graph grp;
    vector<vector<int>> graph = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}};
    grp.loadGraph(graph);
    CHECK(ariel::Algorithms::isConnected(grp) == true);

    vector<vector<int>> graph2 = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}};
    grp.loadGraph(graph2);
    CHECK(ariel::Algorithms::isConnected(grp) == false);
}

TEST_CASE("Test shortestPath")
{
    ariel::Graph grp;
    vector<vector<int>> graph = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}};
    grp.loadGraph(graph);
    CHECK(ariel::Algorithms::shortestPath(grp, 0, 2) == "0->1->2");

    vector<vector<int>> graph2 = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}};
    grp.loadGraph(graph2);
    CHECK(ariel::Algorithms::shortestPath(grp, 0, 4) == "-1");
}
TEST_CASE("Test shortestPath with disconnected graph")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}};
    g.loadGraph(graph);
    CHECK(ariel::Algorithms::shortestPath(g, 0, 3) == "-1");
}
TEST_CASE("Test isContainsCycle")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}};
    g.loadGraph(graph);
    CHECK(ariel::Algorithms::isContainsCycle(g) == false);

    vector<vector<int>> graph2 = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}};
    g.loadGraph(graph2);
    CHECK(ariel::Algorithms::isContainsCycle(g) == true);
}
TEST_CASE("Test isBipartite")
{
    ariel::Graph grp;
    vector<vector<int>> graph = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}};
    grp.loadGraph(graph);
    CHECK(ariel::Algorithms::isBipartite(grp) == "The graph is bipartite: A={0, 2}, B={1}");

    vector<vector<int>> graph2 = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}};
    grp.loadGraph(graph2);
    CHECK(ariel::Algorithms::isBipartite(grp) == "0");

    vector<vector<int>> graph3 = {
        {0, 1, 0, 0, 0},
        {1, 0, 3, 0, 0},
        {0, 3, 0, 4, 0},
        {0, 0, 4, 0, 5},
        {0, 0, 0, 5, 0}};
    grp.loadGraph(graph3);
    CHECK(ariel::Algorithms::isBipartite(grp) == "The graph is bipartite: A={0, 2, 4}, B={1, 3}");
}
TEST_CASE("Test invalid graph")
{
    ariel::Graph grp;
    vector<vector<int>> graph = {
        {0, 1, 2, 0},
        {1, 0, 3, 0},
        {2, 3, 0, 4},
        {0, 0, 4, 0},
        {0, 0, 0, 5}};
    CHECK_THROWS(grp.loadGraph(graph));
}
TEST_CASE("Test loadGraph with valid graph")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, 3, 0, 0},
            {0, 3, 0, 4, 0},
            {0, 0, 4, 0, 5},
            {0, 0, 0, 5, 0}};
    g.loadGraph(graph);
    CHECK(g.getNumberOfNodes() == 5);
}

TEST_CASE("Test loadGraph with empty graph")
{
    ariel::Graph g;
    vector<vector<int>> graph = {};
    CHECK_THROWS_AS(g.loadGraph(graph), std::invalid_argument);
}

TEST_CASE("Test loadGraph with non-square matrix")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0},
            {1, 0, 3, 0},
            {0, 3, 0, 4, 0},
            {0, 0, 4, 0, 5},
            {0, 0, 0, 5, 0}};
    CHECK_THROWS_AS(g.loadGraph(graph), std::invalid_argument);
}
TEST_CASE("Test DFS")
{

    ariel::Graph grp;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, 3, 0, 0},
            {0, 3, 0, 4, 0},
            {0, 0, 4, 0, 5},
            {0, 0, 0, 5, 0}};
    grp.loadGraph(graph);

    std::vector<bool> visited(grp.getNumberOfNodes(), false);
    ariel::Algorithms::DFS(grp, 0, visited);

    // All vertices should be visited
    for (bool v : visited) {
        CHECK(v == true);
    }
}
TEST_CASE("Test DFS with disconnected graph")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}};
    g.loadGraph(graph);

    std::vector<bool> visited(g.getNumberOfNodes(), false);
    ariel::Algorithms::DFS(g, 0, visited);

    // Only vertices 0 and 1 should be visited
    CHECK(visited[0] == true);
    CHECK(visited[1] == true);
    CHECK(visited[2] == false);
    CHECK(visited[3] == false);
    CHECK(visited[4] == false);
}

TEST_CASE("Test DFS with self-loop")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}};
    g.loadGraph(graph);

    std::vector<bool> visited(g.getNumberOfNodes(), false);
    ariel::Algorithms::DFS(g, 0, visited);

    // Only vertex 0 should be visited
    for (size_t i = 0; i < visited.size(); ++i) {
        if (i == 0) {
            CHECK(visited[i] == true);
        } else {
            CHECK(visited[i] == false);
        }
    }
}


TEST_CASE("Test negativeCycle with no negative cycle")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, 3, 0, 0},
            {0, 3, 0, 4, 0},
            {0, 0, 4, 0, 5},
            {0, 0, 0, 5, 0}};
    g.loadGraph(graph);

    CHECK(ariel::Algorithms::negativeCycle(g) == "0");
}

TEST_CASE("Test negativeCycle with negative cycle")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, -3, 0, 0},
            {0, -3, 0, 4, 0},
            {0, 0, 4, 0, 5},
            {0, 0, 0, 5, 0}};
    g.loadGraph(graph);

    CHECK(ariel::Algorithms::negativeCycle(g) != "0");
}

TEST_CASE("Test isCyclicUtil with no cycle")
{
    ariel::Graph g;
    vector<vector<int>> graph = {
            {0, 1, 0, 0, 0},
            {1, 0, 3, 0, 0},
            {0, 3, 0, 4, 0},
            {0, 0, 4, 0, 5},
            {0, 0, 0, 5, 0}};
    g.loadGraph(graph);

    std::vector<bool> visited(g.getNumberOfNodes(), false);
    std::vector<bool> recStack(g.getNumberOfNodes(), false);
    std::vector<size_t> parent(g.getNumberOfNodes(), SIZE_MAX);
    std::vector<size_t> cycle;
    CHECK(ariel::Algorithms::isCyclicUtil(0, visited, recStack, parent, g, cycle) == false);
}

