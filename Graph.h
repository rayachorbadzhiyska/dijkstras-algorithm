#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Edge.h"

///Represents a graph.
class Graph
{
public:
    /// Represents the head node of an adjacency list, containing the graph.
    Node** head;

    /// Constructor, creates a graph with the specified node count and edge count.
    Graph(int nodeCount, int edgeCount);
    /// Destructor, deallocates memory.
    ~Graph();

    /// Adds a graph edge.
    void addEdge(Edge* edge);

private:
    int nodeCount;
    int allEdgeCount;
    int currentEdgeCount = 0;
};

#endif // GRAPH_H
