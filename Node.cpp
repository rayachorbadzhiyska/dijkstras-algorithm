#include "Node.h"

Node::Node(int value, int cost, Node* head)
{
    this->value = value;
    this->cost = cost;
    this->next = head;
}

int Node::getValue() const
{
    return value;
}

int Node::getCost() const
{
    return cost;
}

Node* Node::getNextNode()
{
    return next;
}
