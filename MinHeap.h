#ifndef MINHEAP_H
#define MINHEAP_H

#include "HeapNode.h"

/*!
 * Represents a Min-Heap
 */
class MinHeap
{
public:
    /*!
     * Represents the nodes array of the Min-Heap
     */
    HeapNode** nodes;
    /*!
     * Represents the positions of the nodes in the Heap starting from tha last one and depending on their costs;
     * Example: position[nodesCount-1] = first node in the Heap (the one with the smallest cost); position[0] = the last node in the Heap (the one with biggest cost)
     */
    int* positions;

    /*!
     * Constructor - creates a Min-Heap with the given capacity
     * \param capacity The capacity of the Heap
     */
    MinHeap(int capacity);
    /*!
     * Destructor - deallocates memory
     */
    ~MinHeap();

    /*!
     * Function to update the position of a node at a given position
     * \param position Node's position in the positions array
     * \param value The new position of the node
     */
    void setPosition(int position, int value);

    /*!
     * Function used to decrease cost value of the given node
     * \param value Node's value
     * \param cost The new cost
     */
    void decreaseCost(int value, int cost);
    /*!
     * Function used to extract the minimum node
     */
    HeapNode* getMinimumNode();

    /*!
     * Function to determine if the Heap is empty
     * \return bool If the Heap is empty
     */
    bool isEmpty() const;
    /*!
     * Fucntion to determine if a node with a given value is in the Heap
     * \param value The value of a node
     * \return bool Whether a node exists in the Heap
     */
    bool doesNodeExist(int value) const;

private:
    int nodesCount;
    int capacity;

    /*!
     * Function to swap 2 nodes in the Heap. It is used in the *heapify* function
     * \param first Pointer to pointer to a node
     * \param second Pointer to pointer to a node
     */
    void swapNodes(HeapNode** first, HeapNode** second);

    /*!
     * Function used to reorder the Heap such that a child node is greater thar or equeal to the parent node
     * \param index The index from which the process will start
     */
    void heapify(int index);

    /*!
     * Function to return the nodesCount value
     * \return int Nodes count
     */
    int getNodesCount() const;
    /*!
     * Function to set the nodesCount to a given value
     * \param newValue The new value of nodesCount
     */
    void setNodesCount(int newValue);

    /*!
     * Function used to return the index of the right child of a node at a given index
     * \param index Node's index
     * \return int Index of the right child
     */
    int getRightChild(int index) const;
    /*!
     * Function used to return the index of the left child of a node at a given index
     * \param index Node's index
     * \return int Index of the left child
     */
    int getLeftChild(int index) const;
    /*!
     * Function used to return the index of the parent node of a given one's index
     * \param index Node's index
     * \return int Index of the parent
     */
    int getParent(int index) const;
};

#endif // MINHEAP_H
