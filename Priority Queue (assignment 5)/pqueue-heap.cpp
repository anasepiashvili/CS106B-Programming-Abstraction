/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

HeapPriorityQueue::HeapPriorityQueue() {
	allocatedLength = 1;
	logicalLength = 0;
	heap = new string[allocatedLength];
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete[] heap;
	logicalLength = 0;
}

int HeapPriorityQueue::size() {
	return logicalLength;
}

bool HeapPriorityQueue::isEmpty() {	
	return size() == 0;
}

void HeapPriorityQueue::grow() {
	allocatedLength *= 2;
	string* temp = new string[allocatedLength];
	for (int i = 0; i < logicalLength; i++) {
		temp[i] = heap[i];
	}
	delete[] heap;
	heap = temp;
}

bool HeapPriorityQueue::isLegal(int index) {
	return index <= logicalLength - 1;
}

void HeapPriorityQueue::swap(int index1, int index2) {
	string temp = heap[index1];
	heap[index1] = heap[index2];
	heap[index2] = temp;
}

void HeapPriorityQueue::bubbleUp(int childIndex, int parentIndex) {
	if (childIndex == 0) return;
	if (heap[childIndex] >= heap[parentIndex]) {
		return;
	} else {
		swap(childIndex, parentIndex);
		bubbleUp(parentIndex, (parentIndex - 1) / 2);
	}
}

void HeapPriorityQueue::enqueue(string value) {
	if (allocatedLength == logicalLength) {
		grow();
	}
	heap[logicalLength] = value;
	logicalLength++;
	int childIndex = logicalLength - 1;  // index of last element
	int parentIndex = (childIndex - 1) / 2;  
	bubbleUp(childIndex, parentIndex);
}

string HeapPriorityQueue::peek() {
	if (isEmpty())
		error("Queue is empty.");
	
	return heap[0];
}

void HeapPriorityQueue::bubbleDown(int cur) {
	while (true) {
		// breaks if current index is not legal or element on this index does not have childNode
		if (!isLegal(cur) || !isLegal(2*cur+1)) break; 
		
		if (isLegal(2*cur+2)) { // if element on current index has both childNodes
			if (heap[2*cur+1] < heap[2*cur+2]) { // compares childNodes
												// if first childNode is smaller
				
				if (heap[cur] > heap[2*cur+1]) { // if parentNode is bigger than childNode
					swap(cur, 2*cur+1);
					cur = 2*cur+1;
				} else break;
			} else { // if second childNode is smaller
				if (heap[cur] > heap[2*cur+2]) { // if parentNode is bigger than childNode
				swap(cur, 2*cur+2);
				cur = 2*cur+2;
				} else break;
			}
		} else { // if element on current index has only one childNode
			if (heap[cur] > heap[2*cur+1]) { // if parentNode is bigger than childNode
				swap(cur, 2*cur+1);
				cur = 2*cur+1;
			} else break;
		}
	}
}

string HeapPriorityQueue::dequeueMin() {	
	string result = peek();
	if (logicalLength >= 2)
		swap(0, logicalLength - 1);  // swaps first and last elements of heap
	logicalLength--;
	bubbleDown(0); // bubbles down first element
	return result;
}



