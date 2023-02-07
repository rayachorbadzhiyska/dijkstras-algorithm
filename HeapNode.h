#ifndef HEAPNODE_H
#define HEAPNODE_H

/*!
 * Represents a node from the Min-Heap
 */
class HeapNode
{
public:
    /*!
     * Default constructor
     */
    HeapNode();
    /*!
     * Constructor - creates a node using the specified value and cost
     * \param value The node's value
     * \param cost The node's cost
     */
    HeapNode(int value, int cost);

    /*!
     * Returns the value of the node
     */
    int getValue() const;
    /*!
     * Updates the value of a node
     * \param newValue The new value
     */
    void setValue(int newValue);
    /*!
     * Returns the cost of the node
     */
    int getCost() const;
    /*!
     * Updates the cost of a node
     * \param newValue The new cost
     */
    void setCost(int newValue);

private:
    int value;
    int cost;
};

#endif // HEAPNODE_H
