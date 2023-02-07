#ifndef EDGE_H
#define EDGE_H

/*!
 * Represents a graph's edge.
 */
class Edge
{
public:
    /*!
     * Constructor, creates an edge with the specified source, destination and weight.
     * \param source The edge's source.
     * \param destination The edge's destination.
     * \param weight The edge's weight.
     */
    Edge(int source, int destination, int weight);

    /*!
     *  Gets the edge's source.
     */
    int getSource() const;
    /*!
     *  Gets the edge's destination.
     */
    int getDestination() const;
    /*!
     *  Gets the edge's weight.
     */
    int getWeight() const;

private:
    int source;
    int destination;
    int weight;
};

#endif // EDGE_H
