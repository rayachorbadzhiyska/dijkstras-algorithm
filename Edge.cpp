#include "Edge.h"

Edge::Edge(int source, int destination, int weight)
{
    this->source = source;
    this->destination = destination;
    this->weight = weight;
}

int Edge::getSource() const
{
    return source;
}

int Edge::getDestination() const
{
    return destination;
}

int Edge::getWeight() const
{
    return weight;
}
