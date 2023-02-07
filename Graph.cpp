#include "Graph.h"
#include "DijkstraInputException.h"
#include "MinHeap.h"
#include "GraphWidget.h"
#include <QTimer>

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

void Graph::calculateShortestPath(int source, int destination, GraphWidget* widget) const
{
    // An array to hold the cost of the paths to a node with index i
    int costs[nodeCount];

    // An array to hold the path to a node throught a prevous node with the minimum value
    int path[nodeCount];

    MinHeap heap = MinHeap(nodeCount);

    for(int i = 0; i < nodeCount; ++i)
    {
        costs[i] = INT_MAX;
        path[i] = -1;
        heap.nodes[i] = new HeapNode(i, costs[i]);
        heap.setPosition(i, i);
    }

    // Change the cost of the source to be 0 because it is the first node that is visited
    heap.nodes[source] = new HeapNode(source, costs[source]);
    costs[source] = 0;
    heap.decreaseCost(source, costs[source]);

    // Traverse the Heap until all nodes' shortest paths are calculated
    while(!heap.isEmpty())
    {
        // Get the minimum cost node's value
        int minimumNodeValue = heap.getMinimumNode()->getValue();

        // Traverse all adjacent nodes of the minimum one and update their costs
        Node* adjacent = head[minimumNodeValue];
        while(adjacent != nullptr)
        {
            int value = adjacent->getValue();

            // Check if the cost of the node with the extracted value is calculated and if not =>
            // the cost to this node throught the minimum node is less than the prevoiusly calculated cost
            if(heap.doesNodeExist(value) && costs[minimumNodeValue] != INT_MAX && adjacent->getCost() + costs[minimumNodeValue] < costs[value])
            {
                costs[value] = adjacent->getCost() + costs[minimumNodeValue];
                // store the path throught the node with the minimum value
                path[value] = minimumNodeValue;
                // Update the cost value of the node in the Heap
                heap.decreaseCost(value, costs[value]);
            }

            // Highlight the shortest path form the source to currently visited node in the Min-Heap if the path exists
            if(adjacent && minimumNodeValue)
            {
                widget->scheduleTimerForDrawingPath(minimumNodeValue, value);
            }

            adjacent = adjacent->getNextNode();
        }
    }

    for(int i=0; i<nodeCount; i++)
    {
        printf("positions[%d] = %d; cost of %d = %d\n", i, heap.positions[i], i, costs[i]);
    }
}

int Graph::getCurrentNodeCount() const
{
    return nodeCount;
}

Graph::~Graph()
{
    for (int i = 0; i < nodeCount; i++)
    {
        delete[] head[i];
    }

    delete[] head;
};
