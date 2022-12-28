#ifndef EDGE_H
#define EDGE_H

/// Represents a graph's edge.
class Edge
{
public:
    /// Constructor, creating an edge with the specified source, destination and weight.
    Edge(int source, int destination, int weight);

    /// Gets the edge's source.
    const int getSource();
    /// Gets the edge's destination.
    const int getDestination();
    /// Gets the edge's weight.
    const int getWeight();

private:
    int source;
    int destination;
    int weight;
};

#endif // EDGE_H
