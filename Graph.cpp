#include "Graph.h"
#include "DijkstraInputException.h"

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

bool Graph::doesEdgeExist(int source, int destination)
{
    Node* sourceHead = head[source];

    //Go through all the nodes, leading from the specified edge's source
    //and check whether the specified edge's destination is present as any of the nodes' value.
    while (sourceHead != nullptr)
    {
        int value = sourceHead->getValue();

        if (value == destination)
        {
            return true;
        }

        sourceHead = sourceHead->getNextNode();
    }

    return false;
}

void Graph::addEdge(Edge* edge)
{
    int edgeSource = edge->getSource();
    int edgeDestination = edge->getDestination();
    int edgeWeight = edge->getWeight();

    //Check for out of bounds edge source.
    if (edgeSource < 0 || edgeSource >= nodeCount)
    {
        throw DijkstraInputException("Invalid edge source.");
    }

    //Check for out of bounds edge destination.
    if (edgeDestination < 0 || edgeDestination >= nodeCount)
    {
        throw DijkstraInputException("Invalid edge destination.");
    }

    //Check whether the edge is a self loop.
    if (edgeSource == edgeDestination)
    {
        throw DijkstraInputException("No self loops allowed.");
    }

    //Check whether the edge exists already.
    if (doesEdgeExist(edgeSource, edgeDestination))
    {
        throw DijkstraInputException("Edge already exists.");
    }

    //Create a new node with the new edge's destination as value, new edge's weight as cost,
    //and new edge's source as head.
    Node* newNode = new Node(edgeDestination, edgeWeight, head[edgeSource]);

    //Set the head to the new node.
    head[edgeSource] = newNode;

    setCurrentEdgeCount(++currentEdgeCount);
}

int Graph::getCurrentEdgeCount() const
{
    return currentEdgeCount;
}

void Graph::setCurrentEdgeCount(int edgeCount)
{
    currentEdgeCount = edgeCount;

    emit currentEdgeCountValueChanged(edgeCount);
}

Graph::~Graph()
{
    for (int i = 0; i < nodeCount; i++)
    {
        delete[] head[i];
    }

    delete[] head;
};
