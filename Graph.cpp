#include "Graph.h"

Graph::Graph(int nodeCount, int edgeCount)
{
    this->nodeCount = nodeCount;
    this->allEdgeCount = edgeCount;

    head = new Node*[nodeCount];

    //Initialize the head node for every edge.
    for(int i = 0; i < nodeCount; i++)
    {
        head[i] = nullptr;
    }
}

Graph::~Graph()
{
    for(int i = 0; i < nodeCount; i++)
    {
        delete[] head[i];
    }

    delete[] head;
};

void Graph::addEdge(Edge* edge)
{
    int edgeDestination = edge->getDestination();
    int edgeWeight = edge->getWeight();

    //Create a new node with the new edge's destination as value, new edge's weight as cost,
    //and new edge's source as head.
    Node* newNode = new Node(edgeDestination, edgeWeight, head[edge->getSource()]);

    //Set the head to the new node.
    head[edge->getSource()] = newNode;

    currentEdgeCount++;
}
