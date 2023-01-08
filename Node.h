#ifndef NODE_H
#define NODE_H

/// Represents a node.
class Node
{
public:
    /// Default constructor, creates an empty node.
    Node();
    /// Constructor, creates a node with the specified value, cost and head.
    Node(int value, int cost, Node* head);

    /// Gets the node's value.
    int getValue() const;
    /// Gets the node's cost.
    int getCost() const;
    /// Gets the node's next node.
    Node* getNextNode();

private:
    int value;
    int cost;
    Node* next;
};

#endif // NODE_H
