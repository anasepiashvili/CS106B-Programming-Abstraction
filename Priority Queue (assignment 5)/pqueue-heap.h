/**********************************************
 * File: pqueue-heap.h
 *
 * A priority queue class backed by a binary
 * heap.
 */
#ifndef PQueue_Heap_Included
#define PQueue_Heap_Included

#include <string>
#include <iostream>
using namespace std;

/* A class representing a priority queue backed by an
 * binary heap.
 */
class HeapPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a binary heap. */
	HeapPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~HeapPriorityQueue();
	
	/* Returns the number of elements in the priority queue. */
	int size();
	
	/* Returns whether or not the priority queue is empty. */
	bool isEmpty();
	
	/* Enqueues a new string into the priority queue. */
	void enqueue(string value);
	
	/* Returns, but does not remove, the lexicographically first string in the
	 * priority queue.
	 */
	string peek();
	
	/* Returns and removes the lexicographically first string in the
	 * priority queue.
	 */
	string dequeueMin();

private:

	int allocatedLength;
	
	int logicalLength;
	
	/* Elements of heap are stored in dynamically allocated array of strings */
	string* heap;

	/* Swaps values of elements on given indexes of priority queue */
	void swap(int index1, int index2);
	
	/* Smaller element is swapped with its lexicographically bigger parentNode */
	void bubbleUp(int childIndex, int parentIndex);
	
	/* Lexicographically bigger parentNode is swapped with smaller childNode */
	void bubbleDown(int parentIndex);
	
	/* Doubles allocated length of priority queue */
	void grow();

	/* Returns whether or not element in priority queue is legal on given index */
	bool isLegal(int index);
};

#endif
