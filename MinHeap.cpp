#include "MinHeap.h"
#include "HeapNode.h"

#include <stdexcept>

MinHeap::MinHeap(int capacity)
{
    this->capacity = capacity;
    this->nodes = new HeapNode*[capacity];
    this->nodesCount = capacity;
    this->positions = new int[capacity];
}

void MinHeap::setPosition(int position, int value)
{
    this->positions[position] = value;
}

void MinHeap::swapNodes(HeapNode** first, HeapNode** second)
{
    HeapNode* temp = *first;
    *first = *second;
    *second = temp;
}

HeapNode* MinHeap::getMinimumNode()
{
    if(isEmpty())
    {
        return nullptr;
    }

    // Get the first node in the Heap
    HeapNode* head = nodes[0];

    int nodesCount = getNodesCount();

    // Get the last node and place it in place of the head
    HeapNode* lastNode = nodes[nodesCount - 1];
    nodes[0] = lastNode;

    // Update the position of the last node
    positions[head->getValue()] = nodesCount - 1;
    positions[lastNode->getValue()] = 0;

    // Reduce the size of the Heap and perform heapify process to maintain the Min-Heap property
    setNodesCount(--nodesCount);
    heapify(0);

    return head;
}

void MinHeap::heapify(int index)
{
    int minimumElementIndex = index;
    // Store the indecies of the right and left children of the node at the given index
    int rightChild = getRightChild(index);
    int leftChild = getLeftChild(index);

    // Check if the right child's cost is smaller than the cost of а node at the given index and if so - change the smallest node to be the right child
    if(rightChild < nodesCount && nodes[rightChild]->getCost() < nodes[minimumElementIndex]->getCost())
    {
        minimumElementIndex = rightChild;
    }

    // Check if the left child's cost is smaller than the cost of node at the given index and if so - change the smallest node to be the left child
    if(leftChild < nodesCount && nodes[leftChild]->getCost() < nodes[minimumElementIndex]->getCost())
    {
        minimumElementIndex = leftChild;
    }

    // If the current element is not the smallest swap it with the smaller one and perform the heapify recursive process using the current smallest node index
    // unitl the root node is reached
    if(minimumElementIndex != index)
    {
        // Determine the nodes that will be swapped in the Heap
        HeapNode* minimumNode = nodes[minimumElementIndex];
        HeapNode* givenNode = nodes[index];

        // Update nodes' positions. The positions of the nodes will be in reverse order relative to the Heap's logical representaions.
        // This happens due to the fact that when a node is removed from the heap it is swapped with the last one and their positions are also updated.
        positions[minimumNode->getValue()] = index;
        positions[givenNode->getValue()] = minimumElementIndex;

        // Swap the nodes
        swapNodes(&nodes[minimumElementIndex], &nodes[index]);

        heapify(minimumElementIndex);
    }
}

void MinHeap::decreaseCost(int value, int cost)
{
    // Get the index of the node with a given value
    int index = positions[value];
    // Update its cost
    nodes[index]->setCost(cost);

    // Traverse until the array is not heapified
    while(nodes[index]->getCost() < nodes[getParent(index)]->getCost())
    {
        // Swap the current node with its parent
        int currentNodeValue = nodes[index]->getValue();
        positions[currentNodeValue] = getParent(index);

        int parentValue = nodes[getParent(index)]->getValue();
        positions[parentValue] = index;

        swapNodes(&nodes[index], &nodes[getParent(index)]);

        // Change the parent index
        index = getParent(index);
    }
}

bool MinHeap::isEmpty() const
{
    return nodesCount == 0;
}

bool MinHeap::doesNodeExist(int value) const
{
    return positions[value] < nodesCount;
}

int MinHeap::getNodesCount() const
{
    return this->nodesCount;
}

void MinHeap::setNodesCount(int newValue)
{
    this->nodesCount = newValue;
}

int MinHeap::getRightChild(int index) const
{
    return (2 * index) + 2;
}

int MinHeap::getLeftChild(int index) const
{
    return (2 * index) + 1;
}

int MinHeap::getParent(int index) const
{
    return (index - 1) / 2;
}
