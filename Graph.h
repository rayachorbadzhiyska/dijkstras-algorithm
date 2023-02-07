#ifndef GRAPH_H
#define GRAPH_H
#include "Node.h"
#include "Edge.h"
#include "qobject.h"

class GraphWidget;

/// Represents a graph.
class Graph : public QObject
{
    Q_OBJECT

public:
    /// Represents the head node of an adjacency list, containing the graph.
    Node** head;

    /// Constructor, creates a graph with the specified node count and edge count.
    Graph(int nodeCount, int edgeCount);
    /// Destructor, deallocates memory.
    ~Graph();

    /// Adds a graph edge.
    void addEdge(Edge* edge);

    /// Gets the current edge count.
    int getCurrentEdgeCount() const;
    /// Sets the current edge count.
    void setCurrentEdgeCount(int edgeCount);

    /// Caculates the shortest path between source and destintion nodes using the Dijkstra’s shortest path algorithm
    void calculateShortestPath(int source, int destination, GraphWidget* widget) const;

    int getCurrentNodeCount() const;

signals:
    void currentEdgeCountValueChanged(int newValue);

private:
    int nodeCount;
    int allEdgeCount;
    int currentEdgeCount;

    /// Returns whether the edge exists in the graph.
    bool doesEdgeExist(int source, int destinaion);
};

#endif // GRAPH_H
