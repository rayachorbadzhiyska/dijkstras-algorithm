#include "Node.h"

Node::Node(int value, int cost, Node* head)
{
    this->value = value;
    this->cost = cost;
    this->next = head;
}

const int Node::getValue()
{
    return value;
}

const int Node::getCost()
{
    return cost;
}

Node* Node::getNextNode()
{
    return next;
}
