#ifndef GRAPH_H
#define GRAPH_H
#include "Node.h"
#include "Edge.h"
#include "qobject.h"

class GraphWidget;

/*!
 *  Represents a graph.
 */
class Graph : public QObject
{
    Q_OBJECT

public:
    /*!
     * Represents the head node of an adjacency list, containing the graph.
    */
    Node** head;

    /*!
     * Constructor, creates a graph with the specified node count and edge count.
     * \param nodeCount The count of all the nodes.
     * \param edgeCount The count of all the edges.
     */
    Graph(int nodeCount, int edgeCount);
    /*!
     * Destructor, deallocates memory.
     */
    ~Graph();

    /*!
     * Adds a graph edge.
     * \param edge The edge to be added.
     */
    void addEdge(Edge* edge);

    /*!
     * Gets the current edge count.
     */
    int getCurrentEdgeCount() const;
    /*!
     * Sets the current edge count.
     * \param edgeCount the new edge count.
     */
    void setCurrentEdgeCount(int edgeCount);

    /*!
     * Caculates the shortest path between source and destintion nodes using the Dijkstra’s shortest path algorithm and returns information about the path to the specified source
     * \param source The source node
     * \param destination The destination node
     * \param widget GraphWidget object used to draw the path
     * \return string Representing the shortest path information
     */
    std::string calculateShortestPath(int source, int destination, GraphWidget* widget) const;

    /*!
     * Get the current node count
     * \return int Current count of the nodes
     */
    int getCurrentNodeCount() const;

    /*!
     * Returns whether a node with a specified value exists in the graph
     * \param value The value of the node to be searched for
     * \return bool Whether a node exists
     */
    bool doesNodeExist(int value) const;

signals:
    /*!
     * Signal, which is emitted when the current edge count's value is changed.
     * \param newValue The new value of the current edge count.
     */
    void currentEdgeCountValueChanged(int newValue);

private:
    int nodeCount;
    int allEdgeCount;
    int currentEdgeCount;

    /*!
     * Checks if an edge already exists in the graph.
     * \param source The edge's source.
     * \param destinaion The edge's destination.
     * \return Whether the edge exists in the graph.
     */
    bool doesEdgeExist(int source, int destinaion) const;

    /*!
     * Returns a formatted path from source to destination using stored steps in path array
     * \param source The source node
     * \param destination The destination node
     * \param path Pointer to an array of paths to a nodes throught a prevous node with the minimum value
     * \return string The formatted path from source to destination
     */
    std::string composePathToDestination(int source, int destination, int* path) const;
};

#endif // GRAPH_H
