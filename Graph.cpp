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

bool Graph::doesEdgeExist(int source, int destination) const
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

bool Graph::doesNodeExist(int value) const
{
    return nodeCount > value && value >= 0;
}

void Graph::addNode(Edge* edge)
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

std::string Graph::calculateShortestPath(int source, int destination, GraphWidget* widget) const
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

                // Highlight the shortest path form the source to currently visited node in the Min-Heap if the path exists
                if(adjacent && minimumNodeValue)
                {
                    widget->scheduleTimerForDrawingPath(minimumNodeValue, value);
                }
            }

            adjacent = adjacent->getNextNode();
        }
    }

    std::string pathMessage = "";

    // Check if there is a path to the specified destination
    if(path[destination] != -1)
    {
        std::string steps = composePathToDestination(source, destination, path);
        pathMessage = "Shortest path form " + std::to_string(source) + " to " + std::to_string(destination) + " is " + steps + " and its cost is " + std::to_string(costs[destination]);
    }
    else
    {
        pathMessage = "There is no path form " + std::to_string(source) + " to " + std::to_string(destination);
    }

    return pathMessage;
}

std::string Graph::composePathToDestination(int source, int destination, int* path) const
{
    // String that will be used as a final path representation
    std::string steps = "";

    // Add destination to the final path
    steps += std::to_string(destination) + "-";

    // Get the nodes throught which the source can be accessed and add them to the final path
    int pathDestination = path[destination];
    steps += std::to_string(pathDestination) + "-";
    while(pathDestination != source)
    {
        pathDestination = path[pathDestination];
        steps += std::to_string(pathDestination) + "-";
    }

    // Remove the last '-' from the path
    steps.pop_back();
    // Reverse the path so it can be readable
    std::reverse(steps.begin(), steps.end());

    return steps;
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
