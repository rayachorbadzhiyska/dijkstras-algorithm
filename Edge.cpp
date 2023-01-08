#include "Edge.h"

Edge::Edge(int source, int destination, int weight)
{
    this->source = source;
    this->destination = destination;
    this->weight = weight;
}

const int Edge::getSource()
{
    return source;
}

const int Edge::getDestination()
{
    return destination;
}

const int Edge::getWeight()
{
    return weight;
}
