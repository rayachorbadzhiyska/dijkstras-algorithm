#include "HeapNode.h"

HeapNode::HeapNode()
{
    this->value = 0;
    this->cost = 0;
}

HeapNode::HeapNode(int value, int cost)
{
    this->value = value;
    this->cost = cost;
}

int HeapNode::getValue() const
{
    return this->value;
}

void HeapNode::setValue(int newValue)
{
    this->value = newValue;
}

int HeapNode::getCost() const
{
    return this->cost;
}

void HeapNode::setCost(int newValue)
{
    this->cost = newValue;
}
