#ifndef MINHEAP_H
#define MINHEAP_H

#include "HeapNode.h"

/// Represents a Min-Heap
class MinHeap
{
public:
    /// Represents the nodes array of the Min-Heap
    HeapNode** nodes;
    /// Represents the positions of the nodes in the Heap starting from tha last one and depending on their costs;
    /// Example: position[nodesCount-1] = first node in the Heap (the one with the smallest cost); position[0] = the last node in the Heap (the one with biggest cost)
    int* positions;

    /// Constructor - creates a Min-Heap with the given capacity
    MinHeap(int capacity);

    /// Function to set the value of a node at a given position
    void setPosition(int position, int value);

    /// Function used to decrease cost value of the given node
    void decreaseCost(int value, int cost);
    /// Function used to extract the minimum node
    HeapNode* getMinimumNode();

    /// Function to determine if the Heap is empty
    bool isEmpty() const;
    /// Fucntion to determine if a node with a given value is in the Heap
    bool doesNodeExist(int value) const;

private:
    int nodesCount;
    int capacity;

    /// Function to swap 2 nodes in the Heap. It is used in the *heapify* function
    void swapNodes(HeapNode** first, HeapNode** second);

    /// Function used to reorder the Heap such that a child node is greater thar or equeal to the parent node
    void heapify(int index);

    /// Function to return the nodesCount value
    int getNodesCount() const;
    /// Function to set the nodesCount to a given value
    void setNodesCount(int newValue);

    /// Function used to return the index of the right child of a node at a given index
    int getRightChild(int index) const;
    /// Function used to return the index of the left child of a node at a given index
    int getLeftChild(int index) const;
    /// Function used to return the index of the parent node of a given one's index
    int getParent(int index) const;
};

#endif // MINHEAP_H
