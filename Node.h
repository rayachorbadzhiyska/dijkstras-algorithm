#ifndef NODE_H
#define NODE_H

/// Represents a node.
class Node
{
public:
    /// Default constructor, creates an empty node.
    Node();
    /// Constructor, creates an edge with the specified value, cost and head.
    Node(int value, int cost, Node* head);

    /// Gets the node's value.
    const int getValue();
    /// Gets the node's cost.
    const int getCost();
    /// Gets the node's next node.
    Node* getNextNode();

private:
    int value;
    int cost;
    Node* next;
};

#endif // NODE_H
