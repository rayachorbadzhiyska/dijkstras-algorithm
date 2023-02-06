#ifndef HEAPNODE_H
#define HEAPNODE_H

/// Represents a node from the Min-Heap
class HeapNode
{
public:
    /// Default constructor
    HeapNode();
    /// Constructor - creates a node using the specified value and cost
    HeapNode(int value, int cost);

    /// Returns the value of a node
    int getValue() const;
    /// Updates the value of a node
    void setValue(int newValue);
    /// Returns the cost of a node
    int getCost() const;
    /// Updates the cost of a node
    void setCost(int newValue);

private:
    int value;
    int cost;
};

#endif // HEAPNODE_H
